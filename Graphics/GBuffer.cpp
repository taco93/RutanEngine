#include "GBuffer.h"
#include "../Logger.h"

void GBuffer::BindForWriting(ID3D11DepthStencilView* dsv)
{
	this->context->OMSetRenderTargets(GBUFFER_SIZE, this->rtv->GetAddressOf(), dsv);
}

void GBuffer::BindForReading()
{
	this->context->PSSetShaderResources(0, GBUFFER_SIZE, this->srv->GetAddressOf());
}

void GBuffer::Clear()
{
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	
	for (int i = 0; i < GBUFFER_SIZE; i++) {
		this->context->ClearRenderTargetView(this->rtv[i].Get(), clearColor);
	}
}

void GBuffer::Release()
{
	for (int i = 0; i < GBUFFER_SIZE; i++)
	{
		this->resource[i]->Release();
		this->rtv[i]->Release();
		this->srv[i]->Release();
	}
}

bool GBuffer::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, const int& width, const int& height)
{
	this->context = context;

	if (!this->Create(device, width, height))
		return false;

	return true;
}

bool GBuffer::Create(ID3D11Device* device, const int& width, const int& height)
{
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	srvDesc.Texture2D.MipLevels = 1;

	try {
		HRESULT hr = {};

		for (int i = 0; i < GBUFFER_SIZE; i++)
		{
			hr = device->CreateTexture2D(&textureDesc, nullptr, this->resource[i].GetAddressOf());
			COM_ERROR_IF_FAILED(hr, "Failed to create Texture2D for RenderTarget");
			hr = device->CreateRenderTargetView(this->resource[i].Get(), &rtvDesc, this->rtv[i].GetAddressOf());
			COM_ERROR_IF_FAILED(hr, "Failed to create RenderTargetView for RenderTarget");
			hr = device->CreateShaderResourceView(this->resource[i].Get(), &srvDesc, this->srv[i].GetAddressOf());
			COM_ERROR_IF_FAILED(hr, "Failed to create ShaderResourceView for RenderTarget");
		}
	}
	catch (COMException& exception)
	{
		Logger::Log(exception);
		return false;
	}

	return true;
}
