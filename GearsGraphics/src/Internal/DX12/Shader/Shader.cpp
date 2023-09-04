#include <Internal/DX12/Shader/Shader.h>
#include <filesystem>

Glib::Graphics::Shader::Shader(unsigned int id, ShaderType type, std::wstring_view filePath, std::string_view entryPoint)
    : id_{ id }, type_{ type }, shaderUnit_{ filePath.data() }, entryPoint_{ entryPoint }
{}

Glib::Graphics::Shader::Shader(unsigned int id, ShaderType type, std::string_view code, std::string_view entryPoint)
    : id_{ id }, type_{ type }, shaderUnit_{ code.data() }, entryPoint_{ entryPoint }
{}

unsigned int Glib::Graphics::Shader::Id() const
{
    return id_;
}

Glib::ShaderType Glib::Graphics::Shader::Type() const
{
    return type_;
}

Glib::ShaderCodeType Glib::Graphics::Shader::CodeType() const
{
    return shaderUnit_.index() == 0 ? ShaderCodeType::FILE : ShaderCodeType::CONSTANT;
}

std::string Glib::Graphics::Shader::Name() const
{
    if (shaderUnit_.index() == 1) return "";
    const std::filesystem::path path = std::get<0>(shaderUnit_);
    return path.filename().stem().string();
}

std::wstring Glib::Graphics::Shader::Path() const
{
    if (shaderUnit_.index() == 0) return L"";
    return std::get<0>(shaderUnit_);
}

std::string Glib::Graphics::Shader::Code() const
{
    if (shaderUnit_.index() == 0) return "";
    return std::get<1>(shaderUnit_);
}

std::string Glib::Graphics::Shader::EntryPoint() const
{
    return entryPoint_;
}

std::string Glib::Graphics::Shader::ShaderModel() const
{
    switch (type_)
    {
        case ShaderType::VERTEX:  return "vs_5_0";
        case ShaderType::PIXEL:   return "ps_5_0";
        default:                        return "";
    }
}

bool Glib::Graphics::Shader::operator==(const Shader& shader) const
{
    return id_ == shader.Id() && type_ == shader.Type();
}
