#pragma once
#include "cubo/Cubo.hpp"
#include <array>
#include <QMainWindow>

class QLabel;
class QPushButton;
class QSpinBox;
class VisualizadorCubo;

class JanelaPrincipal : public QMainWindow {
public:
    explicit JanelaPrincipal(QWidget* parent = nullptr);
private:
    cubo::Cubo cubo_;
    VisualizadorCubo* visualizador_;
    QSpinBox* campoSeed_;
    QSpinBox* campoQuantidade_;
    QLabel* rotuloStatus_;
    QLabel* rotuloUltimoMovimento_;
    QLabel* rotuloFaceSelecionada_;
    std::array<QPushButton*, 6> botoesFace_{};
    int faceSelecionada_ = 2; // Começa pela frente (F).

    QWidget* criarPainelMovimentos(QWidget* parent);
    QWidget* criarPainelEmbaralhamento(QWidget* parent);
    void selecionarFace(int indice);
    void aplicarGiro(int tipo);
    void aplicarMovimento(cubo::Movimento movimento);
    void embaralhar();
    void reiniciar();
    void mostrarAjuda();
    void atualizarTela(const QString& acao = {});
};
