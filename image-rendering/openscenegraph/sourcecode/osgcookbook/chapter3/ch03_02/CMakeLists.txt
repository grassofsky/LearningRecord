project(ch03_02)

add_executable(${PROJECT_NAME} extruding_a_2d_shape_to_3d.cpp)
target_link_libraries(${PROJECT_NAME} ${OPENSCENEGRAPH_LIBRARIES})