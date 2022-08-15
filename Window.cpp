#include "WindowContainer.h"
#include "Logger.h"

LRESULT CALLBACK HandleMsgRedirect(HWND hwnd_, UINT uMsg_, WPARAM wParam_, LPARAM lParam_)
{
	switch (uMsg_)
	{
	case WM_CLOSE:
	{
		DestroyWindow(hwnd_);
		return 0;
	}
	default:
	{
		WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd_, GWLP_USERDATA));

		return pWindow->WindowProc(hwnd_, uMsg_, wParam_, lParam_);
	}
	}
}

LRESULT CALLBACK HandleMsgSetup(HWND hwnd_, UINT uMsg_, WPARAM wParam_, LPARAM lParam_)
{
	switch (uMsg_)
	{
	case WM_NCCREATE:
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam_);
		WindowContainer* pWindow = reinterpret_cast<WindowContainer*>(pCreate->lpCreateParams);
		if (pWindow == nullptr)
		{
			Logger::Log("Critical Error: Pointer to window container is null during WM_NCCREATE!");
			exit(EXIT_FAILURE);
		}
		SetWindowLongPtr(hwnd_, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
		SetWindowLongPtr(hwnd_, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));

		return pWindow->WindowProc(hwnd_, uMsg_, wParam_, lParam_);
	}
	default:
		return DefWindowProc(hwnd_, uMsg_, wParam_, lParam_);
	}
}

void Window::RegisterWindowClass()
{
	WNDCLASSEX wc = {};
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = windowClassW.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);
	RegisterClassEx(&wc);
}

Window::~Window()
{
	if (hwnd != NULL)
	{
		UnregisterClass(windowClassW.c_str(), hInstance);
		DestroyWindow(hwnd);
	}
}

bool Window::Initialize(WindowContainer* pWindowContainer, HINSTANCE hInstance_, uint16_t width_, uint16_t height_, std::string title_, std::string class_)
{
	hInstance = hInstance_;
	width = width_;
	height = height_;
	title = title_;
	titleW = StringHelper::StringToWide(title_);
	windowClassW = StringHelper::StringToWide(class_);
	windowClass = class_;

	RegisterWindowClass();

	int centerScreenX = GetSystemMetrics(SM_CXSCREEN) / 2 - this->width / 2;
	int centerScreenY = GetSystemMetrics(SM_CYSCREEN) / 2 - this->height / 2;

	RECT wr;
	wr.left = centerScreenX;
	wr.top = centerScreenY;
	wr.right = wr.left + this->width;
	wr.bottom = wr.top + this->height;

	DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_SIZEBOX;

	AdjustWindowRect(&wr, style, FALSE);


	hwnd = CreateWindowEx(0, //Extended Windows style - we are using the default. For other options 
		windowClassW.c_str(), // Window class name
		titleW.c_str(), // Window Title
		style, // Windows style
		wr.left, // Window X pos
		wr.top, // Window Y pos
		wr.right - wr.left, // window width
		wr.bottom - wr.top, // window height
		NULL, // Handle to parent of THIS window, first window, need no parent
		NULL, // Handle to menu or child window identifier. Can be set to NULL and use menu in WindowClassEx
		hInstance,
		pWindowContainer);

	if (hwnd == NULL)
	{
		DWORD error = GetLastError();
		return false;
	}

	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	return true;
}

bool Window::ProcessMessages()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (PeekMessage(&msg,
		hwnd,
		0,
		0,
		PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_NULL)
	{
		if (!IsWindow(hwnd))
		{
			hwnd = NULL;
			UnregisterClass(windowClassW.c_str(), hInstance);
			return false;
		}
	}

	return true;
}

HWND Window::GetHwnd() const
{
	return hwnd;
}

uint16_t Window::GetWidth() const
{
	return width;
}

uint16_t Window::GetHeight() const
{
	return height;
}

void Window::SetWidth(const uint16_t& width)
{
	this->width = width;
}

void Window::SetHeight(const uint16_t& height)
{
	this->height = height;
}

void Window::ConfineCursor() const
{
	RECT rect;
	GetClientRect(this->hwnd, &rect);
	MapWindowPoints(this->hwnd, nullptr, reinterpret_cast<POINT*>(&rect), 2);
	ClipCursor(&rect);
}

void Window::FreeCursor() const
{
	ClipCursor(nullptr);
}
