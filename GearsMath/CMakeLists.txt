cmake_minimum_required(VERSION 3.1)

# == CONFIG ==
set(SOURCE_FOLDER 
    ./src/Mathf.cpp
    ./src/Vector2.cpp 
    ./src/Vector3.cpp
    ./src/Vector4.cpp
    ./src/Matrix4x4.cpp
    ./src/Color.cpp
    ./src/Random.cpp
)

set(HEADER_FOLDER 
    ./include/Mathf.h
    ./include/Vector2.h
    ./include/Vector3.h
    ./include/Vector4.h
    ./include/Matrix4x4.h
    ./include/Color.h
    ./include/Random.h
)

set(LIB_NAME GearsMath)

add_library(${LIB_NAME} STATIC ${SOURCE_FOLDER} ${HEADER_FOLDER})
target_compile_features(${LIB_NAME} PRIVATE cxx_std_20)
target_include_directories(${LIB_NAME} PUBLIC ./include)
target_link_libraries(${LIB_NAME} GearsUtility)
set_target_properties(${LIB_NAME}
    PROPERTIES
    VERSION ${PROJECT_VERSION})
