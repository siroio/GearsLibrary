cmake_minimum_required(VERSION 3.4)

# == CONFIG ==
set(SOURCE_FILE
    ./src/Internal/PhysXManager.cpp
    ./src/Internal/PhysicsDebugRenderer.cpp
    ./src/Internal/Geometry.cpp
    ./src/Components/Rigidbody.cpp
    ./src/Components/Collider.cpp
    ./src/Components/BoxCollider.cpp
    ./src/Components/SphereCollider.cpp
    ./src/Components/CapsuleCollider.cpp
    ./src/Components/MeshCollider.cpp
    ./src/Physics.cpp
)

set(HEADER_FILE
    ./include/Internal/PhysXManager.h
    ./include/Internal/PhysicsDebugRenderer.h
    ./include/Internal/PhysicsDebugRenderer.h
    ./include/Internal/IRigidbody.h
    ./include/Internal/ICollider.h
    ./include/Internal/Geometry.h
    ./include/Components/Rigidbody.h
    ./include/Components/Collider.h
    ./include/Components/BoxCollider.h
    ./include/Components/SphereCollider.h
    ./include/Components/CapsuleCollider.h
    ./include/Components/MeshCollider.h
    ./include/Physics.h
    ./include/RaycastHit.h
)

set(LIB_NAME GearsPhysics)
create_filter()

add_library(${LIB_NAME} STATIC ${SOURCE_FILE} ${HEADER_FILE})
target_compile_features(${LIB_NAME} PRIVATE cxx_std_20)
target_include_directories(
    ${LIB_NAME}
    PUBLIC
    ./include
    ${PHYSX_ROOT_DIR}/include
)

set(PHYSX_LIB_DIR ${PHYSX_ROOT_DIR}/bin/win.x86_64.vc143.mt)

target_link_libraries(${LIB_NAME}
    debug ${PHYSX_LIB_DIR}/debug/PhysX_64.lib
    debug ${PHYSX_LIB_DIR}/debug/PhysXCommon_64.lib
    debug ${PHYSX_LIB_DIR}/debug/PhysXCooking_64.lib
    debug ${PHYSX_LIB_DIR}/debug/PhysXExtensions_static_64.lib
    debug ${PHYSX_LIB_DIR}/debug/PhysXFoundation_64.lib
)

target_link_libraries(${LIB_NAME} 
    optimized ${PHYSX_LIB_DIR}/release/PhysX_64.lib
    optimized ${PHYSX_LIB_DIR}/release/PhysXCommon_64.lib
    optimized ${PHYSX_LIB_DIR}/release/PhysXCooking_64.lib
    optimized ${PHYSX_LIB_DIR}/release/PhysXExtensions_static_64.lib
    optimized ${PHYSX_LIB_DIR}/release/PhysXFoundation_64.lib
)

target_link_libraries(
    ${LIB_NAME}
    GearsCore
    GearsObject
    GearsUtility
    GearsDebug
)

set_target_properties(${LIB_NAME}
    PROPERTIES
    VERSION ${PROJECT_VERSION}
)
