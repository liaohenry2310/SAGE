#include "Precompiled.h"
#include "CameraService.h"

#include "CameraComponent.h"

using namespace Sage;
using namespace Sage::Graphics;

const Camera& CameraService::GetCamera() const
{
	return mCurrentCamera->GetCamera();
}

void CameraService::Register(const CameraComponent* cameraComponent)
{
	mCameraEntries.push_back(cameraComponent);
	if (mCurrentCamera == nullptr)
		mCurrentCamera = cameraComponent;
}

void CameraService::Unregister(const CameraComponent* cameraComponent)
{
	auto iter = std::find(mCameraEntries.begin(), mCameraEntries.end(), cameraComponent);
	if (iter != mCameraEntries.end())
	{
		if (mCurrentCamera == *iter)
		{
			if (mCameraEntries.empty())
				mCurrentCamera = nullptr;
			else
				mCurrentCamera = mCameraEntries.front();
		}
		mCameraEntries.erase(iter);
	}
}