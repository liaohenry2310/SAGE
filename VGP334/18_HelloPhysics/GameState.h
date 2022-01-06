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
	Sage::Graphics::DirectionalLight mDirectionalLight;
	Sage::Graphics::RenderObject mPaladin;
	Sage::Graphics::RenderObject mCloth;
	Sage::Graphics::Mesh mClothMesh;
	Sage::Graphics::StandardEffect mStandardEffect;
	Sage::Graphics::DissolveEffect mDissolveEffect;
	Sage::Physics::PhysicsWorld mPhysicsWorld;
	Sage::Physics::PhysicsWorld::Settings mPhysicsSettings;

	std::vector<Sage::Physics::Tetrahedron*> mPaladinTransforms;
	std::vector<Sage::Physics::Particle*> mClothParticles;

	Sage::Physics::Fixed* mFixedConstraint1;
	Sage::Physics::Fixed* mFixedConstraint2;
	Sage::Math::Vector3 constraint1;
	Sage::Math::Vector3 constraint2;

	float mFPS = 0.0f;
	float mDissolveTime = 1.0f;
	bool mDrawPaladins = false;
	bool mDrawPhysicsDebug = true;
	bool mClothInitialized = false;
	bool mDrawCloth = false;
	bool mPlayDissolve = false;

	void InitializeClothMesh();
	void UpdateClothVertices();
};