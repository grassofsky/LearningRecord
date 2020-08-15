project(ch02_07)

add_executable(${PROJECT_NAME} background_image.cpp)
target_link_libraries(${PROJECT_NAME} ${OPENSCENEGRAPH_LIBRARIES})