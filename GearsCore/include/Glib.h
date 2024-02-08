﻿#pragma once
/* Linker Header File  */

/* DIRECTX12 LIB */
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "xinput.lib")
#pragma comment(lib, "xaudio2.lib")

#if defined(DEBUG) || defined(_DEBUG)
/* DEBUG BUILD LIB */
#pragma comment(lib, "Gears3DObject_Debug.lib")
#pragma comment(lib, "GearsAudio_Debug.lib")
#pragma comment(lib, "GearsComponent_Debug.lib")
#pragma comment(lib, "GearsCore_Debug.lib")
#pragma comment(lib, "GearsDebug_Debug.lib")
#pragma comment(lib, "GearsGraphics_Debug.lib")
#pragma comment(lib, "GearsInput_Debug.lib")
#pragma comment(lib, "GearsMath_Debug.lib")
#pragma comment(lib, "GearsObject_Debug.lib")
#pragma comment(lib, "GearsScene_Debug.lib")
#pragma comment(lib, "GearsUtility_Debug.lib")
#pragma comment(lib, "DirectXTex_Debug.lib")
#pragma comment(lib, "Effekseer_Debug.lib")
#pragma comment(lib, "EffekseerRendererDX12_Debug.lib")
#pragma comment(lib, "LLGI_Debug.lib")
#pragma comment(lib, "Jolt_Debug.lib")
#else
/* RELEASE BUILD LIB */
#pragma comment(lib, "Gears3DObject.lib")
#pragma comment(lib, "GearsAudio.lib")
#pragma comment(lib, "GearsComponent.lib")
#pragma comment(lib, "GearsCore.lib")
#pragma comment(lib, "GearsDebug.lib")
#pragma comment(lib, "GearsGraphics.lib")
#pragma comment(lib, "GearsInput.lib")
#pragma comment(lib, "GearsMath.lib")
#pragma comment(lib, "GearsObject.lib")
#pragma comment(lib, "GearsScene.lib")
#pragma comment(lib, "GearsUtility.lib")
#pragma comment(lib, "DirectXTex.lib")
#pragma comment(lib, "Effekseer.lib")
#pragma comment(lib, "EffekseerRendererDX12.lib")
#pragma comment(lib, "LLGI.lib")
#pragma comment(lib, "Jolt.lib")
#endif // _DEBUG
