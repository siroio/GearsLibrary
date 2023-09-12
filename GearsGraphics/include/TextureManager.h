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
         * @brief テクスチャの読み込み
         * @param id テクスチャID
         * @param filePath ファイルパス
         * @return 成功 true
         * @return 失敗 false
         */
        bool Load(unsigned int id, std::string_view filePath);

        /**
         * @brief メッシュ用テクスチャの読み込み
         * @param filePath ファイルパス
         * @return 成功 true
         * @return 失敗 false
         */
        WeakPtr<Texture> Load(std::string_view filePath);

        /**
         * @brief テクスチャIDが有効か確認
         * @param id テクスチャID
         * @return 有効 true
         * @return 無効 false
         */
        bool IsValid(unsigned int id);

        /**
         * @brief テクスチャのサイズを取得
         * @param id テクスチャID
         * @return サイズ
         */
        static const Vector2& TextureSize(unsigned int id);

        /**
         * @brief テクスチャを設定
         * @param id テクスチャID
         * @param rootParamIndex ルートパラメータ
         */
        void SetTexture(unsigned int id, unsigned int rootParamIndex);
    };
}
