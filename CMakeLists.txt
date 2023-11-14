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
find_package(Vulkan 1 REQUIRED)

## 3rd Parties --------------------------------
include(FetchContent)

### Declares
FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG        release-2.28.5
)

### MakeAvailable
set(MESSAGE_QUIET TRUE)
FetchContent_MakeAvailable(SDL2)
unset(MESSAGE_QUIET)

## Libraries ----------------------------------

add_library(app
  Source/app.c
)

add_library(global
  Source/global.c
)

add_library(event
  Source/event/event.c
)

add_library(vulkan
  Source/vulkan/vk-global.c
  Source/vulkan/vk-utils.c
  Source/vulkan/vk-std.c
  Source/vulkan/vk.c
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
    PUBLIC SDL2
    PUBLIC Vulkan::Vulkan
)

