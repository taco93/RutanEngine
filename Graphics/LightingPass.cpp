#include "LightingPass.h"

void LightingPass::PreRender()
{
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	this->context->ClearRenderTargetView(this->rtv.Get(), clearColor);
	this->context->OMSetRenderTargets(1, this->rtv.GetAddressOf(), 0);

	this->context->VSSetShader(this->vShader.GetShader(), 0, 0);
	this->context->PSSetShader(this->pShader.GetShader(), 0, 0);

	this->context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	this->gBuffer->BindRead();
	this->context->OMSetDepthStencilState(0, 0);
}

void LightingPass::Draw()
{
	this->context->Draw(4, 0);
}

void LightingPass::PostRender()
{
	this->gBuffer->UnbindRead();
}

bool LightingPass::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, IDXGISwapChain* swapchain, GBuffer* gBuffer)
{
	this->context = context;
	this->gBuffer = gBuffer;

	try {
		ID3D11Texture2D* pBuffer;
		HRESULT hr = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);

		COM_ERROR_IF_FAILED(hr, "Failed to get backbuffer");

		hr = device->CreateRenderTargetView(pBuffer, NULL, rtv.GetAddressOf());

		COM_ERROR_IF_FAILED(hr, "Failed to create RenderTargetView.");

		pBuffer->Release();
	}
	catch (COMException& exception)
	{
		Logger::Log(exception);
		return false;
	}

	if (!this->vShader.Initialize(device, GetShaderFolder() + L"vertexshader.cso", nullptr, 0))
		return false;
	if (!this->pShader.Initialize(device, GetShaderFolder() + L"pixelshader.cso"))
		return false;

	return true;
}

bool LightingPass::Resize(ID3D11Device* device, ID3D11Texture2D* backbuffer, const int& width, const int& height)
{	
	try {
		HRESULT hr = device->CreateRenderTargetView(backbuffer, NULL, this->rtv.GetAddressOf());

		COM_ERROR_IF_FAILED(hr, "Failed to create Render target view.");

		backbuffer->Release();
	}
	catch (COMException& exception)
	{
		Logger::Log(exception);
		return false;
	}

	return true;
}

void LightingPass::ReleaseRenderTarget()
{
	this->rtv->Release();
}
