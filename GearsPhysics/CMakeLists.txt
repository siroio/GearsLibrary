cmake_minimum_required(VERSION 3.4)

# == CONFIG ==
set(SOURCE_FILE
    ./src/Internal/BulletPhysicsManager.cpp
)

set(HEADER_FILE
    ./include/Internal/BulletPhysicsManager.h
)

set(LIB_NAME GearsPhysics)
create_filter()

add_library(${LIB_NAME} STATIC ${SOURCE_FILE} ${HEADER_FILE})
target_compile_features(${LIB_NAME} PRIVATE cxx_std_20)
target_include_directories(
    ${LIB_NAME}
    PUBLIC
    ./include
    ${CMAKE_SOURCE_DIR}/Modules/Bullet/src
)
target_link_libraries(
    ${LIB_NAME}
    GearsCore
    GearsObject
    GearsUtility
    GearsDebug
    LinearMath
    Bullet3Common
    BulletCollision
    BulletDynamics
    BulletSoftBody
    BulletInverseDynamics
)
set_target_properties(${LIB_NAME}
    PROPERTIES
    VERSION ${PROJECT_VERSION})
