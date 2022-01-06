#pragma once
#include "MeshTypes.h"

namespace Sage::Graphics
{
	class MeshBuilder
	{
	public:
		static MeshPC CreateCubePC();
		static MeshPX CreateCubePX();

		static MeshPX CreatePlanePX(uint32_t rows = 10, uint32_t columns = 10, float spacing = 1.0f);
		static Mesh CreatePlane(uint32_t rows = 10, uint32_t columns = 10, float spacing = 1.0f, float tiling = 1.0f);

		static MeshPX CreateCylinderPX(uint32_t rings, uint32_t slices);
		static MeshPX CreateConePX(uint32_t rings, uint32_t slices);
		static MeshPX CreateSpherePX(uint32_t rings = 8, uint32_t slices = 8);

		static Mesh CreatSphere(uint32_t rings = 8, uint32_t slices = 8);

		static MeshPX CreateSkyBox();
	};
}