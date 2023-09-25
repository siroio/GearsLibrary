#pragma once
#include <d3d12.h>
#include <Internal/DX12/d3dx12Inc.h>
#include <Internal/DX12/DescriptorHandle.h>
#include <ComPtr.h>
#include <WeakPtr.h>
#include <Vector2.h>
#include <string_view>

namespace Glib
{
    class Texture
    {
    public:
        Texture() = default;
        bool CreateTexture(std::string_view path);
        void SetTexture(unsigned int rootParamIndex);
        //bool CreateTexture(Vector2 textureSize, DXGI_FORMAT format);
        ComPtr<ID3D12Resource> Get();
        const Vector2& TextureSize() const;

    private:
        bool CreateShaderResourceView(DXGI_FORMAT fmt);

    private:
        Vector2 textureSize_;
        DXGI_FORMAT format_;
        std::shared_ptr<Internal::Graphics::DescriptorHandle> srvHandle_{ nullptr };
        ComPtr<ID3D12Resource> texture_{ nullptr };
    };
}
