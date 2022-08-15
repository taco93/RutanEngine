#pragma once
#include "Mesh.h"

class Model
{
public:
	bool Initialize(const std::string& filepath, ID3D11Device* device, ID3D11DeviceContext* context);
	void Draw(const Matrix& worldMatrix, const Matrix& viewProjectionMatrix);

private:
	std::vector<Mesh> meshes;
	bool LoadModel(const std::string& filepath);
	void ProcessNode(aiNode* node, const aiScene* scene, const Matrix& parentTransform);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, const Matrix& transform);
	TextureStorageType DetermineTextureStorageType(const aiScene* pScene, aiMaterial* pMat, unsigned int index, aiTextureType textureType);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* pMaterial, aiTextureType textureType, const aiScene* pScene);
	int GetTextureIndex(aiString* pStr);

	ID3D11Device*						device = nullptr;
	ID3D11DeviceContext*				context = nullptr;
	ConstantBuffer<CB_VS_vertexshader> cb_vs_vertexshader;

	std::string directory = "";
};