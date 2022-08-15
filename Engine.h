#pragma once
#include "WindowContainer.h"
#include "Timer.h"

class Engine : WindowContainer
{
public:
	bool Initialize(HINSTANCE hInstance, uint16_t width = 800, uint16_t height = 600, std::string title = "Temp Title", std::string class_ = "Temp Class");
	bool ProcessMessages();
	void Update();
	void Render();
	void Close();
	bool IsRunning()const;

private:
	Timer timer;
	bool isRunning;
};