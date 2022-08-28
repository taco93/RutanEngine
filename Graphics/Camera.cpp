#include "Camera.h"
#include <Windows.h>
#include <string>
#include "..\\Utility.h"

Camera::Camera()
{
	this->pos = Vector3(0.0f, 0.0f, 0.0f);
	this->rotVec = Vector3(0.0f, 0.0f, 0.0f);
	this->UpdateMatrix();
}

void Camera::SetProjection(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	float fovRadians = (fovDegrees / 360.0f) * XM_2PI;

	this->projectionMatrix = XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
}

const Matrix& Camera::GetViewMatrix() const
{
	return this->viewMatrix;
}

const Matrix& Camera::GetProjectionMatrix() const
{
	return this->projectionMatrix;
}

void Camera::UpdateMatrix()
{
	// Clamps the camera so it cannot roll over the X-axis 
	this->rotVec.x = clamp(this->rotVec.x, -XM_PIDIV2, XM_PIDIV2);

	// Stay withing the bounds of floating point values to not exceed it's maximum.
	// Since we are using quaternions there wont be any jumps as it would without.
	// Clockwise or anti-clockwise order would then depend on the closest angle, leading
	// to camera jumping around
	this->rotVec.y = fmod(this->rotVec.y, XM_2PI);
	this->rotVec.z = fmod(this->rotVec.z, XM_2PI);

	this->rotation = Quaternion::CreateFromYawPitchRoll(this->rotVec.y, this->rotVec.x, this->rotVec.z);

	Vector3 camTarget = Vector3::Transform(this->DEFAULT_FORWARD_VECTOR, this->rotation);
	Vector3 upDir = Vector3::Transform(this->DEFAULT_UP_VECTOR, this->rotation);

	this->viewMatrix = XMMatrixLookToLH(this->pos, camTarget, upDir);

	this->UpdateDirectionVectors();
}