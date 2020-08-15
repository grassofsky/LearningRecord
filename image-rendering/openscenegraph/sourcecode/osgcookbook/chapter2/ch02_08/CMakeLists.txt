project(ch02_08)

add_executable(${PROJECT_NAME} billboard_node.cpp)
target_link_libraries(${PROJECT_NAME} ${OPENSCENEGRAPH_LIBRARIES})