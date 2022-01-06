#pragma once

#include "Material.h"
#include "MeshBuffer.h"
#include "Model.h"
#include "Skeleton.h"
#include "Texture.h"
#include "Transform.h"

namespace Sage::Graphics
{
	class Animator;

	class RenderObject final
	{
	public:
		void Terminate();

		Transform transform;

		Material material;

		Texture diffuseMap;
		Texture specularMap;
		Texture bumpMap;
		Texture normalMap;

		MeshBuffer meshBuffer;

		const Skeleton* skeleton = nullptr;
		const Animator* animator = nullptr;

		bool backfaceCulling = true;
	};

	using RenderGroup = std::vector<RenderObject>;

	[[nodiscard]] RenderGroup CreateRenderGroup(const Model& model, const Animator* animator = nullptr);
	void CleanUpRenderGroup(RenderGroup& renderGroup);
}