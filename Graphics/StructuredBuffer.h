#pragma once
#include <d3d11.h>
#include <wrl/client.h>

template <class T>
class StructuredBuffer
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;
	ID3D11DeviceContext* context = nullptr;

public:
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context, const T* data);
};

template<class T>
inline bool StructuredBuffer<T>::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, const T* data)
{
	return false;
}
