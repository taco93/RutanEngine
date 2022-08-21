#pragma once
#include "RenderPass.h"
#include <vector>
#include "GBuffer.h"

class LightingPass : public RenderPass
{
public:
	// Inherited via RenderPass
	virtual void PreRender() override;
	virtual void Draw() override;
	virtual void PostRender() override;

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context, IDXGISwapChain* swapchain, GBuffer* gbuffer);

	bool Resize(ID3D11Device* device, ID3D11Texture2D* backbuffer, const int& width, const int& height);
	void ReleaseRenderTarget();

private:
	VertexShader vShader;
	PixelShader pShader;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv;
	GBuffer* gBuffer = nullptr;

	bool SetupBackbuffer(ID3D11Device* device);
};