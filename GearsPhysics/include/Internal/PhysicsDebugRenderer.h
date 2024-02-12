#pragma once
#include <Internal/ISystem.h>
#include <Singleton.h>
#include <FuncOrderDefinition.h>
#include <Internal/FuncOrderConstant.h>
#include <memory>

struct Vector3;
struct Color;

namespace Glib::Internal::Physics
{
    class PhysicsDebugRenderer :
        public Interface::ISystem,
        public SingletonPtr<PhysicsDebugRenderer>,
        public Function::DrawOrderSet<Order::Draw::PHYSICS>
    {
    public:
        bool Initialize();
        void Draw();
        void AddVertex(const Vector3& position, const Color& color);

    private:
    };
}
