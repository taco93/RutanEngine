#pragma once
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

class MouseEvent
{
public:
	enum class EventType
	{
		LPress,
		LRelease,
		RPress,
		RRelease,
		MPress,
		MRelease,
		WheelUp,
		WheelDown,
		Move,
		RAW_MOVE,
		Invalid
	};
private:
	EventType type;
	Vector2 pos;

public:
	MouseEvent();
	MouseEvent(const EventType type, const int x, const int y);
	MouseEvent(const EventType type, const Vector2& pos);
	bool IsValid()const;
	EventType GetType()const;
	Vector2 GetPos()const;
};