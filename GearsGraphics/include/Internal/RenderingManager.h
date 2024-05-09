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
     * @brief レンダラー判定用
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
         * @brief 描画関数の種類
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
         * @brief 環境光の色
         */
        static const Color& LightAmbient();

        /**
         * @brief 環境光の色を設定
         */
        static void LightAmbient(const Color& color);

        /**
         * @brief 拡散光の色
         */
        static const Color& LightDiffuse();

        /**
         * @brief 拡散光の色を設定
         */
        static void LightDiffuse(const Color& color);

        /**
         * @brief 鏡面光の色
         */
        static const Color& LightSpecular();

        /**
         * @brief 鏡面光の色を設定
         */
        static void LightSpecular(const Color& specular);

        /**
         * @brief 環境光の方向
         */
        static const Vector3& LightDirection();

        /**
         * @brief 環境光の方向を設定
         * @param direction
         */
        static void LightDirection(const Vector3& direction);

        /**
         * @brief シャドウマップ用の変換行列を計算
         * @param cameraGaze 注視点
         */
        Matrix4x4 CalculateMatrixForShadowMap(const Vector3& cameraGaze) const;

        /**
         * @brief シャドウマップのバイアスを設定
         */
        static void ShadowMapBias(float bias);

        /**
         * @brief シャドウマップのバイアスを設定
         */
        static void ShadowMapMomentBias(float bias);

        /**
         * @brief シャドウマップのNearとFarを設定
         */
        static void ShadowMapClip(float nearZ, float farZ);

        /**
         * @brief シャドウマップの描画範囲
         */
        static void ShadowMapRange(const Vector2& range);

        /**
         * @brief 環境光の定数を設定
         * @param rootParamIndex
         */
        void SetDirectionalLightConstant(unsigned int rootParamIndex);

        template<class T> requires Concepts::IsRenderer<T>
        void AddRenderer(const std::shared_ptr<T>& renderer);

        void DebugDraw();

    private:
        template<class T> requires HasDrawFunc<T, void, WeakPtr<CameraBase>>
        void AddDraw(const std::shared_ptr<T>& renderer);
        void AddDraw(...)
        {}

        template<class T> requires HasDrawShadowFunc<T, void, WeakPtr<CameraBase>>
        void AddDrawShadow(const std::shared_ptr<T>& renderer);
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
    inline void Graphics::RenderingManager::AddDraw(const std::shared_ptr<T>& renderer)
    {
        renderers_[DrawType::Draw].push_back(
            { renderer, std::make_shared<Function::HasDrawObject<T, void, WeakPtr<CameraBase>>>(renderer) }
        );
    }

    template<class T> requires HasDrawShadowFunc<T, void, WeakPtr<CameraBase>>
    inline void Graphics::RenderingManager::AddDrawShadow(const std::shared_ptr<T>& renderer)
    {
        renderers_[DrawType::Shadow].push_back(
            { renderer, std::make_shared<Function::HasDrawShadowObject<T, void, WeakPtr<CameraBase>>>(renderer) }
        );
    }
}
