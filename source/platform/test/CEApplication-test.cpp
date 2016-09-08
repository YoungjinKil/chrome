#include "CEApplication-test.h"

NS_CE_BEGIN

Application* Application::s_pApplication = nullptr;


//////////////////////////////////////////////////////////////////////////
// Ctor & Dtor
//////////////////////////////////////////////////////////////////////////

Application::Application()
	: _hInstance(0), _hdc(0), _hbm(0), _hwnd(0),
	_pBMMemory(0),
	_width(0), _height(0), _pitch(0), _quit(false)
{
}

Application::~Application()
{
	CE_ASSERT(s_pApplication == this);
	s_pApplication = nullptr;
}

//////////////////////////////////////////////////////////////////////////
// Public member function
//////////////////////////////////////////////////////////////////////////

void Application::QuitApp()
{	
	_quit = true;
}


void Application::Init()
{
	// Is it enough to get just once?
	_hInstance = GetModuleHandle(nullptr);

	WNDCLASSEX cWnd = {};
	cWnd.cbSize = sizeof(WNDCLASSEX);
	cWnd.style = CS_HREDRAW | CS_VREDRAW;
	cWnd.lpfnWndProc = WndProcInitialize;
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

		SetWindowLongPtr(_hwnd, GWL_USERDATA, (LONG_PTR)this);
		SetWindowLongPtr(_hwnd, GWL_WNDPROC, (LONG_PTR)&WndProcInitialize);

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

	while (!_quit)
	{
		while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE) > 0)
		{
			if (msg.message == WM_QUIT)
				_quit = true;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		InvalidateRect(_hwnd, &_wndRect, FALSE);
	}
}
void Application::Shutdown()
{

}
LRESULT CALLBACK Application::WndProcUpdate(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	switch (msg)
	{
	case WM_PAINT:
	{
		TestPaint();
	}break;
	case WM_ERASEBKGND:
	{
		return 1;
	}break;
	case WM_SIZE:
	{
		TestSize(lParam);
	}break;
	case WM_DESTROY:
	{
		QuitApp();
	}break;
	case WM_CLOSE:
	{
		QuitApp();
	}break;
	default:
	{
		result = DefWindowProc(hwnd, msg, wParam, lParam);
	}break;
	}

	return result;
}

//////////////////////////////////////////////////////////////////////////
// Private member function
//////////////////////////////////////////////////////////////////////////
void Application::TestPaint()
{
	PAINTSTRUCT paint;
	HDC hdc;

	hdc = BeginPaint(_hwnd, &paint);

	if (!_hbm)
	{
		_hdc = CreateCompatibleDC(hdc);
		_hbm = CreateCompatibleBitmap(hdc, _width, _height);
	}

	// Fill the double buffer with a color use
	FillRect(hdc, &_wndRect, (HBRUSH)COLOR_MENU);

	// Draw hdc
	// Update(_hdc);

	// Transfer the offscreen DC to the screen
	BitBlt(hdc, 0, 0, _width, _height, _hdc, 0, 0, SRCCOPY);
	/*StretchDIBits(_hdc,
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

	if (_hbm)
	{
		DeleteObject(_hbm);
		_hbm = NULL;
	}
	if (_hdc)
	{
		DeleteDC(_hdc);
		_hdc = NULL;
	}

	_bmInfo.bmiHeader.biSize = sizeof(_bmInfo.bmiHeader);
	_bmInfo.bmiHeader.biWidth = _width;
	_bmInfo.bmiHeader.biHeight = _height;
	_bmInfo.bmiHeader.biPlanes = 1;
	_bmInfo.bmiHeader.biBitCount = 32;
	_bmInfo.bmiHeader.biCompression = BI_RGB;

}

//////////////////////////////////////////////////////////////////////////
// Static member function
//////////////////////////////////////////////////////////////////////////

Application* Application::GetInstance()
{
	if (!s_pApplication)
	{
		s_pApplication = new Application;
		s_pApplication->Init();
	}

	CE_ASSERT(s_pApplication);

	return s_pApplication;
}

LRESULT CALLBACK Application::WndProcInitialize(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Application* pApp = (Application*)(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	if (pApp)
		pApp->WndProcUpdate(hwnd, msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}


//////////////////////////////////////////////////////////////////////////
// Local member function
//////////////////////////////////////////////////////////////////////////

NS_CE_END