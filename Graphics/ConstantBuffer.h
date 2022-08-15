#pragma once
#include "ConstantBufferTypes.h"
#include <d3d11.h>
#include <wrl/client.h>
#include "..\\Logger.h"

template <class T>
class ConstantBuffer {
private:
	ConstantBuffer(const ConstantBuffer<T>& rhs);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	ID3D11DeviceContext* context = nullptr;

public:
	ConstantBuffer() = default;
	T data;

	ID3D11Buffer* Get()const {
		return this->buffer.Get();
	}

	ID3D11Buffer* const* GetAddressOf()const {
		return this->buffer.GetAddressOf();
	}

	bool ApplyChanges() {
		D3D11_MAPPED_SUBRESOURCE mappedResource;

		HRESULT hr = this->context->Map(this->buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		if (FAILED(hr)) {
			Logger::LogError(hr, "Failed to map constant buffer.");
			return false;
		}
		memcpy(mappedResource.pData, &this->data, sizeof(T));
		this->context->Unmap(this->buffer.Get(), 0);

		return true;
	}

	HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext) {
		this->context = deviceContext;

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));

		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = static_cast<UINT>(sizeof(CB_VS_vertexshader) + (16 - sizeof(CB_VS_vertexshader) % 16));
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		HRESULT hr = device->CreateBuffer(&bufferDesc, 0, this->buffer.GetAddressOf());

		return hr;
	}
};