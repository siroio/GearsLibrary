cmake_minimum_required(VERSION 3.1)

# == CONFIG ==
set(SOURCE_FOLDER 
	./src/Internal/ComponentManager.cpp
    ./src/Component.cpp
    ./src/Transform.cpp
)

set(HEADER_FOLDER 
    ./include/Internal/IComponent.h
    ./include/Internal/ComponentFunctionType.h
    ./include/Internal/ComponentManager.h
    ./include/Component.h
    ./include/Transform.h
)

set(LIB_NAME GearsComponent)

add_library(${LIB_NAME} STATIC ${SOURCE_FOLDER} ${HEADER_FOLDER})
target_compile_features(${LIB_NAME} PRIVATE cxx_std_20)
target_include_directories(${LIB_NAME} PUBLIC ./include)
target_link_libraries(${LIB_NAME} GearsObject GearsMath GearsUtility)
set_target_properties(${LIB_NAME}
    PROPERTIES
    VERSION ${PROJECT_VERSION})
