#include <Texture.h>
#include <Internal/DX12/DirectX12.h>
#include <DirectXTex.h>
#include <functional>
#include <filesystem>

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

    size_t AlignmentedSize(size_t size, size_t alignment)
    {
        return size + alignment - size % alignment;
    }
}

namespace
{
    auto s_dx12 = Glib::Internal::Graphics::DirectX12::Instance();
}

bool Glib::Texture::CreateTexture(std::string_view path)
{
    std::filesystem::path filePath = path;
    if (!filePath.has_extension()) return false;
    DirectX::TexMetadata metadata{};
    DirectX::ScratchImage scratchImg{};
    auto extension = filePath.extension().string();
    if ((extension != "tga") || (extension != "dds"))
        extension = "png";
    const auto& func = s_loadFunctions.at(extension);

    // テクスチャのロード
    const auto result = func(filePath.wstring(), &metadata, scratchImg);
    if (!result) return false;

    auto img = scratchImg.GetImage(0, 0, 0);

    // アップロード用バッファ作成
    ComPtr<ID3D12Resource> uploadBuffer{ nullptr };
    auto heapProp = CD3DX12_HEAP_PROPERTIES{ D3D12_HEAP_TYPE_UPLOAD };
    auto resDesc = CD3DX12_RESOURCE_DESC::Buffer(AlignmentedSize(img->rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT) * img->height);

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
    if (FAILED(uploadBuffer->Map(0, nullptr, (void**)&mapped))) return false;

    auto srcAdress = img->pixels;
    auto rowPitch = AlignmentedSize(img->rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);

    for (unsigned int y = 0; y < img->height; y++)
    {
        std::copy_n(srcAdress, rowPitch, mapped);
        srcAdress += img->rowPitch;
        mapped += rowPitch;
    }
    uploadBuffer->Unmap(0, nullptr);

    // コピーの設定
    D3D12_TEXTURE_COPY_LOCATION src{};
    D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint = {};
    UINT nRow{};
    UINT64 rowSize{}, size{};
    auto desc = texture_->GetDesc();
    s_dx12->Device()->GetCopyableFootprints(&desc, 0, 1, 0, &footprint, &nRow, &rowSize, &size);
    src.pResource = uploadBuffer.Get();
    src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
    src.PlacedFootprint = footprint;
    src.PlacedFootprint.Offset = 0;
    src.PlacedFootprint.Footprint.Width = static_cast<UINT>(metadata.width);
    src.PlacedFootprint.Footprint.Height = static_cast<UINT>(metadata.height);
    src.PlacedFootprint.Footprint.Depth = static_cast<UINT>(metadata.depth);
    src.PlacedFootprint.Footprint.RowPitch = static_cast<UINT>(img->rowPitch + ((256 - (img->rowPitch % 256)) % 256));
    src.PlacedFootprint.Footprint.Format = img->format;

    D3D12_TEXTURE_COPY_LOCATION dst{};
    dst.pResource = texture_.Get();
    dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
    dst.SubresourceIndex = 0;

    // テクスチャのコピー
    s_dx12->CommandList()->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);

    // リソースバリアを設定
    s_dx12->Barrier(texture_.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

    // テクスチャサイズの設定
    textureSize_ = Vector2{ static_cast<float>(img->width), static_cast<float>(img->height) };

    // ShaderResourceViewの作成
    CreateShaderResourceView(img->format);

    // コマンドリストを実行してロードを完了させる
    s_dx12->ExecuteCommandList();

    return true;
}

void Glib::Texture::SetTexture(unsigned int rootParamIndex)
{
    s_dx12->CommandList()->SetGraphicsRootDescriptorTable(rootParamIndex, srvHandle_->GPU());
}

ComPtr<ID3D12Resource> Glib::Texture::Get()
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
