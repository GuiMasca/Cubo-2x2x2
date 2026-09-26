#ifndef BUSCA_H
#define BUSCA_H

#include <cstddef>
#include <queue>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

#include "CubeState.h"


struct ResultadoBusca
{
    bool encontrou = false;
    std::size_t estadosVisitados = 0;
    std::vector<std::string> passos;

    // Usado pela profundidade iterativa.
    // Na largura permanece 0 e não interfere em nada.
    std::size_t limiteEncontrado = 0;
};


// FIFO: primeiro que entra é o primeiro que sai.
struct EstruturaBusca
{
    bool (*cancelamentoSolicitado)() = nullptr;

    std::queue<CubeState> estados;
    std::unordered_set<std::string> descobertos;

    void adicionar(const CubeState &estado)
    {
        std::string chave = estado.chave();

        if (descobertos.count(chave) == 0)
        {
            descobertos.insert(chave);
            estados.push(estado);
        }
    }

    CubeState remover()
    {
        CubeState estado = estados.front();
        estados.pop();

        return estado;
    }

    bool vazia() const
    {
        return estados.empty() ||
               (cancelamentoSolicitado != nullptr &&
                cancelamentoSolicitado());
    }
};


// LIFO: último que entra é o primeiro que sai.
struct EstruturaProfundidadeLimitada
{
    bool (*cancelamentoSolicitado)() = nullptr;

    std::size_t limite;

    std::vector<CubeState> estados;

    // Guarda a menor profundidade em que cada configuração
    // foi encontrada durante ESTA busca limitada.
    std::unordered_map<std::string, std::size_t> menorProfundidade;

    explicit EstruturaProfundidadeLimitada(
        std::size_t limiteBusca)
        : limite(limiteBusca)
    {
    }

    void adicionar(const CubeState &estado)
    {
        std::size_t profundidade =
            estado.path.size();

        // Estado ultrapassou o limite desta iteração.
        if (profundidade > limite)
        {
            return;
        }

        std::string chave =
            estado.chave();

        auto encontrado =
            menorProfundidade.find(chave);

        // Aceita: 1. configuração ainda não encontrada; 2. mesma configuração encontrada agora por um caminho mais curto.
        if (encontrado == menorProfundidade.end() ||
            profundidade < encontrado->second)
        {
            menorProfundidade[chave] =
                profundidade;

            estados.push_back(estado);
        }
    }

    CubeState remover()
    {
        CubeState estado =
            estados.back();

        estados.pop_back();

        return estado;
    }

    bool vazia() const
    {
        return estados.empty() ||
               (cancelamentoSolicitado != nullptr &&
                cancelamentoSolicitado());
    }
};

inline bool avaliarEstado(
    const CubeState &estado)
{
    return estado.isGoal();
}

inline std::size_t heuristica(const CubeState &estado)
{
    static const std::vector<std::vector<int>> resolvido = {
        {0, 2, 4}, {0, 2, 5}, {0, 3, 4}, {0, 3, 5},
        {1, 2, 4}, {1, 2, 5}, {1, 3, 4}, {1, 3, 5}
    };

    std::size_t quinasErradas = 0;

    for (int i = 0; i < 8; i++)
    {
        if (estado.cornerColors[i] != resolvido[i])
        {
            quinasErradas++;
        }
    }

    // Arredondamento para cima de quinasErradas / 4.
    return (quinasErradas + 3) / 4;
}

struct NoAEstrela
{
    CubeState estado;
    std::size_t f;
};

struct CompararNoAEstrela
{
    // std::priority_queue é um MAX-heap por padrão. Para virar um
    // min-heap (menor f primeiro), invertemos a comparação.
    bool operator()(const NoAEstrela &a, const NoAEstrela &b) const
    {
        return a.f > b.f;
    }
};

struct EstruturaAEstrela
{
    bool (*cancelamentoSolicitado)() = nullptr;

    std::priority_queue<NoAEstrela, std::vector<NoAEstrela>, CompararNoAEstrela> estados;

    std::unordered_set<std::string> fechados;

    void adicionar(const CubeState &estado)
    {
        std::string chave = estado.chave();

        if (fechados.count(chave) > 0)
        {
            return;
        }

        std::size_t g = estado.path.size();
        std::size_t h = heuristica(estado);

        estados.push({estado, g + h});
    }

    CubeState remover()
    {
        NoAEstrela no = estados.top();
        estados.pop();

        fechados.insert(no.estado.chave());

        return no.estado;
    }

    bool vazia()
    {
        while (!estados.empty() &&
               fechados.count(estados.top().estado.chave()) > 0)
        {
            estados.pop();
        }

        return estados.empty() ||
               (cancelamentoSolicitado != nullptr &&
                cancelamentoSolicitado());
    }
};

template <typename Estrutura>
ResultadoBusca executarBusca(
    CubeState estadoInicial,
    Estrutura &estrutura)
{
    ResultadoBusca resultado;

    // A busca começa no estado atual do cubo.
    
    estadoInicial.path.clear();

    estrutura.adicionar(
        estadoInicial);

    while (!estrutura.vazia())
    {
        CubeState atual =
            estrutura.remover();

        // Estado efetivamente retirado da estrutura e avaliado.
        resultado.estadosVisitados++;

        if (avaliarEstado(atual))
        {
            resultado.encontrou = true;
            resultado.passos = atual.path;

            return resultado;
        }

        // Função sucessora.
        std::vector<CubeState> proximos =
            atual.getSuccessors();

        for (std::size_t i = 0;
             i < proximos.size();
             i++)
        {
            estrutura.adicionar(
                proximos[i]);
        }
    }

    return resultado;
}

inline ResultadoBusca buscaLargura(
    const CubeState &estadoInicial,
    bool (*cancelamentoSolicitado)() = nullptr)
{
    EstruturaBusca estrutura;

    estrutura.cancelamentoSolicitado =
        cancelamentoSolicitado;

    return executarBusca(
        estadoInicial,
        estrutura);
}

inline ResultadoBusca buscaProfundidadeIterativa(
    const CubeState &estadoInicial,
    std::size_t limiteMaximo,
    bool (*cancelamentoSolicitado)() = nullptr)
{
    ResultadoBusca resultadoFinal;

    for (std::size_t limite = 0;
         limite <= limiteMaximo;
         limite++)
    {
        if (cancelamentoSolicitado != nullptr &&
            cancelamentoSolicitado())
        {
            return resultadoFinal;
        }

        EstruturaProfundidadeLimitada estrutura(
            limite);

        estrutura.cancelamentoSolicitado =
            cancelamentoSolicitado;

        ResultadoBusca resultadoAtual =
            executarBusca(
                estadoInicial,
                estrutura);

        // Na profundidade iterativa, estados dos níveis superiores realmente são revisitados em novas iterações. Por isso somamos as visitas.
        resultadoFinal.estadosVisitados +=
            resultadoAtual.estadosVisitados;

        if (cancelamentoSolicitado != nullptr &&
            cancelamentoSolicitado())
        {
            return resultadoFinal;
        }

        if (resultadoAtual.encontrou)
        {
            resultadoFinal.encontrou = true;

            resultadoFinal.passos =
                resultadoAtual.passos;

            resultadoFinal.limiteEncontrado =
                limite;

            return resultadoFinal;
        }
    }

    return resultadoFinal;
}

inline ResultadoBusca buscaAEstrela(
    const CubeState &estadoInicial,
    bool (*cancelamentoSolicitado)() = nullptr)
{
    EstruturaAEstrela estrutura;

    estrutura.cancelamentoSolicitado =
        cancelamentoSolicitado;

    return executarBusca(
        estadoInicial,
        estrutura);
}

#endif