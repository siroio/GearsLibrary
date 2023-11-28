#pragma once
#include <string_view>

/**
 * @brief 3D���f���p�C���^�[�t�F�[�X
 * @param ModelType 3D���f���̃f�[�^�^
 */
class IModel
{
public:
    IModel() = default;

    /**
     * @brief ���f���̓ǂݍ���
     * @param path ���f���p�X
     */
    virtual bool LoadModel(std::string_view path) = 0;

    /**
     * @brief ���f���̏o��
     * @param path �o�̓p�X
     */
    virtual bool WriteModel(std::string_view path) = 0;
};
