#include "cubo/Cubo.hpp"

#include <random>
#include <stdexcept>

namespace cubo {

Cubo::Cubo() {
    estado_ = EstadoCubo::resolvido();
}

Cubo::Cubo(EstadoCubo estadoInicial) {
    estado_ = estadoInicial;
}

const EstadoCubo& Cubo::estado() const noexcept {
    return estado_;
}

void Cubo::mover(Movimento movimento) {
    estado_ = estado_.apos(movimento);
}

void Cubo::reiniciar() {
    estado_ = EstadoCubo::resolvido();
}

std::vector<Movimento> Cubo::embaralhar(std::uint32_t seed, int quantidade) {
    if (quantidade < 0) {
        throw std::invalid_argument("Quantidade não pode ser negativa");
    }

    std::mt19937 gerador(seed);
    std::uniform_int_distribution<int> distribuicao(0, 17);

    std::vector<Movimento> sequencia;
    char ultimaFace = '\0';

    while (static_cast<int>(sequencia.size()) < quantidade) {
        int numeroSorteado = distribuicao(gerador);
        Movimento movimento = static_cast<Movimento>(numeroSorteado);

        // Dois giros seguidos na mesma face seriam um embaralhamento ruim.
        // Por exemplo, R seguido de R' se anularia.
        if (face(movimento) == ultimaFace) {
            continue;
        }

        mover(movimento);
        sequencia.push_back(movimento);
        ultimaFace = face(movimento);
    }
    return sequencia;
}

} // namespace cubo
