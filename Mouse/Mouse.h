#pragma once
#include "MouseEvent.h"
#include <queue>

class Mouse
{
private:
	std::queue<MouseEvent> eventBuffer;
	bool leftIsDown = false;
	bool rightIsDown = false;
	bool mButtonIsDown = false;
	Vector2 pos;

public:
	void OnLeftPressed(int x, int y);
	void OnLeftReleased(int x, int y);
	void OnRightPressed(int x, int y);
	void OnRightReleased(int x, int y);
	void OnMiddlePressed(int x, int y);
	void OnMiddleReleased(int x, int y);
	void OnWheelUp(int x, int y);
	void OnWheelDown(int x, int y);
	void OnMouseMove(int x, int y);
	void OnMouseMoveRaw(int x, int y);

	bool IsLeftDown()const;
	bool IsMiddleDown()const;
	bool IsRightDown()const;

	Vector2 GetPos();

	bool EventBufferIsEmpty()const;
	MouseEvent ReadEvent();
};