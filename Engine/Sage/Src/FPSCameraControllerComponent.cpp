#include "Precompiled.h"
#include "FPSCameraControllerComponent.h"

#include "CameraComponent.h"
#include "GameObject.h"

using namespace Sage;
using namespace Sage::Math;
using namespace Sage::Input;

MEMORY_POOL_DEFINE(FPSCameraControllerComponent, 1000);

void Sage::FPSCameraControllerComponent::Initialize()
{
	mCameraComponent = GetOwner().GetComponent<CameraComponent>();
}

void Sage::FPSCameraControllerComponent::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	auto& camera = mCameraComponent->GetCamera();

	const float moveSpeed = 10.0f;
	const float rotateSpeed = 100.0f * Constants::DegToRad;
	const float turnSpeed = 30.0f * Constants::DegToRad;
	if (inputSystem->IsKeyDown(KeyCode::W))
		camera.Walk(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		camera.Walk(-moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		camera.Strafe(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		camera.Strafe(-moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::E))
		camera.Rise(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::Q))
		camera.Rise(-moveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		camera.Yaw(inputSystem->GetMouseMoveX() * turnSpeed * deltaTime);
		camera.Pitch(inputSystem->GetMouseMoveY() * turnSpeed * deltaTime);
	}
}
