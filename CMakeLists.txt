cmake_minimum_required(VERSION 3.27)

file(READ "${CMAKE_SOURCE_DIR}/src/main.cpp" VERSION_FILE_CONTENT)
string(REGEX MATCH "define VERSION \"([^\"]+)" VERSION_MATCH "${VERSION_FILE_CONTENT}")

set(VERSION ${CMAKE_MATCH_1})
set(PRG_NAME refn)

project(${PRG_NAME} VERSION ${VERSION})
message(STATUS "Current Version: " ${VERSION})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

set(REFN_SOURCES
        src/main.cpp
        src/utils.cpp
        src/utils.hpp
)

if(MSVC)
    list(APPEND REFN_SOURCES
            src/getopt.c
            src/getopt.h
    )
endif ()

add_executable(${PRG_NAME} ${REFN_SOURCES})

if(NOT MSVC)
    target_compile_options(${PRG_NAME} PRIVATE
            -Os
            -fno-ident
            -fno-stack-protector
            -fvisibility=hidden
            -ffunction-sections
            -fdata-sections
            -fno-unwind-tables
            -fno-asynchronous-unwind-tables
            -fno-exceptions
            -fno-rtti
            -fno-unroll-loops
    )
endif ()

target_link_options(${PRG_NAME} PRIVATE
        -Wl,-s
        -Wl,--gc-sections
        -Wl,--build-id=none
        -static
)

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)
if (supported)
    set_property(TARGET ${PRG_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()
