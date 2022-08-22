#include "Graphics.h"
#include "..\\ResourceManager.h"

bool Graphics::Initialize(HWND hwnd, int width, int height)
{
	fpsTimer.Start();
	this->windowWidth = width;
	this->windowHeight = height;

	if (!InitializeDirectX(hwnd))
		return false;

	if (!InitializeRenderPasses())
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

	if (!pShader_nolight.Initialize(this->device.Get(), L"x64\\Debug\\pixelshader_nolight.cso"))
		return false;

	return true;
}

bool Graphics::InitializeScene()
{
	try {
		HRESULT hr = this->cb_ps_light.Initialize(this->device.Get(), this->context.Get());

		COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");

		this->cb_ps_light.data.ambientColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
		this->cb_ps_light.data.ambientStrength = 1.0f;

		// Initialize model(s)
		if (!gameObject.Initialize("Assets\\Objects\\Sponza\\sponza_nobanner.obj", this->device.Get(), this->context.Get()))
			return false;

		if (!light.Initialize(this->device.Get(), this->context.Get()))
			return false;

		camera.SetPosition(0.0f, 0.0f, -2.0f);
		camera.SetProjection(90.0f, static_cast<float>(this->windowWidth) / static_cast<float>(this->windowHeight), 0.1f, 3000.0f);

	}
	catch (COMException& exception) {
		Logger::Log(exception);
		return false;
	}

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
	this->context->PSSetShader(this->pShader_nolight.GetShader(), NULL, 0);
	this->light.Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());
	this->geometryPass.PostRender();

	this->cb_ps_light.data.lightColor = light.lightColor;
	this->cb_ps_light.data.lightStrength = light.lightStrength;
	this->cb_ps_light.data.lightPos = light.GetPosition();
	this->cb_ps_light.data.attenuationA = light.attenuationA;
	this->cb_ps_light.data.attenuationB = light.attenuationB;
	this->cb_ps_light.data.attenuationC = light.attenuationC;
	this->cb_ps_light.ApplyChanges();
	this->context->PSSetConstantBuffers(0, 1, this->cb_ps_light.GetAddressOf());

	this->lightingPass.PreRender();
	this->lightingPass.Draw();
	this->lightingPass.PostRender();

	// ImGui Frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Light Controls");
	ImGui::DragFloat3("Ambient light color", &this->cb_ps_light.data.ambientColor.x, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("Ambient strength", &this->cb_ps_light.data.ambientStrength, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("Light strength", &this->light.lightStrength, 0.01f, 0.0f, 10.0f);
	ImGui::DragFloat("Attenuation A", &this->light.attenuationA, 0.01f, 0.1f, 10.0f);
	ImGui::DragFloat("Attenuation B", &this->light.attenuationB, 0.001f, 0.0f, 10.0f);
	ImGui::DragFloat("Attenuation C", &this->light.attenuationC, 0.001f, 0.0f, 10.0f);
	ImGui::End();
	ImGui::Begin("Deferred images");
	ImGui::Image(this->geometryPass.GetGBufferPtr()->GetImages()[0], { 250, 150 });
	ImGui::Image(this->geometryPass.GetGBufferPtr()->GetImages()[1], { 250, 150 });
	ImGui::Image(this->geometryPass.GetGBufferPtr()->GetImages()[2], { 250, 150 });
	ImGui::End();
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
