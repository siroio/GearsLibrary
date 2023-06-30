#pragma once

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
#pragma comment(lib, "GearsCored.lib")
#pragma comment(lib, "GearsComponentd.lib")
#pragma comment(lib, "GearsGraphicsd.lib")
#pragma comment(lib, "GearsMathd.lib")
#pragma comment(lib, "GearsObjectd.lib")
#pragma comment(lib, "GearsScened.lib")
#pragma comment(lib, "GearsUtilityd.lib")
#pragma comment(lib, "DirectXTexd.lib")
#else
/* RELEASE BUILD LIB */
#pragma comment(lib, "GearsCore.lib")
#pragma comment(lib, "GearsComponent.lib")
#pragma comment(lib, "GearsGraphics.lib")
#pragma comment(lib, "GearsMath.lib")
#pragma comment(lib, "GearsObject.lib")
#pragma comment(lib, "GearsScene.lib")
#pragma comment(lib, "GearsUtility.lib")
#pragma comment(lib, "DirectXTex.lib")
#endif // _DEBUG
