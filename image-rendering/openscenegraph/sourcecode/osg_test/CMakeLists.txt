project(osg_test)

add_executable(${PROJECT_NAME} osg_test.cpp)
target_link_libraries(${PROJECT_NAME} ${OPENSCENEGRAPH_LIBRARIES})