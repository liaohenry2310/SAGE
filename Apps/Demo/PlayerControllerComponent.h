#pragma once
#include <Sage/Inc/Sage.h>
#include "TypeIds.h"

class PlayerControllerComponent final : public Sage::Component
{
public:
	SET_TYPE_ID(GameComponentId::PlayerController);
	MEMORY_POOL_DECLARE;

	void Initialize() override;

	void Update(float deltaTime) override;
	void DebugUI() override;

	void SetWalkSpeed(float walkSpeed) { mWalkSpeed = walkSpeed; }
	void SetRunSpeed(float runSpeed) { mRunSpeed = runSpeed; }

private:
	enum class State
	{
		Attack,
		Move
	};

	Sage::TransformComponent* mTransformComponent = nullptr;
	State mState = State::Move;
	float mWalkSpeed = 1.0f;
	float mRunSpeed = 1.0f;

	void MoveUpdate(float deltaTime);
	void AttackUpdate(float deltaTime);
};