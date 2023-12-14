#pragma once
#include <d3d12.h>

namespace Glib::Internal::Graphics::InputLayout
{
    /**
     * @brief 2D座標
     */
    constexpr D3D12_INPUT_ELEMENT_DESC POSITION_2D
    {
        "POSITION", 0,
        DXGI_FORMAT_R32G32_FLOAT, 0,
        D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
    };

    /**
     * @brief 3D座標
     */
    constexpr D3D12_INPUT_ELEMENT_DESC POSITION_3D
    {
        "POSITION", 0,
        DXGI_FORMAT_R32G32B32_FLOAT, 0,
        D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
    };

    /**
     * @brief 法線座標
     */
    constexpr D3D12_INPUT_ELEMENT_DESC NORMAL
    {
        "NORMAL", 0,
        DXGI_FORMAT_R32G32B32_FLOAT, 0,
        D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
    };

    /**
     * @brief UV座標
     */
    constexpr D3D12_INPUT_ELEMENT_DESC TEXCOORD
    {
        "TEXCOORD", 0,
        DXGI_FORMAT_R32G32_FLOAT, 0,
        D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
    };

    /**
     * @brief 接線
     */
    constexpr D3D12_INPUT_ELEMENT_DESC TANGENT
    {
        "TANGENT", 0,
        DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
        D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
    };

    /**
     * @brief 色
    */
    constexpr D3D12_INPUT_ELEMENT_DESC COLOR
    {
        "COLOR", 0,
        DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
        D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
    };

    /**
     * @brief ボーンインデックス
     */
    constexpr D3D12_INPUT_ELEMENT_DESC BONEINDEX
    {
        "BONEINDEX", 0,
        DXGI_FORMAT_R32G32B32A32_SINT, 0,
        D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
    };

    /**
     * @brief ボーンウェイト
     */
    constexpr D3D12_INPUT_ELEMENT_DESC BONEWEIGHT
    {
        "BONEWEIGHT", 0,
        DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
        D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
    };
}
