#include "Mouse.h"

void Mouse::OnLeftPressed(int x, int y)
{
	leftIsDown = true;
	MouseEvent me(MouseEvent::EventType::LPress, x, y);
	eventBuffer.push(me);
}

void Mouse::OnLeftReleased(int x, int y)
{
	leftIsDown = false;
	MouseEvent me(MouseEvent::EventType::LRelease, x, y);
	eventBuffer.push(me);
}

void Mouse::OnRightPressed(int x, int y)
{
	rightIsDown = true;
	MouseEvent me(MouseEvent::EventType::RPress, x, y);
	eventBuffer.push(me);
}

void Mouse::OnRightReleased(int x, int y)
{
	rightIsDown = false;
	MouseEvent me(MouseEvent::EventType::RRelease, x, y);
	eventBuffer.push(me);
}

void Mouse::OnMiddlePressed(int x, int y)
{
	mButtonIsDown = true;
	MouseEvent me(MouseEvent::EventType::MPress, x, y);
	eventBuffer.push(me);
}

void Mouse::OnMiddleReleased(int x, int y)
{
	mButtonIsDown = false;
	MouseEvent me(MouseEvent::EventType::MRelease, x, y);
	eventBuffer.push(me);
}

void Mouse::OnWheelUp(int x, int y)
{
	MouseEvent me(MouseEvent::EventType::WheelUp, x, y);
	eventBuffer.push(me);
}

void Mouse::OnWheelDown(int x, int y)
{
	MouseEvent me(MouseEvent::EventType::WheelDown, x, y);
	eventBuffer.push(me);
}

void Mouse::OnMouseMove(int x, int y)
{
	this->pos.x = x;
	this->pos.y = y;

	MouseEvent me(MouseEvent::EventType::Move, this->pos);
	eventBuffer.push(me);
}

void Mouse::OnMouseMoveRaw(int x, int y)
{
	MouseEvent me(MouseEvent::EventType::RAW_MOVE, x, y);
	eventBuffer.push(me);
}

bool Mouse::IsLeftDown() const
{
	return leftIsDown;
}

bool Mouse::IsMiddleDown() const
{
	return mButtonIsDown;
}

bool Mouse::IsRightDown() const
{
	return rightIsDown;
}

Vector2 Mouse::GetPos()
{
	return this->pos;
}

bool Mouse::EventBufferIsEmpty() const
{
	return eventBuffer.empty();
}

MouseEvent Mouse::ReadEvent()
{
	if (eventBuffer.empty())
	{
		return MouseEvent();
	}

	MouseEvent e = eventBuffer.front();
	eventBuffer.pop();

	return e;
}