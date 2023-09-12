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

        { "bmp", s_loadFunctions["png"] },
        { "jpg", s_loadFunctions["png"] },
        { "jpeg", s_loadFunctions["png"] },
        { "spa", s_loadFunctions["png"] },
        { "sph", s_loadFunctions["png"] }
    };
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
    const auto& func = s_loadFunctions.at(filePath.extension().string());

    // テクスチャのロード
    const auto result = func(filePath.wstring(), &metadata, scratchImg);
    if (!result) return false;

    auto img = scratchImg.GetImage(0, 0, 0);

    // アップロード用バッファ作成
    ComPtr<ID3D12Resource> uploadBuffer{ nullptr };
    auto aligment = ((256ULL - (img->rowPitch % 256)) % 256);
    UINT64 width = static_cast<UINT64>(img->rowPitch) + aligment * static_cast<UINT64>(img->height);
    auto heapProp = CD3DX12_HEAP_PROPERTIES{ D3D12_HEAP_TYPE_UPLOAD };
    auto resDesc = CD3DX12_RESOURCE_DESC::Buffer(width);

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
    texResDesc.Width = metadata.width;
    texResDesc.Height = metadata.height;
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

    auto adress = img->pixels;
    auto rowPitch = img->rowPitch + aligment;

    for (unsigned int y = 0; y < img->height; ++y)
    {
        std::copy_n(adress, rowPitch, mapped);
        adress += img->rowPitch;
    }
    uploadBuffer->Unmap(0, nullptr);

    // コピーの設定
    D3D12_TEXTURE_COPY_LOCATION src{}, dst{};
    src.pResource = uploadBuffer.Get();
    src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
    src.PlacedFootprint.Offset = 0;
    src.PlacedFootprint.Footprint.Width = img->width;
    src.PlacedFootprint.Footprint.Height = img->height;
    src.PlacedFootprint.Footprint.RowPitch = img->rowPitch + aligment;
    src.PlacedFootprint.Footprint.Format = img->format;

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

Vector2 Glib::Texture::TextureSize() const
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
