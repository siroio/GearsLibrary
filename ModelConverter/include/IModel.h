#pragma once
#include <string_view>

/**
 * @brief 3D���f���p�C���^�[�t�F�[�X
 * @param ModelType 3D���f���̃f�[�^�^
 */
template<class ModelType>
class IModel
{
public:
    IModel() = default;

    /**
     * @brief ���f���̓ǂݍ���
     * @param path ���f���p�X
     */
    virtual void LoadModel(const std::string_view path) = 0;

    /**
     * @brief ���f���f�[�^�̎擾
     */
    virtual const ModelType& GetData() const = 0;
};
