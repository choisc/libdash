/**
 *  config.h
 *
 *  @brief      Standard includes that are used by the library
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

#ifndef CONFIG_H_
#define CONFIG_H_

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#pragma warning(push)
#pragma warning(disable: 4668)
#include <winsdkver.h>
#endif
/********************************
 * Standard includes
 ********************************/

#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <stdint.h>
#include <map>
#include <deque>
#include <iostream>
#include <cstdlib>
#include <string.h>
#ifdef _WIN32
#pragma warning(pop)
#endif

 //Disable the dominance warning since libdash is full of them.
#pragma warning( disable : 4250 ) // virtual inheritance

#endif /* CONFIG_H_ */
