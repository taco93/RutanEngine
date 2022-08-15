#pragma once
#include "..\\Engine.h"

class App
{
public:
	App();
	App(const App& rhs) = delete;
	App& operator=(const App& rhs) = delete;
	bool CreateApp(HINSTANCE hInstance, uint16_t width, uint16_t height, std::string title);
	void Close();

	static App& Get() { return *s_Instance; }

private:
	static App* s_Instance;
};