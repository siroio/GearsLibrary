cmake_minimum_required(VERSION 3.1)

# == CONFIG ==
set(SOURCE_FILE 
    ./src/CSVLoader.cpp
    ./src/MsgBox.cpp
)

set(HEADER_FILE
    ./include/Logger.h
    ./include/WeakPtr.h
    ./include/ComPtr.h
    ./include/Singleton.h
    ./include/StringUtility.h
    ./include/TimeUtility.h
    ./include/CSVLoader.h
    ./include/MsgBox.h
    ./include/Json.h
    ./include/Jsonfwd.h
    ./include/BitFlag.h
    ./include/ObjectPool.h
)

set(LIB_NAME GearsUtility)
create_filter()

add_library(${LIB_NAME} STATIC ${SOURCE_FILE} ${HEADER_FILE})
target_compile_features(${LIB_NAME} PRIVATE cxx_std_20)
target_include_directories(${LIB_NAME} PUBLIC ./include)
target_link_libraries(${LIB_NAME} nlohmann_json)
set_target_properties(${LIB_NAME}
    PROPERTIES
    VERSION ${PROJECT_VERSION})
