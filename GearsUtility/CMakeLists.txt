cmake_minimum_required(VERSION 3.1)

# == CONFIG ==
set(SOURCE_FILE 
    ./src/CSVLoader.cpp
)

set(HEADER_FILE 
    ./include/Debugging.h
    ./include/WeakPtr.h
    ./include/ComPtr.h
    ./include/Singleton.h
    ./include/StringUtility.h
    ./include/CSVLoader.h
)

set(LIB_NAME GearsUtility)

add_library(${LIB_NAME} STATIC ${SOURCE_FILE} ${HEADER_FILE})
target_compile_features(${LIB_NAME} PRIVATE cxx_std_20)
target_include_directories(${LIB_NAME} PUBLIC ./include)
set_target_properties(${LIB_NAME}
    PROPERTIES
    VERSION ${PROJECT_VERSION})
