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
     * @brief �L�����o�X�p�Ǘ��N���X
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
         * @brief �`��
         */
        void Draw();

        /**
         * @brief �L�����o�X��ǉ�
         * @param canvas
         */
        void AddCanvas(const WeakPtr<Canvas>& canvas);

        /**
         * @brief UI��ǉ�
         */
        void AddUI(const WeakPtr<Internal::UIRenderer> renderer, const WeakPtr<Canvas>& canvas);

        /**
         * @brief �L�����o�X�̕`�揇�Ԃ�ύX
         */
        void ChangeCanvasOrder(const WeakPtr<Canvas>& canvas, int order);

    private:

        void DrawUI(const WeakPtr<Canvas>& canvas) const;
    };
}