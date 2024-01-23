cmake_minimum_required(VERSION 3.4)

# == CONFIG ==
set(SOURCE_FILE 
	./src/Internal/EffekseerManager.cpp
	./src/EffectManager.cpp
	./src/Components/EffectSystem.cpp
)

set(HEADER_FILE
    ./include/Internal/EffekseerManager.h
    ./include/EffectManager.h
    ./include/Components/EffectSystem.h
)

set(LIB_NAME GearsEffect)
create_filter()

add_library(${LIB_NAME} STATIC ${SOURCE_FILE} ${HEADER_FILE})
target_compile_features(${LIB_NAME} PRIVATE cxx_std_20)
target_include_directories(
    ${LIB_NAME}
    PUBLIC
    ./include
    ${CMAKE_SOURCE_DIR}/Modules/Effekseer/Dev/Cpp/Effekseer
    ${CMAKE_SOURCE_DIR}/Modules/Effekseer/Dev/Cpp/EffekseerRendererDX12
)

target_link_libraries(
    ${LIB_NAME}
    GearsCore
    GearsMath
    GearsUtility
    Effekseer
    EffekseerRendererDX12
)

set_target_properties(${LIB_NAME}
    PROPERTIES
    VERSION ${PROJECT_VERSION}
)
