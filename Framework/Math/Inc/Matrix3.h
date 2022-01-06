#pragma once

namespace Sage::Math
{
	struct Matrix3
	{
		union
		{
			struct
			{
				float m11, m12, m13, m21, m22, m23, m31, m32, m33;
			};
			std::array<float, 9> entries;
		};
	};
}