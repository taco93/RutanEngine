#include "LightingPass.h"

void LightingPass::PreRender()
{
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	this->context->ClearRenderTargetView(this->renderTarget, clearColor);
	this->context->OMSetRenderTargets(1, &this->renderTarget, 0);

	this->context->VSSetShader(this->vShader.GetShader(), 0, 0);
	this->context->PSSetShader(this->pShader.GetShader(), 0, 0);

	this->context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	this->context->PSSetShaderResources(0, 3, this->shaderResources);
	this->context->OMSetDepthStencilState(this->depthStencilState.Get(), 0);
}

void LightingPass::Draw()
{
	this->context->Draw(4, 0);
}

void LightingPass::PostRender()
{
	this->context->PSSetShaderResources(0, 0, 0);
}

bool LightingPass::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, ID3D11RenderTargetView* renderTarget, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>* shaderResources)
{
    this->context = context;
	this->renderTarget = renderTarget;

	for (int i = 0; i < 3; i++) {
		this->shaderResources[i] = shaderResources[i].Get();
	}

 	if (!this->vShader.Initialize(device, GetShaderFolder() + L"vertexshader.cso", nullptr, 0))
		return false;
	if (!this->pShader.Initialize(device, GetShaderFolder() + L"pixelshader.cso"))
		return false;

	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc = {};
	depthStencilStateDesc.DepthEnable = false;

	try 
	{
		HRESULT hr = device->CreateDepthStencilState(&depthStencilStateDesc, this->depthStencilState.GetAddressOf());

		COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil state.");
	}
	catch (COMException& exception)
	{
		Logger::Log(exception);
		return false;
	}

    return true;
}
