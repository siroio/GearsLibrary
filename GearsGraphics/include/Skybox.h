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
         * @brief �X�J�C�{�b�N�X�p�e�N�X�`���̓ǂݍ���
         * @param up
         * @param down
         * @param left
         * @param right
         * @param front
         * @param back
         * @return ���� : true
         * @return ���s : false
        */
        bool Load(std::string_view up,
                  std::string_view down,
                  std::string_view left,
                  std::string_view right,
                  std::string_view front,
                  std::string_view back);

        /**
         * @brief �`��
        */
        void Draw() const;

    private:
        std::array<Texture, 6> textures_;
    };
}
