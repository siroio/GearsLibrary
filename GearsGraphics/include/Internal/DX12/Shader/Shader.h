#pragma once
#include <Internal/DX12/Shader/ShaderType.h>
#include <variant>
#include <string>

namespace Glib::Graphics
{
    /**
     * @brief �V�F�[�_���N���X
     */
    class Shader
    {
    public:
        Shader(unsigned int id, ShaderType type, std::wstring_view filePath, std::string_view entryPoint);
        Shader(unsigned int id, ShaderType type, std::string_view code, std::string_view entryPoint);

        /**
         * @brief �V�F�[�_�[ID���擾
         */
        unsigned int ID() const;

        /**
         * @brief �V�F�[�_�[�^�C�v���擾
         */
        ShaderType Type() const;

        /**
         * @brief �V�F�[�_�[�R�[�h�̃^�C�v
         */
        ShaderCodeType CodeType() const;

        /**
         * @brief �V�F�[�_�[�����擾
         */
        std::string Name() const;

        /**
         * @brief �V�F�[�_�[�̃p�X���擾
         */
        std::wstring Path() const;

        /**
         * @brief �V�F�[�_�[�̃R�[�h���擾
         */
        std::string Code() const;

        /**
         * @brief �V�F�[�_�[�̃G���g���|�C���g���擾
         */
        std::string EntryPoint() const;

        /**
         * @brief �V�F�[�_�[���f�����擾
         */
        std::string ShaderModel() const;

        /**
         * @brief ID��Type�Ŕ�r
         */
        bool operator == (const Shader& shader) const;

    private:
        unsigned int id_;
        ShaderType type_;
        std::variant<std::wstring, std::string> shaderUnit_;
        std::string entryPoint_;
    };
}
