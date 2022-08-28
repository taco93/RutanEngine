#pragma once
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "Texture.h"

class Mesh
{
public:
	Mesh(ID3D11Device* device, ID3D11DeviceContext* context, std::vector<Vertex>& vertices, std::vector<DWORD>& indices, std::vector<Texture>& textures, const Matrix& transform, CB_PS_materials maps);
	Mesh(const Mesh& mesh);
	void Draw();
	const Matrix& GetTransform();

private:
	VertexBuffer<Vertex> vBuffer;
	IndexBuffer iBuffer;
	ID3D11DeviceContext* context = nullptr;
	std::vector<Texture> textures;
	Matrix transform;
	ConstantBuffer<CB_PS_materials> cb_ps_materials;
};