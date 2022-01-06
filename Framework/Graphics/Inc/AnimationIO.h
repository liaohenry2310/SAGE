#pragma once

#include "Animation.h"

namespace Sage::Graphics
{
	class AnimationIO
	{
	public:
		static void Write(FILE* file, const Animation& animation);
		static void Read(FILE* file, Animation& animation);
	};
}