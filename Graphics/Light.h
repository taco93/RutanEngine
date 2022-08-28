#pragma once
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;

struct Light
{
	Vector3 ambientColor = { 1.0f, 1.0f, 1.0f };
	float ambientStrength = 0.4f;
	Vector3 color = { 1.0f, 1.0f, 1.0f };
	float lightStrength = 1.0f;
	Vector3 position = { 0.0f, 0.0f, 0.0f };
	float attenuationA = 0.0f;;
	float attenuationB = 0.125f;
	float attenuationC = 0.0f;
	uint32_t type = 0;
	float spotAngle = 50.f;
	Vector3 direction = { 0.0f, -1.0f, 0.0f };
	float padding;
};