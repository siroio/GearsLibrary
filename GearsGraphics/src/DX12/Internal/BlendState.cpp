#include <DX12/Internal/BlendState.h>
#include <d3d12.h>

D3D12_RENDER_TARGET_BLEND_DESC Glib::Internal::Graphics::BlendDesc::Create(BlendState state)
{
    switch (state)
    {
        default:
        case Glib::Internal::Graphics::BlendState::Default:
            D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
            blendDesc.BlendEnable = false;
            blendDesc.LogicOpEnable = false;
            blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
            blendDesc.SrcBlend = D3D12_BLEND_ONE;
            blendDesc.DestBlend = D3D12_BLEND_ZERO;
            blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
            blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
            blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
            blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
            return blendDesc;
        case Glib::Internal::Graphics::BlendState::Alpha:
            D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
            blendDesc.BlendEnable = true;
            blendDesc.LogicOpEnable = false;
            blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
            blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
            blendDesc.DestBlend = D3D12_BLEND_DEST_ALPHA;
            blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
            blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
            blendDesc.DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
            blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
            return blendDesc;
        case Glib::Internal::Graphics::BlendState::Add:
            D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
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
        case Glib::Internal::Graphics::BlendState::Substract:
            D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
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
