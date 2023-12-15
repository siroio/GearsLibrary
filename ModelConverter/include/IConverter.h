#pragma once
#include <string_view>

/**
 * @brief �ϊ��p�C���^�[�t�F�[�X
 */
class IConverter
{
public:
    IConverter() = default;
    virtual ~IConverter() = default;

    /**
     * @brief �t�@�C���̓ǂݍ���
     * @param path �t�@�C���ւ̃p�X
     * @return true : ����
     * @return false : ���s
     */
    virtual bool LoadFile(std::string_view path) = 0;

    /**
     * @brief �t�@�C���̏����o��
     * @param path �g���q�܂߂��t�@�C���p�X
     * @return true : ����
     * @return false : ���s
     */
    virtual bool WriteFile(std::string_view path) = 0;
};
