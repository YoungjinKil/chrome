#include "CEApplication-test.h"

NS_CE_BEGIN

Application* Application::s_pApplication = nullptr;

Application::Application()
	: _hInstance(0), _hdc(0), _hbm(0), _hwnd(0),
	_width(0), _height(0), _pitch(0), _quit(false)
{
	Init();
}

Application::~Application()
{
	CE_ASSERT(s_pApplication == this);
	s_pApplication = nullptr;
}

Application* Application::GetInstance()
{
	if (!s_pApplication)
		s_pApplication = new Application;

	CE_ASSERT(s_pApplication);

	return s_pApplication;
}


void Application::Init()
{
	// Is it enough to get just once?
	_hInstance = GetModuleHandle(nullptr);

	WNDCLASSEX cWnd = {};
	cWnd.cbSize = sizeof(WNDCLASSEX);
	cWnd.style = CS_HREDRAW | CS_VREDRAW;
	cWnd.lpfnWndProc = WndProcStatic;
	cWnd.hInstance = _hInstance;
	cWnd.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	cWnd.lpszClassName = "ChromeEngineClass";

	if (RegisterClassEx(&cWnd))
	{
		_hwnd = CreateWindowEx(0,
			cWnd.lpszClassName,
			"Unnammed",
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT,
			0, 0, cWnd.hInstance, 
			this);// Or NULL ???

		CE_ASSERT(_hwnd);

		GetWindowRect(GetDesktopWindow(), &_wndRect);
		_width = _wndRect.right - _wndRect.left;
		_height = _wndRect.bottom - _wndRect.top;

		ShowWindow(_hwnd, TRUE);
		UpdateWindow(_hwnd);
	}
}
void Application::Update()
{
	MSG msg = {};

	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		InvalidateRect(_hwnd, &_wndRect, FALSE);
	}
}
void Application::Shutdown()
{
	
}

LRESULT CALLBACK Application::WndProcStatic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
	{
		Application::GetInstance()->TestPaint();
	}break;
	case WM_ERASEBKGND:
	{
		return 1;
	}break;
	case WM_SIZE:
	{
		Application::GetInstance()->TestSize(lParam);
	}break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}break;
	default:
	{
		DefWindowProc(hwnd, msg, wParam, lParam);
	}break;
	}
}


void Application::TestPaint()
{
	PAINTSTRUCT paint;
	HDC hdc;

	hdc = BeginPaint(_hwnd, &paint);

	if (!_hbm)
	{
		_hdc = CreateCompatibleDC(hdc);
		_hbm = CreateCompatibleBitmap(hdc, _width, _height);
		_hOld = SelectObject(_hdc, _hbm);
	}

	// Fill the double buffer with a color use
	FillRect(_hdc, &_wndRect, (HBRUSH)COLOR_MENU);

	// Draw hdc
	// Update(_hdc);

	// Transfer the offscreen DC to the screen
	BitBlt(hdc, 0, 0, _width, _height, _hdc, 0, 0, SRCCOPY);
	/*StretchDIBits(hdc,
		0, 0, _width, _height,
		)*/

	EndPaint(_hwnd, &paint);
}
void Application::TestSize(const UINT& lParam)
{
	_wndRect.right = LOWORD(lParam);
	_wndRect.bottom = HIWORD(lParam);

	//GetWindowRect(GetDesktopWindow(), &_wndRect);
	_width = _wndRect.right - _wndRect.left;
	_height = _wndRect.bottom - _wndRect.top;

	if(_hbm)
	{
		DeleteObject(_hbm);
		_hbm = NULL;
	}
	if (_hdc)
	{
		DeleteDC(_hdc);
		_hdc = NULL;
	}
}

NS_CE_END