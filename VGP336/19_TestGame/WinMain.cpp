#include "GameState.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	Sage::AppConfig config{L"Hello Physics", 1280, 720, 999999u };

	Sage::MainApp().AddState<GameState>("main");
	Sage::MainApp().Run(config);
	return 0;
}