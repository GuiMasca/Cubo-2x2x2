#pragma once

#include "cubo/EstadoCubo.hpp"

#include <vector>

namespace cubo {

struct Sucessor {
    EstadoCubo estado;
    Movimento movimento;
};

class GeradorSucessores {
public:
    std::vector<Sucessor> gerar(const EstadoCubo& estado) const;
    std::vector<Sucessor> gerar(const EstadoCubo& estado,
                                Movimento movimentoAnterior) const;
};

} // namespace cubo
