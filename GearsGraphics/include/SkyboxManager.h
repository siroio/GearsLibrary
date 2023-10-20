#pragma once
#include <Singleton.h>

namespace Glib
{
    class SkyboxManager : public Singleton<SkyboxManager>
    {
        SkyboxManager() = default;
        friend SkyboxManager& Singleton<SkyboxManager>::Instance();

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
        bool Load(unsigned int id,
                  std::string_view up,
                  std::string_view down,
                  std::string_view left,
                  std::string_view right,
                  std::string_view front,
                  std::string_view back);

        /**
         * @brief �`��X�J�C�{�b�N�X���w��
         * @param id
        */
        void SetSkybox(unsigned int id);

        /**
         * @brief ������
        */
        bool Initialize();

        /**
         * @brief �`��
        */
        void Draw();
    };
}
