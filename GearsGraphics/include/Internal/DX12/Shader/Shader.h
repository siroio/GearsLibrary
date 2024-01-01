#pragma once
#include <Internal/DX12/Shader/ShaderType.h>
#include <variant>
#include <string>

namespace Glib::Graphics
{
    /**
     * @brief シェーダ情報クラス
     */
    class Shader
    {
    public:
        Shader(unsigned int id, ShaderType type, std::wstring_view filePath, std::string_view entryPoint);
        Shader(unsigned int id, ShaderType type, std::string_view code, std::string_view entryPoint);

        /**
         * @brief シェーダーIDを取得
         */
        unsigned int ID() const;

        /**
         * @brief シェーダータイプを取得
         */
        ShaderType Type() const;

        /**
         * @brief シェーダーコードのタイプ
         */
        ShaderCodeType CodeType() const;

        /**
         * @brief シェーダー名を取得
         */
        std::string Name() const;

        /**
         * @brief シェーダーのパスを取得
         */
        std::wstring Path() const;

        /**
         * @brief シェーダーのコードを取得
         */
        std::string Code() const;

        /**
         * @brief シェーダーのエントリポイントを取得
         */
        std::string EntryPoint() const;

        /**
         * @brief シェーダーモデルを取得
         */
        std::string ShaderModel() const;

        /**
         * @brief IDとTypeで比較
         */
        bool operator == (const Shader& shader) const;

    private:
        unsigned int id_;
        ShaderType type_;
        std::variant<std::wstring, std::string> shaderUnit_;
        std::string entryPoint_;
    };
}

