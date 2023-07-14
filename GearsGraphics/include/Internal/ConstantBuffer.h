#pragma once
#include <ComPtr.h>
class DescriptorHandle;
class DescriptorPool;
class ID3D12Resource;

namespace Glib::Internal::Graphcis
{
    class ConstantBuffer
    {
    public:
        bool Create(UINT bufferSize);

    private:
        ComPtr<ID3D12Resource> buffer_{ nullptr };
        DescriptorPool* pool_{ nullptr };
        DescriptorHandle* handle_{ nullptr };
    };
}
