/**
 *  libdash.h
 *
 *  @brief      The main interface to the libary that should be used to create a IDASHManager
 *  @details    ...
 *
 *  @author     bitmovin Softwareentwicklung OG \n
 *              Email: libdash-dev@vicky.bitmovin.net
 *  @version    2.1
 *  @date       2013
 *  @copyright  bitmovin Softwareentwicklung OG, All Rights Reserved \n\n
 *              This source code and its use and distribution, is subject to the terms
 *              and conditions of the applicable license agreement.
 */

#ifndef LIBDASH_H_
#define LIBDASH_H_

#include "config.h"

#if defined _WIN32 || defined _WIN64
#ifndef STATIC_DASH
    #define DASH_EXPORT __declspec(dllexport)
#else
    #define DASH_EXPORT
#endif
#else
#define DASH_EXPORT __attribute__ ((visibility ("default")))
#define __cdecl
#endif

#include "IDASHManager.h"

DASH_EXPORT void __cdecl InitializeLibDash();
DASH_EXPORT dash::IDASHManager* __cdecl CreateDashManager();
DASH_EXPORT void __cdecl DeinitializeLibDash();

#endif /* LIBDASH_H_ */
