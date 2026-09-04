#include "interface/JanelaPrincipal.hpp"

#include "interface/ConfiguracaoVisual.hpp"
#include "interface/VisualizadorCubo.hpp"

#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

namespace {

QLabel* criarTitulo(const QString& texto, QWidget* parent) {
    auto* titulo = new QLabel(texto, parent);
    QFont fonte = titulo->font();
    fonte.setPointSize(16);
    fonte.setBold(true);
    titulo->setFont(fonte);
    return titulo;
}

} // namespace

JanelaPrincipal::JanelaPrincipal(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Cubo Mágico 2x2x2");
    resize(1000, 680);
    setMinimumSize(900, 620);

    auto* central = new QWidget(this);
    auto* layoutPrincipal = new QHBoxLayout(central);

    visualizador_ = new VisualizadorCubo(central);
    layoutPrincipal->addWidget(visualizador_, 1);

    auto* painelLateral = new QWidget(central);
    painelLateral->setFixedWidth(370);
    auto* painel = new QVBoxLayout(painelLateral);
    painel->addWidget(criarTitulo("Como você quer girar?", painelLateral));
    painel->addWidget(criarPainelMovimentos(painelLateral));
    painel->addWidget(criarPainelEmbaralhamento(painelLateral));

    auto* botaoReiniciar = new QPushButton("Voltar ao cubo resolvido", painelLateral);
    connect(botaoReiniciar, &QPushButton::clicked, this, [this] { reiniciar(); });
    painel->addWidget(botaoReiniciar);

    auto* botaoAjuda = new QPushButton("Entender os movimentos", painelLateral);
    connect(botaoAjuda, &QPushButton::clicked, this, [this] { mostrarAjuda(); });
    painel->addWidget(botaoAjuda);

    rotuloStatus_ = new QLabel(painelLateral);
    rotuloUltimoMovimento_ = new QLabel("Pronto para jogar.", painelLateral);
    rotuloUltimoMovimento_->setWordWrap(true);
    painel->addWidget(rotuloStatus_);
    painel->addWidget(rotuloUltimoMovimento_);
    painel->addStretch();

    layoutPrincipal->addWidget(painelLateral);
    setCentralWidget(central);

    selecionarFace(faceSelecionada_);
    atualizarTela();
}

QWidget* JanelaPrincipal::criarPainelMovimentos(QWidget* parent) {
    auto* grupo = new QGroupBox("1. Escolha uma face", parent);
    auto* layout = new QVBoxLayout(grupo);

    auto* instrucao = new QLabel(
        "O mapa abaixo acompanha a posição das faces no cubo aberto.", grupo);
    instrucao->setWordWrap(true);
    layout->addWidget(instrucao);

    auto* mapa = new QGridLayout;
    // Posições formam a conhecida cruz de um cubo aberto.
    const std::array<QPoint, 6> posicoes = {{
        {1, 0}, // Cima
        {2, 1}, // Direita
        {1, 1}, // Frente
        {1, 2}, // Baixo
        {0, 1}, // Esquerda
        {3, 1}  // Trás
    }};

    for (int i = 0; i < static_cast<int>(visual::FACES.size()); ++i) {
        const auto& face = visual::FACES[i];
        auto* botao = new QPushButton(
            QString("%1\n%2").arg(face.nome, face.codigo), grupo);
        botao->setCheckable(true);
        botao->setMinimumSize(76, 52);
        botao->setToolTip(QString("Selecionar a face %1 (%2)").arg(face.nome, face.codigo));
        connect(botao, &QPushButton::clicked, this, [this, i] { selecionarFace(i); });
        botoesFace_[i] = botao;
        mapa->addWidget(botao, posicoes[i].y(), posicoes[i].x());
    }
    layout->addLayout(mapa);

    rotuloFaceSelecionada_ = new QLabel(grupo);
    rotuloFaceSelecionada_->setAlignment(Qt::AlignCenter);
    layout->addWidget(rotuloFaceSelecionada_);

    auto* tituloGiro = new QLabel("2. Escolha o giro", grupo);
    QFont fonte = tituloGiro->font();
    fonte.setBold(true);
    tituloGiro->setFont(fonte);
    layout->addWidget(tituloGiro);

    auto* giros = new QGridLayout;
    const std::array<const char*, 3> textos = {{
        "↻  90°\nhorário", "↻  180°\nmeia-volta", "↺  90°\nanti-horário"
    }};
    for (int tipo = 0; tipo < 3; ++tipo) {
        auto* botao = new QPushButton(textos[tipo], grupo);
        botao->setMinimumHeight(55);
        connect(botao, &QPushButton::clicked, this, [this, tipo] { aplicarGiro(tipo); });
        giros->addWidget(botao, 0, tipo);
    }
    layout->addLayout(giros);

    auto* observacao = new QLabel(
        "O sentido é visto olhando diretamente para a face escolhida.", grupo);
    observacao->setWordWrap(true);
    observacao->setStyleSheet("color: #606770;");
    layout->addWidget(observacao);
    return grupo;
}

QWidget* JanelaPrincipal::criarPainelEmbaralhamento(QWidget* parent) {
    auto* grupo = new QGroupBox("Embaralhar", parent);
    auto* layout = new QGridLayout(grupo);

    campoQuantidade_ = new QSpinBox(grupo);
    campoQuantidade_->setRange(1, 100);
    campoQuantidade_->setValue(10);

    campoSeed_ = new QSpinBox(grupo);
    campoSeed_->setRange(0, 999999999);
    campoSeed_->setValue(2026);
    campoSeed_->setToolTip(
        "Use o mesmo número para repetir exatamente o mesmo embaralhamento.");

    auto* botao = new QPushButton("Embaralhar agora", grupo);
    connect(botao, &QPushButton::clicked, this, [this] { embaralhar(); });

    layout->addWidget(new QLabel("Quantidade de giros:", grupo), 0, 0);
    layout->addWidget(campoQuantidade_, 0, 1);
    layout->addWidget(new QLabel("Número para repetir:", grupo), 1, 0);
    layout->addWidget(campoSeed_, 1, 1);
    layout->addWidget(botao, 2, 0, 1, 2);
    return grupo;
}

void JanelaPrincipal::selecionarFace(int indice) {
    faceSelecionada_ = indice;

    for (int i = 0; i < static_cast<int>(botoesFace_.size()); ++i) {
        const auto& face = visual::FACES[i];
        const QString corTexto = face.usarTextoEscuro ? "#171a21" : "white";
        const QString borda = i == indice ? "3px solid #171a21" : "1px solid #777";
        botoesFace_[i]->setChecked(i == indice);
        botoesFace_[i]->setStyleSheet(
            QString("QPushButton { background:%1; color:%2; border:%3; "
                    "border-radius:6px; font-weight:bold; }")
                .arg(face.cor, corTexto, borda));
    }

    const auto& face = visual::FACES[indice];
    rotuloFaceSelecionada_->setText(
        QString("Face escolhida: <b>%1 (%2)</b>").arg(face.nome, face.codigo));
}

void JanelaPrincipal::aplicarGiro(int tipo) {
    const int indiceMovimento = faceSelecionada_ * 3 + tipo;
    aplicarMovimento(cubo::todosMovimentos()[static_cast<std::size_t>(indiceMovimento)]);
}

void JanelaPrincipal::aplicarMovimento(cubo::Movimento movimento) {
    cubo_.mover(movimento);
    const auto& face = visual::FACES[faceSelecionada_];
    atualizarTela(QString("Última ação: %1 (%2) — movimento %3")
                       .arg(face.nome, face.codigo,
                            QString::fromStdString(cubo::paraString(movimento))));
}

void JanelaPrincipal::embaralhar() {
    cubo_.reiniciar();
    const auto sequencia = cubo_.embaralhar(
        static_cast<std::uint32_t>(campoSeed_->value()), campoQuantidade_->value());

    QString texto = "Embaralhamento:";
    for (auto movimento : sequencia) {
        texto += " " + QString::fromStdString(cubo::paraString(movimento));
    }
    atualizarTela(texto);
}

void JanelaPrincipal::reiniciar() {
    cubo_.reiniciar();
    atualizarTela("Cubo reiniciado.");
}

void JanelaPrincipal::mostrarAjuda() {
    QMessageBox caixa(this);
    caixa.setWindowTitle("Como movimentar o cubo");
    caixa.setIcon(QMessageBox::Information);
    caixa.setText("Escolha uma face e depois o sentido do giro");
    caixa.setInformativeText(
        "As seis faces aparecem em forma de cruz, como se o cubo estivesse aberto. "
        "As mesmas letras aparecem sobre o desenho 3D.\n\n"
        "Horário e anti-horário são sempre vistos de frente para a face escolhida. "
        "Você pode arrastar o cubo com o mouse para enxergá-la melhor.\n\n"
        "As letras U, R, F, D, L e B são apenas a notação internacional mostrada "
        "no histórico. O apóstrofo indica anti-horário e o número 2 indica meia-volta.");
    caixa.exec();
}

void JanelaPrincipal::atualizarTela(const QString& acao) {
    visualizador_->definirEstado(cubo_.estado());

    if (cubo_.estado().estaResolvido()) {
        rotuloStatus_->setText("✓ Cubo resolvido");
        rotuloStatus_->setStyleSheet(
            "color:#168347; font-weight:bold; font-size:15px;");
    } else {
        rotuloStatus_->setText("Cubo ainda não está resolvido");
        rotuloStatus_->setStyleSheet(
            "color:#b42318; font-weight:bold; font-size:15px;");
    }

    if (!acao.isEmpty()) {
        rotuloUltimoMovimento_->setText(acao);
    }
}
