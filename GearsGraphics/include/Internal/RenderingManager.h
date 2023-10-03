#pragma once
#include <Internal/ISystem.h>
#include <Singleton.h>
#include <Internal/FuncOrderConstant.h>
#include <Internal/ComponentFunctionInfo.h>
#include <FuncDefinition.h>
#include <FuncOrderDefinition.h>
#include <concepts>
#include <unordered_map>
#include <list>

struct Color;
struct Vector3;
struct Vector2;
struct Matrix4x4;

namespace Glib::Internal
{
    class Renderer;
    class CameraBase;
}

namespace Glib::Internal::Concepts
{
    /**
     * @brief �����_���[����p
     */
    template<class T>
    concept IsRenderer = std::derived_from<T, Renderer>;
}

namespace Glib::Internal::Graphics
{
    class RenderingManager :
        public Interface::ISystem,
        public SingletonPtr<RenderingManager>,
        public Function::DrawOrderSet<Order::Draw::OBJECT>
    {
        RenderingManager() = default;
        friend WeakPtr<RenderingManager> SingletonPtr<RenderingManager>::Instance();

    private:
        /**
         * @brief �`��֐��̎��
         */
        enum class DrawType
        {
            Draw,
            Shadow
        };

    public:
        bool Initialize();
        void Update();
        void Draw();

        /**
         * @brief �����̐F
         */
        static const Color& LightAmbient();

        /**
         * @brief �����̐F��ݒ�
         */
        static void LightAmbient(const Color& color);

        /**
         * @brief �g�U���̐F
         */
        static const Color& LightDiffuse();

        /**
         * @brief �g�U���̐F��ݒ�
         */
        static void LightDiffuse(const Color& color);

        /**
         * @brief ���ʌ��̐F
         */
        static const Color& LightSpecular();

        /**
         * @brief ���ʌ��̐F��ݒ�
         */
        static void LightSpecular(const Color& specular);

        /**
         * @brief �����̕���
         */
        static const Vector3& LightDirection();

        /**
         * @brief �����̕�����ݒ�
         * @param direction
         */
        static void LightDirection(const Vector3& direction);

        /**
         * @brief �V���h�E�}�b�v�p�̕ϊ��s����v�Z
         * @param gazePoint �����_
         */
        Matrix4x4 CalculateMatrixForShadowMap(const Vector3& gazePoint) const;

        /**
         * @brief �V���h�E�}�b�v�̃o�C�A�X��ݒ�
         */
        static void ShadowMapBias(float bias);

        /**
         * @brief �V���h�E�}�b�v��Near��Far��ݒ�
         */
        static void ShadowMapClip(float nearZ, float farZ);

        /**
         * @brief �V���h�E�}�b�v�̕`��͈�
         */
        static void ShadowMapRange(const Vector2& range);

        /**
         * @brief �����̒萔��ݒ�
         * @param rootParamIndex
         */
        void SetDirectionalLightConstant(unsigned int rootParamIndex);

        template<class T> requires Concepts::IsRenderer<T>
        void AddRenderer(const std::shared_ptr<T>& renderer);

    private:
        template<class T> requires HasDrawFunc<T, void, WeakPtr<CameraBase>>
        void AddDraw(const std::shared_ptr<T> renderer);
        void AddDraw(...)
        {}

        template<class T> requires HasDrawShadowFunc<T, void, WeakPtr<CameraBase>>
        void AddDrawShadow(const std::shared_ptr<T> renderer);
        void AddDrawShadow(...)
        {}

    private:
        std::unordered_map<DrawType, std::list<ComponentFunctionInfo<void, WeakPtr<CameraBase>>>> renderers_;
    };

    template<class T> requires Concepts::IsRenderer<T>
    inline void Graphics::RenderingManager::AddRenderer(const std::shared_ptr<T>& renderer)
    {
        AddDraw(renderer);
        AddDrawShadow(renderer);
    }

    template<class T> requires HasDrawFunc<T, void, WeakPtr<CameraBase>>
    inline void Graphics::RenderingManager::AddDraw(const std::shared_ptr<T> renderer)
    {
        renderers_[DrawType::Draw].push_back(
            { renderer, std::make_shared<Function::HasDrawObject<T, void, WeakPtr<CameraBase>>>(renderer) }
        );
    }

    template<class T> requires HasDrawShadowFunc<T, void, WeakPtr<CameraBase>>
    inline void Graphics::RenderingManager::AddDrawShadow(const std::shared_ptr<T> renderer)
    {
        renderers_[DrawType::Shadow].push_back(
            { renderers_, std::make_shared<Function::HasDrawShadowObject<T, void, WeakPtr<CameraBase>>>(renderer) }
        );
    }
}
