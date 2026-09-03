#include "cubo/Movimento.hpp"

#include <array>
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
    static const std::array<const char*, 18> nomes = {
        "U", "U2", "U'", "R", "R2", "R'", "F", "F2", "F'",
        "D", "D2", "D'", "L", "L2", "L'", "B", "B2", "B'"
    };
    return nomes.at(static_cast<std::size_t>(movimento));
}

Movimento inverso(Movimento movimento) {
    const int valor = static_cast<int>(movimento);
    const int resto = valor % 3;
    if (resto == 0) return static_cast<Movimento>(valor + 2);
    if (resto == 2) return static_cast<Movimento>(valor - 2);
    return movimento;
}

int quantidadeGirosHorario(Movimento movimento) {
    static constexpr int giros[] = {1, 2, 3};
    return giros[static_cast<int>(movimento) % 3];
}

char face(Movimento movimento) {
    static constexpr char faces[] = {'U', 'R', 'F', 'D', 'L', 'B'};
    return faces[static_cast<int>(movimento) / 3];
}

} // namespace cubo
