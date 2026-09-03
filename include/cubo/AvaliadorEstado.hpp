#pragma once

#include "cubo/EstadoCubo.hpp"

namespace cubo {

class AvaliadorEstado {
public:
    bool ehObjetivo(const EstadoCubo& estado) const noexcept;

    // Heurística simples e admissível: cada giro pode corrigir no máximo 4
    // cantos; usa o maior limite inferior entre posição e orientação.
    int heuristicaCantos(const EstadoCubo& estado) const noexcept;
};

} // namespace cubo
