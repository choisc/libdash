/*
 * AbstractChunk.h
 *****************************************************************************
 * Copyright (C) 2012, bitmovin Softwareentwicklung OG, All Rights Reserved
 *
 * Email: libdash-dev@vicky.bitmovin.net
 *
 * This source code and its use and distribution, is subject to the terms
 * and conditions of the applicable license agreement.
 *****************************************************************************/

#ifndef ABSTRACTCHUNK_H_
#define ABSTRACTCHUNK_H_

#include "config.h"

#include "IDownloadableChunk.h"
#include "DownloadStateManager.h"
#include "../helpers/SyncedBlockStream.h"
#include "../metrics/HTTPTransaction.h"
#include "../metrics/TCPConnection.h"
#include "../metrics/ThroughputMeasurement.h"
#include "../helpers/Time.h"

namespace dash
{
    namespace network
    {
        class AbstractChunk : public virtual IDownloadableChunk
        {
            public:
                AbstractChunk          ();
                virtual ~AbstractChunk ();

                /*
                 * Pure virtual IChunk Interface
                 */
                virtual std::string&    AbsoluteURI     ()  = 0;
                virtual std::string&    Host            ()  = 0;
                virtual size_t          Port            ()  = 0;
                virtual std::string&    Path            ()  = 0;
                virtual std::string&    Range           ()  = 0;
                virtual size_t          StartByte       ()  = 0;
                virtual size_t          EndByte         ()  = 0;
                virtual bool            HasByteRange    ()  = 0;
                virtual dash::metrics::HTTPTransactionType  GetType() = 0;
                /*
                 * IDownloadableChunk Interface
                 */
                virtual bool    StartDownload           (IConnection *connection);
                virtual bool    StartDownload           ();
                virtual void    AbortDownload           ();
                virtual int     Read                    (uint8_t *data, size_t len);
                virtual int     Peek                    (uint8_t *data, size_t len);
                virtual int     Peek                    (uint8_t *data, size_t len, size_t offset);
                virtual void    AttachDownloadObserver  (IDownloadObserver *observer);
                virtual void    DetachDownloadObserver  (IDownloadObserver *observer);
                /*
                 * Observer Notification
                 */
                void NotifyDownloadRateChanged ();
                /*
                 * IDASHMetrics
                 */
                const std::vector<dash::metrics::ITCPConnection *>&     GetTCPConnectionList    () const;
                const std::vector<dash::metrics::IHTTPTransaction *>&   GetHTTPTransactionList  () const;

            private:
                std::vector<IDownloadObserver *>    observers;
                THREAD_HANDLE                       dlThread;
                IConnection*                        connection;
                helpers::SyncedBlockStream          blockStream;
                uint64_t                            bytesDownloaded;
                DownloadStateManager                stateManager;

                std::vector<dash::metrics::TCPConnection *>     tcpConnections;
                std::vector<dash::metrics::HTTPTransaction *>   httpTransactions;

                static uint32_t BLOCKSIZE;

                static void*    DownloadExternalConnection(void *chunk);
#ifndef NO_CURL
                static void*    DownloadInternalConnection(void *chunk);
                void            HandleHeaderOutCallback     ();
                void            HandleHeaderInCallback      (const std::string& data);
                class CURLState;
                CURLState* state;
                friend class CURLState;
#endif
        };
    }
}

#endif /* ABSTRACTCHUNK_H_ */
