#pragma once
#include"Common.h"

#ifndef WIN32
#else
namespace NativeMemory
{
    #include <windows.h>
    #include <tchar.h>
    #include <stdio.h>
    #include <intsafe.h>


    /// =========MEMORY ALLOCATION TRACKING STUFF=========================
    #define _CRTDBG_MAP_ALLOC  
    #include <stdlib.h>  
    #include <crtdbg.h>  
    /// ========= END MEMORY ALLOCATION TRACKING STUFF=========================

    int GetHeapInfo();
    void GetMemState();

    struct MemState
    {
        _CrtMemState sh1, sh2, sh_diff;
        void GetMemState();
        void CheckMemory();
    };

    extern MemState GlobalMemState;
}
#endif
