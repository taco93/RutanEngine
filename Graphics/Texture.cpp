#include "Texture.h"
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#pragma comment(lib, "DirectXTex.lib")
#include <DirectXTex.h>
#include "..\\Logger.h"
#include "..\\ResourceManager.h"

Texture::Texture(ID3D11Device* device, const MyEngine::Color& color, aiTextureType type)
{
	this->Initialize1x1ColorTexture(device, color, type);
}

Texture::Texture(ID3D11Device* device, const MyEngine::Color* colorData, UINT width, UINT height, aiTextureType type)
{
	this->InitializeColorTexture(device, colorData, width, height, type);
}

Texture::Texture(ID3D11Device* device, ID3D11DeviceContext* context, const std::string& filepath, aiTextureType type)
{
	this->type = type;

	if (StringHelper::GetFileExtension(filepath) == "dds") {

		HRESULT hr = DirectX::CreateDDSTextureFromFile(device, context, StringHelper::StringToWide(filepath).c_str(), texture.GetAddressOf(), this->textureView.GetAddressOf());

		if (FAILED(hr)) {
			this->Initialize1x1ColorTexture(device, MyEngine::Colors::UnloadedTextureColor, type);
		}
	}
	else if (StringHelper::GetFileExtension(filepath) == "tga")
	{
		DirectX::ScratchImage image;

		try {
			HRESULT hr = DirectX::LoadFromTGAFile(StringHelper::StringToWide(filepath).c_str(), nullptr, image);

			COM_ERROR_IF_FAILED(hr, "Failed to Load TGA.");

			DirectX::ScratchImage mipChain;
			hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_DEFAULT, 0, mipChain);
			COM_ERROR_IF_FAILED(hr, "Failed to Create TGA Mip Maps.");

			hr = DirectX::CreateTexture(device, mipChain.GetImages(), mipChain.GetImageCount(), mipChain.GetMetadata(), this->texture.GetAddressOf());

			COM_ERROR_IF_FAILED(hr, "Failed to Create TGA Texture.");

			hr = DirectX::CreateShaderResourceView(device, mipChain.GetImages(), mipChain.GetImageCount(), mipChain.GetMetadata(), this->textureView.GetAddressOf());

			COM_ERROR_IF_FAILED(hr, "Failed to Create TGA Shader Resource.");
		}
		catch (COMException& exception) {
			Logger::Log(exception);

			this->Initialize1x1ColorTexture(device, MyEngine::Colors::UnloadedTextureColor, type);
		}
	}
	else 
	{
		HRESULT hr = DirectX::CreateWICTextureFromFile(device, context, StringHelper::StringToWide(filepath).c_str(), texture.GetAddressOf(), this->textureView.GetAddressOf());

		if (FAILED(hr)) {
			this->Initialize1x1ColorTexture(device, MyEngine::Colors::UnloadedTextureColor, type);
		}
	}

	ResourceManager::AddResource(StringHelper::GetFileName(filepath), *this);

	return;
}

Texture::Texture(ID3D11Device* device, const uint8_t* pData, size_t size, aiTextureType type)
{
	this->type = type;
	HRESULT hr = DirectX::CreateWICTextureFromMemory(device, pData, size, this->texture.GetAddressOf(), this->textureView.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create Texture from memory.");
}

aiTextureType Texture::GetType()
{
	return this->type;
}

ID3D11ShaderResourceView* Texture::GetTextureResourceView()
{
	return this->textureView.Get();
}

ID3D11ShaderResourceView** Texture::GetTextureResourceViewAddress()
{
	return this->textureView.GetAddressOf();
}

void Texture::Initialize1x1ColorTexture(ID3D11Device* device, const MyEngine::Color& colorData, aiTextureType type)
{
	InitializeColorTexture(device, &colorData, 1, 1, type);
}

void Texture::InitializeColorTexture(ID3D11Device* device, const MyEngine::Color* colorData, UINT width, UINT height, aiTextureType type)
{
	this->type = type;
	CD3D11_TEXTURE2D_DESC textureDesc(DXGI_FORMAT_R8G8B8A8_UNORM, width, height);
	ID3D11Texture2D* p2DTexture = nullptr;
	D3D11_SUBRESOURCE_DATA initialData = {};
	initialData.pSysMem = colorData;
	initialData.SysMemPitch = width * sizeof(MyEngine::Color);

	HRESULT hr = device->CreateTexture2D(&textureDesc, &initialData, &p2DTexture);
	COM_ERROR_IF_FAILED(hr, "Failed to initialize texture from color data.");
	this->texture = static_cast<ID3D11Texture2D*>(p2DTexture);
	CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, textureDesc.Format);

	hr = device->CreateShaderResourceView(texture.Get(), &srvDesc, textureView.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create shader resource view from texture generated from color.");
}