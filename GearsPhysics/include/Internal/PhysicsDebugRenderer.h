#pragma once
#include <Internal/ISystem.h>
#include <Singleton.h>
#include <FuncOrderDefinition.h>
#include <Internal/FuncOrderConstant.h>
#include <Jolt/Jolt.h>
#include <Jolt/Renderer/DebugRenderer.h>
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
    private:
        class InternalRenderer : public JPH::DebugRenderer
        {
            virtual void DrawLine(JPH::RVec3Arg inFrom, JPH::RVec3Arg inTo, JPH::ColorArg inColor);
            void DrawTriangle(JPH::RVec3Arg inV1, JPH::RVec3Arg inV2, JPH::RVec3Arg inV3, JPH::ColorArg inColor, ECastShadow inCastShadow) override;
            Batch CreateTriangleBatch(const Triangle* inTriangles, int inTriangleCount) override;
            Batch CreateTriangleBatch(const Vertex* inVertices, int inVertexCount, const JPH::uint32* inIndices, int inIndexCount) override;
            void DrawGeometry(JPH::RMat44Arg inModelMatrix, const JPH::AABox& inWorldSpaceBounds, float inLODScaleSq, JPH::ColorArg inModelColor, const GeometryRef& inGeometry, ECullMode inCullMode, ECastShadow inCastShadow, EDrawMode inDrawMode) override;
            void DrawText3D(JPH::RVec3Arg inPosition, const std::string_view& inString, JPH::ColorArg inColor, float inHeight) override;
        };

    public:
        bool Initialize();
        void Draw();
        JPH::DebugRenderer& Renderer() const;
        static void AddVertex(const Vector3& position, const Color& color);

    private:
        std::unique_ptr<InternalRenderer> renderer_;
    };
}
