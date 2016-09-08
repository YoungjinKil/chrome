#ifndef CE_PLATFORM_MACRO_H
#define CE_PLATFORM_MACRO_H

#include "platform\CEPlatformConfig.h"

// Generic macros
#ifdef __cplusplus
	#define NS_CE_BEGIN     namespace chrome {
	#define NS_CE_END       }
	#define USING_NS_CE     using namespace chrome
	#define NS_CE			::chrome
#else
	#define NS_CE_BEGIN
	#define NS_CC_END
	#define USING_NS_CE
	#define NS_CE
#endif // __cplusplus

// Define NULL pointer
#ifndef NULL
#ifdef __cplusplus
#define NULL	0
#else
#define NULL	((void*)0)
#endif
#endif // NULL

// Define assert function
#include <assert.h>

#if CE_DISABLE_ASSERT > 0
#else
#define CE_ASSERT(cond)	assert(cond)
#endif
#define CE_UNUSED_PARAM(unusedparam) (void)unusedparam

// TODO(youngjin) : Add some condition for the difference of the platform if that exist.
#define INTERNAL static
#define LOCAL_PERSIST static
#define GLOBAL static

// TODO(youngjin) : Keep looking for the other macro in cocos2d.(CCPlatformDefine, CCPlatformMacros)



#endif // CE_PLATFORM_MACRO_H