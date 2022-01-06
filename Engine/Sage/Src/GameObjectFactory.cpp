#include "Precompiled.h"
#include "GameObjectFactory.h"

#include "GameObject.h"

#include "AnimatorComponent.h"
#include "CameraComponent.h"
#include "ColliderComponent.h"
#include "FollowCameraControllerComponent.h"
#include "FPSCameraControllerComponent.h"
#include "ModelComponent.h"
#include "ParticleEmitterComponent.h"
#include "TransformComponent.h"

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

using namespace Sage;

namespace rj = rapidjson;

namespace
{
	MakeOverride OnMake;
}

void Sage::GameObjectFactory::SetMakeOverride(MakeOverride makeOverride)
{
	OnMake = makeOverride;
}

void Sage::GameObjectFactory::Make(std::filesystem::path templateFile, GameObject& gameObject)
{
	FILE* file = nullptr;
	auto err = fopen_s(&file, templateFile.u8string().c_str(), "r");
	ASSERT(err == 0 && file != nullptr, "GameObjectFactory -- Failed to open template file '%s'", templateFile.u8string().c_str());

	char readBuffer[65536];
	rj::FileReadStream readStream(file, readBuffer, sizeof(readBuffer));
	fclose(file);

	rj::Document document;
	document.ParseStream(readStream);

	auto components = document["Components"].GetObj();
	for (auto& component : components)
	{
		const char* componentName = component.name.GetString();
		if (OnMake(componentName, component.value, gameObject))
			continue;

		if (strcmp(componentName, "AnimatorComponent") == 0)
		{
			const char* componentName = component.name.GetString();
			if (strcmp(componentName, "AnimatorComponent") == 0)
			{
				auto animatorComponent = gameObject.AddComponent<AnimatorComponent>();
				if (component.value.HasMember("AnimSets"))
				{
					const auto& animSets = component.value["AnimSets"].GetArray();
					for (auto& animSet : animSets)
						animatorComponent->AddAnimation(animSet.GetString());
				}
			}
		}
		else if (strcmp(componentName, "CameraComponent") == 0)
		{
			auto camera = gameObject.AddComponent<CameraComponent>();
			//TODO
			// - Read position, direction, look at
			// - Read mode
			// - If perspective, read fov, aspect ratio
			// - if orthographic, read size
			// - read near/far planes
			// - read zoom
		}
		else if (strcmp(componentName, "ColliderComponent") == 0)
		{
			auto collider = gameObject.AddComponent<ColliderComponent>();
			if (component.value.HasMember("Center"))
			{
				const auto& center = component.value["Center"].GetArray();
				const float x = center[0].GetFloat();
				const float y = center[1].GetFloat();
				const float z = center[2].GetFloat();
				collider->SetCenter({ x, y, z });
			}
			if (component.value.HasMember("Extend"))
			{
				const auto& extend = component.value["Extend"].GetArray();
				const float x = extend[0].GetFloat();
				const float y = extend[1].GetFloat();
				const float z = extend[2].GetFloat();
				collider->SetExtend({ x, y, z });
			}
		}
		else if (strcmp(componentName, "FollowCameraControllerComponent") == 0)
		{
			auto followCameraComponent = gameObject.AddComponent<FollowCameraControllerComponent>();
			if (component.value.HasMember("Target"))
			{
				followCameraComponent->SetTarget(component.value["Target"].GetString());
			}
			if (component.value.HasMember("Distance"))
			{
				followCameraComponent->SetDistance(component.value["Distance"].GetFloat());
			}
			if (component.value.HasMember("CameraElevation"))
			{
				followCameraComponent->SetCameraElevation(component.value["CameraElevation"].GetFloat());
			}
			if (component.value.HasMember("LookElevation"))
			{
				followCameraComponent->SetLookElevation(component.value["LookElevation"].GetFloat());
			}
		}
		else if (strcmp(componentName, "FPSCameraControllerComponent") == 0)
		{
			auto camera = gameObject.AddComponent<FPSCameraControllerComponent>();
		}
		else if (strcmp(componentName, "ModelComponent") == 0)
		{
			auto model = gameObject.AddComponent<ModelComponent>();
			if (component.value.HasMember("FileName"))
			{
				model->SetFileName(component.value["FileName"].GetString());
			}
			if (component.value.HasMember("Rotation"))
			{
				auto rotation = component.value["Rotation"].GetArray();
				auto x = rotation[0].GetFloat() * Math::Constants::DegToRad;
				auto y = rotation[1].GetFloat() * Math::Constants::DegToRad;
				auto z = rotation[2].GetFloat() * Math::Constants::DegToRad;
				model->SetRotation({ x, y, z });
			}
		}
		else if (strcmp(componentName, "ParticleEmitterComponent") == 0)
		{
			auto particleEmitter = gameObject.AddComponent<ParticleEmitterComponent>();
			if (component.value.HasMember("Position"))
			{
				auto position = component.value["Position"].GetArray();
				auto x = position[0].GetFloat();
				auto y = position[1].GetFloat();
				auto z = position[2].GetFloat();
				particleEmitter->GetParticleEmitter().SetPosition({ x, y, z });
			}
			if (component.value.HasMember("PosOffset"))
			{
				auto position = component.value["PosOffset"].GetArray();
				auto x = position[0].GetFloat();
				auto y = position[1].GetFloat();
				auto z = position[2].GetFloat();
				particleEmitter->SetPosOffset({ x, y, z });
			}
			if (component.value.HasMember("Direction"))
			{
				auto direction = component.value["Direction"].GetArray();
				auto x = direction[0].GetFloat();
				auto y = direction[1].GetFloat();
				auto z = direction[2].GetFloat();
				particleEmitter->GetParticleEmitter().SetPosition({ x, y, z });
			}
			if (component.value.HasMember("Speed"))
			{
				particleEmitter->GetParticleEmitter().SetSpeed(component.value["Speed"].GetFloat());
			}
			if (component.value.HasMember("StartColor"))
			{
				auto color = component.value["StartColor"].GetArray();
				auto r = color[0].GetFloat();
				auto g = color[1].GetFloat();
				auto b = color[2].GetFloat();
				auto a = color[3].GetFloat();
				particleEmitter->GetParticleEmitter().SetStartColor({ r, g, b, a });
			}
			if (component.value.HasMember("EndColor"))
			{
				auto color = component.value["EndColor"].GetArray();
				auto r = color[0].GetFloat();
				auto g = color[1].GetFloat();
				auto b = color[2].GetFloat();
				auto a = color[3].GetFloat();
				particleEmitter->GetParticleEmitter().SetEndColor({ r, g, b, a });
			}
			if (component.value.HasMember("StartSize"))
			{
				particleEmitter->GetParticleEmitter().SetStartSize(component.value["StartSize"].GetFloat());
			}
			if (component.value.HasMember("EndSize"))
			{
				particleEmitter->GetParticleEmitter().SetEndSize(component.value["EndSize"].GetFloat());
			}
			if (component.value.HasMember("MaxLifeTime"))
			{
				particleEmitter->GetParticleEmitter().SetMaxLifeTime(component.value["MaxLifeTime"].GetFloat());
			}
			if (component.value.HasMember("EmitPosOffset"))
			{
				particleEmitter->GetParticleEmitter().SetEmitPosOffset(component.value["EmitPosOffset"].GetFloat());
			}
		}
		else if (strcmp(componentName, "TransformComponent") == 0)
		{
			auto transform = gameObject.AddComponent<TransformComponent>();
			if (component.value.HasMember("Position"))
			{
				const auto& position = component.value["Position"].GetArray();
				const float x = position[0].GetFloat();
				const float y = position[1].GetFloat();
				const float z = position[2].GetFloat();
				transform->position = { x, y, z };
			}
			if (component.value.HasMember("Rotation"))
			{
				const auto& rotation = component.value["Rotation"].GetArray();
				const float x = rotation[0].GetFloat() * Math::Constants::DegToRad;
				const float y = rotation[1].GetFloat() * Math::Constants::DegToRad;
				const float z = rotation[2].GetFloat() * Math::Constants::DegToRad;
				transform->rotation = Math::Quaternion::RotationEuler({ x, y, z });
			}
			if (component.value.HasMember("Scale"))
			{
				const auto& scale = component.value["Scale"].GetArray();
				const float x = scale[0].GetFloat();
				const float y = scale[1].GetFloat();
				const float z = scale[2].GetFloat();
				transform->scale = { x, y, z };
			}
		}
		//... more components here ...
	}
}
