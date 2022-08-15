#pragma once
#include <DirectXMath.h>
#include "GameObject.h"
using namespace DirectX;

class Camera : public GameObject
{
public:
	Camera();
	void SetProjection(float fovDegrees, float aspectRatio, float nearZ, float farZ);

	const Matrix& GetViewMatrix() const;
	const Matrix& GetProjectionMatrix() const;

private:
	void UpdateMatrix();

	Matrix viewMatrix;
	Matrix projectionMatrix;
};