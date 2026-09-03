# Relatório de desenvolvimento — Cubo 2x2x2

## Etapa 1: lógica do cubo

### Arquitetura orientada a objetos

- `EstadoCubo`: representa um estado por dois vetores de oito posições. Um
  guarda qual peça de canto ocupa cada posição; o outro guarda sua orientação
  (0, 1 ou 2). O objeto valida seus invariantes e produz um novo estado após um
  movimento, sem alterar o estado original. Essa característica simplifica o
  uso seguro dos estados nos algoritmos de busca.
- `Cubo`: objeto mutável usado pelo jogo. Mantém o estado corrente, aplica
  jogadas, reinicia e embaralha.
- `GeradorSucessores`: implementação explícita da **função sucessora**. Gera os
  18 estados alcançáveis pelos movimentos U, R, F, D, L e B, incluindo giros
  inversos e duplos. Há uma variante que elimina a repetição imediata da mesma
  face, útil para reduzir a árvore de busca sem perder soluções.
- `AvaliadorEstado`: implementação explícita da **função avaliadora**. Testa o
  objetivo e disponibiliza uma heurística inicial para o A*.
- `Movimento`: enumeração tipada dos 18 movimentos, com conversão para notação
  convencional, cálculo do inverso, face e quantidade de quartos de volta.

### Representação do estado

As posições dos cantos seguem a ordem `URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB`.
No estado resolvido, a peça de índice `i` está na posição `i`, com orientação
zero. Essa representação ocupa somente 16 bytes de dados e permite comparação
e hash eficientes, importantes porque BFS e A* armazenam muitos estados.

Um estado válido contém cada uma das oito peças exatamente uma vez, orientações
entre 0 e 2 e soma das orientações divisível por 3. Os giros são implementados
por tabelas de permutação e mudança de orientação.

### Heurística inicial

A heurística conta separadamente cantos fora da posição e cantos mal orientados.
Como um giro de face afeta quatro cantos, cada contagem é dividida por quatro e
arredondada para cima. O maior dos dois valores é usado. Esse é um limite
inferior simples (admissível): nunca estima mais movimentos que o necessário.
Uma tabela de padrões poderá ser adicionada depois para melhorar o desempenho.

### Embaralhamento reproduzível

`Cubo::embaralhar(seed, quantidade)` usa `std::mt19937`. A mesma seed e o mesmo
número de movimentos produzem a mesma sequência e o mesmo estado, permitindo
comparar BFS, IDDFS e A* em condições idênticas. Movimentos consecutivos da
mesma face são evitados para não gerar cancelamentos triviais.

### Testes implementados

Os testes verificam:

1. validade e reconhecimento do estado resolvido;
2. os 18 movimentos seguidos de seus inversos;
3. quatro giros de cada face retornando ao estado inicial;
4. validade dos estados produzidos;
5. reprodução do embaralhamento pela seed e solução pela sequência inversa;
6. quantidade e unicidade dos sucessores;
7. teste de objetivo e valores básicos da heurística.

### Organização dos arquivos

```text
include/cubo/       Interfaces das classes
src/                Implementações e executável de demonstração
tests/              Testes automatizados
CMakeLists.txt      Configuração de compilação e testes
Makefile            Compilação alternativa com GNU Make
RELATORIO.md        Este relatório
```

## Próximas etapas

1. Criar uma abstração de fronteira para que o laço de busca exigido permaneça
   igual com fila, pilha limitada e fila de prioridade.
2. Implementar BFS, IDDFS e A*, reconstrução do caminho e contagem de visitados.
3. Medir as buscas e, se necessário, melhorar a heurística do A*.
4. Integrar os solucionadores aos controles e à apresentação passo a passo.

## Etapa 2: interface gráfica

A aplicação ganhou uma janela nativa feita com Qt Widgets, compatível com Qt 6
e Qt 5. O widget `VisualizadorCubo` converte o estado lógico em 24 adesivos e
os desenha como um cubo 3D com perspectiva, remoção de faces ocultas e rotação
interativa pelo mouse. A janela
possui os 18 botões de movimento, reinício, campos de seed e quantidade de
movimentos e botão de embaralhamento. Também apresenta a última ação, informa
se o cubo está resolvido e exibe o valor atual da heurística.

Os controles foram organizados por face e sentido, com nomes em português,
setas e dicas. Um manual integrado explica as letras U/R/F/D/L/B, o sufixo `2`,
o apóstrofo e o ponto de vista dos sentidos horário e anti-horário.

### Correções de estabilidade visual

O painel de comandos passou a ter largura fixa e o histórico de embaralhamento
quebra linhas dentro dessa área. Assim, sequências longas não recalculam a
largura do layout nem comprimem o visualizador, evitando o efeito de zoom. O
cubo agora desenha primeiro um corpo preto sólido para cada face visível e só
então seus quatro adesivos. A seleção das faces visíveis usa as normais externas
definidas explicitamente, eliminando faces ausentes ou aparentemente
transparentes durante a rotação.

A interface não conhece as tabelas internas de movimentos: ela apenas envia um
`Movimento` ao objeto `Cubo` e redesenha o `EstadoCubo`. Essa separação permite
ligar os algoritmos de IA à mesma tela sem duplicar ou alterar a lógica.
