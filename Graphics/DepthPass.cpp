#include "DepthPass.h"

void DepthPass::PreRender()
{
	this->context->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Set new pipeline
	this->context->IASetInputLayout(vShader.GetInputLayout());
	this->context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->context->RSSetState(this->rasterState.Get());
	this->context->OMSetDepthStencilState(this->depthStencilState.Get(), 0);
	this->context->PSSetSamplers(0, 1, this->sampler.GetAddressOf());

	this->context->VSSetShader(vShader.GetShader(), NULL, 0);
	this->context->PSSetShader(pShader.GetShader(), NULL, 0);

	ID3D11RenderTargetView* rtvs[3];
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < 3; i++) {
		rtvs[i] = gBuffer.rtv[i].Get();
		this->context->ClearRenderTargetView(rtvs[i], clearColor);
	}

	this->context->OMSetRenderTargets(3, rtvs, depthStencilView.Get());
}

void DepthPass::Draw()
{
}

void DepthPass::PostRender()
{
	this->context->IASetInputLayout(0);
}

bool DepthPass::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, const int& width, const int& height)
{
	this->context = context;
	this->windowWidth = width;
	this->windowHeight = height;
	
	HRESULT hr = {};

	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT numElements = ARRAYSIZE(layout);

	std::wstring shaderFolder = GetShaderFolder();

	if (!this->vShader.Initialize(device, shaderFolder + L"vs_depth.cso", layout, numElements))
		return false;
	
	if (!this->pShader.Initialize(device, shaderFolder + L"ps_depth.cso"))
		return false;

	try {
		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width = this->windowWidth;
		depthStencilDesc.Height = this->windowHeight;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		hr = device->CreateTexture2D(&depthStencilDesc, nullptr, this->depthBuffer.GetAddressOf());

		COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil buffer.");

		hr = device->CreateDepthStencilView(this->depthBuffer.Get(), NULL, this->depthStencilView.GetAddressOf());

		COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil view.");

		D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
		ZeroMemory(&depthStencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

		depthStencilStateDesc.DepthEnable = true;
		depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		hr = device->CreateDepthStencilState(&depthStencilStateDesc, this->depthStencilState.GetAddressOf());

		COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil state.");

		D3D11_RASTERIZER_DESC rasterizerDesc = {};

		rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;

		hr = device->CreateRasterizerState(&rasterizerDesc, this->rasterState.GetAddressOf());

		COM_ERROR_IF_FAILED(hr, "Failed to create Rasterizer State.");

		D3D11_SAMPLER_DESC samplerDesc = {};

		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		samplerDesc.MipLODBias = 0.0f;

		hr = device->CreateSamplerState(&samplerDesc, this->sampler.GetAddressOf());

		COM_ERROR_IF_FAILED(hr, "Failed to create sampler state.");


		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = this->windowWidth;
		textureDesc.Height = this->windowHeight;
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

		for (int i = 0; i < 3; i++)
		{
			hr = device->CreateTexture2D(&textureDesc, nullptr, this->gBuffer.resource[i].GetAddressOf());
			COM_ERROR_IF_FAILED(hr, "Failed to create Texture2D for RenderTarget");
			hr = device->CreateRenderTargetView(this->gBuffer.resource[i].Get(), &rtvDesc, this->gBuffer.rtv[i].GetAddressOf());
			COM_ERROR_IF_FAILED(hr, "Failed to create RenderTargetView for RenderTarget");
			hr = device->CreateShaderResourceView(this->gBuffer.resource[i].Get(), &srvDesc, this->gBuffer.srv[i].GetAddressOf());
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

ID3D11DepthStencilView** DepthPass::GetDepthStencilViewAddress()
{
	return this->depthStencilView.GetAddressOf();
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>* DepthPass::GetShaderResources()
{
	return this->gBuffer.srv;
}

void DepthPass::Resize(ID3D11Device* device, const int& width, const int height)
{
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = this->windowWidth;
	depthStencilDesc.Height = this->windowHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	this->depthBuffer->Release();

	HRESULT hr = device->CreateTexture2D(&depthStencilDesc, nullptr, this->depthBuffer.GetAddressOf());

	COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil buffer.");

	this->depthStencilView->Release();

	hr = device->CreateDepthStencilView(this->depthBuffer.Get(), NULL, this->depthStencilView.GetAddressOf());

	COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil view.");

	this->context->OMSetRenderTargets(3, &gBuffer.rtv[0], depthStencilView.Get());
}

ID3D11DepthStencilView* DepthPass::GetDepthStencilView()
{
	return this->depthStencilView.Get();
}
