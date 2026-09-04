#include "cubo/Movimento.hpp"

#include <stdexcept>

namespace cubo {

const std::vector<Movimento>& todosMovimentos() {
    static const std::vector<Movimento> movimentos = {
        Movimento::U, Movimento::U2, Movimento::Ui,
        Movimento::R, Movimento::R2, Movimento::Ri,
        Movimento::F, Movimento::F2, Movimento::Fi,
        Movimento::D, Movimento::D2, Movimento::Di,
        Movimento::L, Movimento::L2, Movimento::Li,
        Movimento::B, Movimento::B2, Movimento::Bi
    };
    return movimentos;
}

std::string paraString(Movimento movimento) {
    switch (movimento) {
        case Movimento::U:  return "U";
        case Movimento::U2: return "U2";
        case Movimento::Ui: return "U'";
        case Movimento::R:  return "R";
        case Movimento::R2: return "R2";
        case Movimento::Ri: return "R'";
        case Movimento::F:  return "F";
        case Movimento::F2: return "F2";
        case Movimento::Fi: return "F'";
        case Movimento::D:  return "D";
        case Movimento::D2: return "D2";
        case Movimento::Di: return "D'";
        case Movimento::L:  return "L";
        case Movimento::L2: return "L2";
        case Movimento::Li: return "L'";
        case Movimento::B:  return "B";
        case Movimento::B2: return "B2";
        case Movimento::Bi: return "B'";
    }
    throw std::invalid_argument("Movimento desconhecido");
}

Movimento inverso(Movimento movimento) {
    switch (movimento) {
        case Movimento::U:  return Movimento::Ui;
        case Movimento::Ui: return Movimento::U;
        case Movimento::R:  return Movimento::Ri;
        case Movimento::Ri: return Movimento::R;
        case Movimento::F:  return Movimento::Fi;
        case Movimento::Fi: return Movimento::F;
        case Movimento::D:  return Movimento::Di;
        case Movimento::Di: return Movimento::D;
        case Movimento::L:  return Movimento::Li;
        case Movimento::Li: return Movimento::L;
        case Movimento::B:  return Movimento::Bi;
        case Movimento::Bi: return Movimento::B;

        // Um giro de 180 graus é o inverso dele mesmo.
        case Movimento::U2:
        case Movimento::R2:
        case Movimento::F2:
        case Movimento::D2:
        case Movimento::L2:
        case Movimento::B2:
            return movimento;
    }
    throw std::invalid_argument("Movimento desconhecido");
}

int quantidadeGirosHorario(Movimento movimento) {
    switch (movimento) {
        case Movimento::U:
        case Movimento::R:
        case Movimento::F:
        case Movimento::D:
        case Movimento::L:
        case Movimento::B:
            return 1;

        case Movimento::U2:
        case Movimento::R2:
        case Movimento::F2:
        case Movimento::D2:
        case Movimento::L2:
        case Movimento::B2:
            return 2;

        // Um giro anti-horário equivale a três giros horários.
        case Movimento::Ui:
        case Movimento::Ri:
        case Movimento::Fi:
        case Movimento::Di:
        case Movimento::Li:
        case Movimento::Bi:
            return 3;
    }
    throw std::invalid_argument("Movimento desconhecido");
}

char face(Movimento movimento) {
    switch (movimento) {
        case Movimento::U:
        case Movimento::U2:
        case Movimento::Ui:
            return 'U';

        case Movimento::R:
        case Movimento::R2:
        case Movimento::Ri:
            return 'R';

        case Movimento::F:
        case Movimento::F2:
        case Movimento::Fi:
            return 'F';

        case Movimento::D:
        case Movimento::D2:
        case Movimento::Di:
            return 'D';

        case Movimento::L:
        case Movimento::L2:
        case Movimento::Li:
            return 'L';

        case Movimento::B:
        case Movimento::B2:
        case Movimento::Bi:
            return 'B';
    }
    throw std::invalid_argument("Movimento desconhecido");
}

} // namespace cubo
