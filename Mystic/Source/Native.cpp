#include"Native.h"


#ifndef WIN32
#else
using namespace NativeMemory;
MemState NativeMemory::GlobalMemState;

int NativeMemory::GetHeapInfo()
{
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

    Result = SIZETMult(NumberOfHeaps, sizeof(*aHeaps), &BytesToAllocate);
    if (Result != S_OK) {
        Print("SIZETMult failed with HR " << Result);
        return 1;
	}

    //--------------------------------------------------------
    // Get a handle to the default process heap.
	hDefaultProcessHeap = GetProcessHeap();
	if (hDefaultProcessHeap == NULL) {
		Print("Failed to retrieve the default process heap with LastError" << GetLastError());
		return 1;
	}

    //--------------------------------------------------------
    // Allocate the buffer from the default process heap.
	aHeaps = (PHANDLE)HeapAlloc(hDefaultProcessHeap, 0, BytesToAllocate);
	if (aHeaps == NULL) {
		Print("HeapAlloc failed to allocate" << BytesToAllocate << " bytes.");
		return 1;
	}
    //--------------------------------------------------------
    // Save the original number of heaps because we are going to compare it
    // to the return value of the next GetProcessHeaps call.

    HeapsLength = NumberOfHeaps;

    //--------------------------------------------------------
	// Retrieve handles to the process heaps and print them to stdout. 
	// Note that heap functions should be called only on the default heap of the process
	// or on private heaps that your component creates by calling HeapCreate.

    NumberOfHeaps = GetProcessHeaps(HeapsLength, aHeaps);
	if (NumberOfHeaps == 0)
    {
		Print("Failed to retrieve heaps with LastError " << GetLastError());
		return 1;
	}
	else if (NumberOfHeaps > HeapsLength)
    {

        //-----------------------------------------------------------------------
        // Compare the latest number of heaps with the original number of heaps.
		// If the latest number is larger than the original number, another
		// component has created a new heap and the buffer is too small.

        Print("Another component created a heap between calls. ");
		Print("Please try again");
		return 1;
	}

	Print("Process has " << HeapsLength << "heaps.");
	for (HeapsIndex = 0; HeapsIndex < HeapsLength; ++HeapsIndex)
	{
		Print("Heap" << HeapsIndex << " at address:" << aHeaps[HeapsIndex]);
	}
    return 1;
}
void MemState::GetMemState()
{

    //========================================================================================================================================================================================================================
    // Random Memory stuff I found a few days ago and started messing around with Very useful, still do not have a full grasp on it yet however
    //========================================================================================================================================================================================================================

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
    GetHeapInfo();
  //  std::cout << "Output of CrtDumpMemoryLeaks\n" << std::flush;
  //  _CrtDumpMemoryLeaks(); // any memory leak at the end of execution ? TONSSSSS Much of it from Assimp or maybe my failer to destroy what it creates, idk yet.

    //========================================================================================================================================================================================================================

    _CrtMemCheckpoint(&sh1);
}
void MemState::CheckMemory()
{

    _CrtMemCheckpoint(&sh2);

    std::cout << "Output of CrtCheckMemory\n" << std::flush;
    _CrtCheckMemory(); // shows corrupt memory
    _CrtMemDifference(&sh_diff, &sh1, &sh2); // calcs. diff. between cp1 and cp2

    std::cout << "Output of CrtMemDumpStatistics\n" << std::flush;
    _CrtMemDumpStatistics(&sh_diff); // shows the diff. calculated above

 //   std::cout << "Output of CrtDumpMemoryLeaks\n" << std::flush;
  //  	_CrtDumpMemoryLeaks(); // any memory leak at the end of execution ? TONSSSSS Much of it from Assimp or maybe my failer to destroy what it creates, idk yet.

}
#endif