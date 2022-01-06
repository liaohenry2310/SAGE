#include "Precompiled.h"
#include "SimpleDraw.h"

#include "Camera.h"
#include "ConstantBuffer.h"
#include "MeshBuffer.h"
#include "PixelShader.h"
#include "Transform.h"
#include "VertexShader.h"
#include "VertexTypes.h"

using namespace Sage;
using namespace Sage::Graphics;

namespace
{
	class SimpleDrawImpl
	{
	public:
		void Initialize(uint32_t maxVertexCount);
		void Terminate();

		void AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color);
		void AddFace(const Math::Vector3& v0, const Math::Vector3& v1, const Math::Vector3& v2, const Color& color);

		void Render(const Camera& camera);

	private:
		VertexShader mVertexShader;
		PixelShader mPixelShader;
		ConstantBuffer mConstantBuffer;
		MeshBuffer mMeshBuffer;
		std::unique_ptr<VertexPC[]> mLineVertices;
		std::unique_ptr<VertexPC[]> mFillVertices;
		uint32_t mLineVertexCount = 0;
		uint32_t mFillVertexCount = 0;
		uint32_t mMaxVertexCount = 0;
	};

	void SimpleDrawImpl::Initialize(uint32_t maxVertexCount)
	{
		mVertexShader.Initialize(L"../../Assets/Shaders/SimpleDraw.fx", VertexPC::Format);
		mPixelShader.Initialize(L"../../Assets/Shaders/SimpleDraw.fx");
		mConstantBuffer.Initialize(sizeof(Math::Matrix4));
		mMeshBuffer.Initialize(nullptr, sizeof(VertexPC), maxVertexCount, true);
		mLineVertices = std::make_unique<VertexPC[]>(maxVertexCount);
		mFillVertices = std::make_unique<VertexPC[]>(maxVertexCount);
		mLineVertexCount = 0;
		mFillVertexCount = 0;
		mMaxVertexCount = maxVertexCount;
	}

	void SimpleDrawImpl::Terminate()
	{
		mMeshBuffer.Terminate();
		mConstantBuffer.Terminate();
		mPixelShader.Terminate();
		mVertexShader.Terminate();
	}

	void SimpleDrawImpl::AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color)
	{
		if (mLineVertexCount + 2 <= mMaxVertexCount)
		{
			mLineVertices[mLineVertexCount++] = VertexPC{ v0, color };
			mLineVertices[mLineVertexCount++] = VertexPC{ v1, color };
		}
	}

	void SimpleDrawImpl::AddFace(const Math::Vector3& v0, const Math::Vector3& v1, const Math::Vector3& v2, const Color& color)
	{
		if (mFillVertexCount + 3 <= mMaxVertexCount)
		{
			mFillVertices[mFillVertexCount++] = VertexPC{ v0, color };
			mFillVertices[mFillVertexCount++] = VertexPC{ v1, color };
			mFillVertices[mFillVertexCount++] = VertexPC{ v2, color };
		}
	}

	void SimpleDrawImpl::Render(const Camera& camera)
	{
		auto matView = camera.GetViewMatrix();
		auto matProj = camera.GetProjectionMatrix();
		auto transform = Math::Transpose(matView * matProj);
		mConstantBuffer.Update(&transform);
		mConstantBuffer.BindVS(0);

		mVertexShader.Bind();
		mPixelShader.Bind();

		mMeshBuffer.Update(mLineVertices.get(), mLineVertexCount);
		mMeshBuffer.SetTopology(MeshBuffer::Topology::Lines);
		mMeshBuffer.Render();

		mMeshBuffer.Update(mFillVertices.get(), mFillVertexCount);
		mMeshBuffer.SetTopology(MeshBuffer::Topology::Triangles);
		mMeshBuffer.Render();

		mLineVertexCount = 0;
		mFillVertexCount = 0;
	}

	std::unique_ptr<SimpleDrawImpl> sInstance;
}

void SimpleDraw::StaticInitialize(uint32_t maxVertexCount)
{
	sInstance = std::make_unique<SimpleDrawImpl>();
	sInstance->Initialize(maxVertexCount);
}

void SimpleDraw::StaticTerminate()
{
	sInstance->Terminate();
	sInstance.reset();
}

void SimpleDraw::AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color)
{
	sInstance->AddLine(v0, v1, color);
}

void Sage::Graphics::SimpleDraw::AddCircle(const Math::Vector3& center, const float radius, const Color& color)
{
	int sections = 10 * static_cast<int>(radius);
	float angleStep = Math::Constants::TwoPi / sections;
	for (int i = 0; i < sections; ++i)
	{
		float xx = cos(i * angleStep) * radius;
		float zz = sin(i * angleStep) * radius;
		Math::Vector3 v1 = Math::Vector3{xx, 0.0f, zz} + center;

		xx = cos((i + 1) * angleStep) * radius;
		zz = sin((i + 1) * angleStep) * radius;
		Math::Vector3 v2 = Math::Vector3{ xx, 0.0f, zz } + center;

		sInstance->AddLine(v1, v2, color);
	}
}

void Sage::Graphics::SimpleDraw::AddAABB(const Math::Vector3& center, const Math::Vector3& extend, const Color& color, bool fill)
{
	auto PPP = center + extend;
	auto NPP = center + Math::Vector3{ -extend.x, extend.y, extend.z };
	auto PNP = center + Math::Vector3{ extend.x, -extend.y, extend.z };
	auto PPN = center + Math::Vector3{ extend.x, extend.y, -extend.z };
	auto NNP = center + Math::Vector3{ -extend.x, -extend.y, extend.z };
	auto NPN = center + Math::Vector3{ -extend.x, extend.y, -extend.z };
	auto PNN = center + Math::Vector3{ extend.x, -extend.y, -extend.z };
	auto NNN = center + Math::Vector3{ -extend.x, -extend.y, -extend.z };

	if (fill)
	{
		//Back
		sInstance->AddFace(PPP, NPP, PNP, color);
		sInstance->AddFace(NNP, PNP, NPP, color);

		//Font
		sInstance->AddFace(PPN, PNN, NPN, color);
		sInstance->AddFace(NNN, NPN, PNN, color);

		//Right
		sInstance->AddFace(PPP, PNP, PPN, color);
		sInstance->AddFace(PNN, PPN, PNP, color);

		//Left
		sInstance->AddFace(NPP, NPN, NNP, color);
		sInstance->AddFace(NNN, NNP, NPN, color);

		//Top
		sInstance->AddFace(PPP, PPN, NPP, color);
		sInstance->AddFace(NPN, NPP, PPN, color);

		//Bottom
		sInstance->AddFace(PNP, NNP, PNN, color);
		sInstance->AddFace(NNN, PNN, NNP, color);
	}
	else
	{
		sInstance->AddLine(PPP, NPP, color);
		sInstance->AddLine(PPP, PNP, color);
		sInstance->AddLine(PPP, PPN, color);

		sInstance->AddLine(NNP, PNP, color);
		sInstance->AddLine(NNP, NPP, color);
		sInstance->AddLine(NNP, NNN, color);

		sInstance->AddLine(PNN, NNN, color);
		sInstance->AddLine(PNN, PPN, color);
		sInstance->AddLine(PNN, PNP, color);

		sInstance->AddLine(NPN, PPN, color);
		sInstance->AddLine(NPN, NNN, color);
		sInstance->AddLine(NPN, NPP, color);
	}
}

void Sage::Graphics::SimpleDraw::AddOBB(const Math::OBB& obb, const Color& color)
{
	Math::Vector3 points[] =
	{
		{-1.0f, -1.0f, -1.0f},
		{-1.0f, +1.0f, -1.0f},
		{+1.0f, +1.0f, -1.0f},
		{+1.0f, -1.0f, -1.0f},
		{-1.0f, -1.0f, +1.0f},
		{-1.0f, +1.0f, +1.0f},
		{+1.0f, +1.0f, +1.0f},
		{+1.0f, -1.0f, +1.0f}
	};

	const Math::Matrix4 transform = Transform{ obb.center, obb.rotation, obb.extend }.GetMatrix4();
	for (auto& point : points)
		point = Math::TransformCoord(point, transform);

	sInstance->AddLine(points[0], points[1], color);
	sInstance->AddLine(points[1], points[2], color);
	sInstance->AddLine(points[2], points[3], color);
	sInstance->AddLine(points[3], points[0], color);

	sInstance->AddLine(points[4], points[5], color);
	sInstance->AddLine(points[5], points[6], color);
	sInstance->AddLine(points[6], points[7], color);
	sInstance->AddLine(points[7], points[4], color);

	sInstance->AddLine(points[0], points[4], color);
	sInstance->AddLine(points[1], points[5], color);
	sInstance->AddLine(points[2], points[6], color);
	sInstance->AddLine(points[3], points[7], color);
}

void Sage::Graphics::SimpleDraw::AddCone(const Math::Vector3& base, const Math::Vector3& direction, float radius, float height, const Color& color, int rings, int slices)
{
	float ringSpacing = height / (rings + 1);
	float sliceAngle = Math::Constants::TwoPi / slices;

	int vertexRings = rings + 2;
	int vertexSlices = slices + 1;

	//Create all vertices of the cone.
	std::vector<Math::Vector3> vertices(vertexRings * vertexSlices);
	Math::Vector3 rotationAxis = -Math::Cross(direction, { 0.0f, 1.0f, 0.0f });
	float rotate = acosf(Math::Dot(Math::Normalize(direction), { 0.0f, 1.0f, 0.0f }));
	for (int ring = 0; ring < vertexRings; ++ring)
	{
		for (int slice = 0; slice < vertexSlices; ++slice)
		{
			Math::Vector3 vertex{ 0.0f, ringSpacing * ring, 0.0f };
			vertex = Math::TransformCoord(vertex, Math::Matrix4::Translation({ 0.0f, 0.0f, radius - ((radius / (vertexRings - 1)) * ring) }));
			vertex = Math::TransformCoord(vertex, Math::Matrix4::RotationY(slice * sliceAngle));
			vertex = Math::TransformCoord(vertex, Math::Matrix4::RotationAxis(rotationAxis, rotate));
			vertex = Math::TransformCoord(vertex, Math::Matrix4::Translation(base));
			vertices[ring * vertexSlices + slice] = vertex;
		}
	}

	//Use the vertices to draw the cone's rings.
	for (int ring = 0; ring < vertexRings; ++ring)
	{
		for (int slice = 0; slice < vertexSlices - 1; ++slice)
		{
			sInstance->AddLine(vertices[ring * vertexSlices + slice], vertices[ring * vertexSlices + slice + 1], color);
		}
	}

	//Use the vertices to draw the cone's slices.
	for (int slice = 0; slice < vertexSlices; ++slice)
	{
		for (int ring = 0; ring < vertexRings - 1; ++ring)
		{
			sInstance->AddLine(vertices[ring * vertexSlices + slice], vertices[(ring + 1) * vertexSlices + slice], color);
		}
	}

	//Draw bottom cap of cone.
	for (int slice = 0; slice < vertexSlices; ++slice)
	{
		sInstance->AddLine(base, vertices[slice], color);
	}
}

void Sage::Graphics::SimpleDraw::AddCylinder(const Math::Vector3& base, const Math::Vector3& direction, float radius, float height, const Color& color, int rings, int slices)
{
	float ringSpacing = height / (rings + 1);
	float sliceAngle = Math::Constants::TwoPi / slices;

	int vertexRings = rings + 2;
	int vertexSlices = slices + 1;

	//Create all vertices of the cylinder.
	std::vector<Math::Vector3> vertices(vertexRings * vertexSlices);
	Math::Vector3 rotationAxis = -Math::Cross(direction, { 0.0f, 1.0f, 0.0f });
	float rotate = acosf(Math::Dot(Math::Normalize(direction), { 0.0f, 1.0f, 0.0f }));
	for (int ring = 0; ring < vertexRings; ++ring)
	{
		for (int slice = 0; slice < vertexSlices; ++slice)
		{
			Math::Vector3 vertex{ 0.0f, ringSpacing * ring, 0.0f };
			vertex = Math::TransformCoord(vertex, Math::Matrix4::Translation({ 0.0f, 0.0f, radius }));
			vertex = Math::TransformCoord(vertex, Math::Matrix4::RotationY(slice * sliceAngle));
			vertex = Math::TransformCoord(vertex, Math::Matrix4::RotationAxis(rotationAxis, rotate));
			vertex = Math::TransformCoord(vertex, Math::Matrix4::Translation(base));
			vertices[ring * vertexSlices + slice] = vertex;
		}
	}

	//Use the vertices to draw the cylinder's rings.
	for (int ring = 0; ring < vertexRings; ++ring)
	{
		for (int slice = 0; slice < vertexSlices - 1; ++slice)
		{
			sInstance->AddLine(vertices[ring * vertexSlices + slice], vertices[ring * vertexSlices + slice + 1], color);
		}
	}

	//Use the vertices to draw the cylinder's slices.
	for (int slice = 0; slice < vertexSlices; ++slice)
	{
		for (int ring = 0; ring < vertexRings - 1; ++ring)
		{
			sInstance->AddLine(vertices[ring * vertexSlices + slice], vertices[(ring + 1) * vertexSlices + slice], color);
		}
	}

	//Draw bottom cap of cylinder.
	for (int slice = 0; slice < vertexSlices; ++slice)
	{
		sInstance->AddLine(base, vertices[slice], color);
	}

	//Draw top cap of cylinder.
	for (int slice = 0; slice < vertexSlices; ++slice)
	{
		Math::Vector3 vertex = Math::TransformCoord({ 0.0f, height, 0.0f }, Math::Matrix4::RotationAxis(rotationAxis, rotate));
		vertex += {base.x, base.y, base.z};
		sInstance->AddLine(vertex, vertices[(vertexRings - 1) * vertexSlices + slice], color);
	}
}

void Sage::Graphics::SimpleDraw::AddSphere(const Math::Vector3& center, float radius, const Color& color, int rings, int slices)
{
	float ringSpacing = (radius * 2) / (rings + 1);
	float sliceAngle = Math::Constants::TwoPi / slices;

	int vertexRings = rings + 2;
	int vertexSlices = slices + 1;

	//Create all vertices of the sphere.
	std::vector<Math::Vector3> vertices(vertexRings * vertexSlices);
	for (int ring = 0; ring < vertexRings; ++ring)
	{
		for (int slice = 0; slice < vertexSlices; ++slice)
		{
			Math::Vector3 vertex{0.0f, radius - (ringSpacing * ring), 0.0f};
			float h = abs(vertex.y);
			float r = sqrt(radius * radius - h * h);
			vertex = Math::TransformCoord(vertex, Math::Matrix4::Translation({ 0.0f, 0.0f, r }));
			vertex = Math::TransformCoord(vertex, Math::Matrix4::RotationY(slice * sliceAngle));
			vertex = Math::TransformCoord(vertex, Math::Matrix4::Translation(center));
			vertices[ring * vertexSlices + slice] = vertex;
		}
	}

	//Use the vertices to draw the sphere's rings.
	for (int ring = 0; ring < vertexRings; ++ring)
	{
		for (int slice = 0; slice < vertexSlices - 1; ++slice)
		{
			sInstance->AddLine(vertices[ring * vertexSlices + slice], vertices[ring * vertexSlices + slice + 1], color);
		}
	}

	//Use the vertices to draw the sphere's slices.
	for (int slice = 0; slice < vertexSlices; ++slice)
	{
		for (int ring = 0; ring < vertexRings - 1; ++ring)
		{
			sInstance->AddLine(vertices[ring * vertexSlices + slice], vertices[(ring + 1) * vertexSlices + slice], color);
		}
	}
}

void Sage::Graphics::SimpleDraw::AddCapsule(const Math::Vector3& center, float radius, float height, const Color& color, int rings, int slices)
{
	float ringSpacing = (radius * 2) / (rings + 1);
	float sliceAngle = Math::Constants::TwoPi / slices;

	int vertexRings = rings + 2;
	int vertexSlices = slices + 1;

	if (height < radius)
	{
		height = radius;
	}

	float hOffset = (height - radius) / 2.0f;

	//Create all vertices of the capsule.
	std::vector<Math::Vector3> vertices(vertexRings * vertexSlices);
	for (int ring = 0; ring < vertexRings; ++ring)
	{
		for (int slice = 0; slice < vertexSlices; ++slice)
		{
			Math::Vector3 vertex{ 0.0f, radius - (ringSpacing * ring), 0.0f };
			float h = abs(vertex.y);
			if (ring > vertexRings / 2)
				vertex.y -= hOffset;
			else
				vertex.y += hOffset;
			float r = sqrt(radius * radius - h * h);
			vertex = Math::TransformCoord(vertex, Math::Matrix4::Translation({ 0.0f, 0.0f, r }));
			vertex = Math::TransformCoord(vertex, Math::Matrix4::RotationY(slice * sliceAngle));
			vertex = Math::TransformCoord(vertex, Math::Matrix4::Translation(center));
			vertices[ring * vertexSlices + slice] = vertex;
		}
	}

	//Use the vertices to draw the capsule's rings.
	for (int ring = 0; ring < vertexRings; ++ring)
	{
		for (int slice = 0; slice < vertexSlices - 1; ++slice)
		{
			sInstance->AddLine(vertices[ring * vertexSlices + slice], vertices[ring * vertexSlices + slice + 1], color);
		}
	}

	//Use the vertices to draw the capsule's slices.
	for (int slice = 0; slice < vertexSlices; ++slice)
	{
		for (int ring = 0; ring < vertexRings - 1; ++ring)
		{
			sInstance->AddLine(vertices[ring * vertexSlices + slice], vertices[(ring + 1) * vertexSlices + slice], color);
		}
	}
}

void SimpleDraw::AddTransform(const Math::Matrix4& transform)
{
	auto r = Math::GetRight(transform);
	auto u = Math::GetUp(transform);
	auto l = Math::GetLook(transform);
	auto p = Math::GetTranslation(transform);
	sInstance->AddLine(p, p + r, Colors::Red);
	sInstance->AddLine(p, p + u, Colors::Green);
	sInstance->AddLine(p, p + l, Colors::Blue);
}

void Sage::Graphics::SimpleDraw::AddGroundPlane(int size, const Color& color)
{
	auto p = Math::Vector3{ size / 2.0f, 0.0f, size / 2.0f };
	for (int i = 0; i < size + 1; ++i)
	{
		sInstance->AddLine(Math::Vector3{ p.x - i, p.y, p.z    }, Math::Vector3{  p.x - i, p.y, -p.z    }, color);
		sInstance->AddLine(Math::Vector3{ p.x    , p.y, p.z - i}, Math::Vector3{ -p.x    , p.y,  p.z - i}, color);
	}
}


void SimpleDraw::Render(const Camera& camera)
{
	sInstance->Render(camera);
}