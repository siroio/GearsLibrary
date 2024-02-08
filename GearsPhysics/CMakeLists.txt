cmake_minimum_required(VERSION 3.4)

# == CONFIG ==
set(SOURCE_FILE
    ./src/Internal/JoltPhysicsManager.cpp
    ./src/Components/Rigidbody.cpp
    ./src/Physics.cpp
)

set(HEADER_FILE
    ./include/Internal/JoltPhysicsManager.h
    ./include/Internal/IRigidbody.h
    ./include/Components/Rigidbody.h
    ./include/Physics.h
)

set(LIB_NAME GearsPhysics)
create_filter()

add_library(${LIB_NAME} STATIC ${SOURCE_FILE} ${HEADER_FILE})
target_compile_features(${LIB_NAME} PRIVATE cxx_std_20)
target_include_directories(
    ${LIB_NAME}
    PUBLIC
    ./include
    ./Modules/JoltPhysics/Jolt/..
)

target_link_libraries(
    ${LIB_NAME}
    GearsCore
    GearsObject
    GearsUtility
    GearsDebug
    Jolt
)

set_target_properties(${LIB_NAME}
    PROPERTIES
    VERSION ${PROJECT_VERSION})
