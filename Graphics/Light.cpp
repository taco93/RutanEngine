#include "Light.h"

bool Light::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
	if (!model.Initialize("Assets/Objects/light.fbx", device, context))
		return false;

	this->pos = Vector3(0.0f, 0.0f, 0.0f);
	this->rotVec = Vector3(0.0f, 0.0f, 0.0f);
	this->rotation = Quaternion::CreateFromYawPitchRoll(this->rotVec.y, this->rotVec.x, this->rotVec.z);
	this->UpdateMatrix();

	return true;
}
