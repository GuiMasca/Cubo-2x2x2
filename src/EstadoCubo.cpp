#include "cubo/EstadoCubo.hpp"

#include <algorithm>
#include <array>
#include <sstream>
#include <stdexcept>

namespace cubo {
namespace {

struct TabelaGiro {
    EstadoCubo::VetorCantos origem;
    EstadoCubo::VetorCantos mudancaOrientacao;
};

const TabelaGiro& tabela(char face) {
    static const TabelaGiro U{{3,0,1,2,4,5,6,7}, {0,0,0,0,0,0,0,0}};
    static const TabelaGiro R{{4,1,2,0,7,5,6,3}, {2,0,0,1,1,0,0,2}};
    static const TabelaGiro F{{1,5,2,3,0,4,6,7}, {1,2,0,0,2,1,0,0}};
    static const TabelaGiro D{{0,1,2,3,5,6,7,4}, {0,0,0,0,0,0,0,0}};
    static const TabelaGiro L{{0,2,6,3,4,1,5,7}, {0,1,2,0,0,2,1,0}};
    static const TabelaGiro B{{0,1,3,7,4,5,2,6}, {0,0,1,2,0,0,2,1}};
    switch (face) {
        case 'U': return U; case 'R': return R; case 'F': return F;
        case 'D': return D; case 'L': return L; case 'B': return B;
        default: throw std::invalid_argument("Face de cubo desconhecida");
    }
}

} // namespace

EstadoCubo::EstadoCubo()
    : permutacao_{0,1,2,3,4,5,6,7}, orientacao_{0,0,0,0,0,0,0,0} {}

EstadoCubo::EstadoCubo(VetorCantos permutacao, VetorCantos orientacao)
    : permutacao_(permutacao), orientacao_(orientacao) {
    if (!ehValido()) throw std::invalid_argument("Estado de cubo inválido");
}

EstadoCubo EstadoCubo::resolvido() { return {}; }
const EstadoCubo::VetorCantos& EstadoCubo::permutacao() const noexcept { return permutacao_; }
const EstadoCubo::VetorCantos& EstadoCubo::orientacao() const noexcept { return orientacao_; }

bool EstadoCubo::estaResolvido() const noexcept {
    return *this == EstadoCubo::resolvido();
}

bool EstadoCubo::ehValido() const noexcept {
    std::array<bool, 8> vista{};
    int somaOrientacoes = 0;
    for (std::size_t i = 0; i < 8; ++i) {
        if (permutacao_[i] >= 8 || vista[permutacao_[i]] || orientacao_[i] >= 3) return false;
        vista[permutacao_[i]] = true;
        somaOrientacoes += orientacao_[i];
    }
    return somaOrientacoes % 3 == 0;
}

EstadoCubo EstadoCubo::giroHorario(char faceEscolhida) const {
    const auto& giro = tabela(faceEscolhida);
    VetorCantos novaPermutacao{};
    VetorCantos novaOrientacao{};
    for (std::size_t pos = 0; pos < 8; ++pos) {
        const auto origem = giro.origem[pos];
        novaPermutacao[pos] = permutacao_[origem];
        novaOrientacao[pos] = static_cast<std::uint8_t>(
            (orientacao_[origem] + giro.mudancaOrientacao[pos]) % 3);
    }
    return EstadoCubo(novaPermutacao, novaOrientacao);
}

EstadoCubo EstadoCubo::apos(Movimento movimento) const {
    EstadoCubo resultado = *this;
    for (int i = 0; i < quantidadeGirosHorario(movimento); ++i) {
        resultado = resultado.giroHorario(face(movimento));
    }
    return resultado;
}

std::array<Cor, 24> EstadoCubo::adesivos() const noexcept {
    // Faces U, R, F, D, L, B; adesivos: sup.esq., sup.dir., inf.esq., inf.dir.
    static constexpr std::array<std::array<int, 3>, 8> facelets = {{
        {{3,4,9}}, {{2,8,17}}, {{0,16,21}}, {{1,20,5}},
        {{13,11,6}}, {{12,19,10}}, {{14,23,18}}, {{15,7,22}}
    }};
    static constexpr std::array<std::array<Cor, 3>, 8> coresPecas = {{
        {{Cor::Branco,Cor::Vermelho,Cor::Verde}},
        {{Cor::Branco,Cor::Verde,Cor::Laranja}},
        {{Cor::Branco,Cor::Laranja,Cor::Azul}},
        {{Cor::Branco,Cor::Azul,Cor::Vermelho}},
        {{Cor::Amarelo,Cor::Verde,Cor::Vermelho}},
        {{Cor::Amarelo,Cor::Laranja,Cor::Verde}},
        {{Cor::Amarelo,Cor::Azul,Cor::Laranja}},
        {{Cor::Amarelo,Cor::Vermelho,Cor::Azul}}
    }};
    std::array<Cor, 24> resultado{};
    for (std::size_t pos = 0; pos < 8; ++pos) {
        const auto peca = permutacao_[pos];
        const auto ori = orientacao_[pos];
        for (std::size_t n = 0; n < 3; ++n)
            resultado[facelets[pos][(n + ori) % 3]] = coresPecas[peca][n];
    }
    return resultado;
}

std::string EstadoCubo::descricao() const {
    static constexpr const char* nomes[] = {"URF","UFL","ULB","UBR","DFR","DLF","DBL","DRB"};
    std::ostringstream saida;
    saida << "Posição :";
    for (const auto* nome : nomes) saida << ' ' << nome;
    saida << "\nPeça    :";
    for (auto p : permutacao_) saida << ' ' << nomes[p];
    saida << "\nOrient. :";
    for (auto o : orientacao_) saida << "   " << static_cast<int>(o);
    return saida.str();
}

bool EstadoCubo::operator==(const EstadoCubo& outro) const noexcept {
    return permutacao_ == outro.permutacao_ && orientacao_ == outro.orientacao_;
}
bool EstadoCubo::operator!=(const EstadoCubo& outro) const noexcept { return !(*this == outro); }

std::size_t HashEstadoCubo::operator()(const EstadoCubo& estado) const noexcept {
    std::size_t hash = 1469598103934665603ULL;
    for (auto valor : estado.permutacao()) hash = (hash ^ valor) * 1099511628211ULL;
    for (auto valor : estado.orientacao()) hash = (hash ^ valor) * 1099511628211ULL;
    return hash;
}

} // namespace cubo
