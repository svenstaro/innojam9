cmake_minimum_required(VERSION 3.0)
project(innojam9)

add_definitions("-g -Wall -Wextra -pedantic -Werror -std=c++14 -Wno-unused-parameter")

file(GLOB_RECURSE sources src/*.cpp src/*.hpp)

add_executable(innojam9 ${sources})

if(${EMSCRIPTEN})
    set(CROSS_COMPILING ON)
    add_definitions("-Oz -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s SDL2_IMAGE_FORMATS='[\"png\"]'")
    set(CMAKE_CXX_LINK_FLAGS "-pg -Oz -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s SDL2_IMAGE_FORMATS='[\"png\"]' -s TOTAL_MEMORY=32000000 --emrun --preload-file ../res ../levels -o index.html")
else()
    include(FindPkgConfig)
    pkg_check_modules(SDL2 REQUIRED sdl2 SDL2_image SDL2_mixer SDL2_ttf)
endif()

set(ENTITYX_BUILD_TESTING false CACHE BOOL "")
add_subdirectory(entityx)

include_directories(SYSTEM
    ${SDL2_INCLUDE_DIRS}
    glm/
    entityx/
    strapon/
    src/
    yaml-cpp/yaml-cpp-config.cmake
)

target_link_libraries(innojam9
    ${SDL2_LIBRARIES}
    entityx
    yaml-cpp
)
