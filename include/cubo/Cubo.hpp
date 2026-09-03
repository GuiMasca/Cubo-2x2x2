#pragma once

#include "cubo/EstadoCubo.hpp"

#include <cstdint>
#include <vector>

namespace cubo {

class Cubo {
public:
    Cubo();
    explicit Cubo(EstadoCubo estado);

    const EstadoCubo& estado() const noexcept;
    void mover(Movimento movimento);
    void reiniciar();

    // A mesma seed e quantidade sempre produzem o mesmo embaralhamento.
    std::vector<Movimento> embaralhar(std::uint32_t seed, int quantidade = 10);

private:
    EstadoCubo estado_;
};

} // namespace cubo
