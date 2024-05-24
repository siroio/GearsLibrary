#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <Internal/MemoryCheck.h>

void Glib::Internal::MemoryCheck::Start()
{
    isStart = true;
    if (isStart) return;
    // メモリリーク検出開始
    _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}
#else
void Glib::Internal::MemoryCheck::Start()
{}
#endif
