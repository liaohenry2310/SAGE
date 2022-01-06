#include "GameState.h"

using namespace Sage;
using namespace Sage::Graphics;
using namespace Sage::Input;
using namespace Sage::Math;

namespace
{
	const int clothParticleWidth = 21;
	const int clothParticleLength = 21;
	const float clothScale = 0.1f;

	std::random_device myRandomDevice{};
	std::mt19937 myRandomEngine{ myRandomDevice() };

	float RandomFloat(float min, float max)
	{
		return std::uniform_real_distribution<float>{ min, max }(myRandomEngine);
	}
}

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera.SetPosition({ 0.0f, 0.0f, -5.0f });

	mDirectionalLight.lightDirection = { 0.0f, -10.0f, 10.0f };

	Model model;
	ModelIO::LoadModel("../../Assets/Models/Paladin/paladin.model", model);
	ModelIO::LoadMaterial("../../Assets/Models/Paladin/paladin.material", model);
	mPaladin.meshBuffer.Initialize(model.meshData[0].mesh);
	mPaladin.diffuseMap.Initialize(model.materialData[0].diffuseMapName);
	mPaladin.material = model.materialData[0].material;

	mStandardEffect.Initialize();
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
	mStandardEffect.UseSpecularMap() = true;

	mDissolveEffect.Initialize();
	mDissolveEffect.SetCamera(mCamera);
	mDissolveEffect.SetDirectionalLight(mDirectionalLight);
	mDissolveEffect.UseSpecularMap() = true;

	mPhysicsSettings.drag = 0.5f;

	mPhysicsWorld.Initialize(mPhysicsSettings);
	mPhysicsWorld.AddPlane({});
	//mPhysicsWorld.AddOBB({ {0.0f, 2.0f, 0.0f}, {3.0f, 1.0f, 2.0f}, Quaternion::RotationEuler({0.0f, 0.0f, -0.5f}) });
}

void GameState::Terminate()
{
	mPaladin.Terminate();
	mStandardEffect.Terminate();
	mDissolveEffect.Terminate();
	if (mClothInitialized)
		mCloth.Terminate();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	const float moveSpeed = 5.0f;
	const float rotateSpeed = 100.0f * Constants::DegToRad;
	const float turnSpeed = 30.0f * Constants::DegToRad;
	if (inputSystem->IsKeyDown(KeyCode::W))
		mCamera.Walk(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		mCamera.Walk(-moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		mCamera.Strafe(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		mCamera.Strafe(-moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::E))
		mCamera.Rise(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::Q))
		mCamera.Rise(-moveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(inputSystem->GetMouseMoveX() * turnSpeed * deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() * turnSpeed * deltaTime);
	}

	mFPS = 1.0f / deltaTime;

	mPhysicsWorld.Update(deltaTime);
	
	if (mPlayDissolve)
		mDissolveEffect.DissolveAmount() = Clamp(mDissolveEffect.DissolveAmount() + (1.0f / mDissolveTime) * deltaTime, 0.0f, 1.0f);
	if (mDissolveEffect.DissolveAmount() >= 1.0f)
		mPlayDissolve = false;

	if (mDrawCloth)
		UpdateClothVertices();
}

void GameState::Render()
{
	SimpleDraw::AddGroundPlane(10);
	SimpleDraw::Render(mCamera);
	SimpleDraw::AddTransform(Matrix4::Identity);

	if (mDrawPaladins)
	{
		mStandardEffect.Begin();
		for (auto& t : mPaladinTransforms)
		{
			mPaladin.transform.SetMatrix4(t->GetWorldMatrix());
			mStandardEffect.Render(mPaladin);
		}
		mStandardEffect.End();
	}

	mPhysicsWorld.DebugDraw();

	if (mDrawCloth)
	{
		mDissolveEffect.Begin();
		mCloth.meshBuffer.Update(mClothMesh.vertices.data(), static_cast<uint32_t>(mClothMesh.vertices.size()));
		mDissolveEffect.Render(mCloth);
		mDissolveEffect.End();
	}
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("fps: %f", mFPS);

	if (ImGui::Button("Boom!"))
	{
		mPhysicsWorld.Clear(true);
		mPaladinTransforms.clear();
		mClothParticles.clear();
		mDrawCloth = false;

		for (int i = 0; i < 100; ++i)
		{
			Math::Vector3 vel;
			vel.x = Random::UniformFloat(-1.0f, 1.0f);
			vel.y = Random::UniformFloat(1.0f, 2.0f);
			vel.z = Random::UniformFloat(-1.0f, 1.0f);
			vel *= 0.1f;

			auto particle = mPhysicsWorld.AddParticle(0.5f, 1.0f);
			particle->SetPosition({ 0.0f, 1.0f, 0.0f });
			particle->SetVelocity(vel);
			particle->bounce = 0.5f;
		}
	}

	if (ImGui::Button("Sticks!"))
	{
		mPhysicsWorld.Clear(true);
		mPaladinTransforms.clear();
		mClothParticles.clear();
		mDrawCloth = false;

		for (int i = 0; i < 100; ++i)
		{
			Vector3 vel1 = Random::OnUnitSphere() * 0.1f;
			vel1.y = Abs(vel1.y);
			Vector3 vel2 = Random::OnUnitSphere() * 0.1f;
			vel2.y = Abs(vel2.y);

			auto particle1 = mPhysicsWorld.AddParticle(0.02f, 1.0f);
			particle1->SetPosition({ -0.1f, 1.0f, 0.0f });
			particle1->SetVelocity(vel1);

			auto particle2 = mPhysicsWorld.AddParticle(0.02f, 1.0f);
			particle2->SetPosition({ +0.1f, 1.0f, 0.0f });
			particle2->SetVelocity(vel2);

			mPhysicsWorld.AddConstraint<Physics::Spring>(particle1, particle2);
		}
	}

	if (ImGui::Button("Tetrahedrons!"))
	{
		mPhysicsWorld.Clear(true);
		mPaladinTransforms.clear();
		mClothParticles.clear();
		mDrawCloth = false;

		for (int i = 0; i < 20; ++i)
		{
			auto tetrahedron = mPhysicsWorld.AddTetrahedron();
			for (auto p : tetrahedron->particles)
			{
				Vector3 vel = Random::OnUnitSphere() * 0.1f;
				vel.y = Abs(vel.y);
				p->SetVelocity(vel);
				p->radius = 0.02f;
				p->invMass = 1.0f;
			}
			mPaladinTransforms.push_back(tetrahedron);
		}
	}

	if (ImGui::Button("Cloth!"))
	{
		mPhysicsWorld.Clear(true);
		mPaladinTransforms.clear();
		mClothParticles.clear();

		const int maxX = clothParticleWidth;
		const int maxY = clothParticleLength;

		//Creating cloth particles
		for (int y = 0; y < maxY; ++y)
		{
			for (int x = 0; x < maxX; ++x)
			{
				auto& p = mClothParticles.emplace_back(mPhysicsWorld.AddParticle(0.02f, 100.0f));
				p->SetPosition(Math::Vector3{ x - (maxX - 1) / 2.0f, y - (maxY - 1) / 2.0f, 0.0f } * clothScale);
				p->SetVelocity(Math::Vector3::Zero);
			}
		}

		mFixedConstraint1 = mPhysicsWorld.AddConstraint<Physics::Fixed>(mClothParticles[maxX * (maxY - 1)]);
		mFixedConstraint2 = mPhysicsWorld.AddConstraint<Physics::Fixed>(mClothParticles[maxX * (maxY - 1) + (maxX - 1)]);

		for (int y = 0; y < maxY; ++y)
		{
			for (int x = 0; x < maxX; ++x)
			{
				if (y < maxY - 1)
				{
					mPhysicsWorld.AddConstraint<Physics::Spring>(mClothParticles[maxX * y + x], mClothParticles[maxX * (y + 1) + x]);
					if (x > 0)
						mPhysicsWorld.AddConstraint<Physics::Spring>(mClothParticles[maxX * y + x], mClothParticles[maxX * (y + 1) + (x - 1)], 0.75f);
					if (x < maxX - 1)
						mPhysicsWorld.AddConstraint<Physics::Spring>(mClothParticles[maxX * y + x], mClothParticles[maxX * (y + 1) + (x + 1)], 0.75f);
				}
				if (x < maxX - 1)
				{
					mPhysicsWorld.AddConstraint<Physics::Spring>(mClothParticles[maxX * y + x], mClothParticles[maxX * y + (x + 1)]);
				}
			}
		}
		InitializeClothMesh();
		mDrawCloth = true;
	}

	ImGui::Checkbox("Draw Paladins", &mDrawPaladins);

	ImGui::End();


	mPhysicsWorld.DebugUI();

	ImGui::Begin("Lighting Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::DragFloat3("Light Direction", &mDirectionalLight.lightDirection.x, 0.1f, -10.0f, 10.0f);
	ImGui::ColorEdit4("Light Diffuse", &mDirectionalLight.diffuse.r);
	ImGui::ColorEdit4("Light Ambient", &mDirectionalLight.ambient.r);
	ImGui::ColorEdit4("Light Specular", &mDirectionalLight.specular.r);
	ImGui::End();

	if (mDrawCloth)
	{
		ImGui::Begin("Fixed Contraints", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		constraint1 = mFixedConstraint1->GetPosition();
		constraint2 = mFixedConstraint2->GetPosition();
		if (ImGui::DragFloat3("Constraint 1", &constraint1.x, 0.01f, -5.0f, 5.0f))
		{
			mFixedConstraint1->SetPosition(constraint1);
		}
		if (ImGui::DragFloat3("Constraint 2", &constraint2.x, 0.01f, -5.0f, 5.0f))
		{
			mFixedConstraint2->SetPosition(constraint2);
		}
		ImGui::End();
	}

	if (ImGui::CollapsingHeader(" Dissolve Effect ", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Button(" Play ") && !mPlayDissolve)
		{
			mPlayDissolve = true;
			mDissolveEffect.DissolveAmount() = 0.0f;
		}
		if (ImGui::Button(" Reset Dissolve "))
		{
			mPlayDissolve = false;
			mDissolveEffect.DissolveAmount() = 0.0f;
		}
		ImGui::SliderFloat(" Dissolve Time ", &mDissolveTime, 0.1f, 5.0f);
		ImGui::SliderFloat(" Dissolve Amount ", &mDissolveEffect.DissolveAmount(), 0.0f, 1.0f);
		ImGui::SliderFloat(" Burn Amount ", &mDissolveEffect.BurnAmount(), 0.0f, mDissolveEffect.DissolveAmount());
		ImGui::ColorEdit4(" Burn Color 1 ", &mDissolveEffect.Color1().r, ImGuiColorEditFlags_NoAlpha);
		ImGui::ColorEdit4(" Burn Color 2 ", &mDissolveEffect.Color2().r, ImGuiColorEditFlags_NoAlpha);
		ImGui::ColorEdit4(" Burn Color 3 ", &mDissolveEffect.Color3().r, ImGuiColorEditFlags_NoAlpha);
		static bool shadow = false;
		if (ImGui::Checkbox(" Shadow ", &shadow))
		{
			mStandardEffect.UseShadowMap() = shadow;
			mDissolveEffect.UseShadowMap() = shadow;
		}
	}
}

void GameState::InitializeClothMesh()
{
	if (mClothInitialized)
	{
		mCloth.Terminate();
	}

	mClothMesh.vertices.clear();
	mClothMesh.indices.clear();

	const int maxX = clothParticleWidth;
	const int maxY = clothParticleLength;

	for (auto& particle : mClothParticles)
	{
		auto position = particle->position;
		auto normal = -Vector3::ZAxis;
		auto tangent = Vector3::XAxis;
		auto u = (position.x + ((maxX - 1) / 2.0f) * clothScale) / ((maxX - 1) * clothScale);
		auto v = (position.y + ((maxY - 1) / 2.0f) * clothScale) / ((maxY - 1) * clothScale);
		mClothMesh.vertices.push_back(Vertex{ position, normal, tangent, {u, -v} });
	}

	for (int x = 0; x + 1 < clothParticleWidth; ++x)
	{
		for (int y = 0; y + 1 < clothParticleLength; ++y)
		{
			mClothMesh.indices.push_back((x + 0) + ((y + 0) * maxX));
			mClothMesh.indices.push_back((x + 0) + ((y + 1) * maxX));
			mClothMesh.indices.push_back((x + 1) + ((y + 1) * maxX));

			mClothMesh.indices.push_back((x + 0) + ((y + 0) * maxX));
			mClothMesh.indices.push_back((x + 1) + ((y + 1) * maxX));
			mClothMesh.indices.push_back((x + 1) + ((y + 0) * maxX));
		}
	}

	mCloth.meshBuffer.Initialize(mClothMesh, true);
	mCloth.diffuseMap.Initialize("../../Assets/Images/cloth.jpg");
	mCloth.backfaceCulling = false;
	mClothInitialized = true;

	for (auto& p : mClothParticles)
	{
		p->SetPosition(p->position + Vector3{0.0f, 1.0f, 2.0f});
	}
}

void GameState::UpdateClothVertices()
{
	const int maxX = clothParticleWidth;
	const int maxY = clothParticleLength;

	for (int i = 0; i < mClothParticles.size(); ++i)
	{
		mClothMesh.vertices[i].position = mClothParticles[i]->position;
	}

	for (int y = 0; y < clothParticleLength; ++y)
	{
		for (int x = 0; x < clothParticleWidth; ++x)
		{
			const int currentIndex = x + (y * maxX);
			const int upIndex = (x + 0) + ((y + 1) * maxX);
			const int downIndex = (x + 0) + ((y - 1) * maxX);
			const int leftIndex = (x - 1) + ((y + 0) * maxX);
			const int rightIndex = (x + 1) + ((y + 0) * maxX);
			const int upRightIndex = (x + 1) + ((y + 1) * maxX);
			const int downLeftIndex = (x - 1) + ((y - 1) * maxX);

			Vector3 vertexNormal = Vector3::Zero;

			const auto currentPos = mClothMesh.vertices[currentIndex].position;
			if (y < maxY - 1)
			{
				const auto upPos = mClothMesh.vertices[upIndex].position;
				if (x < maxX - 1)
				{
					const auto rightPos = mClothMesh.vertices[rightIndex].position;
					const auto upRightPos = mClothMesh.vertices[upRightIndex].position;
					vertexNormal += Math::Cross(upPos - currentPos, upRightPos - currentPos);
					vertexNormal += Math::Cross(upRightPos - currentPos, rightPos - currentPos);
				}
				if (x > 0)
				{
					const auto leftPos = mClothMesh.vertices[leftIndex].position;
					vertexNormal += Math::Cross(leftPos - currentPos, upPos - currentPos);
				}
			}
			if (y > 0)
			{
				const auto downPos = mClothMesh.vertices[downIndex].position;
				if (x < maxX - 1)
				{
					const auto rightPos = mClothMesh.vertices[rightIndex].position;
					vertexNormal += Math::Cross(rightPos - currentPos, downPos - currentPos);
				}
				if (x > 0)
				{
					const auto leftPos = mClothMesh.vertices[leftIndex].position;
					const auto downLeftPos = mClothMesh.vertices[downLeftIndex].position;
					vertexNormal += Math::Cross(downPos - currentPos, downLeftPos - currentPos);
					vertexNormal += Math::Cross(downLeftPos - currentPos, leftPos - currentPos);
				}
			}

			mClothMesh.vertices[currentIndex].normal = Math::Normalize(vertexNormal);
			mClothMesh.vertices[currentIndex].tangent = Math::Normalize({ -vertexNormal.z, 0.0f, vertexNormal.x });
		}
	}
}
