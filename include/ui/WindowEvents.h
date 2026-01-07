#ifndef WINDOW_EVENTS_H
#define WINDOW_EVENTS_H

#include <format>

#include "ui/Event.h"

namespace Core
{
class WindowClosedEvent: public Event
{
public:
    WindowClosedEvent() {}

    EVENT_CLASS_TYPE(WindowClose);

};// WindowClosedEvent

class WindowResizedEvent: public Event
{
private:
    unsigned int m_width;
    unsigned int m_height;

public:
    WindowResizedEvent(unsigned int width, unsigned int height):
    m_width(width), m_height(height)
    {}

    inline unsigned int GetWidth() const noexcept
    {
        return m_width;
    }

    inline unsigned int GetHeight() const noexcept
    {
        return m_height;
    }

    std::string ToString() const override
    {
        return std::format("WindowResizedEvent: {}, {}", m_width, m_height);
    }

    EVENT_CLASS_TYPE(WindowResize);

};// WindowResizedEvent
}// namespace Core
#endif
