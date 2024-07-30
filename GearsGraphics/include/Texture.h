#pragma once
#include <d3d12.h>
#include <Internal/DX12/d3dx12Inc.h>
#include <Internal/DX12/DescriptorHandle.h>
#include <ComPtr.h>
#include <WeakPtr.h>
#include <Vector2.h>
#include <string_view>
#include <array>

namespace Glib
{
    class Texture
    {
    public:
        bool CreateTexture(unsigned int width, unsigned int height, const std::array<unsigned char, 4>& color);
        bool CreateTexture(std::string_view path);
        void SetTexture(unsigned int rootParamIndex) const;
        ComPtr<ID3D12Resource> Get() const;
        const Vector2& TextureSize() const;

    private:
        bool CreateShaderResourceView(DXGI_FORMAT fmt);

    private:
        Vector2 textureSize_{ 0 };
        DXGI_FORMAT format_{ DXGI_FORMAT_UNKNOWN };
        std::shared_ptr<Internal::Graphics::DescriptorHandle> srvHandle_{ nullptr };
        ComPtr<ID3D12Resource> texture_{ nullptr };
    };
}
