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

set(CMAKE_BUILD_TYPE "Release")
set(DEPEND_LIBS_RELEASE "")
set(DEPEND_LIBS_DEBUG "")
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(OPENMESH_PATH "/usr/local/include")
    add_compile_options(-std=c++11)
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(OPENMESH_PATH "C:/Program Files (x86)/OpenMesh")
    set(GLFW_PATH "C:/Program Files (x86)/GLFW")
    set(EIGEN_PATH "E:/workplace/github/eigen/")
    set(IMGUI_PATH "E:/workplace/github/imgui/")
    set(GLEW_PATH "C:/Program Files (x86)/glew")

    if (CMAKE_BUILD_TYPE MATCHES "Release")
        list(APPEND DEPEND_LIBS_RELEASE "glew64.lib")
        list(APPEND DEPEND_LIBS_RELEASE "glfw3dll.lib")
        list(APPEND DEPEND_LIBS_RELEASE "OpenGL32.lib")
        list(APPEND DEPEND_LIBS_RELEASE "OpenMeshCore.lib")
    elseif (CMAKE_BUILD_TYPE MATCHES "Debug")
        list(APPEND DEPEND_LIBS_DEBUG "glew64d.lib")
        list(APPEND DEPEND_LIBS_DEBUG "glfw3dll.lib")
        list(APPEND DEPEND_LIBS_DEBUG "OpenGL32.lib")
        list(APPEND DEPEND_LIBS_DEBUG "OpenMeshCored.lib")
    endif()
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

add_definitions(-D_USE_MATH_DEFINES)

add_subdirectory(octree_test)
add_subdirectory(imgui_test)
add_subdirectory(triangle_inter_pair_test)
add_subdirectory(cgal_test)
