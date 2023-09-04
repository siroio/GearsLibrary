#pragma once
#include <Internal/DX12/d3dx12Inc.h>
#include <ComPtr.h>
#include <WeakPtr.h>

namespace Glib::Internal::Graphics
{
    class Texture
    {
    public:
        bool CreateTexture(UINT width, UINT height, DXGI_FORMAT format);
        ComPtr<ID3D12Resource> Get();

    private:
        UINT width_;
        UINT height_;
        DXGI_FORMAT format_;
        ComPtr<ID3D12Resource> texture_;
    };
}
