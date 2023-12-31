#pragma once
#include <string>
#include <iosfwd>

/**
 * @brief �t�@�C���̃G���R�[�h
 */
enum class Encode : unsigned char
{
    UTF8,
    UTF16
};

class FileUtility
{
public:
    /**
     * @brief �g���q�̎擾
     * @param path �t�@�C���p�X
     * @param removePeriod �g���q��[ . ]���폜���邩
     * @return �g���q
     */
    static std::string GetExtension(std::string_view path, bool removePeriod = true);

    /**
     * @brief �o�C�i���֏�������
     * @param file
     * @param data
     * @param length
     */
    static void WriteToBinary(std::ofstream& file, const void* const data, const int length);

    /**
     * @brief �o�C�i������ǂݍ���
     * @param file
     * @param data
     * @param length
     */
    static void ReadForBinary(std::ifstream& file, void* const data, const int length);

    /**
     * @brief [�e�L�X�g�� + �e�L�X�g�{��]�̌`�����o�b�t�@�֓ǂݍ���
     * @param file �t�@�C�����J���Ă���X�g���[��
     * @param text �o�b�t�@
     * @param encode �ǂݍ��݃G���R�[�h
     */
    static void ReadText(std::ifstream& file, std::string& text, Encode encode = Encode::UTF8);
};
