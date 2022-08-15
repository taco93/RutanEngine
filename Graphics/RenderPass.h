#pragma once
#include "Shaders.h"

class RenderPass
{
public:
	virtual void PreRender() = 0;
	virtual void Draw() = 0;
	virtual void PostRender() = 0;

	void Disable();
	void Enable();
	bool IsEnabled()const;
	std::wstring GetShaderFolder();

protected:
	ID3D11DeviceContext* context = nullptr;

private:
	bool enabled = true;
};