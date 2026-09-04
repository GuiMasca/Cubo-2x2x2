#include "cubo/EstadoCubo.hpp"

#include <array>
#include <sstream>
#include <stdexcept>

namespace cubo {
namespace {

// Cada posição do vetor representa um lugar do cubo:
// 0=URF, 1=UFL, 2=ULB, 3=UBR, 4=DFR, 5=DLF, 6=DBL, 7=DRB.
//
// Exemplo: se origemDaPeca[0] vale 3, a nova posição 0 recebe a peça
// que antes estava na posição 3. Assim evitamos oito atribuições por giro.
struct RegraDeGiro {
    EstadoCubo::VetorCantos origemDaPeca;
    EstadoCubo::VetorCantos giroDaPeca;
};

const RegraDeGiro& regraDaFace(char faceEscolhida) {
    static const RegraDeGiro cima = {
        {3, 0, 1, 2, 4, 5, 6, 7},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };
    static const RegraDeGiro direita = {
        {4, 1, 2, 0, 7, 5, 6, 3},
        {2, 0, 0, 1, 1, 0, 0, 2}
    };
    static const RegraDeGiro frente = {
        {1, 5, 2, 3, 0, 4, 6, 7},
        {1, 2, 0, 0, 2, 1, 0, 0}
    };
    static const RegraDeGiro baixo = {
        {0, 1, 2, 3, 5, 6, 7, 4},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };
    static const RegraDeGiro esquerda = {
        {0, 2, 6, 3, 4, 1, 5, 7},
        {0, 1, 2, 0, 0, 2, 1, 0}
    };
    static const RegraDeGiro tras = {
        {0, 1, 3, 7, 4, 5, 2, 6},
        {0, 0, 1, 2, 0, 0, 2, 1}
    };

    switch (faceEscolhida) {
        case 'U': return cima;
        case 'R': return direita;
        case 'F': return frente;
        case 'D': return baixo;
        case 'L': return esquerda;
        case 'B': return tras;
    }

    throw std::invalid_argument("Face de cubo desconhecida");
}

} // namespace

EstadoCubo::EstadoCubo()
    : permutacao_{0,1,2,3,4,5,6,7}, orientacao_{0,0,0,0,0,0,0,0} {}

EstadoCubo::EstadoCubo(VetorCantos permutacao, VetorCantos orientacao)
    : permutacao_(permutacao), orientacao_(orientacao) {
    if (!ehValido()) {
        throw std::invalid_argument("Estado de cubo inválido");
    }
}

EstadoCubo EstadoCubo::resolvido() {
    return EstadoCubo();
}

const EstadoCubo::VetorCantos& EstadoCubo::permutacao() const noexcept {
    return permutacao_;
}

const EstadoCubo::VetorCantos& EstadoCubo::orientacao() const noexcept {
    return orientacao_;
}

bool EstadoCubo::estaResolvido() const noexcept {
    return *this == EstadoCubo::resolvido();
}

bool EstadoCubo::ehValido() const noexcept {
    std::array<bool, 8> pecaJaEncontrada = {false, false, false, false,
                                            false, false, false, false};
    int somaOrientacoes = 0;

    for (std::size_t i = 0; i < 8; ++i) {
        int numeroDaPeca = permutacao_[i];

        if (numeroDaPeca < 0 || numeroDaPeca >= 8) {
            return false;
        }
        if (pecaJaEncontrada[numeroDaPeca]) {
            return false;
        }
        if (orientacao_[i] < 0 || orientacao_[i] >= 3) {
            return false;
        }

        pecaJaEncontrada[numeroDaPeca] = true;
        somaOrientacoes += orientacao_[i];
    }

    return somaOrientacoes % 3 == 0;
}

EstadoCubo EstadoCubo::giroHorario(char faceEscolhida) const {
    const RegraDeGiro& regra = regraDaFace(faceEscolhida);
    VetorCantos novaPermutacao{};
    VetorCantos novaOrientacao{};

    for (int novaPosicao = 0; novaPosicao < 8; ++novaPosicao) {
        int posicaoAntiga = regra.origemDaPeca[novaPosicao];

        novaPermutacao[novaPosicao] = permutacao_[posicaoAntiga];
        novaOrientacao[novaPosicao] =
            (orientacao_[posicaoAntiga] + regra.giroDaPeca[novaPosicao]) % 3;
    }

    return EstadoCubo(novaPermutacao, novaOrientacao);
}

EstadoCubo EstadoCubo::apos(Movimento movimento) const {
    EstadoCubo resultado = *this;
    int quantidade = quantidadeGirosHorario(movimento);

    for (int giro = 0; giro < quantidade; ++giro) {
        resultado = resultado.giroHorario(face(movimento));
    }
    return resultado;
}

std::array<Cor, 24> EstadoCubo::adesivos() const noexcept {
    // Cada canto tem três adesivos. Esta tabela diz onde os três adesivos de
    // cada posição aparecem no desenho. As faces estão na ordem U,R,F,D,L,B.
    static const std::array<std::array<int, 3>, 8> posicoesDosAdesivos = {{
        {{ 3,  4,  9}}, // URF
        {{ 2,  8, 17}}, // UFL
        {{ 0, 16, 21}}, // ULB
        {{ 1, 20,  5}}, // UBR
        {{13, 11,  6}}, // DFR
        {{12, 19, 10}}, // DLF
        {{14, 23, 18}}, // DBL
        {{15,  7, 22}}  // DRB
    }};

    static const std::array<std::array<Cor, 3>, 8> coresDasPecas = {{
        {{Cor::Branco,  Cor::Vermelho, Cor::Verde}},
        {{Cor::Branco,  Cor::Verde,    Cor::Laranja}},
        {{Cor::Branco,  Cor::Laranja,  Cor::Azul}},
        {{Cor::Branco,  Cor::Azul,     Cor::Vermelho}},
        {{Cor::Amarelo, Cor::Verde,    Cor::Vermelho}},
        {{Cor::Amarelo, Cor::Laranja,  Cor::Verde}},
        {{Cor::Amarelo, Cor::Azul,     Cor::Laranja}},
        {{Cor::Amarelo, Cor::Vermelho, Cor::Azul}}
    }};

    std::array<Cor, 24> resultado{};

    for (int posicao = 0; posicao < 8; ++posicao) {
        int peca = permutacao_[posicao];
        int orientacao = orientacao_[posicao];

        for (int ladoDaPeca = 0; ladoDaPeca < 3; ++ladoDaPeca) {
            int ladoNaPosicao = (ladoDaPeca + orientacao) % 3;
            int adesivo = posicoesDosAdesivos[posicao][ladoNaPosicao];
            resultado[adesivo] = coresDasPecas[peca][ladoDaPeca];
        }
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
    // Monta um número como fazemos no sistema decimal, porém usando base 8
    // para as peças e base 3 para as orientações.
    std::size_t codigo = 0;

    for (int peca : estado.permutacao()) {
        codigo = codigo * 8 + static_cast<std::size_t>(peca);
    }
    for (int orientacao : estado.orientacao()) {
        codigo = codigo * 3 + static_cast<std::size_t>(orientacao);
    }

    return codigo;
}

} // namespace cubo
