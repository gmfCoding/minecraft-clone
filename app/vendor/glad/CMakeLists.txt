cmake_minimum_required(VERSION 3.12)
project(glad VERSION 1 DESCRIPTION "Glad OpenGL Loader")
add_library(glad STATIC
    src/glad.c
)

target_include_directories(glad
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include/glad
	${CMAKE_CURRENT_SOURCE_DIR}/include
)


set_target_properties(glad PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(glad PROPERTIES PUBLIC_HEADER include)