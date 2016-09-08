#ifndef CE_APPLICATION_TEST_H
#define CE_APPLICATION_TEST_H

#include "platform\CEPlatformConfig.h"

#if CE_TARGET_PLATFORM == CE_PLATFORM_WIN32

#include "platform\CEPlatformMacro.h"
#include <Windows.h>
#include <string>

NS_CE_BEGIN

class Application{
public:
	virtual ~Application();
	
	static Application* GetInstance();

	void Init();
	void Update();
	void Shutdown();

	// Fucntions For Test
	void TestPaint();
	void TestSize(const UINT& lParam);
protected:
	Application();
	void operator=(const Application&);

	static LRESULT CALLBACK WndProcStatic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	static Application* s_pApplication;

	HINSTANCE _hInstance;
	HDC _hdc;
	HBITMAP _hbm;
	HWND _hwnd;
	HANDLE _hOld;

	RECT _wndRect;
	UINT _width;
	UINT _height;
	UINT _pitch;

	bool _quit;

};

NS_CE_END

#endif // CE_TARGET_PLATFORM == CE_PLATFORM_WIN32

#endif // CE_APPLICATION_TEST_H