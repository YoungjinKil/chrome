#ifndef CE_APPLICATION_H
#define CE_APPLICATION_H

#include "platform/CEPlatformConfig.h"

#if (CE_TARGET_PLATFORM != CE_PLATFORM_WIN32)
	#include "platform/win32/CEApplication-win32.h"
#else
	#include "platform/test/CEApplication-test.h"
#endif

#endif //CE_APPLICATION_H