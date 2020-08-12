project(mesh_smooth)

add_executable(${PROJECT_NAME} mesh_smooth.cpp)

FIND_PACKAGE( OpenSceneGraph PATHS "C:/Program Files (x86)/OpenSceneGraph/cmake"
    COMPONENTS
    osg
    osgUtil
    OpenThreads
    osgViewer
    )

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${OpenSceneGraph_INCLUDE_DIRS})

if (CMAKE_BUILD_TYPE MATCHES "Release")
    target_link_libraries(${PROJECT_NAME} ${DEPEND_LIBS_RELEASE} ${OpenSceneGraph_LIBRARIES})
elseif(CMAKE_BUILD_TYPE MATCHES "Debug")
    target_link_libraries(${PROJECT_NAME} ${DEPEND_LIBS_DEBUG} ${OpenSceneGraph_LIBRARIES})
endif()