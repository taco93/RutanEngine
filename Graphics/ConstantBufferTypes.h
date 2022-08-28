#pragma once
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

struct CB_VS_vertexshader
{
	Matrix wvpMatrix;
	Matrix worldMatrix;
};

struct CB_PS_cameraparams
{
	Vector3 eyePos;
	float padding;
};

struct CB_PS_per_light
{
	Vector3 ambientColor = { 1.0f, 1.0f, 1.0f };
	float ambientStrength = 0.4f;
	Vector3 color;
	float lightStrength;
	Vector3 position;
	float attenuationA;
	float attenuationB;
	float attenuationC;
	uint32_t type;
	float spotAngle;
	Vector3 direction = { 0.0f, -1.0f, 0.0f };
	float padding;
};

struct CB_PS_materials
{
	BOOL hasAlbedoMap = false;
	BOOL hasNormalMap = false;
	BOOL hasAlphaMap = false;
	BOOL other2 = false;
};
