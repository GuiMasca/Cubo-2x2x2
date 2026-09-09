#ifndef CUBESTATE_H
#define CUBESTATE_H

#include <vector>
#include <string>
#include <algorithm>

struct CubeState {
    // 24 adesivos: 6 faces com 4 adesivos cada
    // 0 = Top (U), 1 = Bottom (D), 2 = Front (F), 3 = Back (B), 4 = Left (L), 5 = Right (R)
    std::vector<int> faces;
    std::vector<std::string> path;

    CubeState() {
        faces.resize(24);
        for (int i = 0; i < 24; ++i) {
            faces[i] = i / 4; 
        }
    }

    bool isGoal() const {
        for (int i = 0; i < 24; i += 4) {
            for (int j = 1; j < 4; ++j) {
                if (faces[i + j] != faces[i]) return false;
            }
        }
        return true;
    }

    void rotateFaceClockwise(int f) {
        int base = f * 4;
        int temp = faces[base + 0];
        faces[base + 0] = faces[base + 2];
        faces[base + 2] = faces[base + 3];
        faces[base + 3] = faces[base + 1];
        faces[base + 1] = temp;
    }

    void rotateFaceCounterClockwise(int f) {
        rotateFaceClockwise(f);
        rotateFaceClockwise(f);
        rotateFaceClockwise(f);
    }

    // Movimento U (Topo)
    CubeState moveU() const {
        CubeState next = *this;
        next.path.push_back("U");
        next.rotateFaceClockwise(0);

        int t0 = next.faces[8];
        int t1 = next.faces[9];

        next.faces[8] = next.faces[16];
        next.faces[9] = next.faces[17];

        next.faces[16] = next.faces[12];
        next.faces[17] = next.faces[13];

        next.faces[12] = next.faces[20];
        next.faces[13] = next.faces[21];

        next.faces[20] = t0;
        next.faces[21] = t1;

        return next;
    }

    // Movimento R (Direita)
    CubeState moveR() const {
        CubeState next = *this;
        next.path.push_back("R");
        next.rotateFaceClockwise(5);

        int t0 = next.faces[2];
        int t1 = next.faces[3];

        next.faces[2] = next.faces[10];
        next.faces[3] = next.faces[11];

        next.faces[10] = next.faces[6];
        next.faces[11] = next.faces[7];

        next.faces[6] = next.faces[14];
        next.faces[7] = next.faces[15];

        next.faces[14] = t0;
        next.faces[15] = t1;

        return next;
    }

    // Movimento F (Frente)
    CubeState moveF() const {
        CubeState next = *this;
        next.path.push_back("F");
        next.rotateFaceClockwise(2);

        int t0 = next.faces[2];
        int t1 = next.faces[3];

        next.faces[2] = next.faces[19];
        next.faces[3] = next.faces[17];

        next.faces[17] = next.faces[5];
        next.faces[19] = next.faces[4];

        next.faces[5] = next.faces[12];
        next.faces[4] = next.faces[14];

        next.faces[12] = t0;
        next.faces[14] = t1;

        return next;
    }

    // Função Sucessora gerando todos os movimentos básicos (U, R, F e inversos)
    std::vector<CubeState> getSuccessors() const {
        std::vector<CubeState> successors;
        successors.push_back(this->moveU());
        successors.push_back(this->moveR());
        successors.push_back(this->moveF());
        return successors;
    }
};

#endif