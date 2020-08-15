project(ch02_05)

add_executable(${PROJECT_NAME} mirror_scene.cpp)
target_link_libraries(${PROJECT_NAME} ${OPENSCENEGRAPH_LIBRARIES})