#include "Precompiled.h"
#include "CameraComponent.h"

#include "CameraService.h"
#include "GameObject.h"
#include "GameWorld.h"

using namespace Sage;

MEMORY_POOL_DEFINE(CameraComponent, 1000);

void Sage::CameraComponent::Initialize()
{
	auto cameraService = GetOwner().GetWorld().GetService<CameraService>();
	cameraService->Register(this);

	mCamera.SetPosition({ 1.0f, 1.0f, -5.0f });
	mCamera.LookAt({ 0.0f, 0.0f, 0.0f });
}

void Sage::CameraComponent::Terminate()
{
	auto cameraService = GetOwner().GetWorld().GetService<CameraService>();
	cameraService->Unregister(this);
}
