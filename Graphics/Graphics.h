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
#include "Light.h"
#include "DepthPass.h"
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
	Light light;
	~Graphics();

private:
	bool InitializeDirectX(HWND hwnd);
	bool InitializeRenderPasses();
	bool InitializeScene();

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

	PixelShader pShader_nolight;
	
	ConstantBuffer<CB_PS_light> cb_ps_light;

	DepthPass depthPass;
	LightingPass lightingPass;

	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> spriteFont;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;

	int windowWidth = 0;
	int windowHeight = 0;
	Timer fpsTimer;
};