#ifndef EVENT_H
#define EVENT_H

#include <functional>
#include <string>

namespace Core
{
// All current defined event classes.
enum class EventType
{
    None = 0,
    WindowClose, WindowResize,
    KeyPressed, KeyReleased,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
};

// Defines functions / contract from base class; inline, stringification.
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
    virtual EventType GetEventType() const override { return GetStaticType(); }\
    virtual const char* GetName() const override { return #type; }

// Declares functions all event types must have.
class Event
{
public:
    bool isHandled = false;

    virtual ~Event() {}
    virtual EventType GetEventType() const = 0;
    virtual const char* GetName() const = 0;
    virtual std::string ToString() const { return GetName(); }
};

// Used within Layer::Event() to handle multiple events.
class EventDispatcher
{
private:
    Event &m_event;

public:
    EventDispatcher(Event& event):
    m_event(event)
    {}

    /*
     * @param
     * func: lambda function that calls an events member function, to handle: mouse pressed event, etc.
     */
    template<typename T>
    bool Dispatch(std::function<bool (T&)> func)
    {
        if (m_event.GetEventType() == T::GetStaticType() && !m_event.isHandled)
        {
            m_event.isHandled = func(*(T*)&m_event);
            return true;
        }
        return false;
    }
};
}// namespace Core
#endif
