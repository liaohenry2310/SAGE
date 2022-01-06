#include "Precompiled.h"
#include "MeshBuilder.h"

using namespace Sage::Graphics;
using namespace Sage::Math;

MeshPC MeshBuilder::CreateCubePC()
{
	MeshPC mesh;

	mesh.vertices.emplace_back(Vector3{ -1.0f,  1.0f, -1.0f }, Color{ 1.0f, 1.0f, 0.0f, 1.0f });
	mesh.vertices.emplace_back(Vector3{ 1.0f,  1.0f, -1.0f }, Color{ 1.0f, 1.0f, 0.0f, 1.0f });
	mesh.vertices.emplace_back(Vector3{ 1.0f, -1.0f, -1.0f }, Color{ 0.0f, 1.0f, 1.0f, 1.0f });
	mesh.vertices.emplace_back(Vector3{ -1.0f, -1.0f, -1.0f }, Color{ 0.0f, 1.0f, 1.0f, 1.0f });
	mesh.vertices.emplace_back(Vector3{ -1.0f,  1.0f,  1.0f }, Color{ 1.0f, 0.0f, 1.0f, 1.0f });
	mesh.vertices.emplace_back(Vector3{ 1.0f,  1.0f,  1.0f }, Color{ 1.0f, 0.0f, 1.0f, 1.0f });
	mesh.vertices.emplace_back(Vector3{ 1.0f, -1.0f,  1.0f }, Color{ 1.0f, 1.0f, 1.0f, 1.0f });
	mesh.vertices.emplace_back(Vector3{ -1.0f, -1.0f,  1.0f }, Color{ 1.0f, 1.0f, 1.0f, 1.0f });

	//Front side
	mesh.indices.emplace_back(0);
	mesh.indices.emplace_back(1);
	mesh.indices.emplace_back(2);
	mesh.indices.emplace_back(2);
	mesh.indices.emplace_back(3);
	mesh.indices.emplace_back(0);
	//Back side
	mesh.indices.emplace_back(5);
	mesh.indices.emplace_back(4);
	mesh.indices.emplace_back(7);
	mesh.indices.emplace_back(7);
	mesh.indices.emplace_back(6);
	mesh.indices.emplace_back(5);
	//Top side
	mesh.indices.emplace_back(0);
	mesh.indices.emplace_back(4);
	mesh.indices.emplace_back(5);
	mesh.indices.emplace_back(5);
	mesh.indices.emplace_back(1);
	mesh.indices.emplace_back(0);
	//Bottom side
	mesh.indices.emplace_back(3);
	mesh.indices.emplace_back(2);
	mesh.indices.emplace_back(6);
	mesh.indices.emplace_back(6);
	mesh.indices.emplace_back(2);
	mesh.indices.emplace_back(1);
	//Right side
	mesh.indices.emplace_back(1);
	mesh.indices.emplace_back(5);
	mesh.indices.emplace_back(6);
	mesh.indices.emplace_back(6);
	mesh.indices.emplace_back(7);
	mesh.indices.emplace_back(3);
	//Left side
	mesh.indices.emplace_back(4);
	mesh.indices.emplace_back(0);
	mesh.indices.emplace_back(3);
	mesh.indices.emplace_back(3);
	mesh.indices.emplace_back(7);
	mesh.indices.emplace_back(4);

	return mesh;
}

MeshPX MeshBuilder::CreateCubePX()
{
	MeshPX mesh;

	//Cube Front
	mesh.vertices.emplace_back(Vector3{ -0.5f,  0.5f, -0.5f }, Vector2{ 0.0f, 0.0f });
	mesh.vertices.emplace_back(Vector3{ 0.5f,  0.5f, -0.5f }, Vector2{ 1.0f, 0.0f });
	mesh.vertices.emplace_back(Vector3{ 0.5f, -0.5f, -0.5f }, Vector2{ 1.0f, 1.0f });
	mesh.vertices.emplace_back(Vector3{ -0.5f, -0.5f, -0.5f }, Vector2{ 0.0f, 1.0f });

	//Cube Back
	mesh.vertices.emplace_back(Vector3{ 0.5f,  0.5f, 0.5f }, Vector2{ 0.0f, 0.0f });
	mesh.vertices.emplace_back(Vector3{ -0.5f,  0.5f, 0.5f }, Vector2{ 1.0f, 0.0f });
	mesh.vertices.emplace_back(Vector3{ -0.5f, -0.5f, 0.5f }, Vector2{ 1.0f, 1.0f });
	mesh.vertices.emplace_back(Vector3{ 0.5f, -0.5f, 0.5f }, Vector2{ 0.0f, 1.0f });

	//Cube Right
	mesh.vertices.emplace_back(Vector3{ 0.5f,  0.5f, -0.5f }, Vector2{ 0.0f, 0.0f });
	mesh.vertices.emplace_back(Vector3{ 0.5f,  0.5f,  0.5f }, Vector2{ 1.0f, 0.0f });
	mesh.vertices.emplace_back(Vector3{ 0.5f, -0.5f,  0.5f }, Vector2{ 1.0f, 1.0f });
	mesh.vertices.emplace_back(Vector3{ 0.5f, -0.5f, -0.5f }, Vector2{ 0.0f, 1.0f });

	//Cube Left
	mesh.vertices.emplace_back(Vector3{ -0.5f,  0.5f,  0.5f }, Vector2{ 0.0f, 0.0f });
	mesh.vertices.emplace_back(Vector3{ -0.5f,  0.5f, -0.5f }, Vector2{ 1.0f, 0.0f });
	mesh.vertices.emplace_back(Vector3{ -0.5f, -0.5f, -0.5f }, Vector2{ 1.0f, 1.0f });
	mesh.vertices.emplace_back(Vector3{ -0.5f, -0.5f,  0.5f }, Vector2{ 0.0f, 1.0f });

	//Cube Top
	mesh.vertices.emplace_back(Vector3{ -0.5f,  0.5f,  0.5f }, Vector2{ 0.0f, 0.0f });
	mesh.vertices.emplace_back(Vector3{ 0.5f,  0.5f,  0.5f }, Vector2{ 1.0f, 0.0f });
	mesh.vertices.emplace_back(Vector3{ 0.5f,  0.5f, -0.5f }, Vector2{ 1.0f, 1.0f });
	mesh.vertices.emplace_back(Vector3{ -0.5f,  0.5f, -0.5f }, Vector2{ 0.0f, 1.0f });

	//Cube Bottom
	mesh.vertices.emplace_back(Vector3{ -0.5f, -0.5f, -0.5f }, Vector2{ 0.0f, 0.0f });
	mesh.vertices.emplace_back(Vector3{ 0.5f, -0.5f, -0.5f }, Vector2{ 1.0f, 0.0f });
	mesh.vertices.emplace_back(Vector3{ 0.5f, -0.5f,  0.5f }, Vector2{ 1.0f, 1.0f });
	mesh.vertices.emplace_back(Vector3{ -0.5f, -0.5f,  0.5f }, Vector2{ 0.0f, 1.0f });

	//Front side
	mesh.indices.emplace_back(0);
	mesh.indices.emplace_back(1);
	mesh.indices.emplace_back(2);
	mesh.indices.emplace_back(2);
	mesh.indices.emplace_back(3);
	mesh.indices.emplace_back(0);

	//Back side
	mesh.indices.emplace_back(4);
	mesh.indices.emplace_back(5);
	mesh.indices.emplace_back(6);
	mesh.indices.emplace_back(6);
	mesh.indices.emplace_back(7);
	mesh.indices.emplace_back(4);

	//Right side
	mesh.indices.emplace_back(8);
	mesh.indices.emplace_back(9);
	mesh.indices.emplace_back(10);
	mesh.indices.emplace_back(10);
	mesh.indices.emplace_back(11);
	mesh.indices.emplace_back(8);

	//Left side
	mesh.indices.emplace_back(12);
	mesh.indices.emplace_back(13);
	mesh.indices.emplace_back(14);
	mesh.indices.emplace_back(14);
	mesh.indices.emplace_back(15);
	mesh.indices.emplace_back(12);

	//Left Top
	mesh.indices.emplace_back(16);
	mesh.indices.emplace_back(17);
	mesh.indices.emplace_back(18);
	mesh.indices.emplace_back(18);
	mesh.indices.emplace_back(19);
	mesh.indices.emplace_back(16);

	//Left Bottom
	mesh.indices.emplace_back(20);
	mesh.indices.emplace_back(21);
	mesh.indices.emplace_back(22);
	mesh.indices.emplace_back(22);
	mesh.indices.emplace_back(23);
	mesh.indices.emplace_back(20);

	return mesh;
}

MeshPX MeshBuilder::CreatePlanePX(uint32_t rows, uint32_t columns, float spacing)
{
	MeshPX mesh;

	if (rows <= 0)
		rows = 10;
	if (columns <= 0)
		columns = 10;
	if (spacing <= 0.0f)
		spacing = 1.0f;

	const float width = (columns - 1) * spacing;
	const float height = (rows - 1) * spacing;
	Vector3 offset = { width * -0.5f, 0.0f, height * -0.5f };

	const float uStep = 1.0f / static_cast<float>(columns - 1);
	const float vStep = 1.0f / static_cast<float>(rows - 1);

	for (uint32_t z = 0; z < rows; ++z)
	{
		for (uint32_t x = 0; x < columns; ++x)
		{
			float xx = x * spacing;
			float zz = z * spacing;
			float u = x * uStep;
			float v = 1.0f - z * vStep;
			Vector3 position{ xx, 0.0f, zz };
			Vector2 uv{ u, v };
			mesh.vertices.emplace_back(position + offset, uv);
		}
	}

	for (uint32_t z = 0; z + 1 < rows; ++z)
	{
		for (uint32_t x = 0; x + 1 < columns; ++x)
		{
			mesh.indices.push_back((x + 0) + ((z + 0) * columns));
			mesh.indices.push_back((x + 0) + ((z + 1) * columns));
			mesh.indices.push_back((x + 1) + ((z + 1) * columns));

			mesh.indices.push_back((x + 0) + ((z + 0) * columns));
			mesh.indices.push_back((x + 1) + ((z + 1) * columns));
			mesh.indices.push_back((x + 1) + ((z + 0) * columns));
		}
	}

	return mesh;
}

Mesh MeshBuilder::CreatePlane(uint32_t rows, uint32_t columns, float spacing, float tiling)
{
	Mesh mesh;

	if (rows <= 0)
		rows = 10;
	if (columns <= 0)
		columns = 10;
	if (spacing <= 0.0f)
		spacing = 1.0f;

	const float width = (columns - 1) * spacing;
	const float height = (rows - 1) * spacing;
	Vector3 offset = { width * -0.5f, 0.0f, height * -0.5f };

	const float uStep = 1.0f / static_cast<float>(columns - 1);
	const float vStep = 1.0f / static_cast<float>(rows - 1);

	for (uint32_t z = 0; z < rows; ++z)
	{
		for (uint32_t x = 0; x < columns; ++x)
		{
			float xx = x * spacing;
			float zz = z * spacing;
			float u = x * uStep * tiling;
			float v = 1.0f - z * vStep * tiling;
			Vector3 position{ xx, 0.0f, zz };
			Vector2 uv{ u, v };
			mesh.vertices.emplace_back(position + offset, Vector3::YAxis, Vector3::XAxis, uv);
		}
	}

	for (uint32_t z = 0; z + 1 < rows; ++z)
	{
		for (uint32_t x = 0; x + 1 < columns; ++x)
		{
			mesh.indices.push_back((x + 0) + ((z + 0) * columns));
			mesh.indices.push_back((x + 0) + ((z + 1) * columns));
			mesh.indices.push_back((x + 1) + ((z + 1) * columns));

			mesh.indices.push_back((x + 0) + ((z + 0) * columns));
			mesh.indices.push_back((x + 1) + ((z + 1) * columns));
			mesh.indices.push_back((x + 1) + ((z + 0) * columns));
		}
	}

	return mesh;
}

MeshPX Sage::Graphics::MeshBuilder::CreateCylinderPX(uint32_t rings, uint32_t slices)
{
	return MeshPX();
}

MeshPX Sage::Graphics::MeshBuilder::CreateConePX(uint32_t rings, uint32_t slices)
{
	return MeshPX();
}

MeshPX MeshBuilder::CreateSpherePX(uint32_t rings, uint32_t slices)
{
	MeshPX mesh;

	if (rings <= 0)
		rings = 8;
	if (slices <= 0)
		slices = 8;

	float sliceAngleStep = Math::Constants::TwoPi / static_cast<float>(slices);
	float ringAngleStep = Math::Constants::Pi / static_cast<float>(rings - 1);

	const float uStep = 1.0f / static_cast<float>(slices);
	const float vStep = 1.0f / static_cast<float>(rings - 1);

	for (uint32_t z = 0; z < rings; ++z)
	{
		float yy = sin((z * ringAngleStep) - (Math::Constants::Pi / 2.0f)) * 0.5f;
		float radius = sqrt(0.25f - (yy * yy));

		for (uint32_t x = 0; x <= slices; ++x)
		{
			float xx = cos(x * sliceAngleStep) * radius;
			float zz = sin(x * sliceAngleStep) * radius;

			float u = x * uStep;
			float v = 1.0f - z * vStep;

			Vector3 position{ xx, yy, zz };
			Vector2 uv{ u, v };
			mesh.vertices.emplace_back(position, uv);
		}
	}

	for (uint32_t r = 0; r + 1 < rings; ++r)
	{
		for (uint32_t s = 0; s < slices; ++s)
		{
			mesh.indices.push_back((s + 0) + ((r + 0) * (slices + 1)));
			mesh.indices.push_back((s + 0) + ((r + 1) * (slices + 1)));
			mesh.indices.push_back((s + 1) + ((r + 1) * (slices + 1)));

			mesh.indices.push_back((s + 0) + ((r + 0) * (slices + 1)));
			mesh.indices.push_back((s + 1) + ((r + 1) * (slices + 1)));
			mesh.indices.push_back((s + 1) + ((r + 0) * (slices + 1)));
		}
	}

	return mesh;
}

Mesh Sage::Graphics::MeshBuilder::CreatSphere(uint32_t rings, uint32_t slices)
{
	Mesh mesh;

	if (rings <= 2)
		rings = 8;
	if (slices <= 2)
		slices = 8;

	float sliceAngleStep = Math::Constants::TwoPi / static_cast<float>(slices);
	float ringAngleStep = Math::Constants::Pi / static_cast<float>(rings - 1);

	const float uStep = 1.0f / static_cast<float>(slices);
	const float vStep = 1.0f / static_cast<float>(rings - 1);

	for (uint32_t z = 0; z < rings; ++z)
	{
		float yy = sin((z * ringAngleStep) - (Math::Constants::Pi / 2.0f)) * 0.5f;
		float radius = sqrt(0.25f - (yy * yy));

		for (uint32_t x = 0; x <= slices; ++x)
		{
			float xx = cos(x * sliceAngleStep) * radius;
			float zz = sin(x * sliceAngleStep) * radius;

			float u = x * uStep;
			float v = 1.0f - z * vStep;

			Vector3 position{ xx, yy, zz };
			Vector3 normal = Sage::Math::Normalize(position);
			Vector3 tangent = Sage::Math::Normalize({-normal.z, 0.0f, normal.x});
			Vector2 uv{ u, v };

			mesh.vertices.emplace_back(position, normal, tangent, uv);
		}
	}

	for (uint32_t r = 0; r + 1 < rings; ++r)
	{
		for (uint32_t s = 0; s < slices; ++s)
		{
			mesh.indices.push_back((s + 0) + ((r + 0) * (slices + 1)));
			mesh.indices.push_back((s + 0) + ((r + 1) * (slices + 1)));
			mesh.indices.push_back((s + 1) + ((r + 1) * (slices + 1)));

			mesh.indices.push_back((s + 0) + ((r + 0) * (slices + 1)));
			mesh.indices.push_back((s + 1) + ((r + 1) * (slices + 1)));
			mesh.indices.push_back((s + 1) + ((r + 0) * (slices + 1)));
		}
	}

	return mesh;
}

MeshPX MeshBuilder::CreateSkyBox()
{
	MeshPX mesh;

	//Cube Back
	mesh.vertices.emplace_back(Vector3{ -10.0f,  10.0f, -10.0f }, Vector2{ 0.25f,  0.335f });
	mesh.vertices.emplace_back(Vector3{  10.0f,  10.0f, -10.0f }, Vector2{ 0.50f,  0.335f });
	mesh.vertices.emplace_back(Vector3{  10.0f, -10.0f, -10.0f }, Vector2{ 0.50f,  0.665f });
	mesh.vertices.emplace_back(Vector3{ -10.0f, -10.0f, -10.0f }, Vector2{ 0.25f,  0.665f });

	//Cube Front
	mesh.vertices.emplace_back(Vector3{  10.0f,  10.0f, 10.0f }, Vector2{ 0.75f, 0.335f });
	mesh.vertices.emplace_back(Vector3{ -10.0f,  10.0f, 10.0f }, Vector2{ 0.999f, 0.335f });
	mesh.vertices.emplace_back(Vector3{ -10.0f, -10.0f, 10.0f }, Vector2{ 0.999f, 0.665f });
	mesh.vertices.emplace_back(Vector3{  10.0f, -10.0f, 10.0f }, Vector2{ 0.75f, 0.665f });

	//Cube Right
	mesh.vertices.emplace_back(Vector3{ 10.0f,  10.0f, -10.0f }, Vector2{ 0.50f, 0.335f });
	mesh.vertices.emplace_back(Vector3{ 10.0f,  10.0f,  10.0f }, Vector2{ 0.75F, 0.335f });
	mesh.vertices.emplace_back(Vector3{ 10.0f, -10.0f,  10.0f }, Vector2{ 0.75f, 0.665f });
	mesh.vertices.emplace_back(Vector3{ 10.0f, -10.0f, -10.0f }, Vector2{ 0.50f, 0.665f });

	//Cube Left
	mesh.vertices.emplace_back(Vector3{ -10.0f,  10.0f,  10.0f }, Vector2{ 0.001f, 0.335f });
	mesh.vertices.emplace_back(Vector3{ -10.0f,  10.0f, -10.0f }, Vector2{ 0.25f, 0.335f });
	mesh.vertices.emplace_back(Vector3{ -10.0f, -10.0f, -10.0f }, Vector2{ 0.25f, 0.665f });
	mesh.vertices.emplace_back(Vector3{ -10.0f, -10.0f,  10.0f }, Vector2{ 0.001f, 0.665f });

	//Cube Top
	mesh.vertices.emplace_back(Vector3{ -10.0f,  10.0f,  10.0f }, Vector2{ 0.25f, 0.000f });
	mesh.vertices.emplace_back(Vector3{  10.0f,  10.0f,  10.0f }, Vector2{ 0.50f, 0.000f });
	mesh.vertices.emplace_back(Vector3{  10.0f,  10.0f, -10.0f }, Vector2{ 0.50f, 0.335f });
	mesh.vertices.emplace_back(Vector3{ -10.0f,  10.0f, -10.0f }, Vector2{ 0.25f, 0.335f });

	//Cube Bottom
	mesh.vertices.emplace_back(Vector3{ -10.0f, -10.0f, -10.0f }, Vector2{ 0.25f, 0.665f });
	mesh.vertices.emplace_back(Vector3{  10.0f, -10.0f, -10.0f }, Vector2{ 0.50f, 0.665f });
	mesh.vertices.emplace_back(Vector3{  10.0f, -10.0f,  10.0f }, Vector2{ 0.50f, 1.000f });
	mesh.vertices.emplace_back(Vector3{ -10.0f, -10.0f,  10.0f }, Vector2{ 0.25f, 1.000f });

	//Front side
	mesh.indices.emplace_back(0);
	mesh.indices.emplace_back(1);
	mesh.indices.emplace_back(2);
	mesh.indices.emplace_back(2);
	mesh.indices.emplace_back(3);
	mesh.indices.emplace_back(0);

	//Back side
	mesh.indices.emplace_back(4);
	mesh.indices.emplace_back(5);
	mesh.indices.emplace_back(6);
	mesh.indices.emplace_back(6);
	mesh.indices.emplace_back(7);
	mesh.indices.emplace_back(4);

	//Right side
	mesh.indices.emplace_back(8);
	mesh.indices.emplace_back(9);
	mesh.indices.emplace_back(10);
	mesh.indices.emplace_back(10);
	mesh.indices.emplace_back(11);
	mesh.indices.emplace_back(8);

	//Left side
	mesh.indices.emplace_back(12);
	mesh.indices.emplace_back(13);
	mesh.indices.emplace_back(14);
	mesh.indices.emplace_back(14);
	mesh.indices.emplace_back(15);
	mesh.indices.emplace_back(12);

	//Left Top
	mesh.indices.emplace_back(16);
	mesh.indices.emplace_back(17);
	mesh.indices.emplace_back(18);
	mesh.indices.emplace_back(18);
	mesh.indices.emplace_back(19);
	mesh.indices.emplace_back(16);

	//Left Bottom
	mesh.indices.emplace_back(20);
	mesh.indices.emplace_back(21);
	mesh.indices.emplace_back(22);
	mesh.indices.emplace_back(22);
	mesh.indices.emplace_back(23);
	mesh.indices.emplace_back(20);

	return mesh;
}
