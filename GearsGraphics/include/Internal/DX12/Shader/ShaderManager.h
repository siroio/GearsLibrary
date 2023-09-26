#pragma once
#include <Internal/DX12/Shader/Shader.h>
#include <Internal/ISystem.h>
#include <Singleton.h>
#include <Internal/FuncOrderConstant.h>
#include <FuncOrderDefinition.h>
#include <ComPtr.h>

struct D3D12_GRAPHICS_PIPELINE_STATE_DESC;
struct ID3D10Blob;
typedef ID3D10Blob ID3DBlob;

namespace Glib::Internal::Graphics
{
    class ShaderManager :
        public Interface::ISystem,
        public SingletonPtr<ShaderManager>,
        public Function::InitializeOrderSet<Order::Initialzie::SHADER>
    {
        ShaderManager() = default;
        friend WeakPtr<ShaderManager> SingletonPtr<ShaderManager>::Instance();

    public:

        /**
         * @brief シェーダーの初期化
         */
        bool Initialize();

        /**
         * @brief 頂点シェーダーの設定
         */
        void SetVertexShader(unsigned int id, D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc);

        /**
         * @brief ピクセルシェーダーの設定
         */
        void SetPixelShader(unsigned int id, D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc);

    public:
        /**
         * @brief シェーダーをコンパイル
         * @param shader シェーダー情報
         * @return
         */
        bool CompileShader(const Glib::Graphics::Shader& shader);

    private:
        ComPtr<ID3DBlob> CompileFromFile(const Glib::Graphics::Shader& shader);
        ComPtr<ID3DBlob> CompileFromCode(const Glib::Graphics::Shader& shader);
        void ShaderError(ComPtr<ID3DBlob> error) const;
    };
}
