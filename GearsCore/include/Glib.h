#ifndef GEARS_LIBRARY_H
#define GEARS_LIBRARY_H

/* DIRECTX12 LIB */
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "xinput.lib")
#pragma comment(lib, "xaudio2.lib")

#ifdef _DEBUG
/* DEBUG BUILD LIB */
#else
/* RELEASE BUILD LIB */
#endif // _DEBUG

#endif // !GEARS_LIBRARY_H
