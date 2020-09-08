cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD 11)

project(learnosg)

set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE TYPE INTERNAL FORCE)

#set(OSG_DIR "osg" CACHE PATH "C:\Program Files (x86)\OpenSceneGraph")

set(CMAKE_BUILD_TYPE "Debug")

if (WIN32)
    add_definitions(
        -D_USE_MATH_DEFINES -DNOMINMAX
        -D_CRT_SECURE_NO_WARNINGS
    )
endif ()

set(DEPEND_LIBS_RELEASE "")
set(DEPEND_LIBS_DEBUG "")

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(OPENMESH_PATH "C:/Program Files (x86)/OpenMesh")
	set(EIGEN_PATH "E:/workplace/github/eigen/")

	if (CMAKE_BUILD_TYPE MATCHES "Release")
		list(APPEND DEPEND_LIBS_RELEASE "OpenMeshCore.lib")
	else(CMAKE_BUILD_TYPE MATCHES "Debug")
		list(APPEND DEPEND_LIBS_DEBUG "OpenMeshCored.lib")
	endif()
endif()

find_package(OpenSceneGraph REQUIRED
	osgDB
	osgViewer
	osgGA
	osgUtil
	osgText
	OpenThreads
)

include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS}
	${OPENMESH_PATH}/include
	${EIGEN_PATH}
)

link_directories(
	${OPENMESH_PATH}/lib
)

add_subdirectory(smoothing)