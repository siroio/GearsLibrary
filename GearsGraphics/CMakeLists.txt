cmake_minimum_required(VERSION 3.1)

# == CONFIG ==
set(SOURCE_FILE 
	./src/Internal/DirectX12.cpp
	./src/Internal/DescriptorPool.cpp
    ./src/Internal/RenderTarget.cpp
    ./src/Window.cpp
)

set(HEADER_FILE 
	./include/Internal/DirectX12.h
    ./include/Internal/d3dx12Inc.h
    ./include/Internal/ConstantBuffer.h
    ./include/Internal/IndexBuffer.h
    ./include/Internal/VertexBuffer.h
    ./include/Internal/DescriptorPool.h
    ./include/Internal/RenderTarget.h
    ./include/Window.h
)

set(LIB_NAME GearsGraphics)
create_filter()

add_library(${LIB_NAME} STATIC ${SOURCE_FILE} ${HEADER_FILE})
target_compile_features(${LIB_NAME} PRIVATE cxx_std_20)
target_include_directories(${LIB_NAME} PUBLIC ./include)
target_link_libraries(${LIB_NAME} GearsCore GearsMath GearsUtility DirectXTex)
set_target_properties(${LIB_NAME}
    PROPERTIES
    VERSION ${PROJECT_VERSION})
