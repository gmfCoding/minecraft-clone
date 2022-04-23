cmake_minimum_required(VERSION 3.12)

set(LSOURCE 
main.cpp
Camera.cpp
engine.cpp
Input.cpp
main.cpp
Material.cpp
MaterialSystem.cpp
Object.cpp
PlayerController.cpp
Rendering.cpp
World.cpp
GizmoLine.cpp
)

list(TRANSFORM LSOURCE PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")

message(STATUS "[LSOURCE/extra] files: ${LSOURCE}")
set(SOURCES ${SOURCES} ${LSOURCE} PARENT_SCOPE)