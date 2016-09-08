#ifndef CE_PLATFORM_CONFIG_H
#define CE_PLATFORM_CONFIG_H

//////////////////////////////////////////////////////////////////////////
// pre configure
//////////////////////////////////////////////////////////////////////////

// define supported platform macro.
#define CE_PLATFORM_UNKNOWN         0
#define CE_PLATFORM_WIN32           1
#define CE_PLATFORM_TEST			2

// Determine target platform by compile environment macro.
#define CE_TARGET_PLATFORM          CE_PLATFORM_UNKNOWN

#if defined(_WIN32) && defined(_WINDOWS)
    #undef CE_TARGET_PLATFORM
    #define CE_TARGET_PLATFORM  CE_PLATFORM_WIN32
#endif


//////////////////////////////////////////////////////////////////////////
// pre configure
//////////////////////////////////////////////////////////////////////////
#if ! CE_TARGET_PLATFORM
    #error "Cannot recognize the target platform;"
#endif

// TODO(youngjin) : I'm not sure how to use it practically, so comment it.
/*#if CE_TARGET_PLATFORM == CE_TARGET_WIN32
#ifndef __MINGW32__
#pragma warning (disable:4127)
#endif
#endif*/

#endif