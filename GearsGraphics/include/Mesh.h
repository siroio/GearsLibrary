#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <Internal/DX12/ConstantBuffer.h>
#include <Internal/DX12/VertexBuffer.h>
#include <Internal/DX12/IndexBuffer.h>
#include <WeakPtr.h>
#include <Texture.h>
#include <Bone.h>

namespace Glib
{
    class Mesh
    {
    private:
        struct Subset
        {
            int indexStart;
            int indexCount;
            int materialID;
        };

        struct Material
        {
            Internal::Graphics::ConstantBuffer params;
            WeakPtr<Texture> albedo;
            WeakPtr<Texture> normal;
        };

    public:
        bool Load(std::string_view path);

        void Draw();

        void DrawShadow();

        const std::vector<Bone>& Bone() const;

    private:
        Internal::Graphics::VertexBuffer vertexBuffer_;
        Internal::Graphics::IndexBuffer indexBuffer_;
        std::vector<Subset> subsets_;
        std::vector<Material> materials_;
        std::vector<Glib::Bone> bones_;
    };
}

