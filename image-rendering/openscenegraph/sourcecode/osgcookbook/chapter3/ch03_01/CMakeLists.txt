project(ch03_01)

add_executable(${PROJECT_NAME} create_polygon_with_borderlines.cpp)
target_link_libraries(${PROJECT_NAME} ${OPENSCENEGRAPH_LIBRARIES})