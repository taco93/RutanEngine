#pragma once
#include <SimpleMath.h>

struct CB_VS_vertexshader
{
	DirectX::SimpleMath::Matrix wvpMatrix;
	DirectX::SimpleMath::Matrix worldMatrix;
};

struct CB_PS_light
{
	DirectX::SimpleMath::Vector3 ambientColor;
	float ambientStrength;
	DirectX::SimpleMath::Vector3 lightColor;
	float lightStrength;
	DirectX::SimpleMath::Vector3 lightPos;
	float attenuationA;
	float attenuationB;
	float attenuationC;
	float padding1;
	float padding2;
};

struct CB_PS_toggles
{
	BOOL hasAlbedoMap = false;
	BOOL hasNormalMap = false;
	BOOL hasAlphaMap = false;
	BOOL other2 = false;
};
