#include "cubo/AvaliadorEstado.hpp"
#include "cubo/Cubo.hpp"
#include "cubo/GeradorSucessores.hpp"

#include <cassert>
#include <array>
#include <iostream>
#include <unordered_set>

int main() {
    using namespace cubo;
    const auto resolvido = EstadoCubo::resolvido();
    assert(resolvido.estaResolvido());
    assert(resolvido.ehValido());

    for (auto movimento : todosMovimentos()) {
        const auto movido = resolvido.apos(movimento);
        assert(movido.ehValido());
        assert(movido.apos(inverso(movimento)) == resolvido);
    }

    for (auto giro : {Movimento::U, Movimento::R, Movimento::F,
                      Movimento::D, Movimento::L, Movimento::B}) {
        auto estado = resolvido;
        for (int i = 0; i < 4; ++i) estado = estado.apos(giro);
        assert(estado == resolvido);
    }

    Cubo a, b;
    const auto sequenciaA = a.embaralhar(12345, 20);
    const auto sequenciaB = b.embaralhar(12345, 20);
    assert(sequenciaA == sequenciaB);
    assert(a.estado() == b.estado());

    for (auto it = sequenciaA.rbegin(); it != sequenciaA.rend(); ++it) {
        a.mover(inverso(*it));
    }
    assert(a.estado().estaResolvido());

    GeradorSucessores gerador;
    const auto sucessores = gerador.gerar(resolvido);
    assert(sucessores.size() == 18);
    std::unordered_set<EstadoCubo, HashEstadoCubo> unicos;
    for (const auto& sucessor : sucessores) unicos.insert(sucessor.estado);
    assert(unicos.size() == 18);
    assert(gerador.gerar(resolvido, Movimento::R).size() == 15);

    AvaliadorEstado avaliador;
    assert(avaliador.ehObjetivo(resolvido));
    assert(avaliador.heuristicaCantos(resolvido) == 0);
    assert(avaliador.heuristicaCantos(resolvido.apos(Movimento::R)) == 1);

    const auto adesivosResolvidos = resolvido.adesivos();
    for (int f = 0; f < 6; ++f) {
        for (int i = 1; i < 4; ++i) {
            assert(adesivosResolvidos[f * 4] == adesivosResolvidos[f * 4 + i]);
        }
    }
    std::array<int, 6> contagemCores{};
    for (auto cor : a.estado().adesivos()) {
        ++contagemCores[static_cast<std::size_t>(cor)];
    }
    for (int quantidade : contagemCores) assert(quantidade == 4);

    std::cout << "Todos os testes da lógica passaram.\n";
}
