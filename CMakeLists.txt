cmake_minimum_required(VERSION 3.21)

set(CMAKE_POLICY_VERSION_MINIMUM 3.10)

project(AC.Rogue.PatchFix
    LANGUAGES CXX
    VERSION 2.1.0
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

include(cmake/deps/injector.cmake)
include(cmake/deps/Hooking.Patterns.cmake)
include(cmake/deps/mINI.cmake)
include(cmake/deps/spdlog.cmake)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB_RECURSE SRC_MAIN
    "${SRC_DIR}/**.cpp"
)

add_library(${PROJECT_NAME} SHARED "${SRC_MAIN}")

target_include_directories(${PROJECT_NAME} PRIVATE "${INC_DIR}")

target_link_libraries(${PROJECT_NAME} PRIVATE
    injector
    HookingPatterns
    mINI
    spdlog::spdlog
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    WIN32_LEAN_AND_MEAN
    NOMINMAX
    PROJECT_NAME="${PROJECT_NAME}"
)

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:Clang,AppleClang>:
        -Wall -Wextra -Wpedantic
        -Wno-c++98-compat -Wno-c++98-compat-pedantic
        -Wno-c++20-compat -Wno-c++20-compat-pedantic
        -Wno-padded
        -Wno-covered-switch-default
    >
    $<$<CXX_COMPILER_ID:MSVC>:
        /W4
        /wd4201
    >
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    OUTPUT_NAME "${PROJECT_NAME}"
    SUFFIX ".asi"
    PREFIX ""
)
