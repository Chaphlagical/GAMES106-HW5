cmake_minimum_required(VERSION 3.16)
project(GAMES106-HW5)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(libigl)
include(assimp)

set(CMAKE_CXX_STANDARD 17)

# Add your project files
file(GLOB SRC_FILES src/*.cpp src/*.h)
add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} PUBLIC igl::glfw igl::imgui assimp::assimp)
