#include "cubo/AvaliadorEstado.hpp"

namespace cubo {

bool AvaliadorEstado::ehObjetivo(const EstadoCubo& estado) const noexcept {
    return estado.estaResolvido();
}

int AvaliadorEstado::heuristicaCantos(const EstadoCubo& estado) const noexcept {
    int foraDaPosicao = 0;
    int malOrientados = 0;

    for (std::size_t i = 0; i < 8; ++i) {
        if (estado.permutacao()[i] != static_cast<int>(i)) {
            foraDaPosicao++;
        }

        if (estado.orientacao()[i] != 0) {
            malOrientados++;
        }
    }

    // Um movimento mexe em quatro cantos. A divisão é arredondada para cima.
    const int limitePosicao = (foraDaPosicao + 3) / 4;
    const int limiteOrientacao = (malOrientados + 3) / 4;

    if (limitePosicao > limiteOrientacao) {
        return limitePosicao;
    }
    return limiteOrientacao;
}

} // namespace cubo
