#pragma once
#include <d3d11.h>
#include <wrl/client.h>

struct GBuffer
{
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv[3];
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv[3];
	Microsoft::WRL::ComPtr<ID3D11Texture2D> resource[3];
};