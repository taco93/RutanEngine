#include "RenderPass.h"

void RenderPass::Disable()
{
	this->enabled = false;
}

void RenderPass::Enable()
{
	this->enabled = true;
}

bool RenderPass::IsEnabled() const
{
	return this->enabled;
}

std::wstring RenderPass::GetShaderFolder()
{
	std::wstring shaderFolder = L"";;

#pragma region DetermineShaderPath
#ifdef _DEBUG
#ifdef _WIN64
	shaderFolder = L"x64\\Debug\\";
#else //x86 (Win32)
	shaderFolder = L"Debug\\";
#endif
#else
#ifdef _WIN64
	shaderFolder = L"x64\\Release\\";
#else
	shaderFolder = L"Release\\";
#endif
#endif

	return shaderFolder;
}
