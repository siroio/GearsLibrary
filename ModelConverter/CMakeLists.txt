cmake_minimum_required(VERSION 3.4)

# == CONFIG ==
set(SOURCE_FILE
    ./src/mmd/PmxObject.cpp
    ./src/mmd/VmdObject.cpp
    ./src/utility/FileUtility.cpp
)

set(HEADER_FILE
    ./include/mcMath.h
    ./include/IConverter.h
    ./include/Converter.h
    ./include/mmd/PmxObject.h
    ./include/mmd/VmdObject.h
    ./include/utility/FileUtility.h
)

set(ENTRY_POINT ./src/main.cpp)

set(EXECUTABLE_NAME ModelConverter)
create_filter()

add_executable(${EXECUTABLE_NAME} ${HEADER_FILE} ${SOURCE_FILE} ${ENTRY_POINT})
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
target_include_directories(${EXECUTABLE_NAME} PRIVATE ./include)
target_compile_features(${EXECUTABLE_NAME} PRIVATE cxx_std_20)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE Gears3DObject)
