project(imgui_test)

add_executable(${PROJECT_NAME} 
    ${IMGUI_PATH}/imconfig.h 
    ${IMGUI_PATH}/imgui.h
    ${IMGUI_PATH}/imgui.cpp
    ${IMGUI_PATH}/imgui_demo.cpp
    ${IMGUI_PATH}/imgui_draw.cpp
    ${IMGUI_PATH}/imgui_internal.h
    ${IMGUI_PATH}/imgui_widgets.cpp
    ${IMGUI_PATH}/imstb_rectpack.h
    ${IMGUI_PATH}/imstb_textedit.h
    ${IMGUI_PATH}/imstb_truetype.h
    ${IMGUI_PATH}/examples/imgui_impl_glfw.h 
    ${IMGUI_PATH}/examples/imgui_impl_glfw.cpp 
    ${IMGUI_PATH}/examples/imgui_impl_opengl3.cpp 
    ${IMGUI_PATH}/examples/imgui_impl_opengl3.h
    imgui_test.cpp)

target_link_libraries(${PROJECT_NAME} Debug ${DEPEND_LIBS_DEBUG})
target_link_libraries(${PROJECT_NAME} Release ${DEPEND_LIBS_RELEASE})