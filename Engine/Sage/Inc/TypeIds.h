#pragma once

#include "Common.h"

namespace Sage
{
	enum class ComponentId
	{
		Animator,
		Camera,
		Collider,
		FPSCameraController,
		FollowCameraController,
		Model,
		ParticleEmitter,
		Transform,
		Count
	};

	enum class ServiceId
	{
		Camera,
		Count,
		Render,
		Terrain
	};
}

#define SET_TYPE_ID(id)\
	static uint32_t StaticGetTypeId() { return static_cast<uint32_t>(id); }\
	uint32_t GetTypeId() const override { return StaticGetTypeId(); }