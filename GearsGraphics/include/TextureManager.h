#pragma once
#include <Singleton.h>
#include <string_view>

struct Vector2;

namespace Glib
{
    class Texture;

    class TextureManager :
        public Singleton<TextureManager>
    {
        TextureManager() = default;
        friend TextureManager& Singleton<TextureManager>::Instance();

    public:
        /**
         * @brief �e�N�X�`���̓ǂݍ���
         * @param id �e�N�X�`��ID
         * @param filePath �t�@�C���p�X
         * @return ���� true
         * @return ���s false
         */
        bool Load(unsigned int id, std::string_view filePath);

        /**
         * @brief ���b�V���p�e�N�X�`���̓ǂݍ���
         * @param filePath �t�@�C���p�X
         * @return ���� true
         * @return ���s false
         */
        WeakPtr<Texture> Load(std::string_view filePath);

        /**
         * @brief �e�N�X�`��ID���L�����m�F
         * @param id �e�N�X�`��ID
         * @return �L�� true
         * @return ���� false
         */
        bool IsValid(unsigned int id);

        /**
         * @brief �e�N�X�`���̃T�C�Y���擾
         * @param id �e�N�X�`��ID
         * @return �T�C�Y
         */
        static const Vector2& TextureSize(unsigned int id);

        /**
         * @brief �e�N�X�`����ݒ�
         * @param id �e�N�X�`��ID
         * @param rootParamIndex ���[�g�p�����[�^
         */
        void SetTexture(unsigned int id, unsigned int rootParamIndex);
    };
}
