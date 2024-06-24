#include <Internal/DX12/CommandList.h>
#include <Internal/DX12/DirectX12.h>
#include <Internal/DX12/d3dx12Inc.h>

namespace
{
    auto s_dx12 = Glib::Internal::Graphics::DirectX12::Instance();
}

bool Glib::Internal::Graphics::CommandList::Create(D3D12_COMMAND_LIST_TYPE type, ComPtr<ID3D12CommandAllocator> allocator, ComPtr<ID3D12CommandQueue> queue, CommandList* cmdList)
{
    if (cmdList == nullptr) return false;
    auto hr = s_dx12->Device()->CreateCommandList(
        0, type,
        allocator.Get(),
        nullptr,
        IID_PPV_ARGS(cmdList->cmdList_.ReleaseAndGetAddressOf())
    );

    if (allocator.Get() == nullptr ||
        queue.Get() == nullptr) return false;

    cmdList->cmdAllocator_ = allocator;
    cmdList->cmdQueue_ = queue;

    return SUCCEEDED(hr);
}

bool Glib::Internal::Graphics::CommandList::CreateBundle(CommandList* cmdList)
{
    if (cmdList == nullptr) return false;
    auto hr = s_dx12->Device()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_BUNDLE, IID_PPV_ARGS(cmdList->cmdAllocator_.ReleaseAndGetAddressOf()));
    if (FAILED(hr)) return false;

    hr = s_dx12->Device()->CreateCommandList(
        0, D3D12_COMMAND_LIST_TYPE_BUNDLE,
        cmdList->cmdAllocator_.Get(),
        nullptr,
        IID_PPV_ARGS(cmdList->cmdList_.ReleaseAndGetAddressOf())
    );

    return SUCCEEDED(hr);
}

void Glib::Internal::Graphics::CommandList::Execute() const
{
    ID3D12CommandList* cmdLists[]{ cmdList_.Get() };
    cmdQueue_->ExecuteCommandLists(static_cast<UINT>(std::size(cmdLists)), cmdLists);
}

bool Glib::Internal::Graphics::CommandList::CloseAndBundleToList(const CommandList* list) const
{
    // バンドルタイプか確認
    if (list == nullptr || Type() != D3D12_COMMAND_LIST_TYPE_BUNDLE) return false;
    // バンドルをクローズして追加
    list->List()->Close();
    cmdList_->ExecuteBundle(list->List().Get());
    return true;
}

void Glib::Internal::Graphics::CommandList::CloseList()
{
    cmdList_->Close();
}

void Glib::Internal::Graphics::CommandList::Reset()
{
    cmdAllocator_->Reset();
    cmdList_->Reset(cmdAllocator_.Get(), nullptr);
}

ComPtr<ID3D12CommandAllocator> Glib::Internal::Graphics::CommandList::Allocator() const
{
    return cmdAllocator_;
}

ComPtr<ID3D12GraphicsCommandList> Glib::Internal::Graphics::CommandList::List() const
{
    return cmdList_;
}

ComPtr<ID3D12CommandQueue> Glib::Internal::Graphics::CommandList::Queue() const
{
    return cmdQueue_;
}

D3D12_COMMAND_LIST_TYPE Glib::Internal::Graphics::CommandList::Type() const
{
    return cmdList_->GetType();
}
