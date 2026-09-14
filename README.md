# Cubo-2x2x2
Repositóro dedicado ao projeto cubo 2x2x2 em C++

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