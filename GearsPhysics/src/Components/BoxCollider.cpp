#include <Components/BoxCollider.h>
#include <Internal/Geometry.h>
#include <PxPhysicsAPI.h>
#include <GLGUI.h>
#include <Mathf.h>

void Glib::BoxCollider::Start()
{
    if (Initialize())
    {
        CreateShape(
            Internal::Geometory::CreateBox(GameObject(), size_)
        );
    }
}

const Vector3& Glib::BoxCollider::Size() const
{
    return size_;
}

void Glib::BoxCollider::Size(const Vector3& size)
{
    size_ = size;
}

void Glib::BoxCollider::SyncGeometry()
{
    Shape()->setGeometry(Internal::Geometory::CreateBox(GameObject(), size_));
}

void Glib::BoxCollider::OnGUI()
{
    Collider::OnGUI();
    GLGUI::DragVector3("Size", &size_, 0.01f, Mathf::EPSILON);
}
