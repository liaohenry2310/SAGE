#include <Sage/Inc/Sage.h>

class TealState : public Sage::AppState
{
	void Initialize() override 
	{
		Sage::Graphics::GraphicsSystem::Get()->SetClearColor(Sage::Graphics::Colors::Teal);
	}

	void Update(float deltaTime) override 
	{
		if (Sage::Input::InputSystem::Get()->IsKeyPressed(Sage::Input::KeyCode::SPACE))
		{
			Sage::MainApp().ChangeState("OrangeState");
		}
	}

	void Render() override {}
};

class OrangeState : public Sage::AppState
{
	void Initialize() override
	{
		Sage::Graphics::GraphicsSystem::Get()->SetClearColor(Sage::Graphics::Colors::Orange);
	}

	void Update(float deltaTime) override
	{
		if (Sage::Input::InputSystem::Get()->IsKeyPressed(Sage::Input::KeyCode::SPACE))
		{
			Sage::MainApp().ChangeState("TealState");
		}
	}

	void Render() override {}
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	Sage::AppConfig config{L"Hellow DirectX", 1280, 720};
	Sage::MainApp().AddState<TealState>("TealState");
	Sage::MainApp().AddState<OrangeState>("OrangeState");
	Sage::MainApp().Run(config);
	return 0;
}