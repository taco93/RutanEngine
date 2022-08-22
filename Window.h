#pragma once
#include <Windows.h>
#include <stdint.h>
#include <string>

class WindowContainer;

class Window
{
private:
	void RegisterWindowClass();
	HWND hwnd = NULL;
	HINSTANCE hInstance = NULL;
	std::string title = "";
	std::wstring titleW = L"";
	std::string windowClass = "";
	std::wstring windowClassW = L"";

public:
	~Window();

	uint16_t width = 0;
	uint16_t height = 0;
	bool Initialize(WindowContainer* pWindowContainer, HINSTANCE hInstance, uint16_t width = 800, uint16_t height = 600, 
		std::string title = "Temp Title", std::string class_ = "Temp Class");
	bool ProcessMessages();
	HWND GetHwnd()const;
	void ConfineCursor() const;
	void FreeCursor() const;
};