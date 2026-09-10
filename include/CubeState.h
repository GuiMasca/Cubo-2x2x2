#ifndef CUBESTATE_H
#define CUBESTATE_H

#include <vector>
#include <string>
#include <algorithm>

struct CubeState {
    // 8 quinas, cada uma com 3 cores (U/D, F/B, L/R)
    // Inicialmente no estado resolvido
    std::vector<std::vector<int>> cornerColors;
    std::vector<std::string> path;

    CubeState() {
        // 8 quinas do 2x2x2 com suas cores iniciais padrão
        cornerColors = {
            {0, 2, 4}, // 0: UFL (Branco, Vermelho, Verde)
            {0, 2, 5}, // 1: UFR (Branco, Vermelho, Azul)
            {0, 3, 4}, // 2: UBL (Branco, Laranja, Verde)
            {0, 3, 5}, // 3: UBR (Branco, Laranja, Azul)
            {1, 2, 4}, // 4: DFL (Amarelo, Vermelho, Verde)
            {1, 2, 5}, // 5: DFR (Amarelo, Vermelho, Azul)
            {1, 3, 4}, // 6: DBL (Amarelo, Laranja, Verde)
            {1, 3, 5}  // 7: DBR (Amarelo, Laranja, Azul)
        };
    }

    bool isGoal() const {
        // Verifica se cada quina está na sua cor base correta
        std::vector<std::vector<int>> solved = {
            {0, 2, 4}, {0, 2, 5}, {0, 3, 4}, {0, 3, 5},
            {1, 2, 4}, {1, 2, 5}, {1, 3, 4}, {1, 3, 5}
        };
        return cornerColors == solved;
    }

    // Movimento de cima horário (U) (Topo - Gira as quinas 0, 1, 3, 2)
    CubeState moveU() const {
        CubeState next = *this;
        next.path.push_back("U");
        
        // Permutação das quinas do topo
        std::vector<int> temp = next.cornerColors[0];
        next.cornerColors[0] = next.cornerColors[1];
        next.cornerColors[1] = next.cornerColors[3];
        next.cornerColors[3] = next.cornerColors[2];
        next.cornerColors[2] = temp;

        for (int i : {0, 1, 2, 3}) {
            std::swap(
                next.cornerColors[i][1],
                next.cornerColors[i][2]
            );
        }
        return next;
    }

    //movimento de baixo horário (B)
    CubeState moveB() const {
    CubeState next = *this;
    next.path.push_back("B");

    auto old = cornerColors;

    // Bottom horário olhando diretamente para a face de baixo
    std::vector<int> temp = next.cornerColors[4];
    next.cornerColors[4] = next.cornerColors[6];
    next.cornerColors[6] = next.cornerColors[7];
    next.cornerColors[7] = next.cornerColors[5];
    next.cornerColors[5] = temp;

    // Frente/Trás <-> Esquerda/Direita
    for (int i : {4, 5, 6, 7}) {
        std::swap(
            next.cornerColors[i][1],
            next.cornerColors[i][2]
        );
    }

    return next;
    }

    // Movimento R (Direita - Gira as quinas 1, 5, 7, 3) HORARIO
    CubeState moveR() const {
        CubeState next = *this;
        next.path.push_back("R");
        
        std::vector<int> temp = next.cornerColors[1];
        next.cornerColors[1] = next.cornerColors[5];
        next.cornerColors[5] = next.cornerColors[7];
        next.cornerColors[7] = next.cornerColors[3];
        next.cornerColors[3] = temp;

            // Corrige orientação
        for (int i : {1, 5, 7, 3}) {
            std::swap(
                next.cornerColors[i][0],
                next.cornerColors[i][1]
            );
        }

        return next;
    }

    //movimento da frente no sentido horário (F)
   CubeState moveF() const {
    CubeState next = *this;
    next.path.push_back("F");

    // Guarda o estado antes do movimento
    auto old = cornerColors;

    // Rotação das posições das 4 quinas da frente
    next.cornerColors[1] = old[0]; // UFL -> UFR
    next.cornerColors[5] = old[1]; // UFR -> DFR
    next.cornerColors[4] = old[5]; // DFR -> DFL
    next.cornerColors[0] = old[4]; // DFL -> UFL

    // Ao girar a frente, os eixos CIMA/BAIXO e
    // ESQUERDA/DIREITA trocam de orientação.
    for (int i : {0, 1, 4, 5}) {
        std::swap(
            next.cornerColors[i][0],
            next.cornerColors[i][2]
        );
    }

    return next;
}
    //movimento da esquerda no sentido horário (L)
    CubeState moveL() const {
        CubeState next = *this;
        next.path.push_back("L");

        auto old = cornerColors;

        // L horário olhando diretamente para a face esquerda
        std::vector<int> temp = next.cornerColors[0];
        next.cornerColors[0] = next.cornerColors[2];
        next.cornerColors[2] = next.cornerColors[6];
        next.cornerColors[6] = next.cornerColors[4];
        next.cornerColors[4] = temp;

        // Cima/Baixo <-> Frente/Trás
        for (int i : {0, 2, 4, 6}) {
            std::swap(
                next.cornerColors[i][0],
                next.cornerColors[i][1]
            );
        }

        return next;
    }

    //movimento atrás horário (A)
    CubeState moveA() const {
        CubeState next = *this;
        next.path.push_back("A");

        auto old = cornerColors;

        // Atrás horário olhando diretamente para a face traseira
        std::vector<int> temp = next.cornerColors[2];
        next.cornerColors[2] = next.cornerColors[3];
        next.cornerColors[3] = next.cornerColors[7];
        next.cornerColors[7] = next.cornerColors[6];
        next.cornerColors[6] = temp;

        // Cima/Baixo <-> Esquerda/Direita
        for (int i : {2, 3, 6, 7}) {
            std::swap(
                next.cornerColors[i][0],
                next.cornerColors[i][2]
            );
        }

        return next;
    }

    //AQUI OS MOVIMENTOS ANTI-HORARIO

    //movimento de baixo anti-horário (N)
    CubeState moveN() const {
    CubeState next = *this;
    next.path.push_back("N");

    auto old = cornerColors;

    // Bottom horário olhando diretamente para a face de baixo
    next.cornerColors[6] = old[4]; // DFL -> DBL
    next.cornerColors[7] = old[6]; // DBL -> DBR
    next.cornerColors[5] = old[7]; // DBR -> DFR
    next.cornerColors[4] = old[5]; // DFR -> DFL

    // Frente/Trás <-> Esquerda/Direita
    for (int i : {4, 5, 6, 7}) {
        std::swap(
            next.cornerColors[i][1],
            next.cornerColors[i][2]
        );
    }

    return next;
    }
    
    // Movimento de cima anti-horário (I) (Topo - Gira as quinas 0, 1, 3, 2)
    CubeState moveI() const {
        CubeState next = *this;
        next.path.push_back("I");
        
        // Permutação das quinas do topo
        std::vector<int> temp = next.cornerColors[0];
        next.cornerColors[0] = next.cornerColors[2];
        next.cornerColors[2] = next.cornerColors[3];
        next.cornerColors[3] = next.cornerColors[1];
        next.cornerColors[1] = temp;

        for (int i : {0, 1, 2, 3}) {
            std::swap(
                next.cornerColors[i][1],
                next.cornerColors[i][2]
            );
        }
        return next;
    }

    //movimento da esquerda no sentido anti-horário (K)
    CubeState moveK() const {
        CubeState next = *this;
        next.path.push_back("K");

        auto old = cornerColors;

        // L anti-horário olhando diretamente para a face esquerda
        std::vector<int> temp = next.cornerColors[0];
        next.cornerColors[0] = next.cornerColors[4];
        next.cornerColors[4] = next.cornerColors[6];
        next.cornerColors[6] = next.cornerColors[2];
        next.cornerColors[2] = temp;

        // Cima/Baixo <-> Frente/Trás
        for (int i : {0, 2, 4, 6}) {
            std::swap(
                next.cornerColors[i][0],
                next.cornerColors[i][1]
            );
        }

        return next;
    }

    //movimento atrás anti-horário (S)
    CubeState moveS() const {
        CubeState next = *this;
        next.path.push_back("S");

        auto old = cornerColors;

        // Atrás anti-horário olhando diretamente para a face traseira
        std::vector<int> temp = next.cornerColors[2];
        next.cornerColors[2] = next.cornerColors[6];
        next.cornerColors[6] = next.cornerColors[7];
        next.cornerColors[7] = next.cornerColors[3];
        next.cornerColors[3] = temp;

        // Cima/Baixo <-> Esquerda/Direita
        for (int i : {2, 3, 6, 7}) {
            std::swap(
                next.cornerColors[i][0],
                next.cornerColors[i][2]
            );
        }

        return next;
    }

    //movimento da frente no sentido anti-horário (G)
    CubeState moveG() const {
    CubeState next = *this;
    next.path.push_back("G");

    // Guarda o estado antes do movimento
    auto old = cornerColors;

    // Rotação das posições das 4 quinas da frente
    std::vector<int> temp = next.cornerColors[0];
    next.cornerColors[0] = next.cornerColors[1];
    next.cornerColors[1] = next.cornerColors[5];
    next.cornerColors[5] = next.cornerColors[4];
    next.cornerColors[4] = temp;

    // Ao girar a frente, os eixos CIMA/BAIXO e
    // ESQUERDA/DIREITA trocam de orientação.
    for (int i : {0, 1, 4, 5}) {
        std::swap(
            next.cornerColors[i][0],
            next.cornerColors[i][2]
        );
    }

    return next;
}
    CubeState moveT() const {
        CubeState next = *this;
        next.path.push_back("T");
            
        std::vector<int> temp = next.cornerColors[1];
        next.cornerColors[1] = next.cornerColors[3];
        next.cornerColors[3] = next.cornerColors[7];
        next.cornerColors[7] = next.cornerColors[5];
        next.cornerColors[5] = temp;

            // Corrige orientação
        for (int i : {1, 5, 7, 3}) {
            std::swap(
                next.cornerColors[i][0],
                next.cornerColors[i][1]
            );
        }

        return next;
    }


    std::vector<CubeState> getSuccessors() const {
        return {this->moveU(), this->moveR(), this->moveF()};
    }
};

#endif