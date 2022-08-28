#include "Engine.h"
#include "Graphics\\LightManager.h"

bool Engine::Initialize(HINSTANCE hInstance, uint16_t width, uint16_t height, std::string title, std::string class_)
{
	this->isRunning = true;
	timer.Start(); 

	if (!render_window.Initialize(this, hInstance, width, height, title, class_))
		return false;

	if (!gfx.Initialize(render_window.GetHwnd(), width, height))
		return false;

	return true;
}

bool Engine::ProcessMessages()
{
	return render_window.ProcessMessages();
}

void Engine::Update()
{
	float dt = static_cast<float>(timer.GetElapsedTime());
	timer.Restart();

	while (!keyboard.CharBufferIsEmpty())
	{
		unsigned char ch = keyboard.ReadChar();
	}

	while (!keyboard.KeyBufferIsEmpty())
	{
		KeyboardEvent kbe = keyboard.ReadKey();
		unsigned char keycode = kbe.GetKeyCode();
	}

	while (!mouse.EventBufferIsEmpty())
	{
		MouseEvent me = mouse.ReadEvent();

		if (me.GetType() == MouseEvent::EventType::RAW_MOVE) {
			this->gfx.camera.AdjustRotation((float)me.GetPos().y * 0.004f, (float)me.GetPos().x * 0.004f, 0);
		}
	}

	//this->gfx.gameObject.AdjustRotation(0.0f, 0.001f * dt, 0.0f);

	const float cameraSpeed = 0.5f * dt;

	if (keyboard.KeyIsPressed('W')) {
		this->gfx.camera.AdjustPosition(this->gfx.camera.GetForward() * cameraSpeed);
	}
	if (keyboard.KeyIsPressed('S')) {
		this->gfx.camera.AdjustPosition(-this->gfx.camera.GetForward() * cameraSpeed);
	}
	if (keyboard.KeyIsPressed('A')) {
		this->gfx.camera.AdjustPosition(-this->gfx.camera.GetRight() * cameraSpeed);
	}
	if (keyboard.KeyIsPressed('D')) {
		this->gfx.camera.AdjustPosition(this->gfx.camera.GetRight() * cameraSpeed);
	}
	if (keyboard.KeyIsPressed(VK_SPACE)) {
		this->gfx.camera.AdjustPosition(0.0f, cameraSpeed, 0.0f);
	}
	if (keyboard.KeyIsPressed('Z')) {
		this->gfx.camera.AdjustPosition(0.0f, -cameraSpeed, 0.0f);
	}
	if (keyboard.KeyIsPressed('C')) {
		Vector3 lightPosition = this->gfx.camera.GetPosition();
		lightPosition += this->gfx.camera.GetForward() * 4.f;
		LightManager::GetLight(0)->position = lightPosition;
	}

	if (keyboard.KeyIsPressed(VK_ESCAPE)) {
		this->isRunning = false;
	}
}

void Engine::Render()
{
	this->gfx.Render();
}

void Engine::Close()
{
	this->isRunning = false;
}

bool Engine::IsRunning() const
{
	return this->isRunning;
}
