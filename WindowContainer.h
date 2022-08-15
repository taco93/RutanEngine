#pragma once
#include "Window.h"
#include "Keyboard/Keyboard.h"
#include "Mouse/Mouse.h"
#include "Logger.h"
#include "Graphics/Graphics.h"

class WindowContainer
{
private:

protected:
	Window render_window;
	Keyboard keyboard;
	Mouse mouse;
	Graphics gfx;

public:
	WindowContainer();
	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};