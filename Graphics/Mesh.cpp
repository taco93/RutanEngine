#include "Mesh.h"

Mesh::Mesh(ID3D11Device* device, ID3D11DeviceContext* context, std::vector<Vertex>& vertices, std::vector<DWORD>& indices, std::vector<Texture>& textures, const Matrix& transform, CB_PS_toggles maps)
{
	this->context = context;
	this->textures = textures;
	this->cb_ps_toggles.data = maps;

	try {
		HRESULT hr = this->vBuffer.Initialize(device, vertices.data(), vertices.size());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize vertex buffer for mesh.");

		hr = this->iBuffer.Initialize(device, indices.data(), indices.size());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize index buffer for mesh.");

		hr = this->cb_ps_toggles.Initialize(device, context);
		COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer for mesh.");
	}
	catch (COMException& exception)
	{
		Logger::Log(exception);
	}
}

Mesh::Mesh(const Mesh& mesh)
{
	this->context = mesh.context;
	this->iBuffer = mesh.iBuffer;
	this->vBuffer = mesh.vBuffer;
	this->textures = mesh.textures;
	this->transform = mesh.transform;
	this->cb_ps_toggles = mesh.cb_ps_toggles;
	//this->textureUAV = mesh.textureUAV;
}

void Mesh::Draw()
{
	UINT offset = 0;
	ID3D11ShaderResourceView* nullArray[3] = { nullptr };

	this->context->PSSetShaderResources(0, 3, nullArray);

	for (int i = 0; i < this->textures.size(); i++) {
		if (this->textures[i].GetType() == aiTextureType_DIFFUSE) {
			this->context->PSSetShaderResources(0, 1, this->textures[i].GetTextureResourceViewAddress());
		}
		else if (this->textures[i].GetType() == aiTextureType_HEIGHT) {
			this->context->PSSetShaderResources(1, 1, this->textures[i].GetTextureResourceViewAddress());
		}
		else if (this->textures[i].GetType() == aiTextureType_OPACITY) {
			this->context->PSSetShaderResources(2, 1, this->textures[i].GetTextureResourceViewAddress());
		}
	}

	this->context->IASetVertexBuffers(0, 1, this->vBuffer.GetAddressOf(), this->vBuffer.StridePtr(), &offset);
	this->context->IASetIndexBuffer(this->iBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	this->cb_ps_toggles.ApplyChanges();
	this->context->PSSetConstantBuffers(0, 1, this->cb_ps_toggles.GetAddressOf());
	this->context->DrawIndexed(this->iBuffer.IndexCount(), 0, 0);
}

const Matrix& Mesh::GetTransform()
{
	return this->transform;
}
