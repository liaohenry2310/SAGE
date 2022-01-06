#pragma once

#include "Material.h"
#include "MeshBuffer.h"
#include "MeshTypes.h"
#include "Texture.h"

namespace Sage::Graphics
{
	class Terrain final
	{
	public:
		void Initialize(std::filesystem::path fileName, float heightScale);

		Mesh mesh;
		uint32_t rows = 0;
		uint32_t columns = 0;
	};
}