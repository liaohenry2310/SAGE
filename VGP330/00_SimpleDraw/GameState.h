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
	struct TransformData
	{
		Sage::Math::Matrix4 wvp;
	};

	using TransformBuffer = Sage::Graphics::TypedConstantBuffer<TransformData>;

	Sage::Graphics::Camera mCamera;


	Sage::Graphics::RasterizerState mRasterizerStateWireFrame;
	Sage::Graphics::Skybox mSkybox;
	Sage::Math::Vector3 mRotation;
	bool mUseWireframe = false;
};