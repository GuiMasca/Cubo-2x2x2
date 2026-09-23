#ifndef BUSCA_H
#define BUSCA_H

#include <cstddef>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>
#include <unordered_map>
#include "CubeState.h"

struct ResultadoBusca
{
    bool encontrou;
    std::size_t estadosVisitados;
    std::vector<std::string> passos;
};

// FIFO: o primeiro estado adicionado é o primeiro a ser removido.
struct EstruturaBusca
{
    // Opcional: permite à interface solicitar o cancelamento da busca.
    bool (*cancelamentoSolicitado)() = nullptr;
    std::queue<CubeState> estados;
    std::unordered_set<std::string> descobertos;

    void adicionar(const CubeState &estado)
    {
        // Na largura, a primeira descoberta já tem o menor número de passos.
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
               (cancelamentoSolicitado != nullptr && cancelamentoSolicitado());
    }
};

struct EstruturaProfundidadeLimitada:
{
    bool (*cancelamentoSolicitado)() = nullptr;

    std::size_t limite;
    std::vector<CubeState> estados;

    // Para cada configuração, guarda a menor profundidade já encontrada.
    std::unordered_map<std::string, std::size_t> menorProfundidade;

    EstruturaProfundidadeLimitada(std::size_t limiteBusca)
        : limite(limiteBusca)
        {
        }

    void adicionar (const CubeState &estado)
    {
        std::size_t profundidade = estado.path.size();

        // nao permite ultrapassar o limite atual
        if (profundidade>limite)
            return;
        
        std::string chave = estado.chave();
        auto encontrado = menorProfundidade.find(chave);

        if (encontrado == menorProfundidade.end() || profundidade < encontrado->second)
        {
            menorProfundidade[chave] = profundidade;
            estados.push_back(estado);
        }
    }

    CubeState remover()
    {
        CubeState estado = estados.back();
        estados.pop_back();
        return estado;
    }

    bool vazia() const
    {
        return estados.empty() ||
        (cancelamentoSolicitado != nullptr && cancelamentoSolicitado());
    }
};
  

// O template permite trocar a estrutura sem alterar o laço.
// A estrutura recebida deve estar vazia e oferecer adicionar, remover e vazia.
// Cada estrutura define sua ordem de remoção e seu controle de repetidos.
template <typename Estrutura>
ResultadoBusca executarBusca(CubeState estadoInicial, Estrutura &estrutura)
{
    ResultadoBusca resultado;
    resultado.encontrou = false;
    resultado.estadosVisitados = 0;

    // A solução começa no cubo recebido, sem os movimentos do jogador.
    estadoInicial.path.clear();

    estrutura.adicionar(estadoInicial);

    while (!estrutura.vazia())
    {
        CubeState atual = estrutura.remover();
        // Conta os estados retirados e avaliados, incluindo o objetivo.
        resultado.estadosVisitados++;

        if (avaliarEstado(atual))
        {
            resultado.encontrou = true;
            resultado.passos = atual.path;
            return resultado;
        }

        // Função sucessora: aplica cada movimento possível ao estado atual.
        std::vector<CubeState> proximos = atual.getSuccessors();
        for (std::size_t i = 0; i < proximos.size(); i++)
        {
            estrutura.adicionar(proximos[i]);
        }
    }

    // encontrou == false representa "Sem solução".
    return resultado;
}

// Ao cancelar, encontrou == false não significa que o cubo não tem solução.
inline ResultadoBusca buscaLargura(const CubeState &estadoInicial, bool (*cancelamentoSolicitado)() = nullptr)
{
    EstruturaBusca estrutura;
    estrutura.cancelamentoSolicitado = cancelamentoSolicitado;
    return executarBusca(estadoInicial, estrutura);
}

inline ResultadoBusca buscaProfundidadeIterativa(
    const CubeState &estadoInicial,
    std::size_t limiteMaximo,
    bool (*cancelamentoSolicitado)() = nullptr)
{
    ResultadoBusca resultadoFinal;
    resultadoFinal.encontrou = false;
    resultadoFinal.estadosVisitados = 0;

    for (std::size_t limite=0; limite<=limiteMaximo; limite++)
    {
        if (cancelamentoSolicitado != nullptr && cancelamentoSolicitado())
        {
            return resultadoFinal;
        }

        EstruturaProfundidadeLimitada estrutura (limite);

        estrutura.cancelamentoSolicitado=cancelamentoSolicitado;

        ResultadoBusca resultadoAtual = executarBusca(estadoInicial, estrutura)

        //estados revisitados em limites diferentes também contam como estados visitados pelo algoritmo
        resultadoFinal.estadosVisitados +=  
    }

}

#endif
