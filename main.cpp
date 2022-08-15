#include "Engine.h"
#include "Application/App.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow){
	HRESULT hr = CoInitialize(NULL);

	if (FAILED(hr)) {
		Logger::LogError(hr, "Failed to call CoInitialize.");
		return -1;
	}

	Engine engine;

	if (engine.Initialize(hInstance, 1280, 720, "Title", "MyWindowClass")) {
		while (engine.ProcessMessages() && engine.IsRunning()) {
			engine.Update();
			engine.Render();
		}
	}
	return 0;
}