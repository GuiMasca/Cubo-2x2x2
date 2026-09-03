#pragma once
#include "cubo/Cubo.hpp"
#include <QMainWindow>

class QLabel;
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
    void aplicarMovimento(cubo::Movimento movimento);
    void embaralhar();
    void reiniciar();
    void mostrarAjuda();
    void atualizarTela(const QString& acao = {});
};
