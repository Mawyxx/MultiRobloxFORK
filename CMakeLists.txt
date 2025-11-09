cmake_minimum_required(VERSION 3.16)
project(MultiRoblox LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_definitions(UNICODE _UNICODE)
    add_compile_options(/W4 /permissive- /utf-8)
else()
    message(FATAL_ERROR "MultiRoblox поддерживает только компилятор MSVC на Windows.")
endif()

add_executable(MultiRoblox WIN32
    src/main.cpp
    src/gui.cpp
)

target_include_directories(MultiRoblox PRIVATE src)

target_link_libraries(MultiRoblox PRIVATE user32.lib)

