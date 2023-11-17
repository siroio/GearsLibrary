#include <MeshManager.h>
#include <memory>
#include <Mesh.h>

namespace
{
    std::unordered_map<unsigned int, std::unique_ptr<Glib::Mesh>> s_meshs;
}

bool Glib::MeshManager::Load(unsigned int id, std::string_view path)
{
    auto mesh = std::make_unique<Mesh>();
    if (!mesh->Load(path)) return false;
    s_meshs.emplace(id, std::move(mesh));
    return true;
}

bool Glib::MeshManager::Contains(unsigned int id)
{
    return s_meshs.contains(id);
}

void Glib::MeshManager::Draw(unsigned int id)
{
    if (!s_meshs.contains(id)) return;
    s_meshs.at(id)->Draw();
}

void Glib::MeshManager::DrawShadow(unsigned int id)
{
    if (!s_meshs.contains(id)) return;
    s_meshs.at(id)->DrawShadow();
}
