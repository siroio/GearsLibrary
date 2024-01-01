cmake_minimum_required(VERSION 3.4)

# == CONFIG ==
set(SOURCE_FILE
    ./src/Internal/XAudioSystem.cpp
    ./src/Internal/AudioLoaderFactory.cpp
    ./src/Internal/WavLoader.cpp
    ./src/Components/AudioSource.cpp
    ./src/Components/AudioListener.cpp
    ./src/AudioManager.cpp
    ./src/AudioClip.cpp
)

set(HEADER_FILE
    ./include/Internal/XAudioSystem.h
    ./include/Internal/AudioChannels.h
    ./include/Internal/AudioChannels.h
    ./include/Internal/IAudioLoader.h
    ./include/Internal/AudioLoaderFactory.h
    ./include/Internal/WavLoader.h
    ./include/Components/AudioSource.h
    ./include/Components/AudioListener.h
    ./include/AudioManager.h
    ./include/AudioClip.h
)

set(LIB_NAME GearsAudio)
create_filter()

add_library(${LIB_NAME} STATIC ${SOURCE_FILE} ${HEADER_FILE})
target_compile_features(${LIB_NAME} PRIVATE cxx_std_20)
target_include_directories(${LIB_NAME} PUBLIC ./include)
target_link_libraries(${LIB_NAME} GearsCore GearsComponent GearsObject GearsUtility)
set_target_properties(${LIB_NAME}
    PROPERTIES
    VERSION ${PROJECT_VERSION})
