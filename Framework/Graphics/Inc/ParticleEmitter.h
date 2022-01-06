#pragma once

#include "Colors.h"

#include "BlendState.h"
#include "Camera.h"
#include "ConstantBuffer.h"
#include "MeshBuffer.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "Texture.h"
#include "VertexShader.h"
#include "VertexTypes.h"

namespace Sage::Graphics
{
	struct Particle
	{
		Math::Vector3 position;
		Math::Vector3 velocity;
		Math::Vector3 acceleration;
		Color startColor, endColor;
		float startSize, endSize;
		float lifetime, maxLifeTime;
		int textureIndex = 0;
	};

	class ParticleEmitter
	{
	public:
		void Initialize();
		void Terminate();

		void Start(float emitRate);
		void Stop();

		void Update(float deltatime);
		void Render(const Camera& camera);

		void SetPosition(const Math::Vector3& pos);
		void SetDirection(const Math::Vector3& dir, float span);
		void SetSpeed(const float speed);

		void SetStartColor(const Color& color) { mStartColor = color; }
		void SetEndColor(const Color& color) { mEndColor = color; }
		void SetStartSize(float size) { mStartSize = size; }
		void SetEndSize(float size) { mEndSize = size; }
		void SetMaxLifeTime(float maxlifetime) { mMaxLifeTime = maxlifetime; }
		void SetEmitPosOffset(float offset) { mEmitPosOffset = offset; }

	private:
		MeshBuffer mMeshBuffer;
		ConstantBuffer mConstantBuffer;
		VertexShader mVertexShader;
		PixelShader mPixelShader;
		Sampler mSampler;
		Texture mTexture;
		BlendState mBlendState;

		Math::Vector3 mPosition = Math::Vector3::Zero;
		Math::Vector3 mDirection = Math::Vector3::Zero;
		Math::Vector3 mAcceleration = Math::Vector3::Zero;
		float mSpeed = 0.0f;
		float mDirectionSpan = 0.0f;
		float mEmitRate = 0.0f;
		float mEmitCount = 0.0f;
		Color mStartColor = Colors::White, mEndColor = Colors::White;
		float mStartSize = 1.0f, mEndSize = 1.0f;
		float mMaxLifeTime = 1.0f;
		float mEmitPosOffset = 0.0f;

		std::vector<Particle> mParticles;
	};
}