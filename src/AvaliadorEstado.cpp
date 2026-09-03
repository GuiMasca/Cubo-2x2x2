#include "cubo/AvaliadorEstado.hpp"

#include <algorithm>

namespace cubo {

bool AvaliadorEstado::ehObjetivo(const EstadoCubo& estado) const noexcept {
    return estado.estaResolvido();
}

int AvaliadorEstado::heuristicaCantos(const EstadoCubo& estado) const noexcept {
    int foraDaPosicao = 0;
    int malOrientados = 0;
    for (std::size_t i = 0; i < 8; ++i) {
        foraDaPosicao += estado.permutacao()[i] != i;
        malOrientados += estado.orientacao()[i] != 0;
    }
    const int limitePosicao = (foraDaPosicao + 3) / 4;
    const int limiteOrientacao = (malOrientados + 3) / 4;
    return std::max(limitePosicao, limiteOrientacao);
}

} // namespace cubo
