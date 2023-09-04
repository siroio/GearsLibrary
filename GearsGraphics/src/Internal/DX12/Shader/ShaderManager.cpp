#include <Internal/DX12/Shader/ShaderManager.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <MsgBox.h>
#include <memory>
#include <unordered_map>
#include <vector>

namespace
{
    /* 頂点シェーダー */
    std::unordered_map<unsigned int, ComPtr<ID3DBlob>> s_vertexShader;

    /* ピクセルシェーダー */
    std::unordered_map<unsigned int, ComPtr<ID3DBlob>> s_pixelShader;
}

bool Glib::Internal::Graphics::ShaderManager::Initialize()
{
    std::vector<Glib::Graphics::Shader> shaders;
    shaders.emplace_back(0, ShaderType::VERTEX, "code", "VS");
    return true;
}

void Glib::Internal::Graphics::ShaderManager::SetVertexShader(unsigned int id, D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc)
{
    desc.VS.pShaderBytecode = s_vertexShader.at(id)->GetBufferPointer();
    desc.VS.BytecodeLength = s_vertexShader.at(id)->GetBufferSize();
}

void Glib::Internal::Graphics::ShaderManager::SetPixelShader(unsigned int id, D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc)
{
    desc.PS.pShaderBytecode = s_pixelShader.at(id)->GetBufferPointer();
    desc.PS.BytecodeLength = s_pixelShader.at(id)->GetBufferSize();
}

bool Glib::Internal::Graphics::ShaderManager::CompileShader(const Glib::Graphics::Shader& shader)
{
    ComPtr<ID3D10Blob> shaderBlob{ nullptr };
    switch (shader.CodeType())
    {
        case ShaderCodeType::FILE:
            shaderBlob = Instance()->CompileFromFile(shader);
            break;
        case ShaderCodeType::CONSTANT:
            shaderBlob = Instance()->CompileFromCode(shader);
            break;
    }

    if (shaderBlob == nullptr) return false;

    switch (shader.Type())
    {
        case ShaderType::VERTEX:
            s_vertexShader[shader.Id()] = shaderBlob;
            break;
        case ShaderType::PIXEL:
            s_pixelShader[shader.Id()] = shaderBlob;
            break;
    }
    return true;
}

ComPtr<ID3DBlob> Glib::Internal::Graphics::ShaderManager::CompileFromFile(const Glib::Graphics::Shader& shader)
{
    ComPtr<ID3DBlob> shaderBinary{ nullptr };
    ComPtr<ID3DBlob> errorBlob{ nullptr };
    bool result = SUCCEEDED(D3DCompileFromFile(
        shader.Path().c_str(),
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        shader.EntryPoint().c_str(),
        shader.ShaderModel().c_str(),
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0,
        shaderBinary.ReleaseAndGetAddressOf(),
        errorBlob.ReleaseAndGetAddressOf()
    ));

    if (!result) ShaderError(errorBlob);
    return result ? shaderBinary : nullptr;
}

ComPtr<ID3DBlob> Glib::Internal::Graphics::ShaderManager::CompileFromCode(const Glib::Graphics::Shader& shader)
{
    ComPtr<ID3DBlob> shaderBinary{ nullptr };
    ComPtr<ID3DBlob> errorBlob{ nullptr };
    bool result = SUCCEEDED(D3DCompile(
        shader.Code().c_str(),
        shader.Code().size(),
        nullptr,
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        shader.EntryPoint().c_str(),
        shader.ShaderModel().c_str(),
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0,
        shaderBinary.ReleaseAndGetAddressOf(),
        errorBlob.ReleaseAndGetAddressOf()
    ));

    if (!result) ShaderError(errorBlob);
    return result ? shaderBinary : nullptr;
}

void Glib::Internal::Graphics::ShaderManager::ShaderError(ComPtr<ID3DBlob> error) const
{
    MsgBox::Show(
        static_cast<LPCSTR>(error.Get()->GetBufferPointer()),       // message
        "Shader Compile Error.",                                    // title
        MsgBox::STYLE::OK,                                          // style
        MsgBox::ICON::ERROR_ICON                                    // icon
    );
}
