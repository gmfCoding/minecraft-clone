cmake_minimum_required(VERSION 3.12)
project(app)
set(SOURCES "")
set(IMGUISRC "")

find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} )

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_HEADERS ${CMAKE_SOURCE_DIR}/app/vendor/glfw)

add_subdirectory(app/vendor/glfw-3.3.7)


add_subdirectory(app/vendor/imgui)
add_subdirectory(app/vendor/glad)
add_subdirectory(app/vendor/other)

# Populates SOURCES with the src files defined in app/src/CMakeLists.txt 
add_subdirectory(app/src)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

target_include_directories(${CMAKE_PROJECT_NAME}
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/app/srcinc
    PUBLIC
        ${PROJECT_SOURCE_DIR}/app/vendor/
	${PROJECT_SOURCE_DIR}/app/vendor/glm
	${PROJECT_SOURCE_DIR}/app/vendor/other
	${PROJECT_SOURCE_DIR}/app/vendor/nlohmann
        ${PROJECT_SOURCE_DIR}/app/vendor/imgui
        ${PROJECT_SOURCE_DIR}/app/vendor/imgui/backends
	${PROJECT_SOURCE_DIR}/app/vendor/glad/include/glad
        ${GLFW_HEADERS}
)

# target_link_directories(${CMAKE_PROJECT_NAME} PUBLIC app/vendor/lib)

# link the new hello_library target with the hello_binary target
target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE 
    glad
    imgui
    glfw
	other
	shlwapi
)

message(STATUS "COPY ${PROJECT_SOURCE_DIR}/app/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR}")
# file(COPY ${PROJECT_SOURCE_DIR}/app/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# add_custom_command(TARGET app PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/app/resources/ $<TARGET_FILE_DIR:app>)

add_custom_target(copy-runtime-files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/app/resources ${CMAKE_BINARY_DIR}/resources
    DEPENDS ${MY_TARGET})