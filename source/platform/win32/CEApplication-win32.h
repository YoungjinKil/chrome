#ifndef CE_APPLICATION_WIN32_H
#define CE_APPLICATION_WIN32_H

#include "platform/CEPlatformConfig.h"

//#if CE_TARGET_PLATFORM == CE_PLATFORM_WIN32
#if CE_TARGET_PLATFORM == CE_PLATFORM_WIN32

#include "platform\CEPlatformMacro.h"
#include <string>
#include <windows.h>

NS_CE_BEGIN

struct AppData
{
	AppData() :hwnd(0), hInstance(0), hdc(0), width(0), height(0), pitch(0), title(0), fullscreen(0), quit(0) {}

	HWND hwnd;
	HINSTANCE hInstance;
	HDC hdc;

	// TODO(youngjin) : I think it's not a good idea to deal this in here.
	//					Figure out that how to deal the buffer (or bitmap - I don't know what the different is)
	//					between win api and graphic engine such as OpenGL, DirectX
	BITMAPINFO bufferInfo;
	void* pBuffer;

	UINT width;
	UINT height;
	UINT pitch;

	const char* title;

	bool fullscreen;
	bool quit;
};

class Application
{
public:
	virtual ~Application();

	//void Init();
	void Update();
	//void Shutdown();

	static Application* GetInstance();
	void Resize(const UINT& width, const UINT& height);

private:
	Application();
	void operator=(const Application&);

	static LRESULT CALLBACK WndCallbackStatic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK WndCallback(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	static Application* s_pApplication;

	static const UINT BYTES_PER_PIXEL;

	AppData _appData;
};
NS_CE_END


#endif // CE_TARGET_PLATFORM == CE_PLATFORM_WIN32

#endif // CE_APPLICATION_WIN32_H