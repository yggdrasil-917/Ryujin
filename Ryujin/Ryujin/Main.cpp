

#include "RyujinApp.hpp"


#if PLATFORM_WINDOWS
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	Ryujin::Engine engine;
	engine.Run(__argc, __argv);

	return 0;
}
#else
int main(int argc, char** argv)
{
	Ryujin::Engine engine;
#if PLATFORM_MAC
	Ryujin::RunApplication(engine, argc, argv);
#else
	engine.Run(argc, argv);
#endif

	return 0;
}
#endif
