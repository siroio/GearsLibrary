cmake_minimum_required(VERSION 3.1)

# == CONFIG ==
set(SOURCE_FILE
    ./src/Internal/DinputDevice.cpp
    ./src/Internal/KeyBoardDevice.cpp
    ./src/Internal/MouseDevice.cpp
    ./src/Internal/GamePad.cpp
    ./src/InputManager.cpp
)

set(HEADER_FILE
    ./include/Internal/InputState.h
    ./include/Internal/InputData.h
    ./include/Internal/DinputDevice.h
    ./include/Internal/KeyBoardDevice.h
    ./include/Internal/MouseDevice.h
    ./include/Internal/MouseButton.h
    ./include/Internal/GamePad.h
    ./include/InputManager.h
)

set(LIB_NAME GearsInput)
create_filter()

add_library(${LIB_NAME} STATIC ${SOURCE_FILE} ${HEADER_FILE})
target_compile_features(${LIB_NAME} PRIVATE cxx_std_20)
target_include_directories(${LIB_NAME} PUBLIC ./include)
target_link_libraries(${LIB_NAME} GearsCore GearsMath GearsUtility)
set_target_properties(${LIB_NAME}
    PROPERTIES
    VERSION ${PROJECT_VERSION})