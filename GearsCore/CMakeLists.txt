cmake_minimum_required(VERSION 3.1)

# == CONFIG ==
set(SOURCE_FILE 
    ./src/SystemManager.cpp
    ./src/GameTimer.cpp
    ./src/Window.cpp
    ./src/Game.cpp
)

set(HEADER_FILE
    ./include/Internal/IFunc.h
    ./include/Internal/FuncOrder.h
    ./include/Internal/HasFunction.h
    ./include/Internal/HasFunctionObject.h
    ./include/Internal/ISystem.h
    ./include/Internal/SystemFunctionType.h
    ./include/Internal/FuncOrderConstant.h
    ./include/FuncOrderDefinition.h
    ./include/FuncDefinition.h
    ./include/SystemManager.h
    ./include/GameTimer.h
    ./include/Window.h
    ./include/Game.h
)

set(LIB_NAME GearsCore)

add_library(${LIB_NAME} STATIC ${SOURCE_FILE} ${HEADER_FILE})
target_compile_features(${LIB_NAME} PRIVATE cxx_std_20)
target_include_directories(${LIB_NAME} PUBLIC ./include)
target_link_libraries(${LIB_NAME} GearsObject GearsComponent GearsScene GearsUtility)
set_target_properties(${LIB_NAME}
    PROPERTIES
    VERSION ${PROJECT_VERSION})