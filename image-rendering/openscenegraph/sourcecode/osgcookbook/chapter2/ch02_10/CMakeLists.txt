project(ch02_10)

add_executable(${PROJECT_NAME} Compass.h Compass.cpp main.cpp)
target_link_libraries(${PROJECT_NAME} ${OPENSCENEGRAPH_LIBRARIES})