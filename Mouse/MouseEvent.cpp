#include "MouseEvent.h"

MouseEvent::MouseEvent()
	:type(EventType::Invalid), pos(0, 0)
{

}

MouseEvent::MouseEvent(const EventType type, const int x, const int y)
	:type(type), pos((float)x, (float)y)
{
}

MouseEvent::MouseEvent(const EventType type, const Vector2& pos)
	:type(type), pos(pos)
{
}

bool MouseEvent::IsValid() const
{
	return type != EventType::Invalid;
}

MouseEvent::EventType MouseEvent::GetType() const
{
	return type;
}

Vector2 MouseEvent::GetPos() const
{
	return this->pos;
}
