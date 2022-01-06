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
	Sage::Graphics::Camera mCamera;
	Sage::Graphics::Skybox mSkybox;
	Sage::Graphics::DirectionalLight mDirectionalLight;
	Sage::Graphics::StandardEffect mStandardEffect;
	Sage::Graphics::ShadowEffect mShadowEffect;

	Sage::Graphics::RenderObject mObject;
	Sage::Graphics::RenderObject mGround;

	float mFPS = 0.0f;
	float mTime = 0.0f;
};