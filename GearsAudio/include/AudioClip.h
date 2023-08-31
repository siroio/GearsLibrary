#pragma once
#include <string>
#include <vector>
#include <xaudio2.h>

namespace Glib
{
    class AudioClip
    {
    public:
        explicit AudioClip(
            std::string_view name,
            const WAVEFORMATEX& format,
            const std::vector<unsigned char>& data);

        /**
         * @brief ���O
         */
        std::string Name() const;

        /**
         * @brief �t�H�[�}�b�g
         */
        const WAVEFORMATEX& Format() const;

        /**
         * @brief �����f�[�^
         */
        const std::vector<unsigned char>& RawData() const;

        /**
         * @brief �o�b�t�@
         */
        const XAUDIO2_BUFFER& Buffer() const;

        /**
         * @brief �`�����l����
         */
        unsigned int Channels() const;

        /**
         * @brief ���[�v���ǂ���
         */
        bool Loop() const;

        /**
         * @brief ���[�v�̐؂�ւ�
         * @param enable
         */
        void Loop(bool enable);

        /**
         * @brief �f�[�^�����[�h
         */
        bool Load(unsigned int id, std::string_view filePath);

    private:
        std::string clipName_{};
        WAVEFORMATEX format_{};
        XAUDIO2_BUFFER buffer_{};
        std::vector<unsigned char> audioData_;
    };
}
