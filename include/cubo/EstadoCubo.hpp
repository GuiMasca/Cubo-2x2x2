#pragma once

#include "cubo/Movimento.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

namespace cubo {

enum class Cor : std::uint8_t { Branco, Vermelho, Verde, Amarelo, Laranja, Azul };

// Posições e peças seguem a ordem:
// URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB.
class EstadoCubo {
public:
    using VetorCantos = std::array<std::uint8_t, 8>;

    EstadoCubo();
    EstadoCubo(VetorCantos permutacao, VetorCantos orientacao);

    static EstadoCubo resolvido();

    const VetorCantos& permutacao() const noexcept;
    const VetorCantos& orientacao() const noexcept;
    bool estaResolvido() const noexcept;
    bool ehValido() const noexcept;

    EstadoCubo apos(Movimento movimento) const;
    std::array<Cor, 24> adesivos() const noexcept;
    std::string descricao() const;

    bool operator==(const EstadoCubo& outro) const noexcept;
    bool operator!=(const EstadoCubo& outro) const noexcept;

private:
    VetorCantos permutacao_;
    VetorCantos orientacao_;

    EstadoCubo giroHorario(char face) const;
};

struct HashEstadoCubo {
    std::size_t operator()(const EstadoCubo& estado) const noexcept;
};

} // namespace cubo
