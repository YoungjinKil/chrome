#ifndef CE_EVENT_LISTENER_H
#define CE_EVENT_LISTENER_H

#include "platform\CEPlatformMacro.h"

NS_CE_BEGIN

class EventListener
{
public:
	EventListener();

	virtual ~EventListener();
protected:
};

NS_CE_END

#endif //CE_EVENT_LISTENER_H