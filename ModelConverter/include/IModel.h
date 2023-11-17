#pragma once
#include <string_view>

/**
 * @brief 3Dモデル用インターフェース
 * @param ModelType 3Dモデルのデータ型
 */
template<class ModelType>
class IModel
{
public:
    IModel() = default;

    /**
     * @brief モデルの読み込み
     * @param path モデルパス
     */
    virtual void LoadModel(const std::string_view path) = 0;

    /**
     * @brief モデルデータの取得
     */
    virtual const ModelType& GetData() const = 0;
};
