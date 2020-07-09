cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD 11)

project(collision_test)

if (WIN32)
    add_definitions(
        -D_USE_MATH_DEFINIES -DNOMINMAX
        -D_CRT_SECURE_NO_WARNINGS
    )
endif ()


if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(OPENMESH_PATH "/usr/local/include")
    add_compile_options(-std=c++11)
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(OPENMESH_PATH "C:/Program Files (x86)/OpenMesh")
endif()

include_directories(
    ${OPENMESH_PATH}/include
)

link_directories(
    ${OPENMESH_PATH}/lib
)

add_subdirectory(octree_test)
