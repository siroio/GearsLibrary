#include <Components/MeshCollider.h>
#include <Components/Rigidbody.h>
#include <Components/SkinnedMeshRenderer.h>
#include <Internal/PhysXManager.h>
#include <Internal/Geometry.h>
#include <PxPhysicsAPI.h>
#include <GameObject.h>
#include <MeshManager.h>
#include <Debugger.h>

namespace
{
    auto& s_meshManager = Glib::MeshManager::Instance();
    auto s_physX = Glib::Internal::Physics::PhysXManager::Instance();
}

void Glib::MeshCollider::Start()
{
    if (!enableMesh_)
    {
        Debug::Error("Incorrect mesh selected.");
        return;
    }

    const auto& rigibody = GameObject()->GetComponent<Glib::Rigidbody>();
    if (!rigibody.expired())
    {
        Debug::Error("Do not attach Rigidbody.");
        return;
    }

    const auto& renderer = GameObject()->GetComponent<SkinnedMeshRenderer>();
    if (!renderer.expired())
    {
        Debug::Warn("Skinning is not supported.");
    }

    const auto& transform = GameObject()->Transform();
    rigidStatic_ = static_cast<physx::PxRigidStatic*>(s_physX->CreateRigidBody(
        transform->Position(),
        transform->Rotation(),
        true,
        nullptr));

    CreateMesh();
    if (isConvex_)
    {
        auto* const ptr = std::get<physx::PxConvexMesh*>(meshPtr_);
        CreateShape(Internal::Geometory::CreateConvexMesh(GameObject(), ptr));
    }
    else
    {
        auto* const ptr = std::get<physx::PxTriangleMesh*>(meshPtr_);
        CreateShape(Internal::Geometory::CreateTriangleMesh(GameObject(), ptr));
    }
}

void Glib::MeshCollider::OnDestroy()
{
    std::visit([](auto&& ptr)
    {
        delete& ptr;
    }, meshPtr_);
}

unsigned int Glib::MeshCollider::MeshID() const
{
    return meshID_;
}

void Glib::MeshCollider::MeshID(unsigned int id)
{
    enableMesh_ = s_meshManager.Contains(id);
    if (!enableMesh_) return;
    meshID_ = id;
}

void Glib::MeshCollider::SyncGeometry()
{
    if (isConvex_)
    {
        auto* const ptr = std::get<physx::PxConvexMesh*>(meshPtr_);
        Shape()->setGeometry(Internal::Geometory::CreateConvexMesh(GameObject(), ptr));
    }
    else
    {
        auto* const ptr = std::get<physx::PxTriangleMesh*>(meshPtr_);
        Shape()->setGeometry(Internal::Geometory::CreateTriangleMesh(GameObject(), ptr));
    }
}

void Glib::MeshCollider::CreateMesh()
{
    if (!enableMesh_) return;

    if (isConvex_)
    {
        meshPtr_ = Internal::Geometory::CreateConvexMesh(s_meshManager.Mesh(meshID_));
    }
    else
    {
        meshPtr_ = Internal::Geometory::CreateTriangleMesh(s_meshManager.Mesh(meshID_));
    }
}
