#pragma once
#include "Light.h"
#include <vector>
#include <d3d11.h>
#include <wrl/client.h>
#include "..\\Logger.h"

class LightManager
{
public:
	static void AddPointLight(const Vector3& pos, const Vector3& color);
	static void AddSpotLight(const Vector3& pos, const Vector3& color, const Vector3& direction, float spotAngle);
	static Light* GetLight(UINT index);
	static bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
	static void Update();

private:
	static LightManager& Get()
	{
		static LightManager s_instance;
		return s_instance;
	}

	LightManager() = default;
	LightManager& operator=(const LightManager& rhs) = delete;
	LightManager& operator=(const LightManager&& rhs) = delete;
	LightManager(const LightManager& rhs) = delete;
	LightManager(const LightManager&& rhs) = delete;

	enum class LightType : uint32_t
	{
		POINT_LIGHT,
		SPOT_LIGHT,
		DIRECTIONAL_LIGHT
	};

	bool initialized = false;
	std::vector<Light> lights;
	Microsoft::WRL::ComPtr<ID3D11Buffer> lightBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> lightShaderResource;
	ID3D11DeviceContext* context = nullptr;
	ID3D11Device* device = nullptr;

	bool UpdateBuffers();
};
