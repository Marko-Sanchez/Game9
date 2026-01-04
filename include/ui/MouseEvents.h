#ifndef MOUSE_EVENTS_H
#define MOUSE_EVENTS_H

#include <format>
#include <string>

#include "ui/Event.h"

namespace Core
{
/*
* Classes contained in this header handle input events. They hold the state / information on the event
* to allow them, events, to be easily passed to different layers; where the layers will then have there
* own logic on how to go about handling the event.
*
* Steps: Window Callback creates and fills, the event class, passes (Window::RaiseEvent) it on to Game::RaiseEvent, which then
* is passed to each layer (Layer::OnEvent()) whom then creates a dispatch object to invoke a function member in that layer.
*/
class MouseMovedEvent: public Event
{
private:
    double m_mouseX;
    double m_mouseY;

public:
    MouseMovedEvent(double x, double y):
    m_mouseX(x), m_mouseY(y)
    {}

    inline double GetX() const noexcept
    {
        return m_mouseX;
    }

    inline double GetY() const noexcept
    {
        return m_mouseY;
    }

    std::string ToString() const noexcept override
    {
        return std::format("MouseMovedEvent: {}, {}", m_mouseX, m_mouseY);
    }

    EVENT_CLASS_TYPE(MouseMoved);
};// MouseMovedEvent

class MousedScrolledEvent: public Event
{
private:
    double m_xOffSet;
    double m_yOffSet;

public:
    MousedScrolledEvent(double xOffSet, double yOffSet):
    m_xOffSet(xOffSet), m_yOffSet(yOffSet)
    {}

    inline double GetXoffSet() const noexcept
    {
        return m_xOffSet;
    }

    inline double GetYoffSet() const noexcept
    {
        return m_yOffSet;
    }

    std::string ToString() const noexcept override
    {
        return std::format("MousedScrolledEvent: {}, {}", m_xOffSet, m_yOffSet);
    }

    EVENT_CLASS_TYPE(MouseScrolled);
};// MousedScrolledEvent

class MouseButtonEvent: public Event
{
protected:
    int m_button;

    MouseButtonEvent(int button):
    m_button(button)
    {}

public:
    inline int GetMouseButton() const noexcept
    {
        return m_button;
    }
};// MouseButtonEvent

class MouseButtonPressedEvent: public MouseButtonEvent
{
public:
    MouseButtonPressedEvent(int button):
    MouseButtonEvent(button)
    {}

    std::string ToString() const noexcept override
    {
        return std::format("MouseButtonPressedEvent: {}", m_button);
    }

    EVENT_CLASS_TYPE(MouseButtonPressed);
};// MouseButtonPressedEvent

class MouseButtonReleasedEvent: public MouseButtonEvent
{
public:
    MouseButtonReleasedEvent(int button):
    MouseButtonEvent(button)
    {}

    std::string ToString() const noexcept override
    {
        return std::format("MouseButtonReleasedEvent: {}", m_button);
    }

    EVENT_CLASS_TYPE(MouseButtonReleased);
};// MouseButtonReleasedEvent
}//namespace Core
#endif
