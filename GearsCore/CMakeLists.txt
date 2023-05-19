cmake_minimum_required(VERSION 3.1)

# == CONFIG ==
set(SOURCE_FOLDER 
    ./src/SystemManager.cpp
	./src/Game.cpp
)

set(HEADER_FOLDER
    ./include/Internal/IOrderFunc.h
    ./include/Internal/FUNC_ORDER.h
    ./include/Internal/HasFunction.h
    ./include/Internal/HasFunctionObject.h
    ./include/Internal/ISystem.h
    ./include/FUNC_ORDERDefinition.h
    ./include/FuncDefinition.h
    ./include/SystemManager.h
	./include/Game.h
)

set(LIB_NAME GearsCore)

add_library(${LIB_NAME} STATIC ${SOURCE_FOLDER} ${HEADER_FOLDER})
target_compile_features(${LIB_NAME} PRIVATE cxx_std_20)
target_include_directories(${LIB_NAME} PUBLIC ./include)
target_link_libraries(${LIB_NAME} GearsUtility)
set_target_properties(${LIB_NAME}
    PROPERTIES
    VERSION ${PROJECT_VERSION})
