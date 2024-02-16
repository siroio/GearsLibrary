#include <Components/MeshCollider.h>
#include <Components/Rigidbody.h>
#include <Components/MeshRenderer.h>
#include <Components/SkinnedMeshRenderer.h>
#include <Internal/Renderer.h>
#include <Internal/Geometry.h>
#include <GameObject.h>
#include <MeshManager.h>
#include <Debugger.h>

namespace
{
    auto& s_meshManager = Glib::MeshManager::Instance();
}

void Glib::MeshCollider::Start()
{
    const auto& rigibody = GameObject()->GetComponent<Glib::Rigidbody>();
    if (!rigibody.expired())
    {
        Debug::Log("Do not attach Rigidbody.", LogLevel::Error);
    }

    //if (renderer.expired())
    //{
    //    Debug::Log("Please attach MeshRenderer or SkinnedMeshRenderer.", LogLevel::Error);
    //}

    //if (!s_meshManager.Contains(renderer->MeshID()))
    //{
    //    Debug::Log("Invalid MeshID.", LogLevel::Error);
    //}

    //const auto& mesh = s_meshManager.Mesh(renderer->MeshID());

    //if (isConvex)
    //{
    //    meshPtr_ = Internal::Geometory::CreateConvexMesh(GameObject(), mesh);
    //}
    //else
    //{
    //    meshPtr_ = Internal::Geometory::CreateTriangleMesh(GameObject(), mesh);
    //}
}

void Glib::MeshCollider::SyncGeometry()
{

}
