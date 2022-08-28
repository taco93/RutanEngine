#include "GameObject.h"

const Vector3& GameObject::GetPosition() const
{
	return this->pos;
}

const Vector3& GameObject::GetRotationAngles() const
{
	return this->rotVec;
}

const Quaternion& GameObject::GetRotation() const
{
	return this->rotation;
}

void GameObject::SetScale(const Vector3& size)
{
	this->scale = size;
	this->UpdateMatrix();
}

void GameObject::SetScale(const float& factor)
{
	this->scale *= factor;
	this->UpdateMatrix();
}

void GameObject::SetScale(float x, float y, float z)
{
	this->scale.x = x;
	this->scale.y = y;
	this->scale.z = z;
	this->UpdateMatrix();
}

void GameObject::SetPosition(const Vector3& pos)
{
	this->pos = pos;
	this->UpdateMatrix();
}

void GameObject::SetPosition(float x, float y, float z)
{
	this->pos = Vector3(x, y, z);
	this->UpdateMatrix();
}

void GameObject::AdjustPosition(const Vector3& pos)
{
	this->pos += pos;
	this->UpdateMatrix();
}

void GameObject::AdjustPosition(float x, float y, float z)
{
	this->pos.x += x;
	this->pos.y += y;
	this->pos.z += z;
	this->UpdateMatrix();
}

void GameObject::SetRotation(const Vector3& rot)
{
	this->rotVec = rot;
	this->UpdateMatrix();
}

void GameObject::SetRotation(float x, float y, float z)
{
	this->rotVec = Vector3(x, y, z);
	this->UpdateMatrix();
}

void GameObject::SetRotation(const Quaternion& rot)
{
	this->rotVec = rot.ToEuler();
	this->UpdateMatrix();
}

void GameObject::AdjustRotation(const Vector3& rot)
{
	this->rotVec += rot;
	this->UpdateMatrix();
}

void GameObject::AdjustRotation(float x, float y, float z)
{
	this->rotVec.x += x;
	this->rotVec.y += y;
	this->rotVec.z += z;
	this->UpdateMatrix();
}

void GameObject::AdjustRotation(const Quaternion& rot)
{
	this->rotVec += rot.ToEuler();
	this->UpdateMatrix();
}

void GameObject::SetLookAt(Vector3 pos)
{
	if (pos.x == this->pos.x && pos.y == this->pos.y && pos.z == this->pos.z) return;

	pos.x = this->pos.x - pos.x;
	pos.y = this->pos.y - pos.y;
	pos.z = this->pos.z - pos.z;

	float pitch = 0.0f;
	if (pos.y != 0.0f) {
		const float distance = sqrt(pos.x * pos.x + pos.z * pos.z);
		pitch = atan(pos.y / distance);
	}

	float yaw = 0.0f;

	if (pos.x != 0.0f) {
		yaw = atan(pos.x / pos.z);
	}

	if (pos.z > 0) {
		yaw += DirectX::XM_PI;
	}

	this->SetRotation(pitch, yaw, 0.0f);
}

const Vector3& GameObject::GetForward(bool omitY)
{
	if (omitY) return this->forward_noY;

	return this->forward;
}

const Vector3& GameObject::GetRight(bool omitY)
{
	if (omitY) return this->right_noY;

	return this->right;
}

void GameObject::UpdateMatrix()
{
	assert("UpdateMatrix must be overidden");
}

void GameObject::UpdateDirectionVectors()
{
	this->forward = Vector3::Transform(this->DEFAULT_FORWARD_VECTOR, this->rotation);
	this->right = Vector3::Transform(this->DEFAULT_RIGHT_VECTOR, this->rotation);

	Quaternion rotationNoY = Quaternion::CreateFromYawPitchRoll(this->rotVec.y, 0.0f, 0.0f);
	this->forward_noY = Vector3::Transform(this->DEFAULT_FORWARD_VECTOR, rotationNoY);
	this->right_noY = Vector3::Transform(this->DEFAULT_RIGHT_VECTOR, rotationNoY);
}
