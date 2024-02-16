cmake_minimum_required(VERSION 3.4)

# == CONFIG ==
set(SOURCE_FILE
    ./src/mmd/PmxObject.cpp
    ./src/mmd/VmdObject.cpp
    ./src/utility/FileUtility.cpp
    ./src/external/mikktspace.c
)

set(HEADER_FILE
    ./include/mcMath.h
    ./include/IConverter.h
    ./include/Converter.h
    ./include/mmd/PmxObject.h
    ./include/mmd/VmdObject.h
    ./include/utility/FileUtility.h
    ./include/external/mikktspace.h

)

set(ENTRY_POINT ./src/main.cpp)

set(EXECUTABLE_NAME ModelConverter)
create_filter()

add_executable(${EXECUTABLE_NAME} ${HEADER_FILE} ${SOURCE_FILE} ${ENTRY_POINT})

# MSVC—p
if(MSVC)
    add_compile_options("/Zc:char8_t-")
    target_sources(${EXECUTABLE_NAME} PRIVATE ./src/FixCHCP.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../utf8api.manifest)
else()
    add_compile_options("-fno-char8_t")
endif()

target_include_directories(${EXECUTABLE_NAME} PRIVATE ./include)
target_compile_features(${EXECUTABLE_NAME} PRIVATE cxx_std_20)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE Gears3DObject)
