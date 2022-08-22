#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include "Color.h"
#include <string>
#include <assimp/material.h>
#include "..\COMException.h"
#include "..\GResource.h"

enum class TextureStorageType
{
	Invalid,
	None,
	EmbeddedIndexCompressed,
	EmbeddedIndexNonCompressed,
	EmbeddedCompressed,
	EmbeddedNonCompressed,
	Disk
};

class Texture : public resource::GResource
{
public:
	Texture(ID3D11Device* device, const MyEngine::Color& color, aiTextureType type);
	Texture(ID3D11Device* device, const MyEngine::Color* colorData, UINT width, UINT height, aiTextureType type);
	Texture(ID3D11Device* device, ID3D11DeviceContext* context, const std::string& filepath, aiTextureType type);
	Texture(ID3D11Device* device, const uint8_t* pData, size_t size, aiTextureType type);
	aiTextureType GetType();
	ID3D11ShaderResourceView* GetTextureResourceView();
	ID3D11ShaderResourceView** GetTextureResourceViewAddress();

private:
	void Initialize1x1ColorTexture(ID3D11Device* device, const MyEngine::Color& colorData, aiTextureType type);
	void InitializeColorTexture(ID3D11Device* device, const MyEngine::Color* colorData, UINT width, UINT height, aiTextureType type);
	Microsoft::WRL::ComPtr<ID3D11Resource> texture = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView = nullptr;
	aiTextureType type = aiTextureType::aiTextureType_UNKNOWN;

};