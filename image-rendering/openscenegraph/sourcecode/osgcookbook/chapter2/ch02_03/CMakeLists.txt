project(ch02_03)

add_executable(${PROJECT_NAME} bounding_box.cpp ../../common/CommonFunctions.cpp)
target_link_libraries(${PROJECT_NAME} ${OPENSCENEGRAPH_LIBRARIES})