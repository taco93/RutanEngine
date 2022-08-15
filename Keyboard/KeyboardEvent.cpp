#include "KeyboardEvent.h"

KeyboardEvent::KeyboardEvent()
    :type(EventType::Invalid), key(0u)
{
}

KeyboardEvent::KeyboardEvent(const EventType type, const unsigned char key)
    :type(type), key(key)
{
}

KeyboardEvent::~KeyboardEvent()
{
}

bool KeyboardEvent::IsPressed() const
{
    return type == EventType::Pressed;
}

bool KeyboardEvent::IsReleased() const
{
    return type == EventType::Released;
}

bool KeyboardEvent::IsValid() const
{
    return type != EventType::Invalid;
}

unsigned char KeyboardEvent::GetKeyCode() const
{
    return key;
}
