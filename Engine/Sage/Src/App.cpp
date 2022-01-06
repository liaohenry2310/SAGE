#include "Precompiled.h"
#include "App.h"
#include "AppState.h"

using namespace Sage;
using namespace Sage::Core;
using namespace Sage::Graphics;
using namespace Sage::Input;

void Sage::App::ChangeState(const std::string& name)
{
	if (auto iter = mAppStates.find(name); iter != mAppStates.end())
	{
		LOG("App -- Changing state: %s", name.c_str());
		mNextState = iter->second.get();
	}
}

void App::Run(AppConfig config)
{
	LOG("App -- Running ...");

	//Setup application window
	LOG("App -- Creating window ...");
	mWindow.Initialize(GetModuleHandle(NULL), config.appName, config.appWidth, config.appLength);
	auto handle = mWindow.GetWindowHandle();

	//Initialize the input system
	InputSystem::StaticInitialize(handle);

	//Initialize the graphics systems
	GraphicsSystem::StaticInitialize(handle, false);
	SimpleDraw::StaticInitialize(config.debugDrawLimit);
	DebugUI::StaticInitialize(handle, false, true);

	//Initialize the starting state
	ASSERT(mCurrentState, "App -- No app state found! App must have at least one AppState.");
	mCurrentState->Initialize();
	

	LOG("App -- Entering main loop ...");

	mRunning = true;
	while (mRunning && mWindow.IsActive())
	{
		mWindow.ProcessMessage();

		if (!mWindow.IsActive())
		{
			LOG("App -- Window destroyed. Exiting app ...");
			Quit();
			continue;
		}

		if (mNextState)
		{
			LOG("App -- Changing app state ...");
			mCurrentState->Terminate();
			mCurrentState = std::exchange(mNextState, nullptr);
			mCurrentState->Initialize();
		}

		auto inputSystem = InputSystem::Get();
		inputSystem->Update();

		if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
		{
			LOG("App -- ESC pressed. Exiting app ...");
			Quit();
			continue;
		}

		auto deltaTime = TimeUtil::GetDeltaTime();
		mCurrentState->Update(deltaTime);

		GraphicsSystem::Get()->BeginRender();
		mCurrentState->Render();

		DebugUI::BeginRender();
		mCurrentState->DebugUI();
		DebugUI::EndRender();

		GraphicsSystem::Get()->EndRender();
	}

	LOG("App -- Shutting down ...");

	mCurrentState->Terminate();

	DebugUI::StaticTerminate();
	SimpleDraw::StaticTerminate();
	GraphicsSystem::Get()->StaticTerminate();
	InputSystem::Get()->StaticTerminate();

	mWindow.Terminate();

	LOG("App -- Exiting ...");
}
