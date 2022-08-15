#pragma once
#include "../Logger.h"
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>

struct AdapterData
{
	AdapterData(IDXGIAdapter* pAdapter);
	IDXGIAdapter* pAdapter = nullptr;
	DXGI_ADAPTER_DESC desc;
};

class AdapterReader
{
private:
	static std::vector<AdapterData>	adapters;

public:
	static std::vector<AdapterData> GetAdapterData();

};

