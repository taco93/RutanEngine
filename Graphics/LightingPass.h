#pragma once
#include "RenderPass.h"
#include <vector>
#include "GBuffer.h"
#include "ConstantBuffer.h"

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
	void UpdateCameraParams(Vector3 eyePos);

private:
	VertexShader vShader;
	PixelShader pShader;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv;
	GBuffer* gBuffer = nullptr;
	ConstantBuffer<CB_PS_cameraparams> cBuffer;
};