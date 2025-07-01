cmake_minimum_required(VERSION 3.14)

project(MilitaryCollide VERSION 0.1.0 DESCRIPTION "C SDL2 Vulkan Starter" LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED true)

add_compile_definitions($<$<CONFIG:Debug>:DEBUG>)
add_compile_definitions($<$<CONFIG:Release>:NDEBUG>)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/Scripts/Cmake)
message(STATUS "CMAKE_MODULE_PATHS: ${CMAKE_MODULE_PATH}")
include(platform)
include(override)

include_directories(Source)

## Required Installed Libraries
find_package(Vulkan 1.3 REQUIRED) # use the latest vulkan features (u can downgrade this if u like)
find_package(SDL3 REQUIRED) # ensure to have SDL3 installed

## Libraries ----------------------------------

add_library(app
  Source/app.c
)

target_link_libraries(app
    PUBLIC SDL3::SDL3
    PUBLIC Vulkan::Vulkan
)


add_library(global
  Source/global.c
)

target_link_libraries(global
    PUBLIC SDL3::SDL3
    PUBLIC Vulkan::Vulkan
)

add_library(event
  Source/event/event.c
)

target_link_libraries(event
    PUBLIC SDL3::SDL3
    PUBLIC Vulkan::Vulkan
)

add_library(vulkan
  Source/vulkan/vk-global.c
  Source/vulkan/vk-utils.c
  Source/vulkan/vk-std.c
  Source/vulkan/vk.c
)

target_link_libraries(vulkan
    PUBLIC SDL3::SDL3
    PUBLIC Vulkan::Vulkan
)


## Executables --------------------------------

add_executable(main
    Other-Source/main.c
)

target_compile_options(main
  PRIVATE -Wall -Wextra
)

target_link_libraries(main
    PRIVATE app
    PRIVATE global
    PRIVATE event
    PUBLIC vulkan
    PUBLIC SDL3::SDL3
    PUBLIC Vulkan::Vulkan
)

