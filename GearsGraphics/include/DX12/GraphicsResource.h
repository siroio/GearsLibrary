#pragma once
#include <Internal/ISystem.h>
#include <Singleton.h>

namespace Glib::Graphics
{
    class GraphicsResource :
        public Glib::Internal::Interface::ISystem,
        public SingletonPtr<GraphicsResource>
    {
    private:
        GraphicsResource() = default;
        friend WeakPtr<GraphicsResource> SingletonPtr<GraphicsResource>::Instance();

    public:

        /**
         * @brief リソースの初期化
         */
        void Initialize();

        /**
         * @brief 頂点バッファの設定
         */
        void SetVertexBuffer(unsigned int id);

        /**
         * @brief パイプラインステートの設定
         */
        void SetPipelineState(unsigned int id);

        /**
         * @brief テクスチャの設定
         */
        void SetTexture(unsigned int id);
    private:


    private:
    };
}
