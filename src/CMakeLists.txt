cmake_minimum_required(VERSION 3.27)

add_executable(search_engine
        main.cpp
        ConverterJSON.cpp
        EngineExceptions.cpp
        InvertedIndex.cpp
        SearchServer.cpp
        ${MY_INCLUDE_PATH}/ConverterJSON.h
        ${MY_INCLUDE_PATH}/EngineExceptions.h
        ${MY_INCLUDE_PATH}/InvertedIndex.h
        ${MY_INCLUDE_PATH}/version.h
        ${MY_INCLUDE_PATH}/SearchServer.h)

target_include_directories(search_engine PUBLIC ${MY_INCLUDE_PATH})

file(COPY ../config DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY ../resources DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
