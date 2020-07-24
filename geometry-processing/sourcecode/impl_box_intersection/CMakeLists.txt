project(impl_box_intersection)

add_executable(${PROJECT_NAME} impl_box_intersection.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE 
    "D:/UIH/include/external")

if (CMAKE_BUILD_TYPE MATCHES "Release")
    target_link_libraries(${PROJECT_NAME} ${DEPEND_LIBS_RELEASE})
elseif(CMAKE_BUILD_TYPE MATCHES "Debug")
    target_link_libraries(${PROJECT_NAME} ${DEPEND_LIBS_DEBUG})
endif()