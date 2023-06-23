cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 20)
option(BUILD_DOCUMENTATION "Build Doxygen documentation" ON)

project(GearsLibrary
		VERSION 1.0.0
		DESCRIPTION "DirectX12 Game Library"
		LANGUAGES CXX
)

# == CONFIG ==

set(LIBRARY 
    GearsCore
    GearsGraphics
    GearsObject
    GearsComponent
    GearsScene
    GearsMath
    GearsUtility
    DirectXTex
)
set(DirectXTexDir ${CMAKE_CURRENT_SOURCE_DIR}/DirectXTex)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# .editorconfigのコピー
foreach(lib ${LIBRARY})
    set(library_path "${CMAKE_CURRENT_BINARY_DIR}/${lib}")
    if(EXISTS ${library_path} AND IS_DIRECTORY ${library_path})
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/.editorconfig ${library_path}/.editorconfig COPYONLY)
    endif()
endforeach()

# 関数を定義する
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

set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Lib)
add_definitions(-UNICODE -D_UNICODE)

add_executable(GearsLibrary main.cpp)
add_subdirectory(./GearsCore)
add_subdirectory(./GearsGraphics)
add_subdirectory(./GearsObject)
add_subdirectory(./GearsComponent)
add_subdirectory(./GearsScene)
add_subdirectory(./GearsMath)
add_subdirectory(./GearsUtility)
add_subdirectory(${DirectXTexDir} EXCLUDE_FROM_ALL)
target_compile_features(GearsLibrary PRIVATE cxx_std_20)
target_link_libraries(GearsLibrary ${LIBRARY})

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
