#pragma once
#include <string_view>

/**
 * @brief 3Dモデル用インターフェース
 * @param ModelType 3Dモデルのデータ型
 */
class IModel
{
public:
    IModel() = default;

    /**
     * @brief モデルの読み込み
     * @param path モデルパス
     */
    virtual bool LoadModel(std::string_view path) = 0;

    /**
     * @brief モデルの出力
     * @param path 出力パス
     */
    virtual bool WriteModel(std::string_view path) = 0;
};
