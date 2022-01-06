#pragma once
#include "Service.h"

namespace Sage
{
	class TerrainService final : public Service
	{
	public:
		SET_TYPE_ID(ServiceId::Terrain);

		void Initialize() override;
		void Terminate() override;

		void DebugUI() override;

		const Graphics::RenderObject& GetTerrainRenderObject() const { return mTerrainRenderObject; }

		float GetHeight(const Math::Vector3& position) const;

	private:
		Sage::Graphics::Terrain mTerrain;
		Sage::Graphics::RenderObject mTerrainRenderObject;
	};
}