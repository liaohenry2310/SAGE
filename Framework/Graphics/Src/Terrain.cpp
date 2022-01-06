#include "Precompiled.h"
#include "Terrain.h"

#include "MeshUtil.h"

using namespace Sage;
using namespace Sage::Graphics;

void Terrain::Initialize(std::filesystem::path fileName, float heightScale)
{
	FILE* file = nullptr;
	fopen_s(&file, fileName.u8string().c_str(), "rb");

	fseek(file, 0L, SEEK_END);
	const uint32_t fileSize = ftell(file);
	const uint32_t dimension = (uint32_t)sqrt(static_cast<float>(fileSize));
	fseek(file, 0L, SEEK_SET);

	rows = dimension;
	columns = dimension;

	const float tileCount = 20;

	mesh.vertices.resize(rows * columns);
	for (uint32_t z = 0; z < rows; ++z)
	{
		for (uint32_t x = 0; x < columns; ++x)
		{
			const float height = fgetc(file) / 255.0f * heightScale;
			const uint32_t index = x + (z * rows);

			auto& vertex = mesh.vertices[index];
			vertex.position = Math::Vector3{ static_cast<float>(x), height, static_cast<float>(z) };
			vertex.texCoord.x = static_cast<float>(x) / columns * tileCount;
			vertex.texCoord.y = static_cast<float>(z) / rows * tileCount;
		}
	}

	fclose(file);

	const uint32_t cells = (rows - 1) * (columns - 1);
	mesh.indices.reserve(cells * 6);
	for (uint32_t z = 0; z < rows - 1; ++z)
	{
		for (uint32_t x = 0; x < columns - 1; ++x)
		{
			const uint32_t bottomLeft = x + z * columns;
			const uint32_t topLeft = x + (z + 1) * columns;
			const uint32_t bottomRight = (x + 1) + z * columns;
			const uint32_t topRight = (x + 1) + (z + 1) * columns;

			mesh.indices.push_back(bottomLeft);
			mesh.indices.push_back(topLeft);
			mesh.indices.push_back(topRight);

			mesh.indices.push_back(bottomLeft);
			mesh.indices.push_back(topRight);
			mesh.indices.push_back(bottomRight);
		}
	}

	MeshUtil::ComputeNormal(mesh);
}