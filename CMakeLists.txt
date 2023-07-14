cmake_minimum_required(VERSION 3.16)
project(GAMES106-HW5)

option(QEM_USE_OPENMESH "Link in OpenMesh to be used by QEM algorithm" OFF)
option(QEM_USE_UHEMESH "Include OpenMesh to be used by QEM algorithm" ON)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(libigl)
include(assimp)
if (QEM_USE_OPENMESH)
include(openmesh)
endif()

set(CMAKE_CXX_STANDARD 17)

# Add your project files
file(GLOB SRC_FILES src/*.cpp src/*.h)
add_executable(${PROJECT_NAME} ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(${PROJECT_NAME} PUBLIC igl::glfw igl::imgui assimp::assimp)

if (QEM_USE_OPENMESH)
    target_link_libraries(${PROJECT_NAME} PUBLIC OpenMeshCore OpenMeshTools)
    target_compile_definitions(${PROJECT_NAME} PUBLIC _USE_MATH_DEFINES)
endif()

if (QEM_USE_UHEMESH)
    target_compile_definitions(${PROJECT_NAME} PUBLIC QEM_USE_UHEMESH)
endif()

if (QEM_USE_OPENMESH)
    target_compile_definitions(${PROJECT_NAME} PUBLIC QEM_USE_OPENMESH)
endif()