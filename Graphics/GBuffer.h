#pragma once
#include <d3d11.h>
#include <wrl/client.h>

#define GBUFFER_SIZE 3

class GBuffer 
{
public:
	// Binds the rendertargets for writing
	void BindForWriting(ID3D11DepthStencilView* dsv);
	/*
		Binds the shader resources for reading
		its important to note that the shaders 
		has to be set before calling this function.
	*/
	void BindForReading();
	void Clear();
	void Release();
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context, const int& width, const int& height);
	bool Create(ID3D11Device* device, const int& width, const int& height);

private:
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv[GBUFFER_SIZE];
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv[GBUFFER_SIZE];
	Microsoft::WRL::ComPtr<ID3D11Texture2D> resource[GBUFFER_SIZE];

	ID3D11DeviceContext* context = nullptr;
};