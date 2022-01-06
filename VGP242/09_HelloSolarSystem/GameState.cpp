#include "GameState.h"

using namespace Sage;
using namespace Sage::Graphics;
using namespace Sage::Input;
using namespace Sage::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::DarkBlue);

	mSkybox.Initialize("../../Assets/Images/space.jpg");

	mTransformBuffer.Initialize();

	mSphereMesh = MeshBuilder::CreateSpherePX(24, 24);
	
	//------Construct ring mesh for saturn------
	int sections = 24;
	float angleStep = Math::Constants::TwoPi / sections;
	float uStep = 1.0f / sections;
	for (int i = 0; i <= sections; ++i)
	{
		float xx1 = cos(i * angleStep) * 1.0f;
		float xx2 = cos(i * angleStep) * 1.5f;
		float zz1 = sin(i * angleStep) * 1.0f;
		float zz2 = sin(i * angleStep) * 1.5f;
		float v = i * uStep;

		mRingMesh.vertices.push_back({ Vector3{xx1, 0.0f, zz1}, Vector2{0.0f, v} });
		mRingMesh.vertices.push_back({ Vector3{xx2, 0.0f, zz2}, Vector2{1.0f, v} });
	}
	for (int i = 0; i + 2 <= sections * 2; i += 2)
	{
		mRingMesh.indices.push_back(i + 0);
		mRingMesh.indices.push_back(i + 2);
		mRingMesh.indices.push_back(i + 1);

		mRingMesh.indices.push_back(i + 1);
		mRingMesh.indices.push_back(i + 2);
		mRingMesh.indices.push_back(i + 3);
	}
	//--------------------------------------------

	ModelLoader::LoadObj("../../Assets/Models/sci_fi_fighter/sci_fi_fighter.obj", 10.0f, mSpaceshipMesh);
	
	mSpaceshipMeshBuffer.Initialize(mSpaceshipMesh);
	mSphereMeshBuffer.Initialize(mSphereMesh);
	mRingMeshBuffer.Initialize(mRingMesh);

	mVertexShader.Initialize("../../Assets/Shaders/DoTexturing.fx", VertexPX::Format);
	mPixelShader.Initialize("../../Assets/Shaders/DoTexturing.fx");

	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

	mSpaceshipTexture.Initialize("../../Assets/Models/sci_fi_fighter/sci_fi_fighter_diffuse.jpg");
	mSunTexture.Initialize("../../Assets/Images/sun.jpg");
	mMercuryTexture.Initialize("../../Assets/Images/mercury.jpg");
	mVenusTexture.Initialize("../../Assets/Images/venus.jpg");
	mEarthTexture.Initialize("../../Assets/Images/earth.jpg");
	mMoonTexture.Initialize("../../Assets/Images/moon.jpg");
	mMarsTexture.Initialize("../../Assets/Images/mars.jpg");
	mJupiterTexture.Initialize("../../Assets/Images/jupiter.jpg");
	mSaturnTexture.Initialize("../../Assets/Images/saturn.jpg");
	mSaturnRingTexture.Initialize("../../Assets/Images/saturn_ring.png");
	mUranusTexture.Initialize("../../Assets/Images/uranus.jpg");
	mNeptuneTexture.Initialize("../../Assets/Images/neptune.jpg");

	mRasterizerStateWireframe.Initialize(RasterizerState::CullMode::None, RasterizerState::FillMode::Wireframe);
	mRasterizerStateNoCull.Initialize(RasterizerState::CullMode::None, RasterizerState::FillMode::Solid);
	mDepthStencilStateDisableDepth.Initialize(false);

	mShipPosition = { 0.0f, 0.0f, 0.0f };
	mShipDirection = { 0.0f, 0.0f, 1.0f };

	mCamera.SetPosition(mShipPosition - (mShipDirection * mCameraDistanceOffset) + (Vector3::YAxis * mCameraHeightOffset));
	mCamera.SetDirection({ 0.0f, -1.0f, 2.0f });

	mParticleEmitter.Initialize();
	mParticleEmitter.SetStartColor(Colors::PaleVioletRed);
	mParticleEmitter.SetEndColor(Colors::Purple);
	mParticleEmitter.SetStartSize(1.0f);
	mParticleEmitter.SetEndSize(0.0f);
	mParticleEmitter.SetMaxLifeTime(1.0f);
	mParticleEmitter.SetEmitPosOffset(0.3f);
	mParticleEmitter.Start(50.0f);
}

void GameState::Terminate()
{
	mParticleEmitter.Terminate();

	mDepthStencilStateDisableDepth.Terminate();
	mRasterizerStateWireframe.Terminate();
	mRasterizerStateNoCull.Terminate();

	mNeptuneTexture.Terminate();
	mUranusTexture.Terminate();
	mSaturnRingTexture.Terminate();
	mSaturnTexture.Terminate();
	mJupiterTexture.Terminate();
	mMarsTexture.Terminate();
	mMoonTexture.Terminate();
	mEarthTexture.Terminate();
	mVenusTexture.Terminate();
	mMercuryTexture.Terminate();
	mSunTexture.Terminate();
	mSpaceshipTexture.Terminate();
	mSampler.Terminate();

	mPixelShader.Terminate();
	mVertexShader.Terminate();

	mRingMeshBuffer.Terminate();
	mSphereMeshBuffer.Terminate();
	mSpaceshipMeshBuffer.Terminate();

	mTransformBuffer.Terminate();
	mSkybox.Terminate();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	//const float moveSpeed = 10.0f;
	//const float turnSpeed = 10.0f * Constants::DegToRad;
	//if (inputSystem->IsKeyDown(KeyCode::W))
	//	mCamera.Walk(moveSpeed * deltaTime);
	//if (inputSystem->IsKeyDown(KeyCode::S))
	//	mCamera.Walk(-moveSpeed * deltaTime);
	//if (inputSystem->IsKeyDown(KeyCode::D))
	//	mCamera.Strafe(moveSpeed * deltaTime);
	//if (inputSystem->IsKeyDown(KeyCode::A))
	//	mCamera.Strafe(-moveSpeed * deltaTime);
	//if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	//{
	//	mCamera.Yaw(inputSystem->GetMouseMoveX() * turnSpeed * deltaTime);
	//	mCamera.Pitch(inputSystem->GetMouseMoveY() * turnSpeed * deltaTime);
	//}

	mShipTilt *= 0.95f;

	const float shipTurnSpeed = 0.5f;
	mShipRotation += inputSystem->GetMouseMoveX() * shipTurnSpeed * deltaTime;
	mShipElevation += inputSystem->GetMouseMoveY() * shipTurnSpeed * deltaTime;

	auto yawRotation = Math::Matrix4::RotationY(mShipRotation);
	mShipDirection = Math::TransformNormal(Vector3::ZAxis, yawRotation);

	const Math::Vector3 right = Math::Normalize(Cross(Math::Vector3::YAxis, mShipDirection));
	const Math::Matrix4 pitchRotation = Math::Matrix4::RotationAxis(right, mShipElevation);
	mShipDirection = Math::TransformNormal(mShipDirection, pitchRotation);

	const float shipMoveSpeed = 1.0f;
	if (inputSystem->IsKeyDown(KeyCode::W))
		mShipPosition += mShipDirection * shipMoveSpeed;
	if (inputSystem->IsKeyDown(KeyCode::S))
		mShipPosition -= mShipDirection * shipMoveSpeed;
	if (inputSystem->IsKeyDown(KeyCode::D))
	{
		mShipTilt = Clamp(mShipTilt - (1.0f * deltaTime), -0.5f, 0.5f);
		mShipPosition += right * shipMoveSpeed;
	}
	if (inputSystem->IsKeyDown(KeyCode::A))
	{
		mShipTilt = Clamp(mShipTilt + (1.0f * deltaTime), -0.5f, 0.5f);
		mShipPosition -= right * shipMoveSpeed;
	}

	mCameraTargetPosition = mShipPosition - (mShipDirection * mCameraDistanceOffset) + (Vector3::YAxis * mCameraHeightOffset);
	auto cameraPosition = Lerp(mCamera.GetPosition(), mCameraTargetPosition, 0.5f);
	mCamera.SetPosition(cameraPosition);
	mCamera.SetDirection(mShipDirection);

	mSunSpin += -0.1f * deltaTime;
	mMercurySpin += 2.0f * deltaTime;
	mMercuryOrbit += 2.0f * deltaTime;
	mVenusSpin += 3.5f * deltaTime;
	mVenusOrbit += 1.5f * deltaTime;
	mEarthSpin += 5.0f * deltaTime;
	mEarthOrbit += 1.0f * deltaTime;
	mMoonSpin += 1.5f * deltaTime;
	mMoonOrbit += 1.5f * deltaTime;
	mMarsSpin += 4.0f * deltaTime;
	mMarsOrbit += 0.8f * deltaTime;
	mJupiterSpin += 2.0f * deltaTime;
	mJupiterOrbit += 0.6f * deltaTime;
	mSaturnSpin += 1.8f * deltaTime;
	mSaturnOrbit += 0.5f * deltaTime;
	mUranusSpin += 4.0f * deltaTime;
	mUranusOrbit += 0.3f * deltaTime;
	mNeptuneSpin += 1.5f * deltaTime;
	mNeptuneOrbit += 0.2f * deltaTime;

	mParticleEmitter.SetPosition(mShipPosition - (mShipDirection * 6.0f) + Vector3::YAxis * 1.0f);
	mParticleEmitter.Update(deltaTime);
}

void GameState::Render()
{
	mSkybox.Render(mCamera);

	mUseWireframe ? mRasterizerStateWireframe.Set() : mRasterizerStateWireframe.Clear();

	auto matTilt = Matrix4::RotationZ(mShipTilt);

	const Math::Vector3 l = mShipDirection;
	const Math::Vector3 r = Math::Normalize(Math::Cross(Math::Vector3::YAxis, l));
	const Math::Vector3 u = Math::Normalize(Math::Cross(l, r));
	const Math::Vector3 p = mShipPosition;


	auto matWorld = Matrix4{
		r.x, r.y, r.z, 0.0f,
		u.x, u.y, u.z, 0.0f,
		l.x, l.y, l.z, 0.0f,
		p.x, p.y, p.z, 1.0f
	};
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetProjectionMatrix();

	auto context = GraphicsSystem::Get()->GetContext();

	// Render Spaceship
	TransformData data;
	data.wvp = Transpose(matTilt * matWorld * matView * matProj);
	mTransformBuffer.Update(data);
	mTransformBuffer.BindVS(0);
	mVertexShader.Bind();
	mPixelShader.Bind();
	mSampler.BindPS(0);
	mSpaceshipTexture.BindPS(0);
	mSpaceshipMeshBuffer.Render();

	//Render sun
	matWorld = Matrix4::Scaling(Vector3{ 30.0f }) * 
		Matrix4::RotationY(mSunSpin);
	data.wvp = Transpose(matWorld * matView * matProj);
	mTransformBuffer.Update(data);
	mTransformBuffer.BindVS(0);
	mSunTexture.BindPS(0);
	mSphereMeshBuffer.Render();

	//Render Mercury
	matWorld = Matrix4::Scaling(Vector3{ 7.0f }) *
		Matrix4::RotationY(mMercurySpin) *
		Matrix4::Translation(Vector3{ 50.0f, 0.0f, 0.0f }) *
		Matrix4::RotationY(mMercuryOrbit);
	data.wvp = Transpose(matWorld * matView * matProj);
	mTransformBuffer.Update(data);
	mTransformBuffer.BindVS(0);
	mMercuryTexture.BindPS(0);
	mSphereMeshBuffer.Render();
	SimpleDraw::AddCircle(Vector3{ 0.0f }, 50.0f, Colors::DarkGray);

	//Render Venus
	matWorld = Matrix4::Scaling(Vector3{ 7.0f }) *
		Matrix4::RotationY(mVenusSpin) *
		Matrix4::Translation(Vector3{ 75.0f, 0.0f, 0.0f }) *
		Matrix4::RotationY(mVenusOrbit);
	data.wvp = Transpose(matWorld * matView * matProj);
	mTransformBuffer.Update(data);
	mTransformBuffer.BindVS(0);
	mVenusTexture.BindPS(0);
	mSphereMeshBuffer.Render();
	SimpleDraw::AddCircle(Vector3{ 0.0f }, 75.0f, Colors::DarkGray);

	//Render Earth
	matWorld = Matrix4::Scaling(Vector3{ 10.0f }) * 
		Matrix4::RotationY(mEarthSpin) * 
		Matrix4::Translation(Vector3{100.0f, 0.0f, 0.0f}) * 
		Matrix4::RotationY(mEarthOrbit);
	data.wvp = Transpose(matWorld * matView * matProj);
	mTransformBuffer.Update(data);
	mTransformBuffer.BindVS(0);
	mEarthTexture.BindPS(0);
	mSphereMeshBuffer.Render();
	SimpleDraw::AddCircle(Vector3{ 0.0f }, 100.0f, Colors::DarkGray);

	//Render Moon
	matWorld = Matrix4::Scaling(Vector3{ 5.0f }) * 
		Matrix4::RotationY(mMoonSpin) * 
		Matrix4::Translation(Vector3{ 15.0f, 0.0f, 0.0f }) * 
		Matrix4::RotationY(mMoonOrbit) *
		Matrix4::Translation(Vector3{ 100.0f, 0.0f, 0.0f }) *
		Matrix4::RotationY(mEarthOrbit);
	data.wvp = Transpose(matWorld * matView * matProj);
	mTransformBuffer.Update(data);
	mTransformBuffer.BindVS(0);
	mMoonTexture.BindPS(0);
	mSphereMeshBuffer.Render();

	//Render Mars
	matWorld = Matrix4::Scaling(Vector3{ 12.0f }) *
		Matrix4::RotationY(mMarsSpin) *
		Matrix4::Translation(Vector3{ 125.0f, 0.0f, 0.0f }) *
		Matrix4::RotationY(mMarsOrbit);
	data.wvp = Transpose(matWorld * matView * matProj);
	mTransformBuffer.Update(data);
	mTransformBuffer.BindVS(0);
	mMarsTexture.BindPS(0);
	mSphereMeshBuffer.Render();
	SimpleDraw::AddCircle(Vector3{ 0.0f }, 125.0f, Colors::DarkGray);

	//Render Jupiter
	matWorld = Matrix4::Scaling(Vector3{ 20.0f }) *
		Matrix4::RotationY(mJupiterSpin) *
		Matrix4::Translation(Vector3{ 175.0f, 0.0f, 0.0f }) *
		Matrix4::RotationY(mJupiterOrbit);
	data.wvp = Transpose(matWorld * matView * matProj);
	mTransformBuffer.Update(data);
	mTransformBuffer.BindVS(0);
	mJupiterTexture.BindPS(0);
	mSphereMeshBuffer.Render();
	SimpleDraw::AddCircle(Vector3{ 0.0f }, 175.0f, Colors::DarkGray);

	//Render Saturn
	matWorld = Matrix4::Scaling(Vector3{ 17.0f }) *
		Matrix4::RotationY(mSaturnSpin) *
		Matrix4::Translation(Vector3{ 225.0f, 0.0f, 0.0f }) *
		Matrix4::RotationY(mSaturnOrbit);
	data.wvp = Transpose(matWorld * matView * matProj);
	mTransformBuffer.Update(data);
	mTransformBuffer.BindVS(0);
	mSaturnTexture.BindPS(0);
	mSphereMeshBuffer.Render();
	SimpleDraw::AddCircle(Vector3{ 0.0f }, 225.0f, Colors::DarkGray);

	matWorld = Matrix4::Scaling(Vector3{ 16.0f }) *
		Matrix4::RotationY(mSaturnSpin) *
		Matrix4::Translation(Vector3{ 225.0f, 0.0f, 0.0f }) *
		Matrix4::RotationY(mSaturnOrbit);
	data.wvp = Transpose(matWorld * matView * matProj);
	mTransformBuffer.Update(data);
	mTransformBuffer.BindVS(0);
	mSaturnRingTexture.BindPS(0);
	mRasterizerStateNoCull.Set();
	mRingMeshBuffer.Render();
	mRasterizerStateNoCull.Clear();

	//Render Uranus
	matWorld = Matrix4::Scaling(Vector3{ 15.0f }) *
		Matrix4::RotationY(mUranusSpin) *
		Matrix4::Translation(Vector3{ 275.0f, 0.0f, 0.0f }) *
		Matrix4::RotationY(mUranusOrbit);
	data.wvp = Transpose(matWorld * matView * matProj);
	mTransformBuffer.Update(data);
	mTransformBuffer.BindVS(0);
	mUranusTexture.BindPS(0);
	mSphereMeshBuffer.Render();
	SimpleDraw::AddCircle(Vector3{ 0.0f }, 275.0f, Colors::DarkGray);

	//Render Neptune
	matWorld = Matrix4::Scaling(Vector3{ 14.0f }) *
		Matrix4::RotationY(mNeptuneSpin) *
		Matrix4::Translation(Vector3{ 325.0f, 0.0f, 0.0f }) *
		Matrix4::RotationY(mNeptuneOrbit);
	data.wvp = Transpose(matWorld * matView * matProj);
	mTransformBuffer.Update(data);
	mTransformBuffer.BindVS(0);
	mNeptuneTexture.BindPS(0);
	mSphereMeshBuffer.Render();
	SimpleDraw::AddCircle(Vector3{ 0.0f }, 325.0f, Colors::DarkGray);

	mParticleEmitter.Render(mCamera);

	//SimpleDraw::AddTransform(Matrix4::Identity);
	//SimpleDraw::AddGroundPlane(200.0f);
	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	// UI theme
	const char* themeItems[] = { "Classic", "Dark", "Light" };
	static int themeItemIndex = 0;
	if (ImGui::Combo("UI Theme", &themeItemIndex, themeItems, IM_ARRAYSIZE(themeItems)))
	{
		if (themeItemIndex == 0)
			DebugUI::SetTheme(DebugUI::Theme::Classic);
		else if (themeItemIndex == 1)
			DebugUI::SetTheme(DebugUI::Theme::Dark);
		else
			DebugUI::SetTheme(DebugUI::Theme::Light);
	}

	// Render mode
	const char* renderModeItems[] = { "Solid", "Wireframe" };
	static int renderModeIndex = 0;
	if (ImGui::Combo("Render Mode", &renderModeIndex, renderModeItems, IM_ARRAYSIZE(renderModeItems)))
		mUseWireframe = (renderModeIndex != 0);

	ImGui::End();
}