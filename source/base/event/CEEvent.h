#ifndef CE_EVENT_H
#define CE_EVENT_H

#include "platform\CEPlatformMacro.h"

NS_CE_BEGIN

class Event
{
public:
	enum class Type
	{
		MOUSE,
		KEYBOARD,
		GAME_CONTROLLER
	};

	Event(Type type);

	virtual ~Event();
	inline Type GetType() const { return _type; }

protected:
	Type _type;
};

NS_CE_END

#endif //CE_EVENT_H