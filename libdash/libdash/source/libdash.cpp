/*
 * libdash.cpp
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

#include "../include/libdash.h"
#include "manager/DASHManager.h"

using namespace dash;
#ifdef USE_LIBXML

#if _MSC_VER && _DEBUG
//pipe libxml errors to output debugstring on windows
//on other systems they will be sent to stderr (and /dev/null most likely)
#include <stdarg.h>
extern "C"
{
    void __cdecl handler(void * ctx, const char * format, ...)
    {
        va_list copy;
        va_start(copy, format);
        char buf2[512] = { 0 };
        char buf[sizeof(buf2)] = { 0 };
        ::vsnprintf(buf, sizeof(buf), format, copy);
        _snprintf(buf2, sizeof(buf2)-1, "[LibXML %p:%x] %s", ctx, GetCurrentThreadId(), buf);
        //add newline if missing..
        size_t pos = strlen(buf2);
        if (pos >= sizeof(buf2)) pos -= 2;
        if (buf2[pos - 1] != '\n')
        {
            buf2[pos - 1] = '\n';
            buf2[pos] = 0;
        }
        OutputDebugStringA(buf2);
        va_end(copy);
    }
}
static xmlGenericErrorFunc genericFunc;
#endif
static class cleanup
{
public:
    cleanup()
    {        
#if _MSC_VER && _DEBUG
        //pipe libxml errors to output debugstring on windows
        //on other systems they will be sent to stderr (and /dev/null most likely)
        genericFunc = handler;
        initGenericErrorDefaultFunc(&genericFunc);
#endif
        xmlInitParser();
#ifndef NO_CURL
        curl_global_init(CURL_GLOBAL_ALL);
#endif
    }
    ~cleanup()
    {
#ifndef NO_CURL
        curl_global_cleanup();
#endif
        xmlCleanupParser();
    }
} gXmlLibDummy;
#endif

DASH_EXPORT void __cdecl InitializeLibDash()
{
    //NOTE: use a global object to cleanup/init curl/libxml. since libxml atleast cant handle reinit.
    //curl_global_init(CURL_GLOBAL_ALL);
    //xmlInitParser();
}

DASH_EXPORT void __cdecl DeinitializeLibDash()
{
    //NOTE: use a global object to cleanup/init curl/libxml. since libxml atleast cant handle reinit.
    //xmlCleanupParser();
    //curl_global_cleanup();
}

DASH_EXPORT IDASHManager* __cdecl CreateDashManager()
{
    return new DASHManager();
}
