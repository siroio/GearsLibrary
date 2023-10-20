#pragma once
#include <Internal/ISystem.h>
#include <Singleton.h>

namespace Glib::Internal::Graphics
{
    /**
     * @brief 標準で使うリソース管理クラス
     */
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
        bool Initialize();

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
        void SetTexture(unsigned int id, unsigned int rootParameterIndex);

    private:

        /**
         * @brief テクスチャの作成
         */
        bool CreateTexture(unsigned int id, unsigned char r, unsigned char g, unsigned char b, unsigned char a);

        /*  頂点バッファ作成  */

        bool CreateCameraVertexBuffer();
        bool CreateImageVertexBuffer();

        /*  テクスチャ作成  */

        bool CreateWhiteTexture();
        bool CreateMagentaTexture();
        bool CreateNormalMapTexture();

        /*  パイプライン作成  */

        bool CreateCameraPipelineState();
        bool CreateSpritePipelineState();
        bool CreateImagePipelineState();
        bool CreateLinePipelineState();
        bool CreateMeshPipelineState();
        bool CreateMeshShadowPipelineState();
        bool CreateSkinnedMeshPipelineState();
        bool CreateSkinnedMeshShadowPipelineState();
    };
}
