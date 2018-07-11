/*
 * AbstractChunk.cpp
 *****************************************************************************
 * Copyright (C) 2012, bitmovin Softwareentwicklung OG, All Rights Reserved
 *
 * Email: libdash-dev@vicky.bitmovin.net
 *
 * This source code and its use and distribution, is subject to the terms
 * and conditions of the applicable license agreement.
 *****************************************************************************/
#ifndef NO_CURL
#if _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4574)
#pragma warning(disable: 4668)
#include <curl/curl.h>
#pragma warning(pop)
#else
#include <curl/curl.h>
#endif
#endif
#include "AbstractChunk.h"
using namespace dash::network;
using namespace dash::helpers;
using namespace dash::metrics;

uint32_t AbstractChunk::BLOCKSIZE = 32768;

#ifndef NO_CURL
class AbstractChunk::CURLState
{
public:
    CURL*           curl;
    CURLcode        response;
    static size_t   CurlResponseCallback(void *contents, size_t size, size_t nmemb, void *userp);
    static size_t   CurlDebugCallback(CURL *curl, curl_infotype infoType, char * data, size_t length, void *userdata);
    static int      CurlXFerCallback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);
};
#endif


AbstractChunk::AbstractChunk        ()  :
               connection           (NULL),
               dlThread             (NULL),
               bytesDownloaded      (0)
{
#ifndef NO_CURL
    state = new CURLState;
#endif
}
AbstractChunk::~AbstractChunk       ()
{
    this->AbortDownload();

    DestroyThreadPortable(this->dlThread);
    //
    for (size_t i = 0; i < this->httpTransactions.size(); i++)
        delete(this->httpTransactions.at(i));
#ifndef NO_CURL
    delete state;
#endif
}

void    AbstractChunk::AbortDownload                ()
{
    this->stateManager.CheckAndSet(IN_PROGRESS, REQUEST_ABORT);
    this->stateManager.CheckAndWait(REQUEST_ABORT, ABORTED);
}

#ifndef NO_CURL
int      AbstractChunk::CURLState::CurlXFerCallback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
    AbstractChunk   *chunk = (AbstractChunk *)clientp;        
    if (chunk->stateManager.State() == REQUEST_ABORT)
    {
        return 1;
    }
    /* we could use this for progress..*/
    /*chunk->bytesDownloaded = dlnow;
    chunk->NotifyDownloadRateChanged();*/
    return 0;
}
#endif
bool    AbstractChunk::StartDownload                ()
{
#ifndef NO_CURL
    if(this->stateManager.State() != NOT_STARTED)
        return false;

    state->curl = curl_easy_init();
    curl_easy_setopt(state->curl, CURLOPT_NOPROGRESS, 0);
    curl_easy_setopt(state->curl, CURLOPT_XFERINFOFUNCTION, CURLState::CurlXFerCallback);
    curl_easy_setopt(state->curl, CURLOPT_XFERINFODATA, (void*)this);
    curl_easy_setopt(state->curl, CURLOPT_URL, this->AbsoluteURI().c_str());
    curl_easy_setopt(state->curl, CURLOPT_WRITEFUNCTION, CURLState::CurlResponseCallback);
    curl_easy_setopt(state->curl, CURLOPT_WRITEDATA, (void *)this);
    /* Debug Callback */
    curl_easy_setopt(state->curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(state->curl, CURLOPT_DEBUGFUNCTION, CURLState::CurlDebugCallback);
    curl_easy_setopt(state->curl, CURLOPT_DEBUGDATA, (void *)this);
    curl_easy_setopt(state->curl, CURLOPT_FAILONERROR, true);

    if(this->HasByteRange())
        curl_easy_setopt(state->curl, CURLOPT_RANGE, this->Range().c_str());

    this->dlThread = CreateThreadPortable (DownloadInternalConnection, this);

    if(this->dlThread == NULL)
        return false;

    this->stateManager.State(IN_PROGRESS);

    return true;
#else
    return false;
#endif
}
bool    AbstractChunk::StartDownload                (IConnection *aConnection)
{
    if(this->stateManager.State() != NOT_STARTED)
        return false;

    this->dlThread = CreateThreadPortable (DownloadExternalConnection, this);

    if(this->dlThread == NULL)
        return false;

    this->stateManager.State(IN_PROGRESS);
    this->connection = aConnection;

    return true;
}
int     AbstractChunk::Read                         (uint8_t *data, size_t len)
{
    return (int)this->blockStream.GetBytes(data, len);
}
int     AbstractChunk::Peek                         (uint8_t *data, size_t len)
{
    return (int)this->blockStream.PeekBytes(data, len);
}
int     AbstractChunk::Peek                         (uint8_t *data, size_t len, size_t offset)
{
    return (int)this->blockStream.PeekBytes(data, len, offset);
}
void    AbstractChunk::AttachDownloadObserver       (IDownloadObserver *observer)
{
    this->observers.push_back(observer);
    this->stateManager.Attach(observer);
}
void    AbstractChunk::DetachDownloadObserver       (IDownloadObserver *observer)
{
    int32_t pos = -1;

    for(size_t i = 0; i < this->observers.size(); i++)
        if(this->observers.at(i) == observer)
            pos = (int32_t)i;

    if(pos != -1)
        this->observers.erase(this->observers.begin() + pos);

    this->stateManager.Detach(observer);
}
void*   AbstractChunk::DownloadExternalConnection   (void *abstractchunk)
{
    AbstractChunk   *chunk  = (AbstractChunk *) abstractchunk;
    block_t         *block  = AllocBlock(chunk->BLOCKSIZE);
    int             ret     = 0;

    do
    {
        ret = chunk->connection->Read(block->data, block->len, chunk);
        if(ret > 0)
        {
            block_t *streamblock = AllocBlock((size_t)ret);
            memcpy(streamblock->data, block->data, (size_t)ret);
            chunk->blockStream.PushBack(streamblock);
            chunk->bytesDownloaded += ret;

            chunk->NotifyDownloadRateChanged();
        }
        if(chunk->stateManager.State() == REQUEST_ABORT)
            ret = 0;

    }while(ret);

    DeleteBlock(block);

    if(chunk->stateManager.State() == REQUEST_ABORT)
        chunk->stateManager.State(ABORTED);
    else
        chunk->stateManager.State(COMPLETED);

    chunk->blockStream.SetEOS(true);

    return NULL;
}
#ifndef NO_CURL
void*   AbstractChunk::DownloadInternalConnection   (void *abstractchunk)
{
    AbstractChunk *chunk  = (AbstractChunk *) abstractchunk;

    chunk->state->response = curl_easy_perform(chunk->state->curl);

    curl_easy_cleanup(chunk->state->curl);

    if(chunk->stateManager.State() == REQUEST_ABORT)
        chunk->stateManager.State(ABORTED);
    else
        chunk->stateManager.State(COMPLETED);

    chunk->blockStream.SetEOS(true);
    return NULL;
}
#endif
void    AbstractChunk::NotifyDownloadRateChanged    ()
{
    for(size_t i = 0; i < this->observers.size(); i++)
        this->observers.at(i)->OnDownloadRateChanged(this->bytesDownloaded);
}
#ifndef NO_CURL
size_t  AbstractChunk::CURLState::CurlResponseCallback         (void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    AbstractChunk *chunk = (AbstractChunk *)userp;

    if(chunk->stateManager.State() == REQUEST_ABORT)
        return 0;

    block_t *block = AllocBlock(realsize);

    memcpy(block->data, contents, realsize);
    chunk->blockStream.PushBack(block);

    //We might use the xferfunction for progress, but this is good enough. (xferfunction gets called even though no new data is received, which might distort..).
    chunk->bytesDownloaded += realsize;
    chunk->NotifyDownloadRateChanged();

    return realsize;
}
size_t  AbstractChunk::CURLState::CurlDebugCallback            (CURL * curl, curl_infotype infoType, char * data, size_t /*length*/, void *userdata)
{
    AbstractChunk   *chunk      = (AbstractChunk *)userdata;

    switch (infoType) {
        case CURLINFO_TEXT:
            break;
        case CURLINFO_HEADER_OUT:
            chunk->HandleHeaderOutCallback();
            break;
        case CURLINFO_HEADER_IN:
            chunk->HandleHeaderInCallback(std::string(data));
            break;
        case CURLINFO_DATA_IN:
            break;
        default:
            return 0;
    }
    return 0;
}
void    AbstractChunk::HandleHeaderOutCallback      ()
{
    HTTPTransaction *httpTransaction = new HTTPTransaction();

    httpTransaction->SetOriginalUrl(this->AbsoluteURI());
    httpTransaction->SetRange(this->Range());
    httpTransaction->SetType(this->GetType());
    httpTransaction->SetRequestSentTime(Time::GetCurrentUTCTimeStr());

    this->httpTransactions.push_back(httpTransaction);
}
void    AbstractChunk::HandleHeaderInCallback       (const std::string& data)
{
    HTTPTransaction *httpTransaction = this->httpTransactions.at(this->httpTransactions.size()-1);

    if (data.substr(0,4) == "HTTP")
    {
        httpTransaction->SetResponseReceivedTime(Time::GetCurrentUTCTimeStr());
        httpTransaction->SetResponseCode((uint16_t)strtoul(data.substr(9,3).c_str(), NULL, 10));
    }

    httpTransaction->AddHTTPHeaderLine(data);
}
#endif
const std::vector<ITCPConnection *>&    AbstractChunk::GetTCPConnectionList    () const
{
    return (std::vector<ITCPConnection *> &) this->tcpConnections;
}
const std::vector<IHTTPTransaction *>&  AbstractChunk::GetHTTPTransactionList  () const
{
    return (std::vector<IHTTPTransaction *> &) this->httpTransactions;
}
