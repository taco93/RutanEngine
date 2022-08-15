#pragma once
#include "RenderableGameObject.h"
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;

class Light : public RenderableGameObject
{
public:
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context);

	Vector3 lightColor = Vector3(1.0f, 1.0f, 1.0f);
	float lightStrength = 1.0f;
	float attenuationA = 1.0f;
	float attenuationB = 0.1f;
	float attenuationC = 0.1f;
};