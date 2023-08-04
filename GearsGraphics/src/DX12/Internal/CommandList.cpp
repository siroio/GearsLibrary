#include <DX12/Internal/CommandList.h>
#include <DX12/Internal/DirectX12.h>
#include <DX12/Internal/d3dx12Inc.h>

namespace
{
    auto s_dx12 = Glib::Internal::Graphics::DirectX12::Instance();
}

bool Glib::Internal::Graphics::CommandList::Create(D3D12_COMMAND_LIST_TYPE type, const D3D12_COMMAND_QUEUE_DESC& desc, std::shared_ptr<CommandList>& cmdList)
{
    auto instance = std::make_shared<CommandList>();
    auto hr = s_dx12->Device()->CreateCommandAllocator(type, IID_PPV_ARGS(instance->cmdAllocator_.ReleaseAndGetAddressOf()));
    if (FAILED(hr)) return false;

    hr = s_dx12->Device()->CreateCommandList(
        0, type,
        instance->cmdAllocator_.Get(),
        nullptr,
        IID_PPV_ARGS(instance->cmdList_.ReleaseAndGetAddressOf())
    );
    if (FAILED(hr)) return false;

    hr = s_dx12->Device()->CreateCommandQueue(&desc, IID_PPV_ARGS(instance->cmdQueue_.ReleaseAndGetAddressOf()));
    if (FAILED(hr)) return false;

    cmdList = instance;
    return true;
}

bool Glib::Internal::Graphics::CommandList::CreateBundle(std::shared_ptr<CommandList>& cmdList)
{
    auto instance = std::make_shared<CommandList>();
    auto hr = s_dx12->Device()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_BUNDLE, IID_PPV_ARGS(instance->cmdAllocator_.ReleaseAndGetAddressOf()));
    if (FAILED(hr)) return false;

    hr = s_dx12->Device()->CreateCommandList(
        0, D3D12_COMMAND_LIST_TYPE_BUNDLE,
        instance->cmdAllocator_.Get(),
        nullptr,
        IID_PPV_ARGS(instance->cmdList_.ReleaseAndGetAddressOf())
    );
    if (FAILED(hr)) return false;

    cmdList = instance;
    return true;
}

void Glib::Internal::Graphics::CommandList::Execute() const
{
    ID3D12CommandList* cmdLists[]{ cmdList_.Get() };
    cmdQueue_->ExecuteCommandLists(1, cmdLists);
}

bool Glib::Internal::Graphics::CommandList::CloseAndBundleToList(const Glib::WeakPtr<CommandList>& list) const
{
    // バンドルタイプか確認
    if (list.expired() || Type() != D3D12_COMMAND_LIST_TYPE_BUNDLE) return false;
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

const ComPtr<ID3D12CommandAllocator> Glib::Internal::Graphics::CommandList::Allocator() const
{
    return cmdAllocator_;
}

const ComPtr<ID3D12GraphicsCommandList> Glib::Internal::Graphics::CommandList::List() const
{
    return cmdList_;
}

const ComPtr<ID3D12CommandQueue> Glib::Internal::Graphics::CommandList::Queue() const
{
    return cmdQueue_;
}

D3D12_COMMAND_LIST_TYPE Glib::Internal::Graphics::CommandList::Type() const
{
    return cmdList_->GetType();
}
