cmake_minimum_required(VERSION 3.10)
project(Mini_Instagram)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find necessary packages
find_package(OpenGL REQUIRED)
# If glfw3 is not found as a package, we'll try to find it manually or via pkg-config
find_package(glfw3 QUIET)

# Include directories
include_directories(
    src
    thirdparty/imgui
    thirdparty/imgui/backends
)

# ImGui source files
set(IMGUI_SOURCES
    thirdparty/imgui/imgui.cpp
    thirdparty/imgui/imgui_demo.cpp
    thirdparty/imgui/imgui_draw.cpp
    thirdparty/imgui/imgui_tables.cpp
    thirdparty/imgui/imgui_widgets.cpp
    thirdparty/imgui/backends/imgui_impl_glfw.cpp
    thirdparty/imgui/backends/imgui_impl_opengl3.cpp
)

# Project source files
set(SOURCES
    src/main.cpp
    src/db/Database.cpp
    src/gui/UIManager.cpp
)

# Executable
add_executable(${PROJECT_NAME} ${SOURCES} ${IMGUI_SOURCES})

# Libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${OPENGL_LIBRARIES}
    sqlite3
)

if (glfw3_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
else()
    # Try pkg-config for glfw3 if find_package fails
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW3 REQUIRED glfw3)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${GLFW3_LIBRARIES})
    target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW3_INCLUDE_DIRS})
endif()
