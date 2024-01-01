cmake_minimum_required(VERSION 3.4)

# == CONFIG ==
set(SOURCE_FILE
    ./src/Internal/ComponentFunctionList.cpp
    ./src/Internal/ComponentEventFunctionList.cpp
	./src/Internal/ComponentManager.cpp
    ./src/Component.cpp
    ./src/Components/Transform.cpp
)

set(HEADER_FILE
    ./include/Internal/IComponent.h
    ./include/Internal/ComponentFunctionInfo.h
    ./include/Internal/ComponentFunctionList.h
    ./include/Internal/ComponentEventFunctionList.h
    ./include/Internal/ComponentFunctionType.h
    ./include/Internal/ComponentManager.h
    ./include/Component.h
    ./include/Components/Transform.h
)

set(LIB_NAME GearsComponent)
create_filter()

add_library(${LIB_NAME} STATIC ${SOURCE_FILE} ${HEADER_FILE})
target_compile_features(${LIB_NAME} PRIVATE cxx_std_20)
target_include_directories(${LIB_NAME} PUBLIC ./include)
target_link_libraries(${LIB_NAME} GearsObject GearsMath GearsUtility)
set_target_properties(${LIB_NAME}
    PROPERTIES
    VERSION ${PROJECT_VERSION})
