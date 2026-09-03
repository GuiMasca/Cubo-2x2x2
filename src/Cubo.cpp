#include "cubo/Cubo.hpp"

#include <random>
#include <stdexcept>

namespace cubo {

Cubo::Cubo() : estado_(EstadoCubo::resolvido()) {}
Cubo::Cubo(EstadoCubo estado) : estado_(estado) {}
const EstadoCubo& Cubo::estado() const noexcept { return estado_; }
void Cubo::mover(Movimento movimento) { estado_ = estado_.apos(movimento); }
void Cubo::reiniciar() { estado_ = EstadoCubo::resolvido(); }

std::vector<Movimento> Cubo::embaralhar(std::uint32_t seed, int quantidade) {
    if (quantidade < 0) throw std::invalid_argument("Quantidade não pode ser negativa");
    std::mt19937 gerador(seed);
    std::uniform_int_distribution<int> distribuicao(0, 17);
    std::vector<Movimento> sequencia;
    sequencia.reserve(static_cast<std::size_t>(quantidade));
    char ultimaFace = '\0';
    while (static_cast<int>(sequencia.size()) < quantidade) {
        auto movimento = static_cast<Movimento>(distribuicao(gerador));
        if (face(movimento) == ultimaFace) continue;
        mover(movimento);
        sequencia.push_back(movimento);
        ultimaFace = face(movimento);
    }
    return sequencia;
}

} // namespace cubo
