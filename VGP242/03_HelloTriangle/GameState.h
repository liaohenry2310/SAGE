#pragma once
#include <Sage/Inc/Sage.h>

class GameState : public Sage::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Render() override;

private:
	Sage::Graphics::MeshPC mMesh;
	Sage::Graphics::MeshBuffer mMeshBuffer;
	Sage::Graphics::VertexShader mVertexShader;
	Sage::Graphics::PixelShader mPixelShader;
};