#include "Precompiled.h"
#include "ModelIO.h"

#include "AnimationIO.h"
#include "Model.h"

using namespace Sage;
using namespace Sage::Graphics;

void ModelIO::SaveModel(std::filesystem::path filePath, const Model& model)
{
	if (model.meshData.empty())
		return;

	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "w");
	if (file == nullptr)
		return;

	//Saving meshes
	const uint32_t meshCount = static_cast<uint32_t>(model.meshData.size());
	fprintf_s(file, "MeshCount: %d\n", meshCount);
	for (uint32_t i = 0; i < meshCount; ++i)
	{
		const auto& meshData = model.meshData[i];
		fprintf_s(file, "MaterialIndex: %d\n", meshData.materialIndex);

		const auto& mesh = meshData.mesh;
		const uint32_t vertexCount = static_cast<uint32_t>(mesh.vertices.size());
		fprintf_s(file, "VertexCount: %d\n", vertexCount);

		for (auto& vertex : mesh.vertices)
		{
			fprintf_s(file, "%f %f %f %f %f %f %f %f %f %f %f %d %d %d %d %f %f %f %f\n",
				vertex.position.x, vertex.position.y, vertex.position.z,
				vertex.normal.x,vertex.normal.y,vertex.normal.z,
				vertex.tangent.x,vertex.tangent.y,vertex.tangent.z,
				vertex.texCoord.x,vertex.texCoord.y,
				vertex.boneIndices[0],vertex.boneIndices[1],vertex.boneIndices[2],vertex.boneIndices[3],
				vertex.boneWeights[0],vertex.boneWeights[1], vertex.boneWeights[2], vertex.boneWeights[3]);
		}

		const uint32_t indicesCount = static_cast<uint32_t>(mesh.indices.size());
		fprintf_s(file, "IndexCount: %d\n", indicesCount);

		for (auto& index : mesh.indices)
		{
			fprintf_s(file, "%d\n", index);
		}
	}

	fclose(file);
}

void ModelIO::LoadModel(std::filesystem::path filePath, Model& model)
{
	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "r");
	if (file == nullptr)
		return;

	//Loading meshes
	uint32_t meshCount = 0;
	fscanf_s(file, "MeshCount: %d\n", &meshCount);

	model.meshData.resize(meshCount);
	for (uint32_t i = 0; i < meshCount; ++i)
	{
		auto& meshData = model.meshData[i];
		fscanf_s(file, "MaterialIndex: %d\n", &meshData.materialIndex);

		auto& mesh = meshData.mesh;
		uint32_t vertexCount = 0;
		fscanf_s(file, "VertexCount: %d\n", &vertexCount);

		mesh.vertices.resize(vertexCount);
		for (uint32_t i = 0; i < vertexCount; ++i)
		{
			fscanf_s(file, "%f %f %f %f %f %f %f %f %f %f %f %d %d %d %d %f %f %f %f\n",
				&mesh.vertices[i].position.x, &mesh.vertices[i].position.y,&mesh.vertices[i].position.z,
				&mesh.vertices[i].normal.x, &mesh.vertices[i].normal.y,&mesh.vertices[i].normal.z,
				&mesh.vertices[i].tangent.x, &mesh.vertices[i].tangent.y,&mesh.vertices[i].tangent.z,
				&mesh.vertices[i].texCoord.x, &mesh.vertices[i].texCoord.y,
				&mesh.vertices[i].boneIndices[0], &mesh.vertices[i].boneIndices[1], &mesh.vertices[i].boneIndices[2], &mesh.vertices[i].boneIndices[3],
				&mesh.vertices[i].boneWeights[0], &mesh.vertices[i].boneWeights[1], &mesh.vertices[i].boneWeights[2], &mesh.vertices[i].boneWeights[3]);
		}

		uint32_t indicesCount = 0;
		fscanf_s(file, "IndexCount: %d\n", &indicesCount);

		mesh.indices.resize(indicesCount);
		for (uint32_t i = 0; i < indicesCount; ++i)
		{
			fscanf_s(file, "%d\n", &mesh.indices[i]);
		}
	}

	fclose(file);
}

void ModelIO::SaveMaterial(std::filesystem::path filePath, const Model& model)
{
	if (model.materialData.empty())
		return;

	filePath.replace_extension("material");

	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "w");
	if (file == nullptr)
		return;

	const uint32_t materialCount = static_cast<uint32_t>(model.materialData.size());
	fprintf_s(file, "MaterialCount: %d\n", materialCount);
	for (uint32_t i = 0; i < materialCount; ++i)
	{
		const auto& material = model.materialData[i];

		const auto& diffuse = material.material.diffuse;
		const auto& ambient = material.material.ambient;
		const auto& specular = material.material.specular;
		const auto& power = material.material.power;

		fprintf_s(file, "Diffuse: %f %f %f %f\n", diffuse.r, diffuse.g, diffuse.b, diffuse.a);
		fprintf_s(file, "Ambient: %f %f %f %f\n", ambient.r, ambient.g, ambient.b, ambient.a);
		fprintf_s(file, "Specular: %f %f %f %f\n", specular.r, specular.g, specular.b, specular.a);
		fprintf_s(file, "Power: %f\n", power);

		fprintf_s(file, "%s\n", !material.diffuseMapName.empty() ? material.diffuseMapName.c_str() : "null");
		fprintf_s(file, "%s\n", !material.specularMapName.empty() ? material.specularMapName.c_str() : "null");
		fprintf_s(file, "%s\n", !material.normalMapName.empty() ? material.normalMapName.c_str() : "null");
		fprintf_s(file, "%s\n", !material.bumpMapName.empty() ? material.bumpMapName.c_str() : "null");
	}

	fclose(file);
}

void ModelIO::LoadMaterial(std::filesystem::path filePath, Model& model)
{
	filePath.replace_extension("material");

	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "r");
	if (file == nullptr)
		return;

	uint32_t materialCount = 0;
	fscanf_s(file, "MaterialCount: %d\n", &materialCount);

	auto TryReadTextureName = [&](auto& destName)
	{
		char textureName[1024]{};
		if (fscanf_s(file, "%s\n", textureName, (uint32_t)sizeof(textureName)) && strcmp(textureName, "null") != 0)
		{
			destName = filePath.replace_filename(textureName).u8string().c_str();
		}
	};

	model.materialData.resize(materialCount);
	for (uint32_t i = 0; i < materialCount; ++i)
	{
		auto& material = model.materialData[i];

		auto& diffuse = material.material.diffuse;
		auto& ambient = material.material.ambient;
		auto& specular = material.material.specular;
		auto& power = material.material.power;

		fscanf_s(file, "Diffuse: %f %f %f %f\n", &diffuse.r, &diffuse.g, &diffuse.b, &diffuse.a);
		fscanf_s(file, "Ambient: %f %f %f %f\n", &ambient.r, &ambient.g, &ambient.b, &ambient.a);
		fscanf_s(file, "Specular: %f %f %f %f\n", &specular.r, &specular.g, &specular.b, &specular.a);
		fscanf_s(file, "Power: %f\n", &power);

		TryReadTextureName(material.diffuseMapName);
		TryReadTextureName(material.specularMapName);
		TryReadTextureName(material.bumpMapName);
		TryReadTextureName(material.normalMapName);
	}

	fclose(file);
}

void ModelIO::SaveSkeleton(std::filesystem::path filePath, const Model& model)
{
	if (!model.skeleton)
		return;

	filePath.replace_extension("skeleton");

	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "w");
	if (file == nullptr)
		return;

	const uint32_t boneCount = static_cast<uint32_t>(model.skeleton->bones.size());

	fprintf_s(file, "BoneCount: %d\n", boneCount);

	for (uint32_t i = 0; i < boneCount; ++i)
	{
		const auto& bone = *model.skeleton->bones[i];
		fprintf_s(file, "Name: %s\n", bone.name.c_str());
		fprintf_s(file, "Index: %d\n", bone.index);
		fprintf_s(file, "ParentIndex: %d\n", bone.parentIndex);

		fprintf_s(file, "ToParentTransform: %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
			bone.toParentTransform._11, bone.toParentTransform._12, bone.toParentTransform._13, bone.toParentTransform._14,
			bone.toParentTransform._21, bone.toParentTransform._22, bone.toParentTransform._23, bone.toParentTransform._24,
			bone.toParentTransform._31, bone.toParentTransform._32, bone.toParentTransform._33, bone.toParentTransform._34,
			bone.toParentTransform._41, bone.toParentTransform._42, bone.toParentTransform._43, bone.toParentTransform._44);

		fprintf_s(file, "OffsetTransform: %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
			bone.offsetTransform._11, bone.offsetTransform._12, bone.offsetTransform._13, bone.offsetTransform._14,
			bone.offsetTransform._21, bone.offsetTransform._22, bone.offsetTransform._23, bone.offsetTransform._24,
			bone.offsetTransform._31, bone.offsetTransform._32, bone.offsetTransform._33, bone.offsetTransform._34,
			bone.offsetTransform._41, bone.offsetTransform._42, bone.offsetTransform._43, bone.offsetTransform._44);

		const uint32_t childCount = static_cast<uint32_t>(bone.children.size());
		fprintf_s(file, "ChildCount: %d\n", childCount);

		for (uint32_t j = 0; j < childCount; ++j)
		{
			fprintf_s(file, "%d\n", bone.childIndices[j]);
		}
	}
}

void ModelIO::LoadSkeleton(std::filesystem::path filePath, Model& model)
{
	filePath.replace_extension("skeleton");

	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "r");
	if (file == nullptr)
		return;

	model.skeleton = std::make_unique<Skeleton>();

	uint32_t boneCount = 0;
	uint32_t rootIndex = 0;

	fscanf_s(file, "BoneCount: %d\n", &boneCount);

	model.skeleton->bones.reserve(boneCount);
	for (uint32_t i = 0; i < boneCount; ++i)
	{
		auto& bone = model.skeleton->bones.emplace_back(std::make_unique<Bone>());

		char buffer[256];
		fscanf_s(file, "Name: %s\n", buffer, static_cast<unsigned int>(std::size(buffer)));
		bone->name = buffer;
		fscanf_s(file, "Index: %d\n", &bone->index);
		fscanf_s(file, "ParentIndex: %d\n", &bone->parentIndex);
		fscanf_s(file, "ToParentTransform: %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
			&bone->toParentTransform._11, &bone->toParentTransform._12, &bone->toParentTransform._13, &bone->toParentTransform._14,
			&bone->toParentTransform._21, &bone->toParentTransform._22, &bone->toParentTransform._23, &bone->toParentTransform._24,
			&bone->toParentTransform._31, &bone->toParentTransform._32, &bone->toParentTransform._33, &bone->toParentTransform._34,
			&bone->toParentTransform._41, &bone->toParentTransform._42, &bone->toParentTransform._43, &bone->toParentTransform._44);
		fscanf_s(file, "OffsetTransform: %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
			&bone->offsetTransform._11, &bone->offsetTransform._12, &bone->offsetTransform._13, &bone->offsetTransform._14,
			&bone->offsetTransform._21, &bone->offsetTransform._22, &bone->offsetTransform._23, &bone->offsetTransform._24,
			&bone->offsetTransform._31, &bone->offsetTransform._32, &bone->offsetTransform._33, &bone->offsetTransform._34,
			&bone->offsetTransform._41, &bone->offsetTransform._42, &bone->offsetTransform._43, &bone->offsetTransform._44);

		uint32_t childCount = 0;
		fscanf_s(file, "ChildCount: %d\n", &childCount);
		bone->children.resize(childCount);
		bone->childIndices.resize(childCount);
		for (uint32_t j = 0; j < childCount; ++j)
		{
			fscanf_s(file, "%d\n", &bone->childIndices[j]);
		}
	}

	//Relink all bones
	for (auto& bone : model.skeleton->bones)
	{
		if (bone->parentIndex != -1)
			bone->parent = model.skeleton->bones[bone->parentIndex].get();
		else
			model.skeleton->root = bone.get();

		for (uint32_t i = 0; i < bone->childIndices.size(); ++i)
			bone->children[i] = model.skeleton->bones[bone->childIndices[i]].get();
	}
}

void Sage::Graphics::ModelIO::SaveAnimationSet(std::filesystem::path filePath, const Model& model)
{
	filePath.replace_extension("animation");

	if (model.animationSet.empty())
		return;

	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "w");
	if (file == nullptr)
		return;

	const uint32_t animationCount = static_cast<uint32_t>(model.animationSet.size());
	fprintf_s(file, "%d\n", animationCount);

	for (uint32_t animSetIndex = 0; animSetIndex < animationCount; ++animSetIndex)
	{
		const auto& animClip = model.animationSet[animSetIndex];
		fprintf_s(file, "%s\n", animClip.name.c_str());
		fprintf_s(file, "%f\n", animClip.tickDuration);
		fprintf_s(file, "%f\n", animClip.ticksPerSecond);

		const uint32_t boneAnimCount = static_cast<uint32_t>(animClip.boneAnimations.size());
		fprintf_s(file, "%d\n", boneAnimCount);
		for (uint32_t boneAnimIndex = 0; boneAnimIndex < boneAnimCount; ++boneAnimIndex)
		{
			const auto& boneAnim = animClip.boneAnimations[boneAnimIndex];
			if (boneAnim)
			{
				fprintf_s(file, "1\n");
				AnimationIO::Write(file, *boneAnim);
			}
			else
			{
				fprintf_s(file, "0\n");
			}
		}
	}
}

void Sage::Graphics::ModelIO::LoadAnimationSet(std::filesystem::path filePath, Model& model)
{
	filePath.replace_extension("animation");

	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "r");
	if (file == nullptr)
		return;

	uint32_t animationCount = 0;
	const uint32_t startCount = model.animationSet.size();
	fscanf_s(file, "%d\n", &animationCount);
	model.animationSet.resize(startCount + animationCount);
	for (uint32_t animSetIndex = startCount; animSetIndex < startCount + animationCount; ++animSetIndex)
	{
		auto& animClip = model.animationSet[animSetIndex];
		char buffer[1024];
		fscanf_s(file, "%s\n", &buffer, static_cast<unsigned int>(sizeof(buffer)));
		animClip.name = buffer;
		fscanf_s(file, "%f\n", &animClip.tickDuration);
		fscanf_s(file, "%f\n", &animClip.ticksPerSecond);

		uint32_t boneAnimCount = 0;
		fscanf_s(file, "%d\n", &boneAnimCount);
		for (uint32_t boneAnimIndex = 0; boneAnimIndex < boneAnimCount; ++boneAnimIndex)
		{
			int exists = 0;
			fscanf_s(file, "%d\n", &exists);
			if (exists == 1)
			{
				auto& boneAnim = animClip.boneAnimations.emplace_back(std::make_unique<Animation>());
				AnimationIO::Read(file, *boneAnim);
			}
			else
			{
				animClip.boneAnimations.emplace_back(nullptr);
			}
		}
	}
}
