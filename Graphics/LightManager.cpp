#include "LightManager.h"

void LightManager::AddPointLight(const Vector3& pos, const Vector3& color)
{
	Light light;
	light.position = pos;
	light.color = color;
	light.ambientColor = { 1.0f, 1.0f, 1.0f };
	light.ambientStrength = 1.0f;
	light.attenuationA = 0.0f;
	light.attenuationB = 0.027f;
	light.attenuationC = 0.0f;
	light.type = static_cast<uint32_t>(LightType::POINT_LIGHT);
	LightManager::Get().lights.push_back(light);

	LightManager::Get().UpdateBuffers();
}

void LightManager::AddSpotLight(const Vector3& pos, const Vector3& color, const Vector3& direction, float spotAngle)
{
	Light light;
	light.position = pos;
	light.color = color;
	light.lightStrength = 1.0f;
	light.direction = direction;
	light.spotAngle = spotAngle;
	light.ambientColor = { 1.0f, 1.0f, 1.0f };
	light.ambientStrength = 1.0f;
	light.attenuationA = 1.0f;
	light.attenuationB = 0.2f;
	light.attenuationC = 0.1f;
	light.type = static_cast<uint32_t>(LightType::SPOT_LIGHT);
	LightManager::Get().lights.push_back(light);

	LightManager::Get().UpdateBuffers();
}

Light* LightManager::GetLight(UINT index)
{
	if (index >= LightManager::Get().lights.size()) return nullptr;

	return &LightManager::Get().lights[index];
}

bool LightManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
	LightManager::Get().context = context;
	LightManager::Get().device = device;

	return true;
}

bool LightManager::UpdateBuffers()
{
	int size = (int)LightManager::Get().lights.size();
	HRESULT hr;

	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(Light) * size;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.StructureByteStride = sizeof(Light);
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = LightManager::Get().lights.data();

	try
	{
		if (LightManager::lightBuffer.Get())
		{
			LightManager::lightBuffer->Release();
		}

		hr = LightManager::device->CreateBuffer(&desc, &data, LightManager::Get().lightBuffer.GetAddressOf());

		COM_ERROR_IF_FAILED(hr, "Failed to create lightBuffer.");

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvDesc.BufferEx.FirstElement = 0;
		srvDesc.BufferEx.Flags = 0;
		srvDesc.BufferEx.NumElements = (UINT)LightManager::Get().lights.size();

		if (LightManager::lightShaderResource.Get())
		{
			LightManager::lightShaderResource->Release();
		}

		hr = device->CreateShaderResourceView(LightManager::Get().lightBuffer.Get(), &srvDesc, LightManager::Get().lightShaderResource.GetAddressOf());

		COM_ERROR_IF_FAILED(hr, "Failed to create lightShaderResource.");
	}
	catch (COMException& exception)
	{
		Logger::Log(exception);
		return false;
	}
}

void LightManager::Update()
{
	D3D11_MAPPED_SUBRESOURCE submap;
	LightManager::Get().context->Map(LightManager::Get().lightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &submap);
	memcpy(submap.pData, LightManager::Get().lights.data(), sizeof(Light) * LightManager::Get().lights.size());
	LightManager::Get().context->Unmap(LightManager::Get().lightBuffer.Get(), 0);

	LightManager::Get().context->PSSetShaderResources(3, 1, LightManager::Get().lightShaderResource.GetAddressOf());
}
