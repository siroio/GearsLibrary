#ifndef GEARS_DIRECTX_12_H
#define GEARS_DIRECTX_12_H

#include <Singleton.h>
#include <Internal/ISystem.h>
#include <FuncOrderDefinition.h>
#include <Internal/FuncOrderConstant.h>

struct Color;

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
         * @brief ‰Šú‰»
         */
        bool Initialize();

        /**
         * @brief •`‰æŠJn
         */
        void BeginDraw();

        /**
         * @brief •`‰æI—¹
         */
        void EndDraw();

        /**
         * @brief I—¹
         */
        void Finalize();

        /**
         * @brief ”wŒiF‚Ìæ“¾
         * @return ”wŒiF
         */
        static const Color& BackGroundColor();

        /**
         * @brief ”wŒiF‚Ìİ’è
         * @param ”wŒiF
         */
        static void BackGroundColor(const Color& color);
    };
}

#endif // !GEARS_DIRECTX_12_H
