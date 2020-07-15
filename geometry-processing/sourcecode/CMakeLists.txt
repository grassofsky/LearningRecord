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

set(DEPEND_LIBS "")
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(OPENMESH_PATH "/usr/local/include")
    add_compile_options(-std=c++11)
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(OPENMESH_PATH "C:/Program Files (x86)/OpenMesh")
    set(GLFW_PATH "C:/Program Files (x86)/GLFW")
    set(EIGEN_PATH "E:/workplace/github/eigen/")
    set(IMGUI_PATH "E:/workplace/github/imgui/")
    set(GLEW_PATH "C:/Program Files (x86)/glew")

    list(APPEND DEPEND_LIBS "glew64d.lib")
    list(APPEND DEPEND_LIBS "glfw3dll.lib")
    list(APPEND DEPEND_LIBS "OpenGL32.lib")
endif()

include_directories(
    ${OPENMESH_PATH}/include
    ${GLFW_PATH}/include
    ${EIGEN_PATH}
    ${IMGUI_PATH}
    ${IMGUI_PATH}/examples
    ${GLEW_PATH}/include
)

link_directories(
    ${OPENMESH_PATH}/lib
    ${GLFW_PATH}/lib
    ${GLEW_PATH}/lib
)

add_subdirectory(octree_test)
add_subdirectory(imgui_test)
