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
