#pragma once
#include "RenderPass.h"
#include "GBuffer.h"
#include <vector>

class DepthPass : public RenderPass
{
public:
	// Inherited via RenderPass
	virtual void PreRender() override;
	virtual void Draw() override;
	virtual void PostRender() override;
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context, const int& width, const int& height);
	ID3D11DepthStencilView* GetDepthStencilView();
	ID3D11DepthStencilView** GetDepthStencilViewAddress();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>* GetShaderResources();
	void Resize(ID3D11Device* device, const int& width, const int height);

private:
	VertexShader vShader;
	PixelShader pShader;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterState;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTarget;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;
	GBuffer gBuffer;
	int windowWidth;
	int windowHeight;
};