#ifndef GEARS_DIRECTX_12_H
#define GEARS_DIRECTX_12_H

#include <Singleton.h>
#include <Internal/ISystem.h>
#include <FuncOrderDefinition.h>
#include <Internal/FuncOrderConstant.h>
#include <d3dx12.h>

struct Color;
struct ID3D12Device;
struct ID3D12GraphicsCommandList;
struct ID3D12CommandQueue;
struct IDXGIFactory6;
struct D3D12_GRAPHICS_PIPELINE_STATE_DESC;
struct D3D12_RESOURCE_DESC;

namespace Glib::Internal::Graphics
{
    class DirectX12 :
        public SingletonPtr<DirectX12>,
        public Interface::ISystem,
        public Function::InitializeOrderSet<Order::Initialzie::DX12_INIT>,
        public Function::BeginDrawOrderSet<Order::BeginDraw::DX12_BEGINDRAW>
    {
        DirectX12() = default;
        friend WeakPtr<DirectX12> SingletonPtr<DirectX12>::Instance();

    public:

        /**
         * @brief èâä˙âª
         */
        bool Initialize();

        /**
         * @brief ï`âÊäJén
         */
        void BeginDraw();

        /**
         * @brief ï`âÊèIóπ
         */
        void EndDraw();

        /**
         * @brief èIóπ
         */
        void Finalize();

        /**
         * @brief îwåiêFÇÃéÊìæ
         * @return îwåiêF
         */
        static const Color& BackGroundColor();

        /**
         * @brief îwåiêFÇÃê›íË
         * @param îwåiêF
         */
        static void BackGroundColor(const Color& color);

    private:
        void EnableDebugLayer();
    };
}

#endif // !GEARS_DIRECTX_12_H
