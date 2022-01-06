#pragma once

#include "Common.h"
#include "Particle.h"

namespace Sage::Physics
{
	struct Tetrahedron
	{
		std::array<Particle*, 4> particles;

		Math::Matrix4 GetWorldMatrix() const
		{
			if (!particles[0] || !particles[1] || !particles[2] || !particles[3])
				return Math::Matrix4::Identity;

			const Math::Vector3 basePos = (particles[1]->position + particles[2]->position + particles[3]->position) * 0.3333333f;
			const Math::Vector3 up = Math::Normalize(particles[0]->position - basePos);
			const Math::Vector3 look = Math::Normalize(particles[1]->position - basePos);
			const Math::Vector3 right = Math::Normalize(Math::Cross(up, look));

			return
			{
				right.x, right.y, right.z, 0.0f,
				up.x, up.y, up.z, 0.0f,
				look.x, look.y, look.z, 0.0f,
				basePos.x, basePos.y, basePos.z, 1.0f
			};
		}

		void SetPosition(Math::Vector3 position)
		{
			if (!particles[0] || !particles[1] || !particles[2] || !particles[3])
				return;

			particles[0]->SetPosition(position + Math::Vector3{ 0.0f, 0.8164965f, 0.0f });
			particles[1]->SetPosition(position + Math::Vector3{ 0.0f, 0.0f, 0.5773502f });
			particles[2]->SetPosition(position + Math::Vector3{ 0.5f, 0.0f, -0.2886751f });
			particles[3]->SetPosition(position + Math::Vector3{ -0.5f, 0.0f, -0.2886751f });
		}

		void SetVelocity(Math::Vector3 velocity)
		{
			if (!particles[0] || !particles[1] || !particles[2] || !particles[3])
				return;

			for (auto p : particles)
				p->SetVelocity(velocity);
		}
	};
}

