cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 20)

option(BUILD_DOCUMENTATION "Build Doxygen documentation" ON)

# グローバルプロジェクトの設定
project(GearsLibrary
        VERSION 1.0.0
        DESCRIPTION "DirectX12 Game Library"
        LANGUAGES CXX
)

# == 関数とマクロの定義 ==


function(create_groups FILES_VAR)
    foreach(file ${${FILES_VAR}})
        foreach(dir ${CMAKE_CURRENT_LIST_DIR})
            # ファイルの相対パスを計算
            file(RELATIVE_PATH relative ${CMAKE_CURRENT_LIST_DIR} ${file})

            # 相対パスを階層ごとに分割
            string(REGEX MATCHALL "[^/]+" filters "${relative}")
            list(LENGTH filters output_length)
            math(EXPR iteration "${output_length} - 2")

            # ルートディレクトリの場合は処理をスキップ
            if(${iteration} LESS 0)
                continue()
            endif()

            set(group_string "")
            foreach(i RANGE 0 ${iteration})
                list(GET filters ${i} g)
                set(group_string "${group_string}\\${g}")
            endforeach()

            # SOURCE_GROUPを使用してフィルタを作成
            source_group("${group_string}" FILES ${file})
        endforeach()
    endforeach()
endfunction()

macro(create_filter)
    file(GLOB_RECURSE HEADER_FILES "include/*.h" "include/*.hpp")
    file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
    create_groups(HEADER_FILES)
    create_groups(SOURCE_FILES)
endmacro()

# == グローバル設定 ==

set(LIBRARY 
    GearsCore
    GearsGraphics
    GearsObject
    GearsComponent
    GearsInput
    GearsScene
    GearsMath
    GearsUtility
    GearsDebug
)

set(DIRECTX12_LIBS
    d3d12
    d3dcompiler
    dxgi
    dxguid
    dinput8
    xinput
    xaudio2
)

set(EXTERNAL_LIBS
    DirectXTex
    nlohmann_json
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# .editorconfigのコピー
foreach(lib ${LIBRARY})
    set(library_path "${CMAKE_CURRENT_BINARY_DIR}/${lib}")
    if(EXISTS ${library_path} AND IS_DIRECTORY ${library_path})
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/.editorconfig ${library_path}/.editorconfig COPYONLY)
    endif()
endforeach()

# == プロジェクト構成 ==

# Libフォルダが存在しない場合は作成
set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Lib)
if(NOT EXISTS ${LIBRARY_OUTPUT_DIRECTORY})
    file(MAKE_DIRECTORY ${LIBRARY_OUTPUT_DIRECTORY})
endif()

# nlohmann_json ビルドテストをオフ
set(JSON_BuildTests OFF CACHE INTERNAL "")

# 文字コードをUNICODEに設定
add_definitions(-UNICODE -D_UNICODE)

# メインの実行可能ファイル
add_executable(GearsLibrary main.cpp)

# サブディレクトリの追加
add_subdirectory(./GearsCore)
add_subdirectory(./GearsGraphics)
add_subdirectory(./GearsObject)
add_subdirectory(./GearsComponent)
add_subdirectory(./GearsScene)
add_subdirectory(./GearsMath)
add_subdirectory(./GearsInput)
add_subdirectory(./GearsUtility)
add_subdirectory(./GearsDebug)
add_subdirectory(./Modules/DirectXTex)
add_subdirectory(./Modules/json)

# メインの実行可能ファイルの設定
target_compile_features(GearsLibrary PRIVATE cxx_std_20)
target_link_libraries(GearsLibrary ${LIBRARY} ${DIRECTX12_LIBS} ${EXTERNAL_LIBS})

# imgui 設定
set(IMGUI_SOURCE ./Modules/ImGui)
set(IMGUI_BACKEND ${IMGUI_SOURCE}/backends)
target_include_directories(GearsDebug PRIVATE ${IMGUI_SOURCE} ${IMGUI_BACKEND})

# プロジェクトのフィルタ設定
set_target_properties(DirectXTex PROPERTIES FOLDER Modules)
set_target_properties(texassemble PROPERTIES FOLDER ExternalTools)
set_target_properties(texconv PROPERTIES FOLDER ExternalTools)
set_target_properties(texdiag PROPERTIES FOLDER ExternalTools)
set_target_properties(ddsview PROPERTIES FOLDER ExternalTools)

# ドキュメントのビルドオプション
if(BUILD_DOCUMENTATION)
    find_package(Doxygen REQUIRED)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    add_custom_target(Docs ALL COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Docs
                      COMMENT "Generating Libraray documentation with Doxygen"
                      VERBATIM)
endif()

# Libフォルダに.libファイルを出力する
foreach(lib ${LIBRARY} ${EXTERNAL_LIBS})
    if(TARGET ${lib})
        set_target_properties(${lib} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_DIRECTORY})
        set_target_properties(${lib} PROPERTIES DEBUG_POSTFIX d)
    endif()
endforeach()
