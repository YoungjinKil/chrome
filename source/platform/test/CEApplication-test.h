#ifndef CE_APPLICATION_TEST_H
#define CE_APPLICATION_TEST_H

#include "platform\CEPlatformConfig.h"

#if CE_TARGET_PLATFORM == CE_PLATFORM_WIN32

#include "platform\CEPlatformMacro.h"
#include <Windows.h>
#include <string>

NS_CE_BEGIN

class Application{

	struct AppData
	{
		AppData() : hInstance(0), hwnd(0), pBMMemory(0),
		width(0), height(0), pitch(0), quit(0){}

		HINSTANCE hInstance;
		HWND hwnd;

		BITMAPINFO bmInfo;
		void * pBMMemory;

		RECT wndRect;
		UINT width;
		UINT height;
		UINT pitch;

		bool quit;
	};

public:
	virtual ~Application();
	
	static Application* GetInstance();

	void Init();
	void Update();
	void Shutdown();

protected:
	Application();
	void operator=(const Application&);

	static LRESULT CALLBACK WndProcInitialize(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK WndProcUpdate(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	static Application* s_pApplication;
	static const UINT BYTES_PER_PIXEL;

	AppData _appData;

	// Fucntions For Test
	void TestPaint();
	void TestSize();

};

NS_CE_END

#endif // CE_TARGET_PLATFORM == CE_PLATFORM_WIN32

#endif // CE_APPLICATION_TEST_H