#include "Precompiled.h"
#include "PhysicsWorld.h"

using namespace Sage;
using namespace Sage::Physics;

void Sage::Physics::PhysicsWorld::Initialize(Settings settings)
{
	mSettings = std::move(settings);
}

void Sage::Physics::PhysicsWorld::Update(float deltaTime)
{
	if (mPause)
		return;

	mTimer += deltaTime;
	while (mTimer >= mSettings.timeStep)
	{
		mTimer -= mSettings.timeStep;

		AccumulateForces();
		Integrate();
		SatisfyConstraints();
	}
}

void Sage::Physics::PhysicsWorld::DebugDraw() const
{
	if (!mDrawDebug)
		return;

	for (auto& c : mConstraints)
		c->DebugDraw();

	for (auto& p : mParticles)
		Graphics::SimpleDraw::AddSphere(p->position, p->radius, Graphics::Colors::Cyan, 3, 4);

	for (auto& t : mTetrahedrons)
		Graphics::SimpleDraw::AddTransform(t->GetWorldMatrix());

	for (auto& obb : mOBBs)
		Graphics::SimpleDraw::AddOBB(obb, Graphics::Colors::Magenta);
}

void Sage::Physics::PhysicsWorld::DebugUI()
{
	ImGui::Begin("Physics", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::DragFloat3("Gravity", &mSettings.gravity.x, 0.1f, -10.0f, 10.0f);
	ImGui::DragInt("Constraint Iterations", &mSettings.constraintIterations, 1.0f,  1, 20);
	ImGui::Checkbox("Pause Simulation", &mPause);
	ImGui::Checkbox("Draw Debug", &mDrawDebug);
	ImGui::End();
}

Tetrahedron* Sage::Physics::PhysicsWorld::AddTetrahedron()
{
	auto tetrahedron = mTetrahedrons.emplace_back(std::make_unique<Tetrahedron>()).get();

	for(auto& p : tetrahedron->particles)
		p = AddParticle();

	tetrahedron->SetPosition(Math::Vector3::Zero);
	tetrahedron->SetVelocity(Math::Vector3::Zero);

	AddConstraint<Spring>(tetrahedron->particles[0], tetrahedron->particles[1]);
	AddConstraint<Spring>(tetrahedron->particles[0], tetrahedron->particles[2]);
	AddConstraint<Spring>(tetrahedron->particles[0], tetrahedron->particles[3]);
	AddConstraint<Spring>(tetrahedron->particles[1], tetrahedron->particles[2]);
	AddConstraint<Spring>(tetrahedron->particles[1], tetrahedron->particles[3]);
	AddConstraint<Spring>(tetrahedron->particles[2], tetrahedron->particles[3]);

	return tetrahedron;
}

void Sage::Physics::PhysicsWorld::AddPlane(const Math::Plane& plane)
{
	mPlanes.push_back(plane);
}

void Sage::Physics::PhysicsWorld::AddOBB(const Math::OBB& obb)
{
	mOBBs.push_back(obb);
}

void Sage::Physics::PhysicsWorld::Clear(bool dynamicOnly)
{
	mTetrahedrons.clear();
	mConstraints.clear();
	mParticles.clear();

	if (!dynamicOnly)
	{
		mPlanes.clear();
		mOBBs.clear();
	}
}

void Sage::Physics::PhysicsWorld::AccumulateForces()
{
	for (auto& p : mParticles)
		p->acceleration = mSettings.gravity;
}

void Sage::Physics::PhysicsWorld::Integrate()
{
	constexpr float timeStepSqr = Math::Sqr(1.0f / 60.0f);
	for (auto& p : mParticles)
	{
		const auto newPosition = (p->position * 2.0f) - p->lastPosition + (p->acceleration * timeStepSqr);
		p->lastPosition = p->position;
		p->position = newPosition;
	}
}

void Sage::Physics::PhysicsWorld::SatisfyConstraints()
{
	for (int i = 0; i < mSettings.constraintIterations; ++i)
	{
		for (auto& c : mConstraints)
			c->Apply();
	}

	for (auto& plane : mPlanes)
	{
		for (auto& p : mParticles)
		{
			float a, b = 0.0f;
			if (Math::Intersect(Math::Sphere{p->position, p->radius}, plane, a))
			{
				auto velocity = p->position - p->lastPosition;
				auto velocityPerpendicular = plane.normal * Math::Dot(velocity, plane.normal);
				auto velocityParallel = velocity - velocityPerpendicular;


				if (Math::Intersect(Math::Sphere{ p->lastPosition, p->radius }, plane, b))
				{
					const auto newVelocity = velocityParallel * (1.0f - mSettings.drag);
					p->SetPosition(p->position + (plane.normal * a));
					p->SetVelocity(newVelocity);
				}
				else
				{
					auto newVelocity = (velocityParallel * (1.0f - mSettings.drag)) - (velocityPerpendicular * p->bounce);
					p->SetPosition(p->position - velocityPerpendicular);
					p->SetVelocity(newVelocity);
				}
			}
		}
	}

	for (auto& obb : mOBBs)
	{
		for (auto& p : mParticles)
		{
			if (Math::Intersect(p->position, obb))
			{
				auto velocity = p->position - p->lastPosition;
				auto direction = Math::Normalize(velocity);

				Math::Ray ray{ p->lastPosition, direction };
				Math::Vector3 point, normal;
				Math::Intersect(ray, obb, point, normal);

				auto velocityPerpendicular = normal * Math::Dot(velocity, normal);
				auto velocityParallel = velocity - velocityPerpendicular;
				auto newVelocity = (velocityParallel * (1.0f - mSettings.drag)) - (velocityPerpendicular * p->bounce);
				p->SetPosition(p->position - velocityPerpendicular);
				p->SetVelocity(newVelocity);
			}

		}
	}
}
