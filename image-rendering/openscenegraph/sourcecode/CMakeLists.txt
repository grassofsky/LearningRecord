cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD 11)

project(learnosg)

set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE TYPE INTERNAL FORCE)

#set(OSG_DIR "osg" CACHE PATH "C:\Program Files (x86)\OpenSceneGraph")

find_package(OpenSceneGraph REQUIRED
	osgDB
	osgViewer
	osgGA
	osgUtil
	osgText
)

include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})

add_subdirectory(osg_test)