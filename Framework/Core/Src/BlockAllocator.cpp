#include "Precompiled.h"
#include "BlockAllocator.h"

#include "DebugUtil.h"

using namespace Sage::Core;

BlockAllocator::BlockAllocator(size_t blockSize, size_t capacity) : mBlockSize(blockSize), mCapacity(capacity)
{
	ASSERT(blockSize > 0, "BlockAllocator::BlockAllocator() -- Invalid block size!");
	mData = malloc(blockSize * capacity);
	for (size_t i = 0; i < capacity; ++i)
		mFreeBlocks.push_back(static_cast<uint8_t*>(mData) + (i * mBlockSize));
}

BlockAllocator::~BlockAllocator()
{
	ASSERT(mFreeBlocks.size() == mCapacity, "BlockAllocator::~BlockAllocator() -- Not all blocks are freed!");
	free(mData);
}

void* BlockAllocator::Allocate()
{
	ASSERT(!mFreeBlocks.empty(), "BlockAllocator::Allocate() -- Out of allocated memory!");

	if (mFreeBlocks.empty())
		return nullptr;

	void* ptr = mFreeBlocks.back();
	mFreeBlocks.pop_back();
	return ptr;
}

void BlockAllocator::Free(void* ptr)
{
	const ptrdiff_t diff = static_cast<uint8_t*>(ptr) - static_cast<uint8_t*>(mData);
	const bool isValidPtr = (diff >= 0) && (diff < mBlockSize * mCapacity) && (diff % mBlockSize == 0);
	ASSERT(isValidPtr, "BlockAllocator::Free() -- Attempting to free invalid memory!");

	if (!isValidPtr)
		return;

	mFreeBlocks.push_back(ptr);
}