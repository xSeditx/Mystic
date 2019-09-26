#pragma once 
#include<atomic>
#include"Common.h"

#define WORST_ALIGNMENT 512

// https://www.youtube.com/watch?v=mdPeXJ0eiGc&t=1547s
// https://fgiesen.wordpress.com/2010/12/14/ring-buffers-and-queues/


template<typename T, size_t sz>
class RingBuffer
{
public:
	RingBuffer() = default;

	int& operator[] (const int index)
	{
		return Data[index & RING_SIZE - 1];
	}
	
	uint64_t alloc(uint64_t n)
	{// Lockfree allocation
		return Offset.fetch_add(n)
	}
	uint64_t Aligned_alloc(uint64_t sz)
	{// Aligned alloc to avoid bad cache layout
		uint64_t padded_sz = (sz + WORST_ALIGNMENT - 1) & ~(WORST_ALIGNMENT - 1));
		uint64_t Alloced = Offset.fetch_add(padded_sz);
		assert(Alloced + sz <= RING_SIZE);
		return Alloced;
	}

private:
	T Data[sz + 1];
	const size_t size() { return ElementCount; }
	std::atomic <uint64_t> Offset;
	constexpr size_t ElementCount = sz + 1;
};


#include<mutex>
template<typename T, typename M = std::mutex> 
class guarded
{
public:
	using handle = std::unique_ptr<T, deleter>;
	template<typename ... Us>
	guarded(Us &&... data);
	handle lock();
	handle try_lock();

	template<class Duration>
	handle try_lock_for(const Duration);

	template<class TimePoint>
	handle try_lock_until(const TimePoint);

	class deleter
	{
	public:
		using pointer = T * ;
		deleter(std::unique_lock<M> lock) :mLock(std::move(lock))
		{}

		void operator()(T* ptr)
		{
			if (mlock.owns_lock())
			{
				mLock.unlock();
			}
		}

	private:
		std::unique_lock<M> mLock;
	};
private:
	T object;
	M Mutex;
};
template<typename T, typename M>
template<typename... US> guarded<T, M>::guarded(US &&...data) :object(std::forward <US>(data)...)
{}
template<typename T, typename M> auto guarded<T, M>::lock()->handle
{
	std::unique_lock<M> lock(Mutex);
	return handle(&object, deleter(std::move(lock)));
}
template<typename T, typename M> auto guarded<T, M>::try_lock()->handle
{
	std::unique_lock<M> lock(Mutex, std::try_to_lock);

	if (lock.owns_lock())
	{
		return handle(&object, deleter(std::move(lock)));
	}
	else
	{
		return handle(nullptr, deleter(std::move(lock)));
	}
}




// Microsoft finding memory leaks using CRT library
// https://docs.microsoft.com/en-us/visualstudio/debugger/finding-memory-leaks-using-the-crt-library?view=vs-2015
// SOverflow thread on the topic
// https://stackoverflow.com/questions/910172/track-c-memory-allocations
#include<Windows.h>
int HeapInfo()
{
	//------------- HEAP STUFF--------------------------------
	//------------- HEAP STUFF--------------------------------
	DWORD HeapsIndex;
	DWORD HeapsLength;
	HANDLE hDefaultProcessHeap;
	HRESULT Result;
	PHANDLE aHeaps;
	SIZE_T BytesToAllocate;
	//--------------------------------------------------------
	hDefaultProcessHeap = GetProcessHeap();
	//--------------------------------------------------------
	size_t NumberOfHeaps = GetProcessHeaps(0, NULL);
	if (NumberOfHeaps == 0) {
		Print("Failed to retrieve the number of heaps with LastError %d.\n" << GetLastError());
		return 1;
	}
	//
// Calculate the buffer size.
//
	//Result = SIZETMult(NumberOfHeaps, sizeof(*aHeaps), &BytesToAllocate);
	if (Result != S_OK) {
		Print("SIZETMult failed with HR " << Result);
		return 1;
	}
	//--------------------------------------------------------

	//--------------------------------------------------------
	//
	// Get a handle to the default process heap.
	//
	hDefaultProcessHeap = GetProcessHeap();
	if (hDefaultProcessHeap == NULL) {
		Print("Failed to retrieve the default process heap with LastError" << GetLastError());
		return 1;
	}

	//
	// Allocate the buffer from the default process heap.
	//
	aHeaps = (PHANDLE)HeapAlloc(hDefaultProcessHeap, 0, BytesToAllocate);
	if (aHeaps == NULL) {
		Print("HeapAlloc failed to allocate" << BytesToAllocate << " bytes.");
		return 1;
	}
	// 
// Save the original number of heaps because we are going to compare it
// to the return value of the next GetProcessHeaps call.
//
	HeapsLength = NumberOfHeaps;
	//
	// Retrieve handles to the process heaps and print them to stdout. 
	// Note that heap functions should be called only on the default heap of the process
	// or on private heaps that your component creates by calling HeapCreate.
	//
	NumberOfHeaps = GetProcessHeaps(HeapsLength, aHeaps);
	if (NumberOfHeaps == 0) {
		Print("Failed to retrieve heaps with LastError " << GetLastError());
		return 1;
	}
	else if (NumberOfHeaps > HeapsLength) {

		//
		// Compare the latest number of heaps with the original number of heaps.
		// If the latest number is larger than the original number, another
		// component has created a new heap and the buffer is too small.
		//
		Print("Another component created a heap between calls. ");
		Print("Please try again");
		return 1;
	}

	Print("Process has " << HeapsLength << "heaps.");
	for (HeapsIndex = 0; HeapsIndex < HeapsLength; ++HeapsIndex)
	{
		Print("Heap" << HeapsIndex << " at address:" << aHeaps[HeapsIndex]);
	}
}

