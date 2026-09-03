#include "interface/JanelaPrincipal.hpp"
#include "cubo/AvaliadorEstado.hpp"
#include "interface/VisualizadorCubo.hpp"
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

JanelaPrincipal::JanelaPrincipal(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Cubo Mágico 2x2x2 — Grupo T1");
    resize(930,650);
    setMinimumSize(930,650);
    auto* central=new QWidget(this);
    auto* principal=new QHBoxLayout(central);
    visualizador_=new VisualizadorCubo(central);
    principal->addWidget(visualizador_,1);
    auto* painelWidget=new QWidget(central);
    painelWidget->setFixedWidth(350);
    auto* painel=new QVBoxLayout(painelWidget);
    auto* titulo=new QLabel("Controles",central);
    QFont ft=titulo->font(); ft.setPointSize(16); ft.setBold(true); titulo->setFont(ft);
    painel->addWidget(titulo);
    auto* grupoMov=new QGroupBox("Movimentos",central);
    auto* grade=new QGridLayout(grupoMov);
    grade->addWidget(new QLabel("Face"),0,0); grade->addWidget(new QLabel("↻ Horário"),0,1);
    grade->addWidget(new QLabel("2 giros"),0,2); grade->addWidget(new QLabel("↺ Anti-hor."),0,3);
    static constexpr const char* nomes[] = {"Cima (U)","Direita (R)","Frente (F)",
                                            "Baixo (D)","Esquerda (L)","Trás (B)"};
    const auto& movimentos=cubo::todosMovimentos();
    for (int f=0;f<6;++f) {
        grade->addWidget(new QLabel(nomes[f]),f+1,0);
        for(int tipo=0;tipo<3;++tipo) {
            const auto mov=movimentos[static_cast<std::size_t>(f*3+tipo)];
            auto* b=new QPushButton(QString::fromStdString(cubo::paraString(mov)),grupoMov);
            b->setMinimumSize(52,34);
            const QString sentido=tipo==0?"90° horário":tipo==1?"180°":"90° anti-horário";
            b->setToolTip(QString("%1: %2").arg(nomes[f],sentido));
            connect(b,&QPushButton::clicked,this,[this,mov]{aplicarMovimento(mov);});
            grade->addWidget(b,f+1,tipo+1);
        }
    }
    painel->addWidget(grupoMov);
    auto* grupoMix=new QGroupBox("Embaralhamento reproduzível",central);
    auto* mix=new QGridLayout(grupoMix);
    campoSeed_=new QSpinBox(grupoMix); campoSeed_->setRange(0,999999999); campoSeed_->setValue(2026);
    campoQuantidade_=new QSpinBox(grupoMix); campoQuantidade_->setRange(1,100); campoQuantidade_->setValue(10);
    auto* bMix=new QPushButton("Embaralhar",grupoMix);
    mix->addWidget(new QLabel("Seed:"),0,0); mix->addWidget(campoSeed_,0,1);
    mix->addWidget(new QLabel("Movimentos:"),1,0); mix->addWidget(campoQuantidade_,1,1);
    mix->addWidget(bMix,2,0,1,2);
    connect(bMix,&QPushButton::clicked,this,[this]{ embaralhar(); });
    painel->addWidget(grupoMix);
    auto* bReset=new QPushButton("Reiniciar cubo",central);
    connect(bReset,&QPushButton::clicked,this,[this]{ reiniciar(); });
    painel->addWidget(bReset);
    auto* bAjuda=new QPushButton("? Manual de movimentos",central);
    connect(bAjuda,&QPushButton::clicked,this,[this]{mostrarAjuda();});
    painel->addWidget(bAjuda);
    rotuloStatus_=new QLabel(central);
    rotuloUltimoMovimento_=new QLabel("Pronto para jogar.",central);
    rotuloUltimoMovimento_->setWordWrap(true);
    rotuloUltimoMovimento_->setFixedWidth(325);
    rotuloUltimoMovimento_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
    painel->addWidget(rotuloStatus_); painel->addWidget(rotuloUltimoMovimento_); painel->addStretch();
    principal->addWidget(painelWidget);
    setCentralWidget(central);
    atualizarTela();
}
void JanelaPrincipal::aplicarMovimento(cubo::Movimento m) {
    cubo_.mover(m);
    atualizarTela("Movimento: "+QString::fromStdString(cubo::paraString(m)));
}
void JanelaPrincipal::embaralhar() {
    cubo_.reiniciar();
    const auto seq=cubo_.embaralhar(static_cast<std::uint32_t>(campoSeed_->value()),campoQuantidade_->value());
    QString texto="Embaralhamento:";
    for (auto m:seq) texto+=" "+QString::fromStdString(cubo::paraString(m));
    atualizarTela(texto);
}
void JanelaPrincipal::reiniciar() { cubo_.reiniciar(); atualizarTela("Cubo reiniciado."); }
void JanelaPrincipal::mostrarAjuda() {
    QMessageBox caixa(this); caixa.setWindowTitle("Manual de movimentos");
    caixa.setIcon(QMessageBox::Information); caixa.setText("Como ler os movimentos");
    caixa.setInformativeText(
        "U = cima (Up)          D = baixo (Down)\n"
        "R = direita (Right)     L = esquerda (Left)\n"
        "F = frente (Front)      B = trás (Back)\n\n"
        "R  →  90° no sentido horário\n"
        "R2 →  180° (dois quartos de volta)\n"
        "R' →  90° no sentido anti-horário\n\n"
        "O sentido é sempre observado olhando diretamente para a face que será girada. "
        "Arraste o cubo para enxergar essa face melhor.\n\n"
        "Exemplo: F R U' significa Frente horário, Direita horário e Cima anti-horário.");
    caixa.exec();
}
void JanelaPrincipal::atualizarTela(const QString& acao) {
    visualizador_->definirEstado(cubo_.estado());
    cubo::AvaliadorEstado avaliador;
    if (avaliador.ehObjetivo(cubo_.estado())) {
        rotuloStatus_->setText("✓ Cubo resolvido");
        rotuloStatus_->setStyleSheet("color:#20a05a;font-weight:bold;font-size:15px");
    } else {
        rotuloStatus_->setText(QString("Cubo embaralhado · h = %1").arg(avaliador.heuristicaCantos(cubo_.estado())));
        rotuloStatus_->setStyleSheet("color:#d62828;font-weight:bold;font-size:15px");
    }
    if (!acao.isEmpty()) rotuloUltimoMovimento_->setText(acao);
}
