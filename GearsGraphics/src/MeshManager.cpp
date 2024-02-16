#include <MeshManager.h>
#include <memory>
#include <Mesh.h>

namespace
{
    std::unordered_map<unsigned int, std::unique_ptr<Glib::Mesh>> s_meshs;
    const std::vector<Glib::Bone> s_NullBone;
}

bool Glib::MeshManager::Load(unsigned int id, std::string_view path)
{
    auto mesh = std::make_unique<Glib::Mesh>();
    if (!mesh->Load(path)) return false;
    s_meshs.emplace(id, std::move(mesh));
    return true;
}

bool Glib::MeshManager::Contains(unsigned int id) const
{
    return s_meshs.contains(id);
}

const std::vector<Glib::Bone>& Glib::MeshManager::Bone(unsigned int id)
{
    if (!s_meshs.contains(id)) return s_NullBone;
    return s_meshs.at(id)->Bone();
}

const Glib::Mesh& Glib::MeshManager::Mesh(unsigned int id)
{
    return *s_meshs.at(id);
}

void Glib::MeshManager::Draw(unsigned int id) const
{
    if (!s_meshs.contains(id)) return;
    s_meshs.at(id)->Draw();
}

void Glib::MeshManager::DrawShadow(unsigned int id) const
{
    if (!s_meshs.contains(id)) return;
    s_meshs.at(id)->DrawShadow();
}
