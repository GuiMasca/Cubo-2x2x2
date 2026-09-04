#pragma once

#include <array>

// Este arquivo concentra os textos e as cores da interface.
// Para trocar uma cor ou o nome de uma face, basta alterar a tabela abaixo.
namespace visual {

struct Face {
    const char* codigo;
    const char* nome;
    const char* cor;
    bool usarTextoEscuro;
};

inline constexpr std::array<Face, 6> FACES = {{
    {"U", "Cima",     "#f7f7f7", true},
    {"R", "Direita",  "#d62828", false},
    {"F", "Frente",   "#20a05a", false},
    {"D", "Baixo",    "#ffd60a", true},
    {"L", "Esquerda", "#f77f00", true},
    {"B", "Trás",     "#277da1", false}
}};

inline constexpr const char* COR_FUNDO = "#171a21";
inline constexpr const char* COR_TEXTO_SECUNDARIO = "#bdc7d5";

} // namespace visual
