#pragma once
#include <Sage/Inc/Sage.h>

class GameState : public Sage::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltatime) override;
	void Render() override;

private:
	struct Vertex
	{
		Sage::Math::Vector3 position;
		Sage::Graphics::Color color;
	};

	std::array< std::vector<Vertex>, 3> mVerticesArray;
	std::array<ID3D11Buffer*, 3> mVertexBuffers{nullptr};
	int mCurrentBuffer = 0;

	ID3D11VertexShader* mVertexShader = nullptr;
	ID3D11InputLayout* mInputLayout = nullptr;

	ID3D11PixelShader* mPixelShader = nullptr;
};