#include "CEApplication-test.h"

NS_CE_BEGIN

Application* Application::s_pApplication = nullptr;
const UINT Application::BYTES_PER_PIXEL = 4;


//////////////////////////////////////////////////////////////////////////
// Ctor & Dtor
//////////////////////////////////////////////////////////////////////////

Application::Application()
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

void Application::Run()
{
	MSG msg = {};

	TestSize();

	while (!_appData.quit)
	{
		while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				_appData.quit = true;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		InvalidateRect(_appData.hwnd, &_appData.wndRect, FALSE);
	}
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
	case WM_KEYDOWN:
	case WM_KEYUP:
	{

	}break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEMOVE:
	{

	}break;
	case WM_ERASEBKGND:
	{
		return 1;
	}break;
	case WM_SIZE:
	{
		TestSize();
	}break;
	case WM_DESTROY:
	{
		_appData.quit = true;
	}break;
	case WM_CLOSE:
	{
		_appData.quit = true;
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

void Application::Initialize()
{
	// Is it enough to get just once?
	_appData.hInstance = GetModuleHandle(nullptr);

	WNDCLASSEX cWnd = {};
	cWnd.cbSize = sizeof(WNDCLASSEX);
	cWnd.style = CS_HREDRAW | CS_VREDRAW;
	cWnd.lpfnWndProc = WndProcInitialize;
	cWnd.hInstance = _appData.hInstance;
	cWnd.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	cWnd.lpszClassName = "ChromeEngineClass";

	if (RegisterClassEx(&cWnd))
	{
		_appData.hwnd = CreateWindowEx(0,
			cWnd.lpszClassName,
			"Unnammed",
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			//WS_CAPTION | WS_VISIBLE,
			CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT,
			0, 0, cWnd.hInstance,
			this);// Or NULL ???

		CE_ASSERT(_appData.hwnd);

		SetWindowLongPtr(_appData.hwnd, GWL_USERDATA, (LONG_PTR)this);
		//SetWindowLongPtr(_appData.hwnd, GWL_WNDPROC, (LONG_PTR)&WndProcInitialize);

		GetWindowRect(GetDesktopWindow(), &_appData.wndRect);
		_appData.width = _appData.wndRect.right - _appData.wndRect.left;
		_appData.height = _appData.wndRect.bottom - _appData.wndRect.top;


		ShowWindow(_appData.hwnd, TRUE);
		UpdateWindow(_appData.hwnd);
	}
}

//////////////////////////////////////////////////////////////////////////
// Static member function
//////////////////////////////////////////////////////////////////////////

Application* Application::GetInstance()
{
	if (!s_pApplication)
	{
		s_pApplication = new Application;
		s_pApplication->Initialize();
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

void Application::TestPaint()
{
	PAINTSTRUCT paint;
	HDC hdc;

	hdc = BeginPaint(_appData.hwnd, &paint);

	GetClientRect(_appData.hwnd, &_appData.wndRect);


	// Fill the double buffer with a color use
	//FillRect(hdc, &_appData.wndRect, (HBRUSH)COLOR_MENU);

	// Draw hdc
	// Update(_hdc);

	// Transfer the offscreen DC to the screen
	StretchDIBits(hdc,
		0, 0, _appData.width, _appData.height,
		0, 0, _appData.wndRect.right - _appData.wndRect.left,
		_appData.wndRect.bottom - _appData.wndRect.top,
		_appData.pBMMemory, &_appData.bmInfo, DIB_RGB_COLORS, SRCCOPY);

	EndPaint(_appData.hwnd, &paint);
}
void Application::TestSize()
{
	GetClientRect(_appData.hwnd, &_appData.wndRect);

	if (_appData.pBMMemory)
		VirtualFree(_appData.pBMMemory, 0, MEM_RELEASE);

	_appData.width = _appData.wndRect.right - _appData.wndRect.left;
	_appData.height = _appData.wndRect.bottom - _appData.wndRect.top;

	_appData.bmInfo.bmiHeader.biSize = sizeof(_appData.bmInfo.bmiHeader);
	_appData.bmInfo.bmiHeader.biWidth = _appData.width;
	_appData.bmInfo.bmiHeader.biHeight = _appData.height;
	_appData.bmInfo.bmiHeader.biPlanes = 1;
	_appData.bmInfo.bmiHeader.biBitCount = 32;
	_appData.bmInfo.bmiHeader.biCompression = BI_RGB;

	int bmMemSize = _appData.width*_appData.height*BYTES_PER_PIXEL;
	_appData.pBMMemory = VirtualAlloc(0, bmMemSize, MEM_COMMIT, PAGE_READWRITE);
}
NS_CE_END