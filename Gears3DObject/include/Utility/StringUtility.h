#pragma once
#include <string>
#include <string_view>
#include <filesystem>
#include <fstream>
#include <Utility/IOUtility.h>

namespace fs = std::filesystem;

namespace Glib
{
    inline bool CheckExt(std::string_view view, std::string_view extension)
    {
        fs::path path{ view };
        path = path.lexically_normal(); // パスの正規化
        // 不正なパスかチェック
        if (path == ".") return false;
        return path.extension().generic_string().ends_with(extension);
    }

    inline fs::path GetAbsPath(const fs::path& path)
    {
        return path.is_absolute() ? path : fs::absolute(path);
    }

    inline void ReadText(std::ifstream& file, std::string& str)
    {
        int size{ 0 };
        ReadForBinary(file, &size, sizeof(int));
        str.resize(size);
        ReadForBinary(file, &str[0], sizeof(char) * size);
    }

    inline void WriteText(std::ofstream& file, std::string_view str)
    {
        int size = str.empty() ? 0 : static_cast<int>(str.size());
        WriteToBinary(file, &size, sizeof(int));
        WriteToBinary(file, str.data(), sizeof(char) * size);
    }
}
