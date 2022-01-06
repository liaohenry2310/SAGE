#include "Precompiled.h"
#include "Constraints.h"

using namespace Sage;
using namespace Sage::Physics;

Sage::Physics::Fixed::Fixed(Particle* p) : Fixed(p, p->position) {}

Sage::Physics::Fixed::Fixed(Particle* p, const Math::Vector3& position) : mParticle(p), mPosition(position) {}

void Sage::Physics::Fixed::Apply() const
{
	mParticle->SetPosition(mPosition);
}

void Sage::Physics::Fixed::DebugDraw() const
{
	Graphics::SimpleDraw::AddAABB(mPosition, Math::Vector3(mParticle->radius), Graphics::Colors::Magenta);
}

Sage::Physics::Spring::Spring(Particle* a, Particle* b, float stiffness, float restLength) :
	mParticleA(a), mParticleB(b), mStiffness(stiffness), mRestLength(restLength)
{
	if (mRestLength <= 0.0f)
		mRestLength = Math::Distance(a->position, b->position);

	mStiffness = Math::Clamp(mStiffness, 0.0f, 1.0f);
}

void Sage::Physics::Spring::Apply() const
{
	Math::Vector3 delta = mParticleB->position - mParticleA->position;
	float dist = Math::Magnitude(delta);
	float diff = (dist - mRestLength) / (dist * (mParticleA->invMass + mParticleB->invMass));
	mParticleA->position += delta * diff * mParticleA->invMass * mStiffness;
	mParticleB->position -= delta * diff * mParticleB->invMass * mStiffness;
}

void Sage::Physics::Spring::DebugDraw() const
{
	Graphics::SimpleDraw::AddLine(mParticleA->position, mParticleB->position, Graphics::Colors::Yellow);
}


