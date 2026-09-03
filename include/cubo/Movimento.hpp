#pragma once

#include <string>
#include <vector>

namespace cubo {

enum class Movimento {
    U, U2, Ui,
    R, R2, Ri,
    F, F2, Fi,
    D, D2, Di,
    L, L2, Li,
    B, B2, Bi
};

const std::vector<Movimento>& todosMovimentos();
std::string paraString(Movimento movimento);
Movimento inverso(Movimento movimento);
int quantidadeGirosHorario(Movimento movimento);
char face(Movimento movimento);

} // namespace cubo
