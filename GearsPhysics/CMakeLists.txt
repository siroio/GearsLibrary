cmake_minimum_required(VERSION 3.4)

# == CONFIG ==
set(SOURCE_FILE
    ./src/Internal/PhysXManager.cpp
    ./src/Internal/PhysicsDebugRenderer.cpp
    ./src/Internal/Geometry.cpp
    ./src/Components/Rigidbody.cpp
    ./src/Components/Collider.cpp
    ./src/Components/BoxCollider.cpp
    ./src/Components/CapsuleCollider.cpp
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
    ./include/Components/CapsuleCollider.h
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

target_link_directories(
    ${LIB_NAME}
    PUBLIC
    ${PHYSX_ROOT_DIR}/bin/win.x86_64.vc143.mt/debug
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
