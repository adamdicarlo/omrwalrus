#ifndef __KT_EVENT_INCLUDED__
#define __KT_EVENT_INCLUDED__

namespace kt {

enum EventType {
	EVENT_INPUT
};

class IEvent {
public:
	virtual ~IEvent() = 0;
	virtual EventType GetEventType() const = 0;
};


class EventManager {
public:


private:

};


} // namespace kt

#endif // __KT_EVENT_INCLUDED__
