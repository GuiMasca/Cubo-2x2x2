# Simulador de Cubo Mágico 2x2x2

Projeto em C++ orientado a objetos com lógica do cubo e interface gráfica 2D em
Qt Widgets. As próximas etapas acrescentarão as buscas BFS, IDDFS e A* e uma
interface comum para suas estruturas de fronteira.

## Compilar e testar

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
./build/cubo2x2x2
```

É necessário ter o módulo Qt Widgets (Qt 6 ou Qt 5) para compilar a aplicação.
Ao executar, use os botões dos 18 movimentos ou embaralhe o cubo informando uma
seed. Arraste o cubo 3D com o mouse para observá-lo por qualquer ângulo. O botão
`Manual de movimentos` explica a notação e o sentido de cada giro.

Alternativamente, apenas com GNU Make e um compilador C++:

```bash
make
make test
./build/cubo2x2x2
```

Consulte [RELATORIO.md](RELATORIO.md) para a descrição das decisões e do que já
foi implementado.
