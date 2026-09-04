#include "cubo/GeradorSucessores.hpp"

namespace cubo {

std::vector<Sucessor> GeradorSucessores::gerar(const EstadoCubo& estado) const {
    std::vector<Sucessor> resultado;

    for (auto movimento : todosMovimentos()) {
        Sucessor novoSucessor;
        novoSucessor.estado = estado.apos(movimento);
        novoSucessor.movimento = movimento;
        resultado.push_back(novoSucessor);
    }
    return resultado;
}

std::vector<Sucessor> GeradorSucessores::gerar(
    const EstadoCubo& estado, Movimento movimentoAnterior) const {
    std::vector<Sucessor> resultado;

    for (auto movimento : todosMovimentos()) {
        // Repetir a mesma face é redundante: U seguido de U equivale a U2.
        if (face(movimento) != face(movimentoAnterior)) {
            Sucessor novoSucessor;
            novoSucessor.estado = estado.apos(movimento);
            novoSucessor.movimento = movimento;
            resultado.push_back(novoSucessor);
        }
    }
    return resultado;
}

} // namespace cubo
