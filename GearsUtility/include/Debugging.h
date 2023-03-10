#pragma once
#include <assert.h>

#ifdef _DEBUG

#define PLATFORM_DEBUG

#endif

#ifdef PLATFORM_DEBUG 

#define Debug_Assert(exp)            assert(exp)
#define Debug_AssertMsg(exp, msg)    (void)(!!(exp)) || (_wassert(L"\"" ## TEMP_STR1(exp) ## "\" - " ## msg, _CRT_WIDE(__FILE__), __LINE__), 0)

#endif
