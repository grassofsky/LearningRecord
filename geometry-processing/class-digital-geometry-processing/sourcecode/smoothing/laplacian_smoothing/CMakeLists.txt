project(laplacian_smoothing)

add_executable(${PROJECT_NAME} laplacian_smoothing.cpp ../../common/const_var.h)

if (CMAKE_BUILD_TYPE MATCHES "Release")
    target_link_libraries(${PROJECT_NAME} 
        ${OPENSCENEGRAPH_LIBRARIES}
        ${DEPEND_LIBS_RELEASE})
else(CMAKE_BUILD_TYPE MATCHES "Debug")
    target_link_libraries(${PROJECT_NAME} 
        ${OPENSCENEGRAPH_LIBRARIES}
        ${DEPEND_LIBS_DEBUG})
endif()