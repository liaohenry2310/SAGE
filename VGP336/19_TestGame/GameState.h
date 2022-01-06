#pragma once
#include <Sage/Inc/Sage.h>

class GameState : public Sage::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	Sage::GameWorld mGameWorld;
	Sage::GameObjectHandle mGameObjectHandle;

	float mFPS = 0.0f;
};