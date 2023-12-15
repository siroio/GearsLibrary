cmake_minimum_required(VERSION 3.1)

# == CONFIG ==
set(SOURCE_FILE
    ./src/mmd/PmxObject.cpp
)

set(HEADER_FILE
    ./include/mcMath.h
    ./include/IConverter.h
    ./include/Converter.h
    ./include/mmd/PmxObject.h
)

set(EXECUTABLE_NAME ModelConverter)
create_filter()

add_executable(${EXECUTABLE_NAME} ${HEADER_FILE} ${SOURCE_FILE} src/main.cpp)
target_include_directories(${EXECUTABLE_NAME} PRIVATE ./include)
target_compile_features(${EXECUTABLE_NAME} PRIVATE cxx_std_20)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE Gears3DObject)
