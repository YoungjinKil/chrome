#include <Windows.h>
#include "platform\CEApplication.h"


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR cmdLine, int showCode)
{

	NS_CE::Application::GetInstance()->Run();

	return 0;
}