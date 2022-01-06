#pragma once

#include "TypeIds.h"

namespace Sage
{
	class GameObject;

	class Component
	{
	public:
		Component() = default;
		virtual ~Component() = default;

		Component(const Component&) = delete;
		Component& operator=(const Component&) = delete;
		Component(Component&&) = delete;
		Component& operator=(Component&&) = delete;

		virtual uint32_t GetTypeId() const = 0;

		virtual void Initialize() {}
		virtual void Terminate() {}

		virtual void Update(float deltaTime) {}
		virtual void DebugUI() {}

		GameObject& GetOwner() { return *mWorld; }
		const GameObject& GetOwner() const { return *mWorld; }

	private:
		friend class GameObject;
		GameObject* mWorld = nullptr;
	};
}