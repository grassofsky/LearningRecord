project(cgal_test)

add_executable(${PROJECT_NAME} cgal_test.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE 
    "C:/dev/CGAL-5.0.2/include"
    "D:/UIH/include/external")
target_link_directories(${PROJECT_NAME} PRIVATE "C:/dev/CGAL-5.0.2/lib")
target_link_libraries(${PROJECT_NAME} "libgmp-10.lib" "libmpfr-4.lib")