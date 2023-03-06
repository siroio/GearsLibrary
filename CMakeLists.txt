cmake_minimum_required(VERSION 3.1)

project(GearsLibrary
		VERSION 1.0.0
		DESCRIPTION "DirectX12 Game Library"
		LANGUAGES CXX)

# == CONFIG ==
set(LIBRARY GearsMath GearsUtility)

add_executable(GearsLibrary main.cpp)
add_subdirectory(./GearsMath)
add_subdirectory(./GearsUtility)

target_compile_features(GearsLibrary PRIVATE cxx_std_20)
target_link_libraries(GearsLibrary ${LIBRARY})