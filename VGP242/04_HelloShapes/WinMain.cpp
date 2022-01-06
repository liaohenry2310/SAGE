#include "GameState.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	Sage::AppConfig config{L"Hello Shapes", 1280, 1280 };

	Sage::MainApp().AddState<GameState>("main");
	Sage::MainApp().Run(config);
	return 0;
}