#include "platform\CEPlatformConfig.h"
#if CE_TARGET_PLATFORM != CE_PLATFORM_WIN32

#include "platform\CEApplication.h"

NS_CE_BEGIN
Application* Application::s_pApplication = nullptr;
const UINT Application::BYTES_PER_PIXEL = 4;


//////////////////////////////////////////////////////////////////////////
// Ctor & Dtor
//////////////////////////////////////////////////////////////////////////

Application::Application()
{
	_appData.hInstance = GetModuleHandle(nullptr);

	WNDCLASSEX cWnd = {};
	cWnd.cbSize = sizeof(WNDCLASSEX);
	cWnd.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	cWnd.lpfnWndProc = WndCallbackStatic;
	cWnd.hInstance = _appData.hInstance;
	cWnd.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	cWnd.lpszClassName = "ChromeEngineClass";

	if (RegisterClassEx(&cWnd))
	{
		_appData.hwnd = CreateWindowEx(0,
			cWnd.lpszClassName,
			"Unnamed",
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT,
			0, 0, cWnd.hInstance, this);

		CE_ASSERT(_appData.hwnd);
		
		//SetWindowLongPtr(_appData.hwnd, GWL_USERDATA, (LONG_PTR)this);
		//SetWindowLongPtr(_appData.hwnd, GWL_WNDPROC, (LONG_PTR)&WndCallbackStatic);

		// TODO(youngjin) : I can't get nCmdShow from Winmain. Is there any way to deal with this problem?
		ShowWindow(_appData.hwnd, TRUE);
		UpdateWindow(_appData.hwnd);
	}
	else
	{

	}
}

Application::~Application()
{
	CE_ASSERT(this == s_pApplication);
	s_pApplication = nullptr;
}


//////////////////////////////////////////////////////////////////////////
// Public member function
//////////////////////////////////////////////////////////////////////////


void Application::Update()
{
	Resize(1280, 720);
	while (!_appData.quit)
	{
		MSG msg = {};

		//while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		if(GetMessage(&msg, NULL, NULL, NULL))
		{
			//UpdateWindow(_appData.hwnd);

			if (msg.message == WM_QUIT)
				_appData.quit = true;

			TranslateMessage(&msg);
			DispatchMessage(&msg);

		//	if (_appData.quit)
		//		break;
		}
		HDC hdc = GetDC(_appData.hwnd);

		// TODO(youngjin) : It's messy. this need to optimize.
		RECT clientWndRect;
		GetClientRect(_appData.hwnd, &clientWndRect);

		InvalidateRect(_appData.hwnd, &clientWndRect, FALSE);

		StretchDIBits(hdc,
			0, 0, _appData.width, _appData.height,
			0, 0, clientWndRect.right - clientWndRect.left, clientWndRect.bottom - clientWndRect.top,
			_appData.pBuffer, &_appData.bufferInfo,
			DIB_RGB_COLORS, SRCCOPY);

		ReleaseDC(_appData.hwnd, hdc);
	}
}


void Application::Resize(const UINT& width, const UINT& height)
{
	if (_appData.pBuffer)
		VirtualFree(_appData.pBuffer, 0, MEM_RELEASE);

	_appData.width = width;
	_appData.height = height;
	_appData.pitch = width * BYTES_PER_PIXEL;


	_appData.bufferInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	_appData.bufferInfo.bmiHeader.biWidth = width;
	_appData.bufferInfo.bmiHeader.biHeight = -height; // negative for the top-down
	_appData.bufferInfo.bmiHeader.biPlanes = 1;
	_appData.bufferInfo.bmiHeader.biBitCount = 32;
	_appData.bufferInfo.bmiHeader.biCompression = BI_RGB;

	// TODO(youngjin) : Find a difference between HeapAlloc and VirtualAlloc function.
	_appData.pBuffer = VirtualAlloc(0, _appData.height * _appData.pitch,
		MEM_COMMIT, PAGE_READWRITE);
}

//////////////////////////////////////////////////////////////////////////
// Protected member function
//////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK Application::WndCallback(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	switch (msg)
	{
		case WM_DESTROY:
		{
			_appData.quit = true;
		}break;
		case WM_CLOSE:
		{
			_appData.quit = true;
		}break;
		case WM_PAINT:
		{
			PAINTSTRUCT paint;
			_appData.hdc = BeginPaint(_appData.hwnd, &paint);


			EndPaint(_appData.hwnd, &paint);
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
		default:
		{
			result = DefWindowProc(hwnd, msg, wParam, lParam);
		}break;
	}

	return result;
}


//////////////////////////////////////////////////////////////////////////
// Static member function
//////////////////////////////////////////////////////////////////////////
Application* Application::GetInstance()
{
	if (!s_pApplication)
	{
		s_pApplication = new Application;
	}

	CE_ASSERT(s_pApplication);
	return s_pApplication;
}

LRESULT CALLBACK Application::WndCallbackStatic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Application* pApp = (Application*)(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	if (pApp)
		pApp->WndCallback(hwnd, msg, wParam, lParam);

	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////
// Local member function
//////////////////////////////////////////////////////////////////////////




NS_CE_END

#endif // CE_TARGET_PLATFORM == CE_PLATFORM_WIN32