#pragma once
#include "cubo/EstadoCubo.hpp"
#include <QWidget>
#include <QPoint>

class VisualizadorCubo : public QWidget {
public:
    explicit VisualizadorCubo(QWidget* parent = nullptr);
    void definirEstado(const cubo::EstadoCubo& estado);
    QSize minimumSizeHint() const override;
protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
private:
    cubo::EstadoCubo estado_;
    QPoint ultimoMouse_;
    double anguloX_ = -25.0;
    double anguloY_ = 35.0;
};
