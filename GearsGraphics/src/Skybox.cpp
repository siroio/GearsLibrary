#include <Skybox.h>
#include <Internal/DX12/DirectX12.h>
#include <Texture.h>

namespace
{
    auto s_dx12 = Glib::Internal::Graphics::DirectX12::Instance();
}

bool Glib::Skybox::Load(std::string_view up, std::string_view down, std::string_view left, std::string_view right, std::string_view front, std::string_view back)
{
    if (textures_.at(0).CreateTexture(up)) return false;
    if (textures_.at(1).CreateTexture(down)) return false;
    if (textures_.at(2).CreateTexture(left)) return false;
    if (textures_.at(3).CreateTexture(right)) return false;
    if (textures_.at(4).CreateTexture(front)) return false;
    if (textures_.at(5).CreateTexture(back)) return false;
    return true;
}

void Glib::Skybox::Draw() const
{
    for (int i = 0; i < textures_.size(); i++)
    {
        textures_.at(i).SetTexture(0);
        s_dx12->CommandList()->DrawInstanced(0, 1, i * 4, 0);
    }
}
