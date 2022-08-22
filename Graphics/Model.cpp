#include "Model.h"
#include "..\\ResourceManager.h"

bool Model::Initialize(const std::string& filepath, ID3D11Device* device, ID3D11DeviceContext* context)
{
	this->device = device;
	this->context = context;

	try
	{
		HRESULT hr = this->cb_vs_vertexshader.Initialize(device, context);

		COM_ERROR_IF_FAILED(hr, "Failed to Initialize models constant buffer.");

		if (!this->LoadModel(filepath))
			return false;
	}
	catch (COMException& exception) {
		Logger::Log(exception);

		return false;
	}

	return true;
}


void Model::Draw(const Matrix& worldMatrix, const Matrix& viewProjectionMatrix)
{
	this->context->VSSetConstantBuffers(0, 1, this->cb_vs_vertexshader.GetAddressOf());

	for (UINT i = 0; i < this->meshes.size(); i++) {
		this->cb_vs_vertexshader.data.wvpMatrix = this->meshes[i].GetTransform() * worldMatrix * viewProjectionMatrix;
		this->cb_vs_vertexshader.data.worldMatrix = this->meshes[i].GetTransform() * worldMatrix;
		this->cb_vs_vertexshader.ApplyChanges();
		this->meshes[i].Draw();
	}
}

bool Model::LoadModel(const std::string& filepath)
{
	this->directory = StringHelper::GetDirectoryFromPath(filepath);
	Assimp::Importer importer;
	
	const aiScene* pScene = importer.ReadFile(filepath,
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded |
		aiProcess_CalcTangentSpace |
		aiProcess_JoinIdenticalVertices | 
		aiProcess_GenNormals);

	if (pScene == NULL) return false;

	this->ProcessNode(pScene->mRootNode, pScene, Matrix::Identity);

	return true;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene, const Matrix& parentTransform)
{
	Matrix nodeTransform = Matrix(&node->mTransformation.a1) * parentTransform;

	for (UINT i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(this->ProcessMesh(mesh, scene, nodeTransform));
	}

	for (UINT i = 0; i < node->mNumChildren; i++) {
		this->ProcessNode(node->mChildren[i], scene, nodeTransform);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, const Matrix& transform)
{
	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;
	bool hasNormalMap = false;

	for (UINT i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;

		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;

		vertex.normal.x = mesh->mNormals[i].x;
		vertex.normal.y = mesh->mNormals[i].y;
		vertex.normal.z = mesh->mNormals[i].z;

		if (mesh->mTangents) {
			vertex.tangent.x = mesh->mTangents[i].x;
			vertex.tangent.y = mesh->mTangents[i].y;
			vertex.tangent.z = mesh->mTangents[i].z;
		}

		if (mesh->mTextureCoords[0]) {
			vertex.texCoord.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.texCoord.y = (float)mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(vertex);
	}

	for (UINT i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	std::vector<Texture> textures;
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	std::vector<Texture> diffuseTextures = LoadMaterialTextures(material, aiTextureType_DIFFUSE, scene);
	textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());

	std::vector<Texture> normalTextures = LoadMaterialTextures(material, aiTextureType_HEIGHT, scene);
	textures.insert(textures.end(), normalTextures.begin(), normalTextures.end());

	std::vector<Texture> alphaTextures = LoadMaterialTextures(material, aiTextureType_OPACITY, scene);
	textures.insert(textures.end(), alphaTextures.begin(), alphaTextures.end());

	CB_PS_toggles maps;

	if (diffuseTextures.size() > 0)
	{
		maps.hasAlbedoMap = true;
	}

	if (normalTextures.size() > 0)
	{
		maps.hasNormalMap = true;
	}

	if (alphaTextures.size() > 0)
	{
		maps.hasAlphaMap = true;
	}

	return Mesh(this->device, this->context, vertices, indices, textures, transform, maps);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* pMaterial, aiTextureType textureType, const aiScene* pScene)
{
	std::vector<Texture> materialTextures;
	TextureStorageType storetype = TextureStorageType::Invalid;
	unsigned int textureCount = pMaterial->GetTextureCount(textureType);

	for (UINT i = 0; i < textureCount; i++) {
		aiString path;
		pMaterial->GetTexture(textureType, i, &path);
		TextureStorageType storetype = DetermineTextureStorageType(pScene, pMaterial, i, textureType);

		switch (storetype)
		{
		case TextureStorageType::Disk:
		{
			std::string filename = this->directory + '\\' + path.C_Str();
			Texture* tex = ResourceManager::GetResource<Texture>(filename);
			if (tex) {
				materialTextures.push_back(*tex);
				break;
			}
			Texture diskTexture(this->device, this->context, filename, textureType);
			materialTextures.push_back(diskTexture);
			break;
		}
		case TextureStorageType::EmbeddedCompressed:
		{
			const aiTexture* pTexture = pScene->GetEmbeddedTexture(path.C_Str());
			Texture embeddedTexture(this->device,
				reinterpret_cast<uint8_t*>(pTexture->pcData),
				pTexture->mWidth,
				textureType);
			materialTextures.push_back(embeddedTexture);
			break;
		}
		case TextureStorageType::EmbeddedIndexCompressed:
		{
			int index = GetTextureIndex(&path);
			Texture embeddedIndexedTexture(this->device,
				reinterpret_cast<uint8_t*>(pScene->mTextures[index]->pcData),
				pScene->mTextures[index]->mWidth,
				textureType);
			materialTextures.push_back(embeddedIndexedTexture);
			break;
		}
		}
	}

	return materialTextures;
}

TextureStorageType Model::DetermineTextureStorageType(const aiScene* pScene, aiMaterial* pMat, unsigned int index, aiTextureType textureType)
{
	aiString path;
	pMat->GetTexture(textureType, index, &path);
	std::string texturePath = path.C_Str();

	if (texturePath[0] == '*') {
		if (pScene->mTextures[0]->mHeight == 0) {
			return TextureStorageType::EmbeddedIndexCompressed;
		}
		
		assert("SUPPORT DOES NOT EXIST YET FOR INDEXED NON COMPRESSED TEXTURE!" && 0);
		return TextureStorageType::EmbeddedIndexNonCompressed;
	}

	if (auto pTex = pScene->GetEmbeddedTexture(texturePath.c_str())) {
		if (pTex->mHeight == 0) {
			return TextureStorageType::EmbeddedCompressed;
		}
		assert("SUPPORT DOES NOT EXIST YET FOR INDEXED NON COMPRESSED TEXTURE!" && 0);
		return TextureStorageType::EmbeddedNonCompressed;
	}

	if (texturePath.find('.') != std::string::npos) {
		return TextureStorageType::Disk;
	}

	return TextureStorageType::None;
}

int Model::GetTextureIndex(aiString* pStr)
{
	assert(pStr->length >= 2);

	return atoi(&pStr->C_Str()[1]);
}
