#pragma once
#include <memory>
#include <WeakPtr.h>
#include <ComPtr.h>
#include <initializer_list>

struct ID3D12CommandAllocator;
struct ID3D12GraphicsCommandList;
struct ID3D12CommandQueue;
struct D3D12_COMMAND_QUEUE_DESC;
enum D3D12_COMMAND_LIST_TYPE;

namespace Glib::Internal::Graphics
{
    /**
     * @brief �R�}���h���X�g�Ǘ��N���X
     */
    class CommandList
    {
    public:
        CommandList() = default;

        /**
         * @brief �R�}���h���X�g�̍쐬
         * @return ���� true
         * @return ���s false
         */
        static bool Create(D3D12_COMMAND_LIST_TYPE type, const D3D12_COMMAND_QUEUE_DESC& desc, std::shared_ptr<CommandList>& cmdList);

        /**
         * @brief �o���h���p�̃R�}���h���X�g���쐬
         * @return ���� true
         * @return ���s false
         */
        static bool CreateBundle(std::shared_ptr<CommandList>& cmdList);

    public:
        /**
         * @brief �R�}���h�̎��s
         */
        void Execute() const;

        /**
         * @brief �o���h�������R�}���h�����X�g�ɒǉ�
         */
        bool CloseAndBundleToList(const Glib::WeakPtr<CommandList>& lists) const;

        /**
         * @brief ���X�g�̃N���[�Y
         */
        void CloseList();

        /**
         * @brief ���X�g�̃��Z�b�g
         */
        void Reset();

        /**
         * @brief �A���P�[�^�[�̎擾
         */
        ComPtr<ID3D12CommandAllocator> Allocator() const;

        /**
         * @brief ���X�g�̎擾
         */
        ComPtr<ID3D12GraphicsCommandList> List() const;

        /**
         * @brief �L���[�̎擾
         */
        ComPtr<ID3D12CommandQueue> Queue() const;

        /**
         * @brief �R�}���h���X�g�̃^�C�v���擾
         * @return �^�C�v
         */
        D3D12_COMMAND_LIST_TYPE Type() const;

    private:
        CommandList(const CommandList&) = delete;
        CommandList& operator = (const CommandList&) = delete;

    private:
        ComPtr<ID3D12CommandAllocator> cmdAllocator_{ nullptr };
        ComPtr<ID3D12GraphicsCommandList> cmdList_{ nullptr };
        ComPtr<ID3D12CommandQueue> cmdQueue_{ nullptr };
    };
}
