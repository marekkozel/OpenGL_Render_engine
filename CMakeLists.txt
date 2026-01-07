cmake_minimum_required(VERSION 3.10)
project(MyOpenGLProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(assimp REQUIRED)

file(GLOB_RECURSE SRC_FILES
    "src/Transformations*.cpp"
    "src/*.cpp"
    "main.cpp"
)

add_executable(main ${SRC_FILES})

# === Include directories ===
target_include_directories(main PRIVATE
    ${OPENGL_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${ASSIMP_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/common
    ${CMAKE_SOURCE_DIR}/include/Transformations

    /usr/include # include system headers
    /usr/include/stb # for stb_image.h if installed system-wide
)

# === Link libraries ===
target_link_libraries(main
    ${OPENGL_LIBRARIES}
    GLEW
    glfw
    X11
    Xxf86vm
    Xrandr
    pthread
    Xi
    dl
    Xinerama
    Xcursor
    assimp
)

set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
