#include "pch.h"
#include "srcs/GameApp.h"
using namespace DW;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
		PSTR cmdLine, int showCmd)
{
	// not use
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(cmdLine);
	UNREFERENCED_PARAMETER(showCmd);
	// 允许在Debug版本进行运行时内存分配和泄漏检测
#if defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	GameApp app(hInstance);

	if (!app.Init())
	{
		return 1;
	}
	return app.Run();
}

