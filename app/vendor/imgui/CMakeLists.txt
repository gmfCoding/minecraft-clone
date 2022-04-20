# cmake_minimum_required(VERSION 3.12)

# set(LSOURCE 
# imgui_demo.cpp
# imgui_draw.cpp
# imgui_tables.cpp
# imgui_widgets.cpp
# imgui.cpp
# backends/imgui_impl_opengl3.cpp
# backends/imgui_impl_glfw.cpp
# )

# list(TRANSFORM LSOURCE PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")

# message(STATUS "[Vendor/IMGUI] files: ${LSOURCE}")
# set(IMGUISRC ${IMGUISRC} ${LSOURCE} PARENT_SCOPE)

cmake_minimum_required(VERSION 3.12)
project(imgui VERSION 1 DESCRIPTION "Dear ImGUI")

add_library(imgui STATIC
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    imgui.cpp
    backends/imgui_impl_opengl3.cpp
    backends/imgui_impl_glfw.cpp
)

message(STATUS "[ImGUI_COMPILE]${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "[ImGUI_COMPILE]${GLFW_HEADERS}")
target_include_directories(imgui
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${GLFW_HEADERS}
)