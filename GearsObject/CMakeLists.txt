cmake_minimum_required(VERSION 3.1)

# == CONFIG ==
set(SOURCE_FILE
    ./src/GameObjectManager.cpp
    ./src/GameObject.cpp
)

set(HEADER_FILE
    ./include/Internal/IGameObject.h
    ./include/GameObjectPtr.h
	./include/GameObjectManager.h
	./include/GameObject.h
)

set(LIB_NAME GearsObject)

add_library(${LIB_NAME} STATIC ${SOURCE_FILE} ${HEADER_FILE})
target_compile_features(${LIB_NAME} PRIVATE cxx_std_20)
target_include_directories(${LIB_NAME} PUBLIC ./include)
target_link_libraries(${LIB_NAME} GearsCore GearsComponent GearsUtility)
set_target_properties(${LIB_NAME}
    PROPERTIES
    VERSION ${PROJECT_VERSION})
