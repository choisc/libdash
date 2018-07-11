/*
 * HTTPTransaction.cpp
 *****************************************************************************
 * Copyright (C) 2012, bitmovin Softwareentwicklung OG, All Rights Reserved
 *
 * Email: libdash-dev@vicky.bitmovin.net
 *
 * This source code and its use and distribution, is subject to the terms
 * and conditions of the applicable license agreement.
 *****************************************************************************/

#include "HTTPTransaction.h"

using namespace dash::metrics;

HTTPTransaction::HTTPTransaction () :
                 tcpId           (0),
                 type            (dash::metrics::Other),
                 responseCode    (0),
                 interval        (0),
                 url             (""),
                 actualUrl       (""),
                 range           (""),
                 tRequest        (""),
                 tResponse       (""),
                 httpHeader      ("")
{
}
HTTPTransaction::~HTTPTransaction()
{
    for (size_t i = 0; i < trace.size(); i++)
        delete trace.at(i);
}

uint32_t                                        HTTPTransaction::TCPId                      () const
{
    return this->tcpId;
}
void                                            HTTPTransaction::SetTCPId                   (uint32_t atcpId)
{
    this->tcpId = atcpId;
}
HTTPTransactionType                             HTTPTransaction::Type                       () const
{
    return this->type;
}
void                                            HTTPTransaction::SetType                    (HTTPTransactionType atype)
{
    this->type = atype;
}
const std::string&                              HTTPTransaction::OriginalUrl                () const
{
    return this->url;
}
void                                            HTTPTransaction::SetOriginalUrl             (const std::string& aorigUrl)
{
    this->url = aorigUrl;
}
const std::string&                              HTTPTransaction::ActualUrl                  () const
{
    return this->actualUrl;
}
void                                            HTTPTransaction::SetActualUrl               (const std::string& aactUrl)
{
    this->actualUrl = aactUrl;
}
const std::string&                              HTTPTransaction::Range                      () const
{
    return this->range;
}
void                                            HTTPTransaction::SetRange                   (const std::string& arange)
{
    this->range = arange;
}
const std::string&                              HTTPTransaction::RequestSentTime            () const
{
    return this->tRequest;
}
void                                            HTTPTransaction::SetRequestSentTime         (std::string atRequest)
{
    this->tRequest = atRequest;
}
const std::string&                              HTTPTransaction::ResponseReceivedTime       () const
{
    return this->tResponse;
}
void                                            HTTPTransaction::SetResponseReceivedTime    (std::string atResponse)
{
    this->tResponse = atResponse;
}
uint16_t                                        HTTPTransaction::ResponseCode               () const
{
    return this->responseCode;
}
void                                            HTTPTransaction::SetResponseCode            (uint16_t arespCode)
{
    this->responseCode = arespCode;
}
uint64_t                                        HTTPTransaction::Interval                   () const
{
    return this->interval;
}
void                                            HTTPTransaction::SetInterval                (uint64_t ainterval)
{
    this->interval = ainterval;
}
const std::vector<IThroughputMeasurement *>&    HTTPTransaction::ThroughputTrace            () const
{
    return (std::vector<IThroughputMeasurement *> &) this->trace;
}
void                                            HTTPTransaction::AddThroughputMeasurement   (ThroughputMeasurement *athrouputEntry)
{
    this->trace.push_back(athrouputEntry);
}
const std::string&                              HTTPTransaction::HTTPHeader                 () const
{
    return this->httpHeader;
}
void                                            HTTPTransaction::AddHTTPHeaderLine          (std::string aheaderLine)
{
    this->httpHeader.append(aheaderLine);
}
