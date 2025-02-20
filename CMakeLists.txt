project(mygame)

cmake_minimum_required(VERSION 3.3)
# Local header files here ONLY

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

add_executable(${PROJECT_NAME} src/main.cpp src/GameObject.cpp src/TextureManager.cpp src/Game.cpp src/Scientist.cpp src/Mouse.cpp src/Bullet.cpp
    src/Map.cpp
    src/player.cpp
src/Tile.cpp)

INCLUDE(FindPkgConfig)
include(FetchContent)

#---------------- SDL ----------------
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

#---------------- GLM ----------------
FetchContent_Declare(
    glm
    GIT_REPOSITORY    https://github.com/g-truc/glm.git
    GIT_TAG     bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE
    ${SDL2_LIBRARIES}
    ${SDL2IMAGE_LIBRARIES}
    glm::glm

    )
set(ASSETS_DIR ${CMAKE_SOURCE_DIR}/assets)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

add_custom_target(CopyAssets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ASSETS_DIR} ${CMAKE_BINARY_DIR}/assets
)


