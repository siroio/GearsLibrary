#pragma once
#include <filesystem>

namespace Glib
{
    using FilePath = std::filesystem::path;

    /**
     * @brief �t�@�C���p�X�����񂩂�p�X�ւ̕ϊ�
     * @param path �t�@�C���p�X
     */
    FilePath ConvertPath(std::string_view path)
    {
        return FilePath{ path };
    }

    /**
     * @brief �g���q���擾
     * @param path �t�@�C���p�X
     * @param removePeriod �g���q�̃h�b�g���폜���邩
     * @return �g���q
     */
    std::string GetExtension(std::string_view path, bool removePeriod = true)
    {
        auto ext = std::filesystem::path{ path }.extension().generic_string();
        if (removePeriod) std::erase(ext, '.');
        return ext;
    }

    /**
     * @brief �t�@�C�������݂��Ă��邩
     * @param path �t�@�C���p�X
     * @return
     */
    bool ExistsFile(std::string_view path)
    {
        return std::filesystem::exists(ConvertPath(path));
    }

    /**
     * @brief �t�@�C���̊g���q���`�F�b�N
     * @param path �t�@�C���p�X
     * @param extension �g���q
     */
    bool ExtensionCheck(std::string_view path, std::string_view extension)
    {
        auto ext = GetExtension(path);
        return extension.ends_with(ext);
    }
}
