#include "Shaders.h"

bool VertexShader::Initialize(ID3D11Device* device, std::wstring shaderPath, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements)
{
	try {
		HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(), this->shader_buffer.GetAddressOf());

		COM_ERROR_IF_FAILED(hr, "Failed to load vertex shader " + StringHelper::WideToString(shaderPath));

		hr = device->CreateVertexShader(this->shader_buffer.Get()->GetBufferPointer(), this->shader_buffer.Get()->GetBufferSize(), NULL, this->shader.GetAddressOf());

		COM_ERROR_IF_FAILED(hr, "Failed to create vertex shader");

		if (numElements > 0)
		{
			hr = device->CreateInputLayout(layoutDesc, numElements, this->shader_buffer->GetBufferPointer(), this->shader_buffer->GetBufferSize(), this->inputLayout.GetAddressOf());

			COM_ERROR_IF_FAILED(hr, "Failed to create input layout");
		}
	}
	catch (COMException& exception)
	{
		Logger::Log(exception);
		return false;
	}

	return true;
}

ID3D11VertexShader* VertexShader::GetShader()
{
	return this->shader.Get();
}

ID3D10Blob* VertexShader::GetBuffer()
{
	return this->shader_buffer.Get();
}

ID3D11InputLayout* VertexShader::GetInputLayout()
{
	return this->inputLayout.Get();
}

bool PixelShader::Initialize(ID3D11Device* device, std::wstring shaderPath)
{
	HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(), this->shader_buffer.GetAddressOf());

	if (FAILED(hr)) {
		std::wstring errorMsg = L"Failed to load shader: " + shaderPath;
		Logger::LogError(hr, errorMsg);
		return false;
	}

	hr = device->CreatePixelShader(this->shader_buffer.Get()->GetBufferPointer(), this->shader_buffer.Get()->GetBufferSize(), NULL, this->shader.GetAddressOf());

	if (FAILED(hr)) {
		std::wstring errorMsg = L"Failed to create pixel shader: " + shaderPath;
		Logger::LogError(hr, errorMsg);
		return false;
	}

	return true;
}

ID3D11PixelShader* PixelShader::GetShader()
{
	return this->shader.Get();
}

ID3D10Blob* PixelShader::GetBuffer()
{
	return this->shader_buffer.Get();
}
