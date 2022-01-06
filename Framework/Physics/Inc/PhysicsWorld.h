#pragma once

#include "Constraints.h"
#include "Particle.h"
#include "Tetrahedron.h"

namespace Sage::Physics
{
	class PhysicsWorld
	{
	public:
		struct Settings
		{
			Math::Vector3 gravity{ 0.0f, -9.8f, 0.0f };
			float timeStep = 1.0f / 60.0f;
			float drag = 0.0f;
			int constraintIterations = 10;
		};

		void Initialize(Settings settings = {});
		void Update(float deltaTime);
		void DebugDraw() const;
		void DebugUI();

		//Simulation
		template <class... Args>
		Particle* AddParticle(Args&&... args)
		{
			return mParticles.emplace_back(std::make_unique<Particle>(std::forward<Args>(args)...)).get();
		}

		template <class ConstraintType, class... Args>
		ConstraintType* AddConstraint(Args&&... args)
		{
			auto& newConstraint = mConstraints.emplace_back(std::make_unique<ConstraintType>(std::forward<Args>(args)...));
			return static_cast<ConstraintType*>(newConstraint.get());
		}
		Tetrahedron* AddTetrahedron();

		//Environment
		void AddPlane(const Math::Plane& plane);
		void AddOBB(const Math::OBB& obb);

		void Clear(bool dynamicOnly = false);

	private:
		void AccumulateForces();
		void Integrate();
		void SatisfyConstraints();

		std::vector<std::unique_ptr<Constraint>> mConstraints;
		std::vector<std::unique_ptr<Particle>> mParticles;
		std::vector<std::unique_ptr<Tetrahedron>> mTetrahedrons;
		std::vector<Math::Plane> mPlanes;
		std::vector<Math::OBB> mOBBs;

		Settings mSettings;

		float mTimer = 0.0f;
		bool mPause = false;
		bool mDrawDebug = true;
	};
}