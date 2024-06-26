cmake_minimum_required(VERSION 3.4)

# == CONFIG ==
set(SOURCE_FILE
    ./src/Internal/XinputGamePad.cpp
    ./src/Internal/GamePadDevice.cpp
    ./src/Internal/KeyBoardDevice.cpp
    ./src/Internal/MouseDevice.cpp
    ./src/InputState.cpp
    ./src/InputSystem.cpp
)

set(HEADER_FILE
    ./include/Internal/DirectInput8.h
    ./include/Internal/IGamePad.h
    ./include/Internal/XInputState.h
    ./include/Internal/KeyBoardKeys.h
    ./include/Internal/GamePadKeys.h
    ./include/Internal/GamePadDevice.h
    ./include/Internal/XinputGamePad.h
    ./include/Internal/KeyBoardDevice.h
    ./include/Internal/MouseDevice.h
    ./include/Internal/MouseButton.h
    ./include/InputState.h
    ./include/InputSystem.h
)

set(LIB_NAME GearsInput)
create_filter()

add_library(${LIB_NAME} STATIC ${SOURCE_FILE} ${HEADER_FILE})
target_compile_features(${LIB_NAME} PRIVATE cxx_std_20)
target_include_directories(${LIB_NAME} PUBLIC ./include)
target_link_libraries(${LIB_NAME} GearsCore GearsMath GearsUtility xinput)
set_target_properties(${LIB_NAME}
    PROPERTIES
    VERSION ${PROJECT_VERSION})
