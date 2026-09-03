CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -O2
INCLUDES = -Iinclude
FONTES = src/EstadoCubo.cpp src/Movimento.cpp src/GeradorSucessores.cpp \
         src/AvaliadorEstado.cpp src/Cubo.cpp
QT_CFLAGS = $(shell pkg-config --cflags Qt6Widgets 2>/dev/null || pkg-config --cflags Qt5Widgets)
QT_LIBS = $(shell pkg-config --libs Qt6Widgets 2>/dev/null || pkg-config --libs Qt5Widgets)
FONTES_INTERFACE = src/interface/JanelaPrincipal.cpp src/interface/VisualizadorCubo.cpp

.PHONY: all test clean

all: build/cubo2x2x2

build/cubo2x2x2: $(FONTES) $(FONTES_INTERFACE) src/main.cpp
	mkdir -p build
	$(CXX) $(CXXFLAGS) -fPIC -no-pie $(INCLUDES) $(QT_CFLAGS) $^ -o $@ $(QT_LIBS)

build/testes_logica: $(FONTES) tests/testes_logica.cpp
	mkdir -p build
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

test: build/testes_logica
	./build/testes_logica

clean:
	rm -f build/cubo2x2x2 build/testes_logica
