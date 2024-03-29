cmake_minimum_required(VERSION 3.4)

# == CONFIG ==
set(SOURCE_FILE 
    ./src/Internal/MemoryCheck.cpp
    ./src/SystemManager.cpp
    ./src/GameTimer.cpp
    ./src/Game.cpp
)

set(HEADER_FILE
    ./include/Internal/IFunc.h
    ./include/Internal/FuncOrder.h
    ./include/Internal/HasFunctionObject.h
    ./include/Internal/ISystem.h
    ./include/Internal/SystemFunctionType.h
    ./include/Internal/FuncOrderConstant.h
    ./include/Internal/MemoryCheck.h
    ./include/FuncOrderDefinition.h
    ./include/FuncDefinition.h
    ./include/SystemManager.h
    ./include/GameTimer.h
    ./include/Game.h
    ./include/Glib.h
)

set(LIB_NAME GearsCore)
create_filter()

add_library(${LIB_NAME} STATIC ${SOURCE_FILE} ${HEADER_FILE})
target_compile_features(${LIB_NAME} PRIVATE cxx_std_20)
target_include_directories(${LIB_NAME} PUBLIC ./include)

target_link_libraries(
    ${LIB_NAME}
    GearsComponent
    GearsObject
    GearsGraphics
    GearsInput
    GearsAudio
    GearsEffect
    GearsScene
    GearsUtility
    GearsPhysics
)
set_target_properties(${LIB_NAME}
    PROPERTIES
    VERSION ${PROJECT_VERSION})
