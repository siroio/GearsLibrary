#pragma once
#include <fstream>

namespace Glib
{
    /**
     * @brief �o�C�i���t�@�C���֏����o��
     * @param file �o�C�i���t�@�C��
     * @param data �f�[�^�ւ̃|�C���^
     * @param length �f�[�^��
     */
    inline void WriteToBinary(std::ofstream& file, const void* const data, const int length)
    {
        file.write(reinterpret_cast<const char*>(data), length);
    }

    /**
     * @brief �o�C�i������ǂݏo��
     * @param file �o�C�i���t�@�C��
     * @param data �f�[�^�ւ̃|�C���^
     * @param length �f�[�^��
     */
    inline void ReadForBinary(std::ifstream& file, void* const data, const int length)
    {
        file.read(reinterpret_cast<char*>(data), length);
    }
}
