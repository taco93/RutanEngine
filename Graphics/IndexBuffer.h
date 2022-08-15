#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>

class IndexBuffer {
private:
	IndexBuffer(const IndexBuffer& rhs);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	UINT indexCount = 0;

public:
	IndexBuffer() = default;

	ID3D11Buffer* Get()const {
		return this->buffer.Get();
	}

	ID3D11Buffer* const* GetAddressOf() const {
		return this->buffer.GetAddressOf();
	}

	UINT IndexCount() const {
		return this->indexCount;
	}

	HRESULT Initialize(ID3D11Device* device, DWORD* data, UINT indexCount) {
		if (this->buffer.Get() != nullptr) {
			this->buffer.Reset();
		}

		this->indexCount = indexCount;

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.ByteWidth = sizeof(DWORD) * indexCount;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA bufferData;
		ZeroMemory(&bufferData, sizeof(D3D11_SUBRESOURCE_DATA));
		bufferData.pSysMem = data;

		HRESULT hr = device->CreateBuffer(&bufferDesc, &bufferData, this->buffer.GetAddressOf());

		return hr;
	}
};