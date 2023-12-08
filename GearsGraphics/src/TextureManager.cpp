#include <TextureManager.h>
#include <WeakPtr.h>
#include <Texture.h>
#include <unordered_map>

namespace
{
    std::unordered_map<unsigned int, std::shared_ptr<Glib::Texture>> s_textureResource;
    std::unordered_map<std::string, std::shared_ptr<Glib::Texture>> s_meshTextureResource;
}

bool Glib::TextureManager::Load(unsigned int id, std::string_view filePath)
{
    if (!s_textureResource.contains(id))
    {
        auto tex = std::make_shared<Texture>();
        if (!tex->CreateTexture(filePath)) return false;
        s_textureResource.emplace(id, tex);
    }
    return true;
}

Glib::WeakPtr<Glib::Texture> Glib::TextureManager::Load(std::string_view filePath)
{
    if (!s_meshTextureResource.contains(filePath.data()))
    {
        auto tex = std::make_shared<Texture>();
        if (!tex->CreateTexture(filePath)) return WeakPtr<Texture>{};
        s_meshTextureResource.emplace(filePath.data(), tex);
    }
    return WeakPtr<Texture>{ s_meshTextureResource.at(filePath.data()) };
}

bool Glib::TextureManager::Contains(unsigned int id)
{
    return s_textureResource.contains(id);
}

const Vector2& Glib::TextureManager::TextureSize(unsigned int id)
{
    return s_textureResource.at(id)->TextureSize();
}

void Glib::TextureManager::SetTexture(unsigned int id, unsigned int rootParamIndex)
{
    s_textureResource.at(id)->SetTexture(rootParamIndex);
}
