cmake_minimum_required(VERSION 3.16)
project(Cubo2x2x2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(cubo_logica
    src/EstadoCubo.cpp
    src/Movimento.cpp
    src/GeradorSucessores.cpp
    src/AvaliadorEstado.cpp
    src/Cubo.cpp
)
target_include_directories(cubo_logica PUBLIC include)
target_compile_options(cubo_logica PRIVATE -Wall -Wextra -Wpedantic)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
add_executable(cubo2x2x2 src/main.cpp src/interface/JanelaPrincipal.cpp
    src/interface/VisualizadorCubo.cpp)
target_link_libraries(cubo2x2x2 PRIVATE cubo_logica Qt${QT_VERSION_MAJOR}::Widgets)

include(CTest)
if(BUILD_TESTING)
    add_executable(testes_logica tests/testes_logica.cpp)
    target_link_libraries(testes_logica PRIVATE cubo_logica)
    add_test(NAME testes_logica COMMAND testes_logica)
endif()
