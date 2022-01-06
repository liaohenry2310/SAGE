#pragma once

#include "Colors.h"

namespace Sage::Graphics
{
	class Camera;
}

namespace Sage::Graphics::SimpleDraw
{
	void StaticInitialize(uint32_t maxVertexCount = 64000);
	void StaticTerminate();

	void AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color);
	
	void AddCircle(const Math::Vector3& center, const float radius, const Color& color);
	void AddAABB(const Math::Vector3& center, const Math::Vector3& extend, const Color& color, bool fill = false);
	void AddOBB(const Math::OBB& obb, const Color& color);
	void AddCone(const Math::Vector3& base, const Math::Vector3& direction, float radius, float height, const Color& color, int rings = 0, int slices = 8);
	void AddCylinder(const Math::Vector3& base, const Math::Vector3& direction, float radius, float height, const Color& color, int rings = 0, int slices = 8);
	void AddSphere(const Math::Vector3& center, float radius, const Color& color, int rings = 8, int slices = 16);
	void AddCapsule(const Math::Vector3& center, float radius, float height, const Color& color, int rings = 8, int slices = 16);
	void AddTransform(const Math::Matrix4& transform);
	void AddGroundPlane(int size, const Color& color = Colors::DarkGray);

	void Render(const Camera& camera);
}