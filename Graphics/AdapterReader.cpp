#include "AdapterReader.h"

std::vector<AdapterData> AdapterReader::adapters;

std::vector<AdapterData> AdapterReader::GetAdapterData()
{
    if (adapters.size() > 0)
    {
        return adapters;
    }

    Microsoft::WRL::ComPtr<IDXGIFactory> pFactory;

    HRESULT hr = CreateDXGIFactory(_uuidof(IDXGIFactory), reinterpret_cast<void**>(pFactory.GetAddressOf()));
    if (FAILED(hr))
    {
        Logger::Log(hr, "Failed to create DXGIFactory for enumerating adapters.");
        exit(EXIT_FAILURE);
    }

    IDXGIAdapter* pAdapter;
    UINT index = 0;

    while (SUCCEEDED(pFactory->EnumAdapters(index, &pAdapter)))
    {
        adapters.push_back(AdapterData(pAdapter));
        index++;
    }

    return adapters;
}

AdapterData::AdapterData(IDXGIAdapter* pAdapter)
{
    this->pAdapter = pAdapter;

    HRESULT hr = pAdapter->GetDesc(&this->desc);

    if (FAILED(hr))
    {
        Logger::Log(hr, "Failed to get description for IDXGIAdapter.");
    }
}
