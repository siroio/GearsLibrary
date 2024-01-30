cmake_minimum_required(VERSION 3.4)
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
endfunction()

macro(create_filter)
    file(GLOB_RECURSE HEADER_FILES "include/*.h" "include/*.hpp")
    file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
    create_groups(HEADER_FILES)
    create_groups(SOURCE_FILES)
endmacro()

# == グローバル設定 ==
set(LIBRARY
    Gears3DObject
    GearsAudio
    GearsCore
    GearsComponent
    GearsDebug
    GearsEffect
    GearsGraphics
    GearsInput
    GearsMath
    GearsObject
    GearsPhysics
    GearsScene
    GearsUtility
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

# Supress Bullet Warning
cmake_policy(SET CMP0048 NEW)
cmake_policy(SET CMP0115 NEW)

# Setup Effekseer
set(BUILD_EXAMPLES OFF)
set(BUILD_GL OFF)

# Libフォルダが存在しない場合は作成
set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Lib)
if(NOT EXISTS ${LIBRARY_OUTPUT_DIRECTORY})
    file(MAKE_DIRECTORY ${LIBRARY_OUTPUT_DIRECTORY})
endif()

# nlohmann_json ビルドテストをオフ
set(JSON_BuildTests OFF CACHE INTERNAL "")

# 文字コードをUNICODEに設定
# add_definitions(-UNICODE -D_UNICODE)

# UTF8に変更
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# ランタイムライブラリの設定
if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
endif()

# メインの実行可能ファイル
add_executable(${PROJECT_NAME} WIN32 main.cpp $<$<CXX_COMPILER_ID:MSVC>:utf8api.manifest>)

set(LIB_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(MODULE_ROOT ${LIB_ROOT}/Modules)

# サブディレクトリの追加
add_subdirectory(./Modules/DirectXTex)
add_subdirectory(./Modules/json)
add_subdirectory(./Modules/Effekseer)
add_subdirectory(./Modules/Bullet)

add_subdirectory(./Gears3DObject)
add_subdirectory(./GearsAudio)
add_subdirectory(./GearsComponent)
add_subdirectory(./GearsCore)
add_subdirectory(./GearsDebug)
add_subdirectory(./GearsEffect)
add_subdirectory(./GearsGraphics)
add_subdirectory(./GearsInput)
add_subdirectory(./GearsMath)
add_subdirectory(./GearsObject)
add_subdirectory(./GearsPhysics)
add_subdirectory(./GearsScene)
add_subdirectory(./GearsUtility)

# ツール類のサブディレクトリの追加
add_subdirectory(./ModelConverter)

# メインの実行可能ファイルの設定
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBRARY} ${DIRECTX12_LIBS} ${EXTERNAL_LIBS})

# プロジェクトのフィルタ設定
# DirectXTex
set_target_properties(DirectXTex PROPERTIES FOLDER Modules/DirectXTex)
set_target_properties(texassemble PROPERTIES FOLDER Modules/DirectXTex)
set_target_properties(texconv PROPERTIES FOLDER Modules/DirectXTex)
set_target_properties(texdiag PROPERTIES FOLDER Modules/DirectXTex)
set_target_properties(ddsview PROPERTIES FOLDER Modules/DirectXTex)

# Effekser
set_target_properties(
    Effekseer
    EffekseerRendererDX9
    EffekseerRendererDX11
    EffekseerRendererDX12
    EffekseerSoundDSound
    EffekseerSoundXAudio2
    LLGI
    PROPERTIES FOLDER Modules/Effekseer
)

# Bullet
set_target_properties(
    App_BasicExample
    App_ExampleBrowser
    App_HelloWorld
    App_HelloBulletRobotics
    App_obj2sdf
    App_PhysicsServer_SharedMemory
    App_PhysicsServer_SharedMemory_GUI
    App_RobotSimulator
    App_RobotSimulator_NoGUI
    App_TwoJoint
    AppBasicExampleGui
    BulletXmlWorldImporter
    Bullet2FileLoader
    Bullet3Collision
    Bullet3Common
    Bullet3Dynamics
    Bullet3Geometry
    Bullet3OpenCL_clew
    BulletCollision
    BulletDynamics
    BulletExampleBrowserLib
    BulletFileLoader
    BulletInverseDynamics
    BulletInverseDynamicsUtils
    BulletRobotics
    BulletRoboticsGUI
    BulletSoftBody
    BulletWorldImporter
    BussIK
    clsocket
    ConvexDecomposition
    GIMPACTUtils
    gtest
    gwen
    HACD
    LinearMath
    OpenGLWindow
    Test_btKinematicCharacterController
    Test_BulletDynamics
    Test_BulletInverseDynamics
    Test_BulletInverseDynamicsJacobian
    Test_BulletInverseForwardDynamics
    Test_Collision
    Test_PhysicsClientServer
    PROPERTIES FOLDER Modules/Bullet
)

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

file(GLOB_RECURSE LIB_HEADERS "include/*.h" "include/*.hpp")
foreach(header ${LIB_HEADERS})
    get_filename_component(header_path ${header} DIRECTORY)
    get_filename_component(relative_path ${header_path} RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    set(destination_path ${CMAKE_BINARY_DIR}/Lib/Include/${relative_path})
    file(MAKE_DIRECTORY ${destination_path})
    configure_file(${header} ${destination_path} COPYONLY)
endforeach()
