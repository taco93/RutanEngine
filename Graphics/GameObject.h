#pragma once
#include "Model.h"
#include "..//Utility.h"
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

class GameObject
{
public:
	const Vector3& GetPosition() const;
	const Vector3& GetRotationAngles() const;
	const Quaternion& GetRotation() const;

	void SetScale(const Vector3& size);
	void SetScale(const float& factor);
	void SetScale(float x, float y, float z);
	void SetPosition(const Vector3& pos);
	void SetPosition(float x, float y, float z);
	void AdjustPosition(const Vector3& pos);
	void AdjustPosition(float x, float y, float z);
	void SetRotation(const Vector3& rot);
	void SetRotation(float x, float y, float z);
	void SetRotation(const Quaternion& rot);
	void AdjustRotation(const Vector3& rot);
	void AdjustRotation(float x, float y, float z);
	void AdjustRotation(const Quaternion& rot);
	void SetLookAt(Vector3 pos);

	const Vector3& GetForward(bool omitY = false);
	const Vector3& GetRight(bool omitY = false);

protected:
	virtual void UpdateMatrix();
	void UpdateDirectionVectors();

	const Vector3 DEFAULT_FORWARD_VECTOR = Vector3(0.0f, 0.0f, 1.0f);
	const Vector3 DEFAULT_UP_VECTOR = Vector3(0.0f, 1.0f, 0.0f);
	const Vector3 DEFAULT_RIGHT_VECTOR = Vector3(1.0f, 0.0f, 0.0f);

	Vector3 forward;
	Vector3 right;

	Vector3 forward_noY;
	Vector3 right_noY;

	Vector3 pos;
	Vector3 rotVec;
	Quaternion rotation;
	Vector3 scale = Vector3(1.0f, 1.0f, 1.0f);
};