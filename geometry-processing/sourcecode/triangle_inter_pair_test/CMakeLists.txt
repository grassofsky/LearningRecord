project(triangle_inter_pair_test)

add_executable(${PROJECT_NAME} triangle_inter_pair_test.cpp)

if (CMAKE_BUILD_TYPE MATCHES "Release")
    target_link_libraries(${PROJECT_NAME} ${DEPEND_LIBS_RELEASE})
elseif(CMAKE_BUILD_TYPE MATCHES "Debug")
    target_link_libraries(${PROJECT_NAME} ${DEPEND_LIBS_DEBUG})
endif()