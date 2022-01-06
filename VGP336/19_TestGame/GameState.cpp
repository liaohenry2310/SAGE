#include "GameState.h"

using namespace Sage;
using namespace Sage::Graphics;
using namespace Sage::Input;
using namespace Sage::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mGameWorld.AddService<CameraService>();
	mGameWorld.AddService<RenderService>();
	mGameWorld.AddService<TerrainService>();
	mGameWorld.Initialize(1000);

	mGameWorld.CreateGameObject("../../Assets/Templates/fps_camera.json");
	mGameObjectHandle = mGameWorld.CreateGameObject("../../Assets/Templates/test.json")->GetHandle();
	mGameWorld.CreateGameObject("../../Assets/Templates/player.json");
}

void GameState::Terminate()
{
	mGameWorld.Terminate();
}

void GameState::Update(float deltaTime)
{
	mGameWorld.Update(deltaTime);
}

void GameState::Render()
{
	mGameWorld.Render();
}

void GameState::DebugUI()
{
	mGameWorld.DebugUI();
}