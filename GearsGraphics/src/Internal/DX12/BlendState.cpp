#include <Internal/DX12/BlendState.h>
#include <d3d12.h>

D3D12_RENDER_TARGET_BLEND_DESC Glib::Internal::Graphics::BlendDesc::Create(BlendState state)
{
    D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
    switch (state)
    {
        default:
        case BlendState::Default:
            blendDesc.BlendEnable = true;
            blendDesc.LogicOpEnable = false;
            blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
            blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
            blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
            blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
            blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
            blendDesc.DestBlendAlpha = D3D12_BLEND_ONE;
            blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
            return blendDesc;
        case BlendState::Alpha:
            blendDesc.BlendEnable = true;
            blendDesc.LogicOpEnable = false;
            blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
            blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
            blendDesc.DestBlend = D3D12_BLEND_ONE;
            blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
            blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
            blendDesc.DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
            blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
            return blendDesc;
        case BlendState::Add:
            blendDesc.BlendEnable = true;
            blendDesc.LogicOpEnable = false;
            blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
            blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
            blendDesc.DestBlend = D3D12_BLEND_ONE;
            blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
            blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
            blendDesc.DestBlendAlpha = D3D12_BLEND_ONE;
            blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
            return blendDesc;
        case BlendState::Substract:
            blendDesc.BlendEnable = true;
            blendDesc.LogicOpEnable = false;
            blendDesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
            blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
            blendDesc.DestBlend = D3D12_BLEND_ONE;
            blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
            blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
            blendDesc.DestBlendAlpha = D3D12_BLEND_ONE;
            blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
            return blendDesc;
    }
}
