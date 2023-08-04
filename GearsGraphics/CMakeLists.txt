cmake_minimum_required(VERSION 3.1)

# == CONFIG ==
set(SOURCE_FILE 
	./src/DX12/Internal/DirectX12.cpp
	./src/DX12/Internal/CommandList.cpp
    ./src/DX12/Internal/GraphicsPipeline.cpp
	./src/DX12/Internal/DescriptorPool.cpp
    ./src/DX12/Internal/ConstantBuffer.cpp
    ./src/DX12/Internal/IndexBuffer.cpp
    ./src/DX12/Internal/VertexBuffer.cpp
    ./src/DX12/Internal/BlendState.cpp
    ./src/DX12/RenderTarget.cpp
    ./src/DX12/GraphicsResource.cpp
    ./src/Window.cpp
)

set(HEADER_FILE
    ./include/DX12/Internal/d3dx12Inc.h
	./include/DX12/Internal/DirectX12.h
	./include/DX12/Internal/CommandList.h
    ./include/DX12/Internal/GraphicsPipeline.h
    ./include/DX12/Internal/DescriptorPool.h
    ./include/DX12/Internal/ConstantBuffer.h
    ./include/DX12/Internal/IndexBuffer.h
    ./include/DX12/Internal/VertexBuffer.h
    ./include/DX12/Internal/BlendState.h
    ./include/DX12/RenderTarget.h
    ./include/DX12/GraphicsResource.h
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
