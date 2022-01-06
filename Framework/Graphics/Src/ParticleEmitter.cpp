#include "Precompiled.h"
#include "ParticleEmitter.h"

using namespace Sage::Graphics;

namespace
{
	std::random_device myRandomDevice{};
	std::mt19937 myRandomEngine{ myRandomDevice() };

	int RandI(int min, int max)
	{
		return std::uniform_int_distribution<>{ min, max }(myRandomEngine);
	}

	float RandF(float min, float max)
	{
		return std::uniform_real_distribution<float>{ min, max }(myRandomEngine);
	}
}


void ParticleEmitter::Initialize()
{
	mMeshBuffer.Initialize(nullptr, sizeof(VertexPCX), 1000 * 6, true);
	mConstantBuffer.Initialize(sizeof(Math::Matrix4));

	mVertexShader.Initialize("../../Assets/Shaders/Particles.fx", VertexPCX::Format);
	mPixelShader.Initialize("../../Assets/Shaders/Particles.fx");

	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Clamp);
	mTexture.Initialize("../..//Assets/Images/particle1.jpg");

	mBlendState.Initialize(BlendState::Mode::Additive);
}

void ParticleEmitter::Terminate()
{
	mBlendState.Terminate();
	mTexture.Terminate();
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mConstantBuffer.Terminate();
	mMeshBuffer.Terminate();
}

void ParticleEmitter::Start(float emitRate)
{
	mEmitRate = emitRate;
}

void ParticleEmitter::Stop()
{
	mEmitRate = 0.0f;
}

void ParticleEmitter::Update(float deltatime)
{
	//Emit new particles
	mEmitCount += mEmitRate * deltatime;
	int count = static_cast<int>(mEmitCount);
	for (int i = 0; i < count; ++i)
	{
		Particle p;
		p.position = mPosition;
		p.position.x += RandF(-mEmitPosOffset, mEmitPosOffset);
		p.position.y += RandF(-mEmitPosOffset, mEmitPosOffset);
		p.position.z += RandF(-mEmitPosOffset, mEmitPosOffset);
		p.velocity = mDirection * mSpeed;
		p.acceleration = mAcceleration;
		p.startColor = mStartColor;
		p.endColor = mEndColor;
		p.startSize = mStartSize;
		p.endSize = mEndSize;
		p.lifetime = p.maxLifeTime = RandF(mMaxLifeTime * 0.5f, mMaxLifeTime);
		p.textureIndex = RandI(0, 4);
		mParticles.emplace_back(p);
	}
	mEmitCount -= count;

	//Update and prune existing particles
	for (auto& p : mParticles)
	{
		p.lifetime -= deltatime;
		p.velocity += p.acceleration * deltatime;
		p.position += p.velocity * deltatime;
	}

	mParticles.erase(std::remove_if(mParticles.begin(), mParticles.end(), [](auto& p) { return p.lifetime <= 0.0f; }), mParticles.end());
}

void ParticleEmitter::Render(const Camera& camera)
{
	auto matView = camera.GetViewMatrix();
	auto matProj = camera.GetProjectionMatrix();
	auto transform = Math::Transpose(matView * matProj);
	mConstantBuffer.Update(&transform);
	mConstantBuffer.BindVS(0);

	mVertexShader.Bind();
	mPixelShader.Bind();

	std::vector<VertexPCX> vertices;

	Math::Vector2 uv[] = { {0.0f, 0.0f}, {0.5f, 0.0f}, {0.0f, 0.5f}, {0.5f, 0.5f} };

	Math::Vector3 cameraPos = camera.GetPosition();
	std::sort(mParticles.begin(), mParticles.end(), [&cameraPos](auto& a, auto& b)
	{
		float distA = Math::MagnitudeSqr(cameraPos - a.position);
		float distB = Math::MagnitudeSqr(cameraPos - b.position);
		return distA > distB;
	});

	Math::Vector3 up = { matView._21, matView._22, matView._23 };
	for (int i = 0; i < 1000 && i < mParticles.size(); ++i)
	{
		auto& p = mParticles[i];
		auto dirToCam = Normalize(cameraPos - p.position);
		auto side = Cross(up, dirToCam);
		VertexPCX tl, tr, bl, br;
		float t = 1.0f - p.lifetime / p.maxLifeTime;
		float s = Math::Lerp(p.startSize, p.endSize, t);
		Color c = Math::Lerp(p.startColor, p.endColor, t);
		tl.position = p.position + (side * s) + (up * s);
		tr.position = p.position - (side * s) + (up * s);
		bl.position = p.position + (side * s) - (up * s);
		br.position = p.position - (side * s) - (up * s);
		tl.color = c;
		tr.color = c;
		bl.color = c;
		br.color = c;
		tl.uv = uv[p.textureIndex];
		tr.uv = tl.uv + Math::Vector2{ 0.5f, 0.0f };
		bl.uv = tl.uv + Math::Vector2{ 0.0f, 0.5f };
		br.uv = tl.uv + Math::Vector2{ 0.5f, 0.5f };
		vertices.push_back(tl);
		vertices.push_back(tr);
		vertices.push_back(bl);
		vertices.push_back(tr);
		vertices.push_back(br);
		vertices.push_back(bl);
	}

	mMeshBuffer.Update(vertices.data(), static_cast<uint32_t>(vertices.size()));
	mMeshBuffer.SetTopology(MeshBuffer::Topology::Triangles);

	mSampler.BindPS(0);
	mTexture.BindPS(0);

	mBlendState.Set();
	mMeshBuffer.Render();
	BlendState::ClearState();
}

void ParticleEmitter::SetPosition(const Math::Vector3& pos)
{
	mPosition = pos;
}

void ParticleEmitter::SetDirection(const Math::Vector3& dir, float span)
{
	mDirection = Normalize(dir);
	mDirectionSpan = span;
}

void Sage::Graphics::ParticleEmitter::SetSpeed(const float speed)
{
	mSpeed = speed;
}
