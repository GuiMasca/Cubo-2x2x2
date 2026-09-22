#ifndef BUSCA_H
#define BUSCA_H

#include <cstddef>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>
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

// Função avaliadora da largura: verifica se o estado é o objetivo.
inline bool avaliarEstado(const CubeState &estado)
{
    return estado.isGoal();
}

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
inline ResultadoBusca buscaLargura(const CubeState &estadoInicial,
                                  bool (*cancelamentoSolicitado)() = nullptr)
{
    EstruturaBusca estrutura;
    estrutura.cancelamentoSolicitado = cancelamentoSolicitado;
    return executarBusca(estadoInicial, estrutura);
}

#endif
