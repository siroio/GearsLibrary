#pragma once
#include <Internal/ISystem.h>
#include <Singleton.h>
#include <Internal/FuncOrderConstant.h>
#include <FuncOrderDefinition.h>

namespace Glib
{
    class SkyboxManager :
        public Internal::Interface::ISystem,
        public SingletonPtr<SkyboxManager>,
        public Internal::Function::DrawOrderSet<Internal::Order::Draw::SKYBOX>
    {
        SkyboxManager() = default;
        friend WeakPtr<SkyboxManager> SingletonPtr<SkyboxManager>::Instance();

    public:
        /**
         * @brief スカイボックス用テクスチャの読み込み
         * @param up
         * @param down
         * @param left
         * @param right
         * @param front
         * @param back
         * @return 成功 : true
         * @return 失敗 : false
        */
        bool Load(unsigned int id,
                  std::string_view up,
                  std::string_view down,
                  std::string_view left,
                  std::string_view right,
                  std::string_view front,
                  std::string_view back);

        /**
         * @brief 描画スカイボックスを指定
         * @param id
        */
        void SetSkybox(unsigned int id);

        /**
         * @brief 初期化
        */
        bool Initialize();

        /**
         * @brief 描画
        */
        void Draw();
    };
}
