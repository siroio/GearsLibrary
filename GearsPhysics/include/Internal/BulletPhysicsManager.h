#include <Singleton.h>
#include <Internal/ISystem.h>
#include <vector>

struct Vector3;
class btVector3;
class btRigidBody;

namespace Glib
{
    struct RaycastHit;
}

namespace Glib::Internal::Interface
{
    class IRigidbody;
}

namespace Glib::Internal::Physics
{
    class BulletPhysicsManager :
        public Interface::ISystem,
        public SingletonPtr<BulletPhysicsManager>
    {
        BulletPhysicsManager() = default;
        friend WeakPtr<BulletPhysicsManager> SingletonPtr<BulletPhysicsManager>::Instance();

    public:
        bool Initialize();
        void Update();

        void ExecuteTriggerCallbacks();
        void ExecuteCollisionCallbacks();

        bool Raycast(const Vector3& origin, const Vector3& direction, float maxDistance, RaycastHit* hit);
        bool RaycastAll(const Vector3& origin, const Vector3& direction, float maxDistance, std::vector<RaycastHit>* hit);

        bool AddRigidbody(const WeakPtr<Interface::IRigidbody>& rigidbody);

    private:
        btVector3 TobtVector3(const Vector3& v);
    };
}
