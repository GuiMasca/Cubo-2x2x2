cmake_minimum_required(VERSION 3.16)
project(MeuCubo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# Adicionado OpenGL nos pacotes requeridos do Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets)

add_executable(meucubo 
    src/main.cpp
    include/CubeState.h
)

target_include_directories(meucubo PRIVATE include)

# Adicionado Qt6::OpenGL na linkagem
target_link_libraries(meucubo PRIVATE Qt6::Widgets Qt6::OpenGLWidgets Qt6::OpenGL)