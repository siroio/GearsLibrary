#pragma once
#include <Internal/ISystem.h>
#include <Singleton.h>
#include <FuncOrderDefinition.h>
#include <Internal/FuncOrderConstant.h>

namespace Glib
{
    namespace Internal
    {
        class UIRenderer;
    }

    class Canvas;
}

namespace Glib::Internal::Graphics
{
    /**
     * @brief キャンバス用管理クラス
     */
    class CanvasManager :
        public Interface::ISystem,
        public SingletonPtr<CanvasManager>,
        public Function::DrawOrderSet<Order::Draw::UI>
    {
        CanvasManager() = default;
        friend WeakPtr<CanvasManager> SingletonPtr<CanvasManager>::Instance();

    public:
        /**
         * @brief 描画
         */
        void Draw();

        /**
         * @brief キャンバスを追加
         * @param canvas
         */
        void AddCanvas(const WeakPtr<Canvas>& canvas);

        /**
         * @brief UIを追加
         */
        void AddUI(const WeakPtr<Internal::UIRenderer> renderer, const WeakPtr<Canvas>& canvas);

        /**
         * @brief キャンバスの描画順番を変更
         */
        void ChangeCanvasOrder(const WeakPtr<Canvas>& canvas, int order);

    private:

        void DrawUI(const WeakPtr<Canvas>& canvas) const;
    };
}