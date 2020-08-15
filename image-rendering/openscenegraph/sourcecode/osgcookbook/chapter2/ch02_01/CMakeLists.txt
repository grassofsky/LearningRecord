project(ch02_01)

add_executable(${PROJECT_NAME} observer_ptr.cpp ../../common/CommonFunctions.cpp)
target_link_libraries(${PROJECT_NAME} ${OPENSCENEGRAPH_LIBRARIES})