#pragma once
#include <array>
#include <string_view>

namespace Glib
{
    class Texture;

    class Skybox
    {
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
        bool Load(std::string_view up,
                  std::string_view down,
                  std::string_view left,
                  std::string_view right,
                  std::string_view front,
                  std::string_view back);

        /**
         * @brief 描画
        */
        void Draw() const;

    private:
        std::array<Texture, 6> textures_;
    };
}
