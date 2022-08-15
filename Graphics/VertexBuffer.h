#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <memory>

template <class T>
class VertexBuffer {
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	UINT stride = sizeof(T);
	UINT vertexCount = 0;

public:
	VertexBuffer() = default;
	VertexBuffer(const VertexBuffer<T>& rhs)
	{
		this->buffer = rhs.buffer;
		this->stride = rhs.stride;
		this->vertexCount = rhs.vertexCount;
	}
	VertexBuffer<T>& operator=(const VertexBuffer<T>& rhs)
	{
		this->buffer = rhs.buffer;
		this->stride = rhs.stride;
		this->vertexCount = rhs.vertexCount;
		return *this;
	}

	ID3D11Buffer* Get()const {
		return this->buffer.Get();
	}

	ID3D11Buffer* const* GetAddressOf()const {
		return this->buffer.GetAddressOf();
	}

	UINT VertexCount() const {
		return this->vertexCount;
	}

	const UINT Stride() const {
		return this->stride;
	}

	const UINT* StridePtr() const {
		return &this->stride;
	}

	HRESULT Initialize(ID3D11Device* device, T* data, UINT vertexCount) {
		if (this->buffer.Get() != nullptr) {
			this->buffer.Reset();
		}

		this->vertexCount = vertexCount;

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));

		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = this->stride * vertexCount;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA bufferData;
		ZeroMemory(&bufferData, sizeof(bufferData));
		bufferData.pSysMem = data;

		HRESULT hr = device->CreateBuffer(&bufferDesc, &bufferData, this->buffer.GetAddressOf());

		return hr;
	}
};