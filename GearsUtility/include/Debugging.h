#ifndef GEARS_DEBUGGING
#define GEARS_DEBUGGING

#ifdef _DEBUG

#include <cassert>

#define Debug_Assert(exp)            assert(exp)
#define Debug_AssertMsg(exp, msg)    (void)(!!(exp)) || (_wassert(L"\"" ## TEMP_STR1(exp) ## "\" - " ## msg, _CRT_WIDE(__FILE__), __LINE__), 0)

#else

#define Debug_Assert(exp)            ((void)0)
#define Debug_AssertMsg(exp, msg)    ((void)0)

#endif // _DEBUG

#endif // !GEARS_DEBUGGING
