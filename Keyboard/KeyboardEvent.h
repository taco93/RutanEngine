#pragma once
#include "../pch.h"

class KeyboardEvent
{
public:
	enum class EventType
	{
		Pressed,
		Released,
		Invalid
	};
private:

	unsigned char key;
	EventType type;

public:
	KeyboardEvent();
	KeyboardEvent(const EventType type, const unsigned char key);
	~KeyboardEvent();
	bool IsPressed() const;
	bool IsReleased()const;
	bool IsValid()const;
	unsigned char GetKeyCode() const;
};