#include <MeshManager.h>
#include <Mesh.h>

namespace
{
    std::unordered_map<unsigned int, Glib::Mesh> s_meshs;
}

bool Glib::MeshManager::Load(unsigned int id, std::string_view path)
{
    Mesh mesh{};
    if (!mesh.Load(path)) return false;
    s_meshs.emplace(id, std::move(mesh));
}

bool Glib::MeshManager::Contains(unsigned int id)
{
    return s_meshs.contains(id);
}

void Glib::MeshManager::Draw(unsigned int id)
{
    if (!s_meshs.contains(id)) return;
    s_meshs.at(id).Draw();
}

void Glib::MeshManager::DrawShadow(unsigned int id)
{
    if (!s_meshs.contains(id)) return;
    s_meshs.at(id).DrawShadow();
}
