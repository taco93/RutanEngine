#pragma once
#include "AdapterReader.h"
#include "Model.h"
#include "Shaders.h"
#include <WICTextureLoader.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include "Camera.h"
#include "..\\Timer.h"
#include "ImGui\\imgui.h"
#include "ImGui\\imgui_impl_win32.h"
#include "ImGui\\imgui_impl_dx11.h"
#include "RenderableGameObject.h"
#include "GeometryPass.h"
#include "LightingPass.h"

class Graphics
{
public:
	bool Initialize(HWND hwnd, int width, int height);
	void Render();
	void OnResize(UINT width, UINT height);
	bool IsFullScreen()const;

	Camera camera;
	RenderableGameObject gameObject;
	~Graphics();

private:
	bool InitializeDirectX(HWND hwnd);
	bool InitializeRenderPasses();
	bool InitializeScene();
	void CreateViewport();

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;

	GeometryPass geometryPass;
	LightingPass lightingPass;

	int windowWidth = 0;
	int windowHeight = 0;
	Timer fpsTimer;
};