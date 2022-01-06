#include "Precompiled.h"
#include "AnimationIO.h"

void Sage::Graphics::AnimationIO::Write(FILE* file, const Animation& animation)
{
	const uint32_t postionKeysCount = static_cast<uint32_t>(animation.mPositionKeys.size());
	fprintf_s(file, "%d\n", postionKeysCount);
	for (uint32_t i = 0; i < postionKeysCount; ++i)
	{
		const auto& positionKey = animation.mPositionKeys[i];
		fprintf_s(file, "%f\n", positionKey.time);
		fprintf_s(file, "%f %f %f\n", positionKey.key.x, positionKey.key.y, positionKey.key.z);
	}

	const uint32_t scaleKeysCount = static_cast<uint32_t>(animation.mScaleKeys.size());
	fprintf_s(file, "%d\n", scaleKeysCount);
	for (uint32_t i = 0; i < scaleKeysCount; ++i)
	{
		const auto& scaleKey = animation.mScaleKeys[i];
		fprintf_s(file, "%f\n", scaleKey.time);
		fprintf_s(file, "%f %f %f\n", scaleKey.key.x, scaleKey.key.y, scaleKey.key.z);
	}

	const uint32_t rotationKeysCount = static_cast<uint32_t>(animation.mRotationKeys.size());
	fprintf_s(file, "%d\n", rotationKeysCount);
	for (uint32_t i = 0; i < rotationKeysCount; ++i)
	{
		const auto& rotationKey = animation.mRotationKeys[i];
		fprintf_s(file, "%f\n", rotationKey.time);
		fprintf_s(file, "%f %f %f %f \n", rotationKey.key.w, rotationKey.key.x, rotationKey.key.y, rotationKey.key.z);
	}
}

void Sage::Graphics::AnimationIO::Read(FILE* file, Animation& animation)
{
	uint32_t postionKeysCount = 0;
	fscanf_s(file, "%d\n", &postionKeysCount);
	animation.mPositionKeys.resize(postionKeysCount);
	for (uint32_t i = 0; i < postionKeysCount; ++i)
	{
		auto& positionKey = animation.mPositionKeys[i];
		fscanf_s(file, "%f\n", &positionKey.time);
		fscanf_s(file, "%f %f %f\n", &positionKey.key.x, &positionKey.key.y, &positionKey.key.z);
	}

	uint32_t scaleKeysCount = 0;
	fscanf_s(file, "%d\n", &scaleKeysCount);
	animation.mScaleKeys.resize(scaleKeysCount);
	for (uint32_t i = 0; i < scaleKeysCount; ++i)
	{
		auto& scaleKey = animation.mScaleKeys[i];
		fscanf_s(file, "%f\n", &scaleKey.time);
		fscanf_s(file, "%f %f %f\n", &scaleKey.key.x, &scaleKey.key.y, &scaleKey.key.z);
	}

	uint32_t rotationKeysCount = 0;
	fscanf_s(file, "%d\n", &rotationKeysCount);
	animation.mRotationKeys.resize(rotationKeysCount);
	for (uint32_t i = 0; i < rotationKeysCount; ++i)
	{
		auto& rotationKey = animation.mRotationKeys[i];
		fscanf_s(file, "%f\n", &rotationKey.time);
		fscanf_s(file, "%f %f %f %f \n", &rotationKey.key.w, &rotationKey.key.x, &rotationKey.key.y, &rotationKey.key.z);
	}
}
