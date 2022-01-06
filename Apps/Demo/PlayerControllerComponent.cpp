#include "PlayerControllerComponent.h"

using namespace Sage;
using namespace Sage::Input;
using namespace Sage::Math;

MEMORY_POOL_DEFINE(PlayerControllerComponent, 1000)

void PlayerControllerComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
}

void PlayerControllerComponent::Update(float deltaTime)
{
	switch (mState)
	{
	case PlayerControllerComponent::State::Attack:
		AttackUpdate(deltaTime);
		break;
	case PlayerControllerComponent::State::Move:
		MoveUpdate(deltaTime);
		break;
	}

	// Rotation
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		auto mouseX = inputSystem->GetMouseMoveX() * deltaTime;
		auto rotation = Quaternion::RotationEuler({ 0.0f, mouseX, 0.0f });
		mTransformComponent->rotation *= rotation;
	}
}

void PlayerControllerComponent::DebugUI()
{

}

void PlayerControllerComponent::MoveUpdate(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	const bool isShiftPressed = inputSystem->IsKeyDown(KeyCode::LSHIFT);
	const float moveSpeed = isShiftPressed ? mRunSpeed : mWalkSpeed;
	const float turnSpeed = 0.1f;

	auto animator = GetOwner().GetComponent<AnimatorComponent>();
	Matrix4 transform = mTransformComponent->GetMatrix4();
	Vector3 velocity = Vector3::Zero;

	if (inputSystem->IsMouseDown(MouseButton::LBUTTON))
	{
		mState = State::Attack;
		return;
	}

	if (inputSystem->IsKeyDown(KeyCode::W))
	{
		velocity = GetLook(transform) * moveSpeed;
		animator->GetAnimator().PlayAnimation(isShiftPressed ? 2 : 1);
	}
	else if (inputSystem->IsKeyDown(KeyCode::S))
	{
		velocity = GetLook(transform) * -moveSpeed;
		animator->GetAnimator().PlayAnimation(isShiftPressed ? 4 : 3);
	}
	else if (inputSystem->IsKeyDown(KeyCode::D))
	{
		velocity = GetRight(transform) * moveSpeed;
		animator->GetAnimator().PlayAnimation(isShiftPressed ? 6 : 5);
	}
	else if (inputSystem->IsKeyDown(KeyCode::A))
	{
		velocity = GetRight(transform) * -moveSpeed;
		animator->GetAnimator().PlayAnimation(isShiftPressed ? 8 : 7);
	}
	else
	{
		animator->GetAnimator().PlayAnimation(0);
	}

	Vector3 position = GetTranslation(transform);
	position += velocity * deltaTime;

	// Snap to terrain
	auto terrainService = GetOwner().GetWorld().GetService<TerrainService>();
	float height = terrainService->GetHeight(position);
	auto newPosition = Math::Vector3{ position.x, height, position.z };
	mTransformComponent->position = newPosition;
}

void PlayerControllerComponent::AttackUpdate(float deltaTime)
{
	auto animator = GetOwner().GetComponent<AnimatorComponent>();
	animator->GetAnimator().PlayAnimation(9, false);

	auto emitter = GetOwner().GetComponent<ParticleEmitterComponent>();

	if(animator->GetAnimator().GetAnimationProgress() >= 0.8f)
		emitter->GetParticleEmitter().Stop();
	else if (animator->GetAnimator().GetAnimationProgress() >= 0.3f)
		emitter->GetParticleEmitter().Start(64.0f);

	if (animator->GetAnimator().IsAnimationDone())
	{
		animator->GetAnimator().PlayAnimation(0);
		mState = State::Move;
	}
}
