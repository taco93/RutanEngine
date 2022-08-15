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
	uint16_t width = 0;
	uint16_t height = 0;

public:
	~Window();

	bool Initialize(WindowContainer* pWindowContainer, HINSTANCE hInstance, uint16_t width = 800, uint16_t height = 600, 
		std::string title = "Temp Title", std::string class_ = "Temp Class");
	bool ProcessMessages();
	HWND GetHwnd()const;
	uint16_t GetWidth()const;
	uint16_t GetHeight()const;
	void SetWidth(const uint16_t& width);
	void SetHeight(const uint16_t& height);
	void ConfineCursor() const;
	void FreeCursor() const;
};