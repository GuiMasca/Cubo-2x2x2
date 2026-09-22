# Cubo-2x2x2
Repositóro dedicado ao projeto cubo 2x2x2 em C++

## Linux

Requer CMake, Make, compilador C++ e Qt6 com Widgets e OpenGL.
Antes de compilar, salve os arquivos no editor: a compilação usa o conteúdo salvo no disco.

Na primeira vez, execute a partir da pasta principal do projeto (onde está o `CMakeLists.txt`):

```bash
cmake -S . -B build
cmake --build build --parallel 2 && ./build/meucubo
```

Depois de alterar o código, se já estiver na pasta `build`:

```bash
make -j2 && ./meucubo
```

O comando acima usa o Makefile gerado pelo CMake. A configuração atual do Linux
usa `Unix Makefiles`. A alternativa abaixo também funciona com outros geradores do CMake:

```bash
cmake --build . --parallel 2 && ./meucubo
```

O `&&` abre o programa somente se a compilação terminar com sucesso.
Se executar apenas `./meucubo`, será aberto o último executável compilado,
sem incorporar alterações feitas depois da compilação.

Se precisar recompilar tudo, ainda dentro de `build`:

```bash
cmake --build . --clean-first --parallel 2 && ./meucubo
```

Se o Makefile ainda não existir, configure primeiro, dentro de `build`:

```bash
cmake -S .. -B .
make -j2 && ./meucubo
```

Use sempre a mesma pasta de compilação (`build`) para evitar abrir um executável
antigo de outra pasta. Sem alterações no código, basta executar `./meucubo`.

## Windows

Como rodar o Projeto:
    Rodamos no terminal:
        cd build
        cmake --build .
        .\meucubo.exe

Quando trocar de computador, faça:
Remove-Item -Recurse -Force build
mkdir build
cd build

e depois configura dnv:
& "C:\Qt\Tools\CMake_64\bin\cmake.exe" ..

QUANDO RODAR O .EXE, observa o cubo na exata posição em que ele abriu (a face da frente é a vermelha):
COMANDOS - 
A roda a face ATRÁS
F roda a face da FRENTE
U roda a face de CIMA
B roda a face de BAIXO
R roda a face da DIREITA
L roda a face da ESQUERDA

PARA RODAR NO ANTI - HORÁRIO:
A (atrás) anti-horário é S
F (frente) anti-horário é G
B (de baixo) anti-horário é N
U (de cima) anti-horário é I
R (direita) anti-horário é T
L (esquerda) anti-horário é K