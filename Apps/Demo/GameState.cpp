#include "GameState.h"

#include "PlayerControllerComponent.h"

using namespace Sage;
using namespace Sage::Graphics;
using namespace Sage::Input;
using namespace Sage::Math;

namespace
{
	bool OnMake(const char* componentName, const rapidjson::Value& value, GameObject& gameObject)
	{
		if (strcmp(componentName, "PlayerControllerComponent") == 0)
		{
			auto transformComponent = gameObject.AddComponent<PlayerControllerComponent>();
			if (value.HasMember("WalkSpeed"))
			{
				const float walkSpeed = value["WalkSpeed"].GetFloat();
				transformComponent->SetWalkSpeed(walkSpeed);
			}
			if (value.HasMember("RunSpeed"))
			{
				const float runSpeed = value["RunSpeed"].GetFloat();
				transformComponent->SetRunSpeed(runSpeed);
			}
			return true;
		}
		return false;
	}
}


void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mGameWorld.AddService<CameraService>();
	mGameWorld.AddService<RenderService>();
	mGameWorld.AddService<TerrainService>();
	mGameWorld.Initialize(1000);

	GameObjectFactory::SetMakeOverride(OnMake);
	mGameWorld.CreateGameObject("player.json")->SetName("Player");
	mGameWorld.CreateGameObject("../../Assets/Templates/follow_camera.json");

	auto player = mGameWorld.FindGameObject("Player");
	player->GetComponent<TransformComponent>()->position = { 75.0f, 0.0f, 75.0f };
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