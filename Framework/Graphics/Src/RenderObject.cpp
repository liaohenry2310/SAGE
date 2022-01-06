#include "Precompiled.h"
#include "RenderObject.h"

using namespace Sage;
using namespace Sage::Graphics;

void RenderObject::Terminate()
{
	diffuseMap.Terminate();
	specularMap.Terminate();
	bumpMap.Terminate();
	normalMap.Terminate();
	meshBuffer.Terminate();
}

RenderGroup Sage::Graphics::CreateRenderGroup(const Model& model, const Animator* animator)
{
	RenderGroup renderGroup;
	renderGroup.reserve(model.meshData.size());

	auto TryLoadTexture = [](auto& texture, const auto& textureName)
	{
		if (!textureName.empty())
			texture.Initialize(textureName);
	};

	for (auto& meshData : model.meshData)
	{
		auto& renderObject = renderGroup.emplace_back();

		auto& materialData = model.materialData[meshData.materialIndex];
		renderObject.material = materialData.material;

		TryLoadTexture(renderObject.diffuseMap, materialData.diffuseMapName);
		TryLoadTexture(renderObject.specularMap, materialData.specularMapName);
		TryLoadTexture(renderObject.bumpMap, materialData.bumpMapName);
		TryLoadTexture(renderObject.normalMap, materialData.normalMapName);

		renderObject.skeleton = model.skeleton.get();
		renderObject.animator = animator;

		renderObject.meshBuffer.Initialize(meshData.mesh);

		renderObject.backfaceCulling = false;
	}

	return renderGroup;
}

void Sage::Graphics::CleanUpRenderGroup(RenderGroup& renderGroup)
{
	for (auto& renderObject : renderGroup)
		renderObject.Terminate();
}
