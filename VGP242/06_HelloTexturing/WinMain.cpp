#include "GameState.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	Sage::AppConfig config{L"Hello Texturing", 1280, 720 };

	Sage::MainApp().AddState<GameState>("main");
	Sage::MainApp().Run(config);
	return 0;
}