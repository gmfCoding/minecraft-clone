cmake_minimum_required(VERSION 3.12)
project(other VERSION 1 DESCRIPTION "Other")
add_library(other STATIC
	AtulPwd.cpp
	fileio.cpp
    gmfc_image.cpp
)

target_include_directories(other
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)


set_target_properties(glad PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(glad PROPERTIES PUBLIC_HEADER include)