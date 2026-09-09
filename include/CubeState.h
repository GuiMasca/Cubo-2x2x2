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

    // Movimento U (Topo - Gira as quinas 0, 1, 3, 2 no sentido horário)
    CubeState moveU() const {
        CubeState next = *this;
        next.path.push_back("U");
        
        // Permutação das quinas do topo
        std::vector<int> temp = next.cornerColors[0];
        next.cornerColors[0] = next.cornerColors[2];
        next.cornerColors[2] = next.cornerColors[3];
        next.cornerColors[3] = next.cornerColors[1];
        next.cornerColors[1] = temp;

        return next;
    }

    // Movimento R (Direita - Gira as quinas 1, 5, 7, 3)
    CubeState moveR() const {
        CubeState next = *this;
        next.path.push_back("R");
        
        std::vector<int> temp = next.cornerColors[1];
        next.cornerColors[1] = next.cornerColors[3];
        next.cornerColors[3] = next.cornerColors[7];
        next.cornerColors[7] = next.cornerColors[5];
        next.cornerColors[5] = temp;

        return next;
    }

    // Movimento F (Frente - Gira as quinas 0, 1, 5, 4)
    CubeState moveF() const {
        CubeState next = *this;
        next.path.push_back("F");
        
        std::vector<int> temp = next.cornerColors[0];
        next.cornerColors[0] = next.cornerColors[4];
        next.cornerColors[4] = next.cornerColors[5];
        next.cornerColors[5] = next.cornerColors[1];
        next.cornerColors[1] = temp;

        return next;
    }

    std::vector<CubeState> getSuccessors() const {
        return {this->moveU(), this->moveR(), this->moveF()};
    }
};

#endif