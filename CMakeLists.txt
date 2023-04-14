cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 20)
option(BUILD_DOCUMENTATION "Build Doxygen documentation" ON)

project(GearsLibrary
		VERSION 1.0.0
		DESCRIPTION "DirectX12 Game Library"
		LANGUAGES CXX)

# == CONFIG ==
set(LIBRARY 
	GearsMath 
	GearsUtility)

add_executable(GearsLibrary main.cpp)
add_subdirectory(./GearsMath)
add_subdirectory(./GearsUtility)

target_compile_features(GearsLibrary PRIVATE cxx_std_20)
target_link_libraries(GearsLibrary ${LIBRARY})

if(BUILD_DOCUMENTATION)
  find_package(Doxygen REQUIRED)
  set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
  set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
  configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
  add_custom_target(docs ALL COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs
                    COMMENT "Generating API documentation with Doxygen"
                    VERBATIM)
endif()
