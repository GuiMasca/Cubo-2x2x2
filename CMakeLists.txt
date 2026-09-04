cmake_minimum_required(VERSION 3.16)
project(MeuCubo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)   # necessário para classes Qt com sinais/slots

find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)

add_executable(meucubo src/main.cpp)
target_include_directories(meucubo PRIVATE include)
target_link_libraries(meucubo PRIVATE Qt6::Widgets Qt6::OpenGLWidgets)