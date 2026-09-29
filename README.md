# Cubo Mágico 2x2x2 com Inteligência Artificial

Projeto desenvolvido em **C++17**, **Qt 6** e **OpenGL** para simular um Cubo Mágico 2x2x2 e resolvê-lo usando algoritmos de busca.

## Funcionalidades

- Cubo 2x2x2 em 3D
- Movimentação manual pelo teclado
- Embaralhamento por seed
- Verificação automática de cubo resolvido
- Busca em Largura
- Busca em Profundidade Limitada Iterativa
- Busca A*
- Exibição da quantidade de estados visitados
- Exibição da sequência de movimentos da solução

## Controles do cubo

Os movimentos são feitos pelo teclado.

| Tecla | Movimento|

| `U` | Face superior — sentido horário|

| `I` | Face superior — sentido anti-horário|

| `R` | Face direita — sentido horário|

| `T` | Face direita — sentido anti-horário|

| `F` | Face frontal — sentido horário|

| `G` | Face frontal — sentido anti-horário|

| `L` | Face esquerda — sentido horário |

| `K` | Face esquerda — sentido anti-horário|

| `B` | Face inferior — sentido horário|

| `N` | Face inferior — sentido anti-horário|

| `A` | Face traseira — sentido horário|

| `S` | Face traseira — sentido anti-horário|

Os sentidos horário e anti-horário são considerados olhando diretamente para a face que está sendo girada.

### Pares de movimentos

Cada movimento possui sua operação inversa:

U ↔ I

R ↔ T

F ↔ G

L ↔ K

B ↔ N

A ↔ S

## Tecnologias

- C++17
- Qt 6
- OpenGL
- CMake

## Como compilar

Na pasta principal do projeto:

```powershell
cmake -S . -B build -G Ninja -DCMAKE_PREFIX_PATH="C:/Qt/6.11.2/mingw_64"
cmake --build build