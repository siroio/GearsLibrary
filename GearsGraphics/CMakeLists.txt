cmake_minimum_required(VERSION 3.1)

# == CONFIG ==
set(SOURCE_FILE 
	./src/Internal/DX12/DirectX12.cpp
	./src/Internal/DX12/CommandList.cpp
    ./src/Internal/DX12/BlendState.cpp
    ./src/Internal/DX12/GraphicsPipeline.cpp
	./src/Internal/DX12/DescriptorHandle.cpp
	./src/Internal/DX12/DescriptorPool.cpp
    ./src/Internal/DX12/ConstantBuffer.cpp
    ./src/Internal/DX12/IndexBuffer.cpp
    ./src/Internal/DX12/VertexBuffer.cpp
    ./src/Internal/DX12/GraphicsResource.cpp
    ./src/Internal/DX12/Shader/Shader.cpp
    ./src/Internal/DX12/Shader/ShaderManager.cpp
    ./src/Internal/CameraManager.cpp
    ./src/Internal/RenderingManager.cpp
    ./src/Internal/CanvasManager.cpp
    ./src/Components/Camera.cpp
    ./src/Components/Canvas.cpp
    ./src/Components/Image.cpp
    ./src/TextureManager.cpp
    ./src/Texture.cpp
    ./src/RenderTarget.cpp
    ./src/Window.cpp
)

set(HEADER_FILE
    ./include/Internal/DX12/d3dx12Inc.h
	./include/Internal/DX12/DirectX12.h
	./include/Internal/DX12/CommandList.h
    ./include/Internal/DX12/BlendState.h
    ./include/Internal/DX12/GraphicsPipeline.h
    ./include/Internal/DX12/DescriptorHandle.h
    ./include/Internal/DX12/DescriptorPool.h
    ./include/Internal/DX12/ConstantBuffer.h
    ./include/Internal/DX12/IndexBuffer.h
    ./include/Internal/DX12/VertexBuffer.h
    ./include/Internal/DX12/InputLayout.h
    ./include/Internal/DX12/GraphicsResource.h
    ./include/Internal/DX12/GraphicsResourceID.h
    ./include/Internal/DX12/Shader/ShaderType.h
    ./include/Internal/DX12/Shader/Shader.h
    ./include/Internal/DX12/Shader/ShaderManager.h
    ./include/Internal/DX12/ShaderCode/CameraShader.h
    ./include/Internal/DX12/ShaderCode/GaussianBlurShader.h
    ./include/Internal/DX12/ShaderCode/ImageShader.h
    ./include/Internal/DX12/ShaderCode/LineShader.h
    ./include/Internal/DX12/ShaderCode/MeshShader.h
    ./include/Internal/DX12/ShaderCode/SkinnedMeshShader.h
    ./include/Internal/DX12/ShaderCode/SkyboxShader.h
    ./include/Internal/DX12/ShaderCode/SpriteShader.h
    ./include/Internal/ICamera.h
    ./include/Internal/CameraBase.h
    ./include/Internal/CameraManager.h
    ./include/Internal/RenderingManager.h
    ./include/Internal/CanvasManager.h
    ./include/Internal/UIRenderer.h
    ./include/Components/Camera.h
    ./include/Components/Canvas.h
    ./include/Components/Image.h
    ./include/TextureManager.h
    ./include/Texture.h
    ./include/RenderTarget.h
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
