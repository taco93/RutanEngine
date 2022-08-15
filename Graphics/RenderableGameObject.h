#pragma once
#include "GameObject.h"

class RenderableGameObject : public GameObject
{
public:
	bool Initialize(const std::string& filepath, ID3D11Device* device, ID3D11DeviceContext* context);
	void Draw(const Matrix& viewProjectionMatrix);

protected:
	Model model;

	void UpdateMatrix() override;

	Matrix worldMatrix = Matrix::Identity;
};