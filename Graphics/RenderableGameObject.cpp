#include "RenderableGameObject.h"

bool RenderableGameObject::Initialize(const std::string& filepath, ID3D11Device* device, ID3D11DeviceContext* context)
{
	if (!model.Initialize(filepath, device, context))
		return false;

	this->pos = Vector3(0.0f, 0.0f, 0.0f);
	this->rotVec = Vector3(0.0f, 0.0f, 0.0f);
	this->UpdateMatrix();

	return true;
}

void RenderableGameObject::Draw(const Matrix& viewProjectionMatrix)
{
	model.Draw(this->worldMatrix, viewProjectionMatrix);
}

void RenderableGameObject::UpdateMatrix()
{
	this->rotVec.y = fmod(this->rotVec.y, DirectX::XM_2PI);
	this->rotVec.z = fmod(this->rotVec.z, DirectX::XM_2PI);
	this->rotation = Quaternion::CreateFromYawPitchRoll(this->rotVec.y, this->rotVec.x, this->rotVec.z);

	this->worldMatrix = Matrix::CreateFromQuaternion(this->rotation) * Matrix::CreateTranslation(this->pos);
	this->UpdateDirectionVectors();
}