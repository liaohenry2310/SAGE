#pragma once

#include "Common.h"

//App headers
#include "App.h"
#include "AppState.h"

//Component headers
#include "AnimatorComponent.h"
#include "CameraComponent.h"
#include "ColliderComponent.h"
#include "FollowCameraControllerComponent.h"
#include "FPSCameraControllerComponent.h"
#include "ModelComponent.h"
#include "ParticleEmitterComponent.h"
#include "TransformComponent.h"

//Service headers
#include "CameraService.h"
#include "RenderService.h"
#include "TerrainService.h"

//World headers
#include "Component.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "GameObjectHandle.h"
#include "GameWorld.h"
#include "Service.h"

namespace Sage
{
	App& MainApp();
}