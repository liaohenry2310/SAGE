#include "Precompiled.h"
#include "ModelLoader.h"

using namespace Sage::Graphics;

void ModelLoader::LoadObj(std::filesystem::path fileName, float scale, MeshPX& mesh)
{
	FILE* file = nullptr;
	fopen_s(&file, fileName.u8string().c_str(), "r");
	ASSERT(file, "ModelLoader -- Failed to open model %s", fileName.u8string().c_str());

	std::vector<Math::Vector3> positions;
	std::vector<Math::Vector2> uvs;
	std::vector<uint32_t> positionIndices, uvIndices;

	while (true)
	{
		char buffer[256];
		int result = fscanf_s(file, "%s", buffer, (int)std::size(buffer));

		if (result == EOF)
			break;

		if (strcmp(buffer, "v") == 0)
		{
			Math::Vector3 position;
			fscanf_s(file, "%f %f %f\n", &position.x, &position.y, &position.z);
			positions.push_back(position * scale);
		}
		else if (strcmp(buffer, "vt") == 0)
		{
			Math::Vector2 uv;
			fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
			uvs.push_back({ uv.x, 1.0f - uv.y });
		}
		else if (strcmp(buffer, "f") == 0)
		{
			uint32_t v[3], t[3], n[3];
			fgets(buffer, (int)std::size(buffer), file);

			if (sscanf_s(buffer, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
				&v[0], &t[0], &n[0],
				&v[1], &t[1], &n[1], 
				&v[2], &t[2], &n[2]) == 9)
			{
				positionIndices.push_back(v[0]);
				positionIndices.push_back(v[1]);
				positionIndices.push_back(v[2]);
				uvIndices.push_back(t[0]);
				uvIndices.push_back(t[1]);
				uvIndices.push_back(t[2]);
			}
		}
	}

	fclose(file);

	//Reconstruct the vertices.
	std::vector<VertexPX> vertices;
	vertices.reserve(positionIndices.size());
	for (size_t i = 0;  i < positionIndices.size(); ++i)
	{
		VertexPX vertex;
		vertex.position = positions[positionIndices[i] - 1];
		vertex.uv = uvs[uvIndices[i] - 1];
		vertices.push_back(vertex);
	}

	// Generate indices
	std::vector<uint32_t> indices;
	indices.resize(vertices.size());
	std::iota(indices.begin(), indices.end(), 0);

	//Initialize mesh
	mesh.vertices = std::move(vertices);
	mesh.indices = std::move(indices);
}
