#include "Precompiled.h"
#include "Animation.h"

using namespace Sage;
using namespace Sage::Graphics;


Math::Vector3 Animation::GetPosition(float time) const
{
	int start = 0;
	int end = static_cast<int>(mPositionKeys.size());
	int index = static_cast<int>(mPositionKeys.size() / 2);

	if (mPositionKeys.empty())
	{
		return Math::Vector3::Zero;
	}
	else if (time <= mPositionKeys[0].time)
	{
		return mPositionKeys[0].key;
	}
	else if (time >= mPositionKeys[mPositionKeys.size() - 1].time)
	{
		return mPositionKeys[mPositionKeys.size() - 1].key;
	}
	else
	{
		while (!(mPositionKeys[index].time <= time && time <= mPositionKeys[index + 1].time))
		{
			int temp = index;
			if (time > mPositionKeys[index].time)
			{
				index = (index + end) / 2;
				start = temp;
			}
			else
			{
				index = (index + start) / 2;
				end = temp;
			}
		}
	}

	auto keyA = mPositionKeys[index];
	auto keyB = mPositionKeys[index + 1];

	float t = (time - keyA.time) / (keyB.time - keyA.time);

	return Math::Lerp(keyA.key, keyB.key, t);
}

Math::Quaternion Animation::GetRotation(float time) const
{
	int start = 0;
	int end = static_cast<int>(mRotationKeys.size());
	int index = static_cast<int>(mRotationKeys.size() / 2);

	if (mRotationKeys.empty())
	{
		return Math::Quaternion::Identity;
	}
	else if (time <= mRotationKeys[0].time)
	{
		return mRotationKeys[0].key;
	}
	else if (time >= mRotationKeys[mRotationKeys.size() - 1].time)
	{
		return mRotationKeys[mRotationKeys.size() - 1].key;
	}
	else
	{
		while (!(mRotationKeys[index].time <= time && time <= mRotationKeys[index + 1].time))
		{
			int temp = index;
			if (time > mRotationKeys[index].time)
			{
				index = (index + end) / 2;
				start = temp;
			}
			else
			{
				index = (index + start) / 2;
				end = temp;
			}
		}
	}

	auto keyA = mRotationKeys[index];
	auto keyB = mRotationKeys[index + 1];

	float t = (time - keyA.time) / (keyB.time - keyA.time);

	return Math::Slerp(keyA.key, keyB.key, t);
}

Math::Vector3 Animation::GetScale(float time) const
{
	int start = 0;
	int end = static_cast<int>(mScaleKeys.size());
	int index = static_cast<int>(mScaleKeys.size() / 2);

	if (mScaleKeys.empty())
	{
		return Math::Vector3::One;
	}
	else if (time <= mScaleKeys[0].time)
	{
		return mScaleKeys[0].key;
	}
	else if (time >= mScaleKeys[mScaleKeys.size() - 1].time)
	{
		return mScaleKeys[mScaleKeys.size() - 1].key;
	}
	else
	{
		while (!(mScaleKeys[index].time <= time && time <= mScaleKeys[index + 1].time))
		{
			int temp = index;
			if (time > mScaleKeys[index].time)
			{
				index = (index + end) / 2;
				start = temp;
			}
			else
			{
				index = (index + start) / 2;
				end = temp;
			}
		}
	}

	auto keyA = mScaleKeys[index];
	auto keyB = mScaleKeys[index + 1];

	float t = (time - keyA.time) / (keyB.time - keyA.time);

	return Math::Lerp(keyA.key, keyB.key, t);
}

Math::Matrix4 Animation::GetTransform(float time) const
{
	const auto& position = GetPosition(time);
	const auto& rotation = GetRotation(time);
	const auto& scale = GetScale(time);

	return Transform{ position, rotation, scale }.GetMatrix4();
}

float Animation::GetDuration() const
{
	float posTime = mPositionKeys.back().time;
	float rotTime = mRotationKeys.back().time;
	float scaleTime = mScaleKeys.back().time;

	return Math::Max(posTime, Math::Max(rotTime, scaleTime));
}
