#include "cubo/GeradorSucessores.hpp"

namespace cubo {

std::vector<Sucessor> GeradorSucessores::gerar(const EstadoCubo& estado) const {
    std::vector<Sucessor> resultado;
    resultado.reserve(todosMovimentos().size());
    for (auto movimento : todosMovimentos()) {
        resultado.push_back({estado.apos(movimento), movimento});
    }
    return resultado;
}

std::vector<Sucessor> GeradorSucessores::gerar(
    const EstadoCubo& estado, Movimento movimentoAnterior) const {
    std::vector<Sucessor> resultado;
    resultado.reserve(15);
    for (auto movimento : todosMovimentos()) {
        // Repetir a mesma face é redundante: U seguido de U equivale a U2.
        if (face(movimento) != face(movimentoAnterior)) {
            resultado.push_back({estado.apos(movimento), movimento});
        }
    }
    return resultado;
}

} // namespace cubo
