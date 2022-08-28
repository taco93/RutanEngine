#include "Graphics.h"
#include "..\\ResourceManager.h"
#include "LightManager.h"

bool Graphics::Initialize(HWND hwnd, int width, int height)
{
	fpsTimer.Start();
	this->windowWidth = width;
	this->windowHeight = height;

	if (!InitializeDirectX(hwnd))
		return false;

	if (!InitializeRenderPasses())
		return false;

	if (!LightManager::Initialize(device.Get(), context.Get()))
		return false;

	if (!InitializeScene())
		return false;

	// Setup ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(this->device.Get(), this->context.Get());
	ImGui::StyleColorsDark();

	return true;
}

Graphics::~Graphics()
{
	this->swapchain->SetFullscreenState(FALSE, nullptr);
	ResourceManager::Destroy();
}

bool Graphics::InitializeDirectX(HWND hwnd)
{
	try
	{
		std::vector<AdapterData> adapters = AdapterReader::GetAdapterData();

		if (adapters.size() == 0)
		{
			Logger::Log("No DXGI Adapters found.");
			return false;
		}

		DXGI_SWAP_CHAIN_DESC scd = {};

		scd.BufferDesc.Width = this->windowWidth;
		scd.BufferDesc.Height = this->windowHeight;
		scd.BufferDesc.RefreshRate.Numerator = 60;
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount = 1;
		scd.OutputWindow = hwnd;
		scd.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;
		scd.Windowed = TRUE;
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		HRESULT hr = D3D11CreateDeviceAndSwapChain(
			adapters[0].pAdapter, // IDXGIAdapter
			D3D_DRIVER_TYPE_UNKNOWN,
			NULL, // FOR SOFTWARE DRIVER 
			NULL, // FLAGS FOR RUNTIME LAYER
			NULL, // FEATURE LEVELS ARRAY
			0, //# OF FEATURE LEVELS
			D3D11_SDK_VERSION,
			&scd, // Swapchain Description
			this->swapchain.GetAddressOf(),
			this->device.GetAddressOf(),
			NULL, // SUPPORTED FEATURE LEVEL
			this->context.GetAddressOf());

		COM_ERROR_IF_FAILED(hr, "Failed to create device and swapchain.");

		this->CreateViewport();
	}
	catch (COMException& exception) {
		Logger::Log(exception);
		return false;
	}

	return true;
}

bool Graphics::InitializeRenderPasses()
{
	if (!geometryPass.Initialize(device.Get(), context.Get(), this->windowWidth, this->windowHeight))
		return false;

	if (!lightingPass.Initialize(device.Get(), context.Get(), this->swapchain.Get(), this->geometryPass.GetGBufferPtr()))
		return false;

	return true;
}

bool Graphics::InitializeScene()
{
	// Initialize model(s)
	if (!gameObject.Initialize("Assets\\Objects\\Sponza\\sponza_nobanner.obj", this->device.Get(), this->context.Get()))
		return false;

	LightManager::AddPointLight(Vector3(30.f, 30.f, 0.f), Vector3(1.0f, 1.0f, 1.0f));
	LightManager::AddPointLight(Vector3(200.f, 65.f, 0.f), Vector3(1.0f, 0.0f, 0.0f));
	LightManager::AddPointLight(Vector3(30.f, 65.f, 0.f), Vector3(0.0f, 1.0f, 0.0f));

	camera.SetPosition(0.0f, 50.0f, -50.0f);
	camera.SetProjection(90.0f, static_cast<float>(this->windowWidth) / static_cast<float>(this->windowHeight), 0.1f, 3000.0f);

	return true;
}

void Graphics::CreateViewport()
{
	D3D11_VIEWPORT vp;
	vp.Width = static_cast<FLOAT>(this->windowWidth);
	vp.Height = static_cast<FLOAT>(this->windowHeight);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	this->context->RSSetViewports(1, &vp);
}

void Graphics::Render()
{
	this->geometryPass.PreRender();
	this->gameObject.Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());
	this->geometryPass.PostRender();

	this->lightingPass.PreRender();
	this->lightingPass.UpdateCameraParams(this->camera.GetPosition());
	LightManager::Update();
	this->lightingPass.Draw();
	this->lightingPass.PostRender();

	// ImGui Frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Light Controls");
	ImGui::DragFloat3("Ambient light color", &LightManager::GetLight(0)->ambientColor.x, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat3("Position", &LightManager::GetLight(0)->position.x, 0.0f, 0.0f, 10000.f);
	ImGui::DragFloat("Ambient strength", &LightManager::GetLight(0)->ambientStrength, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("Light strength", &LightManager::GetLight(0)->lightStrength, 0.01f, 0.0f, 10.0f);
	ImGui::DragFloat("Attenuation A", &LightManager::GetLight(0)->attenuationA, 0.01f, 0.1f, 10.0f);
	ImGui::DragFloat("Attenuation B", &LightManager::GetLight(0)->attenuationB, 0.001f, 0.0f, 10.0f);
	ImGui::DragFloat("Attenuation C", &LightManager::GetLight(0)->attenuationC, 0.001f, 0.0f, 10.0f);
	ImGui::DragFloat("Spotangle", &LightManager::GetLight(0)->spotAngle, 0.001f, 0.0f, 10.0f);
	ImGui::End();
	ImGui::Begin("G-buffer");
	ImGui::Image(this->geometryPass.GetGBufferPtr()->GetImages()[0], { 300, 200 });
	ImGui::Image(this->geometryPass.GetGBufferPtr()->GetImages()[1], { 300, 200 });
	ImGui::Image(this->geometryPass.GetGBufferPtr()->GetImages()[2], { 300, 200 });
	ImGui::End();
	ImGui::ShowMetricsWindow();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	swapchain->Present(0, NULL);
}

void Graphics::OnResize(UINT width, UINT height)
{
	if (this->swapchain.Get()) {
		this->context->OMSetRenderTargets(0, 0, 0);
		this->windowWidth = width;
		this->windowHeight = height;

		try {
			this->geometryPass.ReleaseBuffers();
			this->lightingPass.ReleaseRenderTarget();

			HRESULT hr = this->swapchain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

			COM_ERROR_IF_FAILED(hr, "Failed to Resize buffers.");

			ID3D11Texture2D* pBuffer;
			hr = this->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);

			COM_ERROR_IF_FAILED(hr, "Failed to get backbuffer");

			this->lightingPass.Resize(this->device.Get(), pBuffer, width, height);
			this->geometryPass.Resize(this->device.Get(), width, height);

			this->CreateViewport();
		}
		catch (COMException& exception)
		{
			Logger::Log(exception);
		}
	}
}

bool Graphics::IsFullScreen() const
{
	BOOL fullscreen;
	this->swapchain->GetFullscreenState(&fullscreen, nullptr);
	return fullscreen;
}
