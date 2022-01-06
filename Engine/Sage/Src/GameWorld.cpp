#include "Precompiled.h"
#include "GameWorld.h"

#include "GameObjectFactory.h"

using namespace Sage;

void Sage::GameWorld::Initialize(int32_t capacity)
{
	ASSERT(!mInitialized, "GameWorld - Already initialized.");

	for (auto& service : mServices)
		service->Initialize();

	mGameObjectSlots.resize(capacity);
	mFreeSlots.resize(capacity);
	std::iota(mFreeSlots.rbegin(), mFreeSlots.rend(), 0);

	mInitialized = true;
}

void Sage::GameWorld::Terminate()
{
	ASSERT(!mUpdating, "GameWorld - Cannot terminate world during update.");

	if (!mInitialized)
		return;

	for (auto gameObject : mUpdateList)
		DestroyGameObject(gameObject->GetHandle());
	ProcessDestroyList();
	ASSERT(mUpdateList.empty(), "GameWorld - Failed to clean up objects.");

	for (auto& service : mServices)
		service->Terminate();

	mInitialized = false;
}

void Sage::GameWorld::Update(float deltaTime)
{
	ASSERT(!mUpdating, "GameWorld - Already updating the world");

	mUpdating = true;

	for (auto& service : mServices)
		service->Update(deltaTime);

	for (size_t i = 0; i < mUpdateList.size(); ++i)
	{
		GameObject* gameObject = mUpdateList[i];
		if (IsValid(gameObject->GetHandle()))
			gameObject->Update(deltaTime);
	}

	mUpdating = false;

	ProcessDestroyList();
}

void Sage::GameWorld::Render()
{
	for (auto& service : mServices)
		service->Render();
}

void Sage::GameWorld::DebugUI()
{
	for (auto& service : mServices)
		service->DebugUI();
}

GameObject* Sage::GameWorld::CreateGameObject(std::filesystem::path templateFile)
{
	ASSERT(mInitialized, "GameWorld - World must be intialized before creating game objects.");

	if (mFreeSlots.empty())
		return nullptr;

	const uint32_t freeSlot = mFreeSlots.back();
	mFreeSlots.pop_back();

	auto& slot = mGameObjectSlots[freeSlot];
	auto& newObject = slot.gameObject;
	newObject = std::make_unique<GameObject>();

	//Attach components
	GameObjectFactory::Make(templateFile, *newObject);

	//Initialize handle
	GameObjectHandle handle;
	handle.mIndex = freeSlot;
	handle.mGeneration = slot.generation;

	//Initialize GameObject
	newObject->mWorld = this;
	newObject->mHandle = handle;
	newObject->Initialize();

	//Add GameObject to update list
	mUpdateList.push_back(newObject.get());

	return newObject.get();
}

GameObject* Sage::GameWorld::FindGameObject(const std::string& name)
{
	auto iter = std::find_if(
		mUpdateList.begin(), 
		mUpdateList.end(), 
		[&](auto gameObject) { return gameObject->GetName() == name; });

	return iter == mUpdateList.end() ? nullptr : *iter;
}

GameObject* Sage::GameWorld::GetGameObject(GameObjectHandle handle)
{
	if (IsValid(handle))
		return mGameObjectSlots[handle.mIndex].gameObject.get();

	return nullptr;
}

void Sage::GameWorld::DestroyGameObject(GameObjectHandle handle)
{
	if (!IsValid(handle))
		return;

	auto& slot = mGameObjectSlots[handle.mIndex];
	slot.generation++;
	mToBeDestroyed.push_back(handle.mIndex);
}

bool Sage::GameWorld::IsValid(GameObjectHandle handle) const
{
	return
		handle.mIndex >= 0 &&
		handle.mIndex < mGameObjectSlots.size() &&
		mGameObjectSlots[handle.mIndex].generation == handle.mGeneration;
}

void Sage::GameWorld::ProcessDestroyList()
{
	ASSERT(!mUpdating, "GameWorld = Cannot destroy game objects during update.");

	for (auto index : mToBeDestroyed)
	{
		auto& slot = mGameObjectSlots[index];

		GameObject* gameObject = slot.gameObject.get();
		ASSERT(!IsValid(gameObject->GetHandle()), "GameWorld - Game object is still valid!");

		//Remove from mUpdateList
		mUpdateList.erase(std::remove(mUpdateList.begin(), mUpdateList.end(), gameObject));

		//Terminate GameObject
		gameObject->Terminate();

		//Delete the game object and recycle the slot
		slot.gameObject.reset();
		mFreeSlots.push_back(index);
	}
}
