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
Ao executar, primeiro escolha uma face no mapa colorido e depois clique no giro
desejado. As faces também aparecem identificadas sobre o cubo 3D. Arraste o
cubo com o mouse para observá-lo por qualquer ângulo.

## Alterações simples

Os nomes e as cores usados na tela ficam juntos em
`include/interface/ConfiguracaoVisual.hpp`. A tabela `FACES` tem uma linha para
cada face. Por exemplo, trocar `"#20a05a"` na linha `Frente` muda o verde dessa
face tanto no botão quanto no cubo. Os textos maiores da tela estão em
`src/interface/JanelaPrincipal.cpp` e possuem nomes diretos, como
`botaoReiniciar` e `tituloGiro`.

O campo **Número para repetir** é opcional no uso comum: ele só existe para que
o mesmo número e a mesma quantidade produzam sempre o mesmo embaralhamento.

## Como estudar a lógica

Uma ordem simples para ler o projeto é:

1. `Movimento.cpp`: explica cada movimento, seu inverso e sua face.
2. `EstadoCubo.cpp`: guarda as oito peças e realiza os giros.
3. `Cubo.cpp`: reúne as ações de mover, reiniciar e embaralhar.
4. `GeradorSucessores.cpp` e `AvaliadorEstado.cpp`: funções pequenas que serão
   usadas pelos algoritmos de busca.

A implementação usa principalmente `for`, `if` e `switch`. Nas tabelas de giro,
cada número informa de qual posição antiga vem a peça da nova posição.

Alternativamente, apenas com GNU Make e um compilador C++:

```bash
make
make test
./build/cubo2x2x2
```

Consulte [RELATORIO.md](RELATORIO.md) para a descrição das decisões e do que já
foi implementado.
