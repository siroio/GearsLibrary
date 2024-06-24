#include <Internal/DX12/ConstantBufferAddress.h>

D3D12_GPU_VIRTUAL_ADDRESS Glib::Internal::Graphics::ConstantBufferAddress::Address() const
{
    return address_;
}
