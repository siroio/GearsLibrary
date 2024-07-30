#include <Texture.h>
#include <Internal/DX12/DirectX12.h>
#include <Internal/DX12/CommandAllocator.h>
#include <Internal/DX12/CommandList.h>
#include <Internal/DX12/Fence.h>
#include <DirectXTex.h>
#include <StringUtility.h>
#include <Color.h>
#include <vector>
#include <functional>
#include <filesystem>

using namespace Glib::Internal::Graphics;

namespace
{
    using TextureLoader = std::function<bool(std::wstring_view, DirectX::TexMetadata*, DirectX::ScratchImage&)>;

    /* 画像読み込み用関数テーブル */
    std::unordered_map<std::string, TextureLoader> s_loadFunctions
    {
        { "tga", [](std::wstring_view path, DirectX::TexMetadata* meta, DirectX::ScratchImage& img)
        {
            return SUCCEEDED(DirectX::LoadFromTGAFile(path.data(), meta, img));
        } },

        { "dds", [](std::wstring_view path, DirectX::TexMetadata* meta, DirectX::ScratchImage& img)
        {
            return SUCCEEDED(DirectX::LoadFromDDSFile(path.data(), DirectX::DDS_FLAGS_NONE, meta, img));
        } },

        { "png", [](std::wstring_view path, DirectX::TexMetadata* meta, DirectX::ScratchImage& img)
        {
            return SUCCEEDED(DirectX::LoadFromWICFile(path.data(), DirectX::WIC_FLAGS_NONE, meta, img));
        } },
    };

    /* アライメント計算 */
    size_t AlignmentedSize(const size_t& size)
    {
        return (size + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1) & ~(D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1);
    }
}

namespace
{
    auto s_dx12 = DirectX12::Instance();
}

bool Glib::Texture::CreateTexture(unsigned int width, unsigned int height, const std::array<unsigned char, 4>& color)
{
    const size_t dataSize = color.size() * width * height;

    CD3DX12_HEAP_PROPERTIES heapProp{ D3D12_HEAP_TYPE_CUSTOM, 0U, 0U };
    heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
    heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
    CD3DX12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, width, height, 1U, 1U);
    auto hr = s_dx12->Device()->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
        nullptr,
        IID_PPV_ARGS(texture_.ReleaseAndGetAddressOf())
    );
    if (FAILED(hr)) return false;

    std::vector<unsigned char> texData(dataSize);
    for (int i = 0; i < texData.size(); i++)
    {
        int idx = i % 4;
        switch (idx)
        {
            case 0: texData.at(i) = color[0]; break;
            case 1: texData.at(i) = color[1]; break;
            case 2: texData.at(i) = color[2]; break;
            case 3: texData.at(i) = color[3]; break;
        }
    }

    hr = texture_->WriteToSubresource(
        0U,
        nullptr,
        texData.data(),
        static_cast<UINT>(width * 4), // 横幅 * 4byte
        0U
    );
    if (FAILED(hr)) return false;

    CreateShaderResourceView(resDesc.Format);

    return true;
}

bool Glib::Texture::CreateTexture(std::string_view path)
{
    std::filesystem::path filePath = Glib::CharConv(path);
    if (!filePath.is_absolute()) filePath = std::filesystem::absolute(filePath);
    if (!filePath.has_extension()) return false;
    DirectX::TexMetadata metadata{};
    DirectX::ScratchImage scratchImg{};
    auto extension = filePath.extension().generic_string();
    if (!extension.ends_with("tga") || !extension.ends_with("dds"))
        extension = "png";
    const auto& func = s_loadFunctions.at(extension);

    // テクスチャのロード
    const auto result = func(filePath.generic_wstring(), &metadata, scratchImg);
    if (!result) return false;

    auto img = scratchImg.GetImage(0, 0, 0);

    // アップロード用バッファ作成
    ComPtr<ID3D12Resource> uploadBuffer{ nullptr };
    auto heapProp = CD3DX12_HEAP_PROPERTIES{ D3D12_HEAP_TYPE_UPLOAD };
    auto resDesc = CD3DX12_RESOURCE_DESC::Buffer(AlignmentedSize(img->rowPitch) * img->height);

    if (FAILED(s_dx12->Device()->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(uploadBuffer.ReleaseAndGetAddressOf())
        ))) return false;

    D3D12_RESOURCE_DESC texResDesc{};
    texResDesc.Format = metadata.format;
    texResDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);
    texResDesc.Width = static_cast<UINT>(metadata.width);
    texResDesc.Height = static_cast<UINT>(metadata.height);
    texResDesc.DepthOrArraySize = static_cast<UINT16>(metadata.arraySize);
    texResDesc.MipLevels = static_cast<UINT16>(metadata.mipLevels);
    texResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    texResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    texResDesc.SampleDesc.Count = 1;
    texResDesc.SampleDesc.Quality = 0;

    // テクスチャのバッファを作成
    auto texHeapProp = CD3DX12_HEAP_PROPERTIES{ D3D12_HEAP_TYPE_DEFAULT };
    if (FAILED(s_dx12->Device()->CreateCommittedResource(
        &texHeapProp,
        D3D12_HEAP_FLAG_NONE,
        &texResDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(texture_.ReleaseAndGetAddressOf())
        ))) return false;

    // アップロード用のバッファにコピー
    uint8_t* mapped{ nullptr };
    if (FAILED(uploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mapped)))) return false;
    std::copy_n(img->pixels, img->slicePitch, mapped);
    uploadBuffer->Unmap(0, nullptr);

    // コピーの設定
    D3D12_TEXTURE_COPY_LOCATION src{};
    src.pResource = uploadBuffer.Get();
    src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
    src.PlacedFootprint.Offset = 0;
    src.PlacedFootprint.Footprint.Width = static_cast<UINT>(metadata.width);
    src.PlacedFootprint.Footprint.Height = static_cast<UINT>(metadata.height);
    src.PlacedFootprint.Footprint.Depth = static_cast<UINT>(metadata.depth);
    src.PlacedFootprint.Footprint.RowPitch = static_cast<UINT>(AlignmentedSize(img->rowPitch));
    src.PlacedFootprint.Footprint.Format = img->format;

    D3D12_TEXTURE_COPY_LOCATION dst{};
    dst.pResource = texture_.Get();
    dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
    dst.SubresourceIndex = 0;

    // アロケーターの作成
    auto cmdAlloc = std::make_shared<CommandAllocator>();
    const auto cmdType = D3D12_COMMAND_LIST_TYPE_DIRECT;
    if (!CommandAllocator::Create(cmdType, cmdAlloc.get())) return false;

    // リストの作成
    auto cmdList = std::make_shared<CommandList>();
    if (!CommandList::Create(cmdType, cmdAlloc->Allocator(), s_dx12->CommandQueue(), cmdList.get())) return false;

    // フェンスの作成
    auto fence = std::make_shared<Fence>();
    if (!Fence::Create(0, D3D12_FENCE_FLAG_NONE, fence.get())) return false;

    // テクスチャのコピー
    cmdList->List()->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);

    // リソースバリアを設定
    s_dx12->Barrier(texture_.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

    // テクスチャサイズの設定
    textureSize_ = Vector2{ static_cast<float>(img->width), static_cast<float>(img->height) };

    // ShaderResourceViewの作成
    CreateShaderResourceView(img->format);

    // コマンドの実行
    cmdList->CloseList();
    cmdList->Execute();
    fence->Signal(cmdList.get());
    fence->WaitGPU();

    return true;
}

void Glib::Texture::SetTexture(unsigned int rootParamIndex) const
{
    s_dx12->CommandList()->SetGraphicsRootDescriptorTable(rootParamIndex, srvHandle_->GPU());
}

ComPtr<ID3D12Resource> Glib::Texture::Get() const
{
    return texture_;
}

const Vector2& Glib::Texture::TextureSize() const
{
    return textureSize_;
}

bool Glib::Texture::CreateShaderResourceView(DXGI_FORMAT fmt)
{
    auto srvPool = s_dx12->DescriptorPool(Internal::Graphics::DirectX12::PoolType::RES);
    srvHandle_ = srvPool->GetHandle();
    if (srvHandle_ == nullptr) return false;

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Format = fmt;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Texture2D.MipLevels = 1;

    // ShaderResourceViewの作成
    s_dx12->Device()->CreateShaderResourceView(
        texture_.Get(),
        &srvDesc,
        srvHandle_->CPU()
    );

    return true;
}
