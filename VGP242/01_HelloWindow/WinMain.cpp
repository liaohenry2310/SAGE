#include <Sage/Inc/Sage.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	Sage::Core::Window window;
	window.Initialize(hInstance, L"Hello Window", 1280, 720);

	bool running = true;
	while (running && window.IsActive())
	{
		window.ProcessMessage();

		if (GetAsyncKeyState(VK_ESCAPE))
		{
			running = false;
		}
	}

	window.Terminate();
	return 0;
}