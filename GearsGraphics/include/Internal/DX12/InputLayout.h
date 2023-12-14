#pragma once
#include <d3d12.h>

namespace Glib::Internal::Graphics::InputLayout
{
    /**
     * @brief 2D���W
     */
    constexpr D3D12_INPUT_ELEMENT_DESC POSITION_2D
    {
        "POSITION", 0,
        DXGI_FORMAT_R32G32_FLOAT, 0,
        D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
    };

    /**
     * @brief 3D���W
     */
    constexpr D3D12_INPUT_ELEMENT_DESC POSITION_3D
    {
        "POSITION", 0,
        DXGI_FORMAT_R32G32B32_FLOAT, 0,
        D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
    };

    /**
     * @brief �@�����W
     */
    constexpr D3D12_INPUT_ELEMENT_DESC NORMAL
    {
        "NORMAL", 0,
        DXGI_FORMAT_R32G32B32_FLOAT, 0,
        D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
    };

    /**
     * @brief UV���W
     */
    constexpr D3D12_INPUT_ELEMENT_DESC TEXCOORD
    {
        "TEXCOORD", 0,
        DXGI_FORMAT_R32G32_FLOAT, 0,
        D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
    };

    /**
     * @brief �ڐ�
     */
    constexpr D3D12_INPUT_ELEMENT_DESC TANGENT
    {
        "TANGENT", 0,
        DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
        D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
    };

    /**
     * @brief �F
    */
    constexpr D3D12_INPUT_ELEMENT_DESC COLOR
    {
        "COLOR", 0,
        DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
        D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
    };

    /**
     * @brief �{�[���C���f�b�N�X
     */
    constexpr D3D12_INPUT_ELEMENT_DESC BONEINDEX
    {
        "BONEINDEX", 0,
        DXGI_FORMAT_R32G32B32A32_SINT, 0,
        D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
    };

    /**
     * @brief �{�[���E�F�C�g
     */
    constexpr D3D12_INPUT_ELEMENT_DESC BONEWEIGHT
    {
        "BONEWEIGHT", 0,
        DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
        D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
    };
}
