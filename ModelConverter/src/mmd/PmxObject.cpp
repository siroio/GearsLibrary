#include <mmd/PmxObject.h>
#include <filesystem>
#include <fstream>
#include <GLObjectConstant.h>

namespace fs = std::filesystem;

namespace
{
    // Šg’£Žq‚ðŽæ“¾
    std::string GetExt(std::string_view file)
    {
        return fs::path{ file }.extension().string();
    }
}

bool PmxModel::LoadModel(std::string_view path)
{
    // Šg’£Žq‚ªŠÔˆá‚Á‚Ä‚¢‚½‚çŽ¸”s
    if (GetExt(path) != GL_OBJECT_EXTENSION) return false;

    std::ifstream file{ path.data(), std::ios::binary };

    return true;
}

const int& PmxModel::GetData() const
{
    return 0;
}
