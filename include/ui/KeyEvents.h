#ifndef KEY_EVENTS_H
#define KEY_EVENTS_H

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
class KeyEvent: public Event
{
public:
    int m_keyCode;

protected:
    KeyEvent(int keycode):
    m_keyCode(keycode)
    {}

    inline int GetKeyCode() const
    {
        return m_keyCode;
    }
};// KeyEvent

class KeyPressedEvent: public KeyEvent
{
private:
    bool m_isRepeat;

public:
    KeyPressedEvent(int keycode, bool isRepeat):
    KeyEvent(keycode), m_isRepeat(isRepeat)
    {}

    inline bool IsRepeat() const noexcept
    {
        return m_isRepeat;
    }

    std::string ToString() const noexcept override
    {
        return std::format("KeyPressedEvent: {} (repeat={})", m_keyCode, m_isRepeat);
    }

    EVENT_CLASS_TYPE(KeyPressed);
};// KeyPressedEvent

class KeyReleasedEvent: public KeyEvent
{
public:
    KeyReleasedEvent(int keycode):
    KeyEvent(keycode)
    {}

    std::string ToString() const noexcept override
    {
        return std::format("KeyReleasedEvent: {}", m_keyCode);
    }

    EVENT_CLASS_TYPE(KeyReleased);
};// KeyReleasedEvent
}// namespace Core
#endif
