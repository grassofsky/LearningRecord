cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD 11)

project(collision_test)

set(OPENMESH_PATH "C:/Program Files (x86)/OpenMesh")

include_directories(
    ${OPENMESH_PATH}/include
)

link_directories(
    ${OPENMESH_PATH}/lib
)

if (WIN32)
    add_definitions(
        -D_USE_MATH_DEFINIES -DNOMINMAX
        -D_CRT_SECURE_NO_WARNINGS
    )
endif ()

add_subdirectory(octree_test)
