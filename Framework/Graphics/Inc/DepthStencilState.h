#pragma once

namespace Sage::Graphics
{
	class DepthStencilState
	{
	public:
		enum class CullMode { Front, Back, None };
		enum class FillMode { Solid, Wireframe };

		DepthStencilState() = default;
		~DepthStencilState();

		DepthStencilState(const DepthStencilState&) = delete;
		DepthStencilState& operator=(const DepthStencilState&) = delete;

		void Initialize(bool depthEnable);
		void Terminate();

		void Set();
		void Clear();

	private:
		ID3D11DepthStencilState * mDepthStencilState = nullptr;
	};
}


