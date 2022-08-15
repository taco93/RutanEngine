#pragma once
#include "RenderPass.h"
#include <vector>
class LightingPass : public RenderPass
{
public:
	// Inherited via RenderPass
	virtual void PreRender() override;
	virtual void Draw() override;
	virtual void PostRender() override;

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context, ID3D11RenderTargetView* renderTarget, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>* shaderResources);

private:
	VertexShader vShader;
	PixelShader pShader;
	ID3D11RenderTargetView* renderTarget = nullptr;
	ID3D11ShaderResourceView* shaderResources[3] = { nullptr };
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
};