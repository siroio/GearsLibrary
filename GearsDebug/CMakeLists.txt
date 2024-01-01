cmake_minimum_required(VERSION 3.4)

# == CONFIG ==
set(SOURCE_FILE
    ./src/Internal/ImGuiManager.cpp
    ./src/GLGUI.cpp
    ./src/Debugger.cpp
)

set(HEADER_FILE
    ./include/Internal/ImGuiInc.h
    ./include/Internal/ImGuiManager.h
    ./include/Internal/ImGuiFont.h
	./include/GLGUI.h
	./include/Debugger.h
)

set(LIB_NAME GearsDebug)

# Setup imgui
set(IMGUI ${CMAKE_CURRENT_SOURCE_DIR}../../Modules/ImGui)
set(IMGUI_BACKEND ${IMGUI}/backends)
set(IMGUI_HEADERS
    ${IMGUI}/imconfig.h
    ${IMGUI}/imgui.h
    ${IMGUI}/imgui_internal.h
    ${IMGUI}/imstb_rectpack.h
    ${IMGUI}/imstb_textedit.h
    ${IMGUI}/imstb_truetype.h
    ${IMGUI_BACKEND}/imgui_impl_dx12.h
    ${IMGUI_BACKEND}/imgui_impl_win32.h
)

set(IMGUI_SOURCES
    ${IMGUI}/imgui.cpp
    ${IMGUI}/imgui_demo.cpp
    ${IMGUI}/imgui_draw.cpp
    ${IMGUI}/imgui_tables.cpp
    ${IMGUI}/imgui_widgets.cpp
    ${IMGUI_BACKEND}/imgui_impl_dx12.cpp
    ${IMGUI_BACKEND}/imgui_impl_win32.cpp
)

source_group("imgui" FILES ${IMGUI_HEADERS})
source_group("imgui" FILES ${IMGUI_SOURCES})
create_filter()

add_library(${LIB_NAME} STATIC ${SOURCE_FILE} ${HEADER_FILE} ${IMGUI_HEADERS} ${IMGUI_SOURCES})
target_compile_features(${LIB_NAME} PRIVATE cxx_std_20)
target_include_directories(${LIB_NAME} PUBLIC ./include ${IMGUI} ${IMGUI_BACKENDS})
target_link_libraries(${LIB_NAME} GearsCore GearsGraphics GearsUtility)
set_target_properties(${LIB_NAME}
    PROPERTIES
    VERSION ${PROJECT_VERSION})
