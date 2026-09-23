#include <QApplication>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QRandomGenerator>
#include <QStackedWidget>
#include <QDialog>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QProgressDialog>
#include <QThread>
#include <exception>
#include <iostream>
#include <cstdlib> 
#include <ctime>
#include "CubeState.h"
#include "Busca.h"

// Apenas o cálculo roda nesta thread, os widgets ficam na thread da interface.
class ThreadBuscaLargura : public QThread
{
public:
    CubeState estadoInicial;
    ResultadoBusca resultado;
    QString erro;
    bool cancelada = false;

    ThreadBuscaLargura(const CubeState &estado, QObject *parent)
        : QThread(parent), estadoInicial(estado) {}

protected:
    void run() override
    {
        try {
            resultado = buscaLargura(estadoInicial, verificarCancelamento);
        }
        catch (const std::exception &excecao) {
            erro = QString::fromUtf8(excecao.what());
        }
        cancelada = isInterruptionRequested();
    }

private:
    static bool verificarCancelamento()
    {
        return QThread::currentThread()->isInterruptionRequested();
    }
};

class ThreadBuscaProfundidade : public QThread
{
    public:
        CubeState estadoInicial;
        ResultadoBusca resultado;
        QString erro;
        bool cancelada = false;

        std::size_t limiteMaximo;

        ThreadBuscaProfundidade(
            const CubeState &estado,
            std::size_t limiteBusca,
            QObject *parent = nullptr)
            : QThread(parent),
            estadoInicial(estado),
            limiteMaximo(limiteBusca)
        {
        }

    protected:
        void run() override
        {
            try
            {
                resultado = buscaProfundidadeIterativa(
                    estadoInicial,
                    limiteMaximo,
                    verificarCancelamento);
            }
            catch (const std::exception &excecao)
            {
                erro = QString::fromUtf8(
                    excecao.what());
            }

            cancelada = isInterruptionRequested();
        }

    private:
        static bool verificarCancelamento()
        {
            return QThread::currentThread()
                ->isInterruptionRequested();
        }
};

class CuboWidget : public QOpenGLWidget,
                   protected QOpenGLFunctions
{
public:

    CubeState getEstadoAtual() const {
        return meuCubo;
    }

    int getMovimentosJogador() const {
        return movimentosJogador;
    }

    void setLabelMovimentos(QLabel* label) {
        labelMovimentos = label;
    }

    void setLabelStatus(QLabel* label){
        labelStatus = label;
        atualizarStatus();
    }

    void embaralharComSeed(unsigned int seed) {
        movimentosJogador = 0;
        meuCubo = CubeState(); // Reseta para o estado resolvido
        srand(seed);           // Define a seed desejada
        embaralharCubo(3);    // Executa os 10 movimentos de embaralhamento

        atualizarStatus(); // Atualiza o status do cubo após o embaralhamento
    }

    void embaralharCubo(int passos = 10) {
    // Lista de ponteiros para as funções de movimento disponíveis
    // Utilizando as letras correspondentes aos movimentos do seu CubeState
    std::vector<std::string> movimentos = {"R", "U", "F", "L", "B", "A", "N", "I", "K", "S", "G", "T"};
    
    std::string ultimoMovimento = "";
    
    for (int i = 0; i < passos; ++i) {
        std::string movimentoEscolhido;
        do {
            int idx = rand() % movimentos.size();
            movimentoEscolhido = movimentos[idx];
        } while (movimentoEscolhido == ultimoMovimento); // Evita repetir o mesmo movimento consecutivamente
        
        ultimoMovimento = movimentoEscolhido;
        
        // Aplica o movimento correspondente no estado do cubo
        if (movimentoEscolhido == "R") meuCubo = meuCubo.moveR();
        else if (movimentoEscolhido == "U") meuCubo = meuCubo.moveU();
        else if (movimentoEscolhido == "F") meuCubo = meuCubo.moveF();
        else if (movimentoEscolhido == "L") meuCubo = meuCubo.moveL();
        else if (movimentoEscolhido == "B") meuCubo = meuCubo.moveB();
        else if (movimentoEscolhido == "A") meuCubo = meuCubo.moveA();
        else if (movimentoEscolhido == "N") meuCubo = meuCubo.moveN();
        else if (movimentoEscolhido == "I") meuCubo = meuCubo.moveI();
        else if (movimentoEscolhido == "K") meuCubo = meuCubo.moveK();
        else if (movimentoEscolhido == "S") meuCubo = meuCubo.moveS();
        else if (movimentoEscolhido == "G") meuCubo = meuCubo.moveG();
        else if (movimentoEscolhido == "T") meuCubo = meuCubo.moveT();
    }
    update(); // Redesenha a tela com o cubo embaralhado
}

    CuboWidget(QWidget *parent = nullptr)
        : QOpenGLWidget(parent), m_program(nullptr), m_vbo(QOpenGLBuffer::VertexBuffer)
    {
        srand(time(nullptr));
    }

    ~CuboWidget()
    {
        makeCurrent();
        m_vbo.destroy();
        delete m_program;
        doneCurrent();
    }

protected:
    CubeState meuCubo;
    float rotacaoX = 30.0f;
    float rotacaoY = -45.0f;
    QPoint ultimaPosicaoMouse;
    int movimentosJogador = 0;
    QLabel* labelMovimentos = nullptr;
    QLabel* labelStatus = nullptr;

    QOpenGLShaderProgram *m_program;
    QOpenGLBuffer m_vbo;
    QOpenGLVertexArrayObject m_vao;

    

    void initializeGL() override
    {
        initializeOpenGLFunctions();
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.12f, 0.12f, 0.16f, 1.0f);

        const char *vertexShaderSource =
            "#version 330 core\n"
            "layout(location = 0) in vec3 aPos;\n"
            "layout(location = 1) in vec3 aColor;\n"
            "uniform mat4 mvp;\n"
            "out vec3 ourColor;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = mvp * vec4(aPos, 1.0);\n"
            "   ourColor = aColor;\n"
            "}\n";

        const char *fragmentShaderSource =
            "#version 330 core\n"
            "in vec3 ourColor;\n"
            "out vec4 FragColor;\n"
            "void main()\n"
            "{\n"
            "   FragColor = vec4(ourColor, 1.0);\n"
            "}\n";

        m_program = new QOpenGLShaderProgram(this);
        m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
        m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
        m_program->link();

        m_vao.create();
        m_vao.bind();

        m_vbo.create();
        m_vbo.bind();
        m_vbo.allocate(36 * 6 * sizeof(float));

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        m_vbo.release();
        m_vao.release();
    }

    void resizeGL(int largura, int altura) override
    {
        glViewport(0, 0, largura, altura);
    }

    void mousePressEvent(QMouseEvent *event) override
    {
        ultimaPosicaoMouse = event->pos();
    }

    void mouseMoveEvent(QMouseEvent *event) override
    {
        int dx = event->pos().x() - ultimaPosicaoMouse.x();
        int dy = event->pos().y() - ultimaPosicaoMouse.y();

        if (event->buttons() & Qt::LeftButton) {
            rotacaoX += dy * 0.5f;
            rotacaoY += dx * 0.5f;
            update();
        }

        ultimaPosicaoMouse = event->pos();
    }

    // Controles por teclado para girar as faces do cubo interativamente
    void keyPressEvent(QKeyEvent *event) override
    {
        if (event->key() == Qt::Key_R) {
            meuCubo = meuCubo.moveR();
            movimentosJogador++;
            std::cout << "Movimento R executado!" << std::endl;
        } 
        else if (event->key() == Qt::Key_U) {
            meuCubo = meuCubo.moveU();
            movimentosJogador++;
            std::cout << "Movimento U executado!" << std::endl;
        }
        else if (event->key() == Qt::Key_F) {
            meuCubo = meuCubo.moveF();
            movimentosJogador++;
            std::cout << "Movimento F executado!" << std::endl;
        }
        else if (event->key() == Qt::Key_L) {
            meuCubo = meuCubo.moveL();
            movimentosJogador++;
            std::cout << "Movimento L executado!" << std::endl;
        }
        else if (event->key() == Qt::Key_A) {
            meuCubo = meuCubo.moveA();
            movimentosJogador++;
            std::cout << "Movimento A executado!" << std::endl;
        }
        else if (event->key() == Qt::Key_B) {
            meuCubo = meuCubo.moveB();
            movimentosJogador++;
            std::cout << "Movimento B executado!" << std::endl;
        }
        else if (event->key() == Qt::Key_N) {
            meuCubo = meuCubo.moveN();
            movimentosJogador++;
            std::cout << "Movimento de baixo anti-horario (N) executado!" << std::endl;
        }
        else if (event->key() == Qt::Key_I) {
            meuCubo = meuCubo.moveI();
            movimentosJogador++;
            std::cout << "Movimento de cima anti-horario (I) executado!" << std::endl;
        }
        else if (event->key() == Qt::Key_K) {
            meuCubo = meuCubo.moveK();
            movimentosJogador++;
            std::cout << "Movimento da esquerda anti-horario (K) executado!" << std::endl;
        }
        else if (event->key() == Qt::Key_S) {
            meuCubo = meuCubo.moveS();
            movimentosJogador++;
            std::cout << "Movimento de tras anti-horario (S) executado!" << std::endl;
        }
        else if (event->key() == Qt::Key_G) {
            meuCubo = meuCubo.moveG();
            movimentosJogador++;
            std::cout << "Movimento da frente anti-horario (G) executado!" << std::endl;
        }
        else if (event->key() == Qt::Key_T) {
            meuCubo = meuCubo.moveT();
            movimentosJogador++;
            std::cout << "Movimento da direita anti-horario (T) executado!" << std::endl;
        }
        if (labelMovimentos) {
            labelMovimentos->setText(
            "Movimentos: " + QString::number(movimentosJogador)
        );
}
        
        atualizarStatus();

        update(); // Solicita o redesenho imediato da tela com as novas cores atualizadas
    }

void paintGL() override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        QMatrix4x4 proj;
        proj.perspective(45.0f, (float)width() / height(), 0.1f, 100.0f);

        QMatrix4x4 view;
        view.translate(0.0f, 0.0f, -5.5f);
        view.rotate(rotacaoX, 1.0f, 0.0f, 0.0f);
        view.rotate(rotacaoY, 0.0f, 1.0f, 0.0f);

        m_program->bind();
        m_vao.bind();

        float corPaleta[6][3] = {
            {0.95f, 0.95f, 0.95f}, // 0: Branco (U)
            {0.95f, 0.85f, 0.10f}, // 1: Amarelo (D)
            {0.90f, 0.10f, 0.10f}, // 2: Vermelho (F)
            {0.95f, 0.50f, 0.10f}, // 3: Laranja (B)
            {0.10f, 0.80f, 0.10f}, // 4: Verde (L)
            {0.10f, 0.30f, 0.90f}  // 5: Azul (R)
        };
        float cBlack[3] = {0.05f, 0.05f, 0.05f};

        int cornerMapping[2][2][2] = {
            { {6, 4}, {2, 0} }, // x = -1 (L): baixo primeiro, cima depois
            { {7, 5}, {3, 1} }  // x =  1 (R): baixo primeiro, cima depois
        };

        for (int x = -1; x <= 1; x += 2) {
            for (int y = -1; y <= 1; y += 2) {
                for (int z = -1; z <= 1; z += 2) {
                    QMatrix4x4 model;
                    model.translate(x * 0.48f, y * 0.48f, z * 0.48f);
                    QMatrix4x4 mvp = proj * view * model;
                    m_program->setUniformValue("mvp", mvp);

                    float s = 0.46f;
                    
                    int xi = (x < 0) ? 0 : 1;
                    int yi = (y < 0) ? 0 : 1;
                    int zi = (z < 0) ? 0 : 1;
                    int cornerIdx = cornerMapping[xi][yi][zi];

                    const auto& qColors = meuCubo.cornerColors[cornerIdx];

                    const float* colU = (y == 1)  ? corPaleta[qColors[0]] : cBlack;
                    const float* colD = (y == -1) ? corPaleta[qColors[0]] : cBlack;
                    const float* colF = (z == 1)  ? corPaleta[qColors[1]] : cBlack;
                    const float* colB = (z == -1) ? corPaleta[qColors[1]] : cBlack;
                    const float* colL = (x == -1) ? corPaleta[qColors[2]] : cBlack;
                    const float* colR = (x == 1)  ? corPaleta[qColors[2]] : cBlack;

                    GLfloat miniVertices[] = {
                        // Frente
                        -s,  s,  s,  colF[0], colF[1], colF[2],
                        -s, -s,  s,  colF[0], colF[1], colF[2],
                         s, -s,  s,  colF[0], colF[1], colF[2],
                         s, -s,  s,  colF[0], colF[1], colF[2],
                         s,  s,  s,  colF[0], colF[1], colF[2],
                        -s,  s,  s,  colF[0], colF[1], colF[2],
                        // Trás
                         s,  s, -s,  colB[0], colB[1], colB[2],
                         s, -s, -s,  colB[0], colB[1], colB[2],
                        -s, -s, -s,  colB[0], colB[1], colB[2],
                        -s, -s, -s,  colB[0], colB[1], colB[2],
                        -s,  s, -s,  colB[0], colB[1], colB[2],
                         s,  s, -s,  colB[0], colB[1], colB[2],
                        // Cima
                        -s,  s, -s,  colU[0], colU[1], colU[2],
                        -s,  s,  s,  colU[0], colU[1], colU[2],
                         s,  s,  s,  colU[0], colU[1], colU[2],
                         s,  s,  s,  colU[0], colU[1], colU[2],
                         s,  s, -s,  colU[0], colU[1], colU[2],
                        -s,  s, -s,  colU[0], colU[1], colU[2],
                        // Baixo
                        -s, -s,  s,  colD[0], colD[1], colD[2],
                        -s, -s, -s,  colD[0], colD[1], colD[2],
                         s, -s, -s,  colD[0], colD[1], colD[2],
                         s, -s, -s,  colD[0], colD[1], colD[2],
                         s, -s,  s,  colD[0], colD[1], colD[2],
                        -s, -s,  s,  colD[0], colD[1], colD[2],
                        // Direita
                         s,  s,  s,  colR[0], colR[1], colR[2],
                         s, -s,  s,  colR[0], colR[1], colR[2],
                         s, -s, -s,  colR[0], colR[1], colR[2],
                         s, -s, -s,  colR[0], colR[1], colR[2],
                         s,  s, -s,  colR[0], colR[1], colR[2],
                         s,  s,  s,  colR[0], colR[1], colR[2],
                        // Esquerda
                        -s,  s, -s,  colL[0], colL[1], colL[2],
                        -s, -s, -s,  colL[0], colL[1], colL[2],
                        -s, -s,  s,  colL[0], colL[1], colL[2],
                        -s, -s,  s,  colL[0], colL[1], colL[2],
                        -s,  s,  s,  colL[0], colL[1], colL[2],
                        -s,  s, -s,  colL[0], colL[1], colL[2]
                    };

                    m_vbo.bind();
                    m_vbo.allocate(miniVertices, sizeof(miniVertices));
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                    m_vbo.release();
                }
            }
        }

        m_vao.release();
        m_program->release();
    }
    void atualizarStatus()
    {
        if (!labelStatus)
            return;

        if (meuCubo.isGoal()) {
            labelStatus->setText("CUBO RESOLVIDO!");
            labelStatus->setStyleSheet(
                "color: #22C55E;"
                "font-weight: bold;"
                "font-size: 18px;"
            );
        }
        else {
            labelStatus->setText("Cubo não resolvido");
            labelStatus->setStyleSheet(
                "color: #F9FAFB;"
                "font-size: 16px;"
            );
        }
    }
};

class JanelaPrincipal : public QWidget
{
public:
    JanelaPrincipal(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setWindowTitle("Cubo Mágico 2x2x2");
        resize(900, 700);

        aplicarEstilo();

        // Permite trocar entre diferentes "telas"
        telas = new QStackedWidget(this);

        criarMenuPrincipal();
        criarTelaJogo();
        criarMenuBuscas();

        // Layout principal da janela
        QVBoxLayout *layoutPrincipal = new QVBoxLayout(this);
        layoutPrincipal->addWidget(telas);

        // Começa mostrando o menu
        telas->setCurrentWidget(menuPrincipal);
    }

private:
    QStackedWidget *telas;

    QWidget *menuPrincipal;
    QWidget *telaJogo;
    QWidget *menuBuscas;

    CuboWidget *cubo;

    void aplicarEstilo()
{
    setStyleSheet(R"(

        QWidget {
            background-color: #111827;
            color: #F9FAFB;
            font-family: Arial;
            font-size: 16px;
        }

        QLabel#titulo {
            font-size: 30px;
            font-weight: bold;
            color: #FFFFFF;
        }

        QPushButton {
            background-color: #374151;
            color: white;

            border: 1px solid #4B5563;
            border-radius: 10px;

            padding: 12px 25px;

            font-size: 16px;
            font-weight: bold;

            min-height: 25px;
        }

        QPushButton:hover {
            background-color: #4B5563;
            border: 1px solid #60A5FA;
        }

        QPushButton:pressed {
            background-color: #1F2937;
        }

        QPushButton#botaoPrincipal {
            background-color: #2563EB;
            border: none;
        }

        QPushButton#botaoPrincipal:hover {
            background-color: #3B82F6;
        }

        QPushButton#botaoPrincipal:pressed {
            background-color: #1D4ED8;
        }

        QPushButton#botaoSecundario {
            background-color: #1F2937;
        }

    )");
}

    void mostrarInstrucoes()
{
    QDialog dialog(this);

    dialog.setWindowTitle("Instruções");
    dialog.setFixedSize(400, 700);

    QVBoxLayout *layout =
        new QVBoxLayout(&dialog);

    QLabel *titulo =
        new QLabel("COMANDOS DO CUBO");

    titulo->setObjectName("titulo");
    titulo->setAlignment(Qt::AlignCenter);

    QLabel *comandos = new QLabel(
        "<b>R</b> - Girar face direita<br><br>"
        "<b>U</b> - Girar face superior<br><br>"
        "<b>F</b> - Girar face frontal<br><br>"
        "<b>L</b> - Girar face esquerda<br><br>"
        "<b>A</b> - Girar face traseira<br><br>"
        "<b>B</b> - Girar face inferior<br><br>"
        "<b>T</b> - Girar face direita no sentido anti-horário<br><br>"
        "<b>I</b> - Girar face superior no sentido anti-horário<br><br>"
        "<b>G</b> - Girar face frontal no sentido anti-horário<br><br>"
        "<b>K</b> - Girar face esquerda no sentido anti-horário<br><br>"
        "<b>S</b> - Girar face traseira no sentido anti-horário<br><br>"
        "<b>N</b> - Girar face inferior no sentido anti-horário<br><br>"
        "<br>"
        "Use o teclado enquanto estiver na tela do cubo.<br><br>"
        "Segure o botão esquerdo do mouse e arraste para girar a visualização do cubo."
    );

    comandos->setAlignment(Qt::AlignLeft);
    comandos->setWordWrap(true);

    QPushButton *fechar =
        new QPushButton("Fechar");

    fechar->setObjectName("botaoPrincipal");

    layout->addWidget(titulo);

    layout->addSpacing(20);

    layout->addWidget(comandos);

    layout->addStretch();

    layout->addWidget(fechar);

    connect(
        fechar,
        &QPushButton::clicked,
        &dialog,
        &QDialog::accept
    );

    dialog.exec();

    // Devolve o foco ao cubo
    cubo->setFocus();
}

    void criarMenuPrincipal()
    {
        menuPrincipal = new QWidget();

    QVBoxLayout *layout =
        new QVBoxLayout(menuPrincipal);

    QLabel *titulo =
        new QLabel("CUBO MÁGICO 2x2x2");

    titulo->setObjectName("titulo");
    titulo->setAlignment(Qt::AlignCenter);

    QPushButton *botaoJogar =
        new QPushButton("JOGAR");

    QPushButton *botaoBuscas =
        new QPushButton("BUSCAS");

    botaoJogar->setObjectName("botaoPrincipal");
    botaoBuscas->setObjectName("botaoPrincipal");

    botaoJogar->setFixedWidth(300);
    botaoBuscas->setFixedWidth(300);

    layout->addStretch();

    layout->addWidget(titulo);

    layout->addSpacing(50);

    layout->addWidget(
        botaoJogar,
        0,
        Qt::AlignCenter
    );

    layout->addSpacing(15);

    layout->addWidget(
        botaoBuscas,
        0,
        Qt::AlignCenter
    );

    layout->addStretch();

    telas->addWidget(menuPrincipal);

    connect(
        botaoJogar,
        &QPushButton::clicked,
        this,
        [this]()
        {
            telas->setCurrentWidget(telaJogo);
            cubo->setFocus();
        }
    );

    connect(
        botaoBuscas,
        &QPushButton::clicked,
        this,
        [this]()
        {
            telas->setCurrentWidget(menuBuscas);
        }
    );
    }

    unsigned int seedAtual = 0; // Será inicializada na criação da tela
    QLabel *labelSeedInfo;

    void criarTelaJogo()
{
    telaJogo = new QWidget();

    QVBoxLayout *layout =
        new QVBoxLayout(telaJogo);

    // CUBO

    cubo = new CuboWidget();

    cubo->setFocusPolicy(Qt::StrongFocus);

    labelSeedInfo = new QLabel();
    labelSeedInfo->setAlignment(Qt::AlignCenter);

    QLabel *labelMovimentos =
    new QLabel("Movimentos: 0");

    labelMovimentos->setAlignment(Qt::AlignCenter);

    cubo->setLabelMovimentos(labelMovimentos);

    QLabel *labelStatus =
    new QLabel("Cubo não resolvido");

    labelStatus->setAlignment(Qt::AlignCenter);

    cubo->setLabelStatus(labelStatus);

    // CONTROLES DO JOGO

    QPushButton *botaoGerarSeed =
        new QPushButton("Gerar Nova Seed (Trocar)");

    QPushButton *botaoEmbaralhar =
        new QPushButton("Reiniciar com a Seed Atual");

    botaoGerarSeed->setObjectName("botaoControle");
    botaoEmbaralhar->setObjectName("botaoControle");

    QHBoxLayout *barraControles =
        new QHBoxLayout();

    barraControles->addStretch();

    barraControles->addWidget(botaoGerarSeed);
    barraControles->addWidget(botaoEmbaralhar);

    barraControles->addStretch();

    // BARRA INFERIOR

    QPushButton *botaoVoltar =
        new QPushButton("Voltar");

    QPushButton *botaoInstrucoes =
        new QPushButton("Instruções");

    botaoVoltar->setObjectName("botaoSecundario");
    botaoInstrucoes->setObjectName("botaoPrincipal");

    QHBoxLayout *barraInferior =
        new QHBoxLayout();

    barraInferior->addWidget(botaoVoltar);

    barraInferior->addStretch();

    barraInferior->addWidget(botaoInstrucoes);

    // =========================
    // MONTA A TELA
    // =========================

    layout->addWidget(cubo, 1);

    layout->addWidget(labelSeedInfo);
    layout->addLayout(barraControles);
    layout->addWidget(labelMovimentos);
    layout->addWidget(labelStatus);

    layout->addLayout(barraInferior);

    telas->addWidget(telaJogo);

    //botões de controle da tela 

    connect(
        botaoVoltar,
        &QPushButton::clicked,
        this,
        [this]()
        {
            telas->setCurrentWidget(menuPrincipal);
        }
    );

    connect(
        botaoInstrucoes,
        &QPushButton::clicked,
        this,
        [this]()
        {
            mostrarInstrucoes();
        }
    );


    seedAtual = static_cast<unsigned int>(time(nullptr));
    cubo->embaralharComSeed(seedAtual);
    labelSeedInfo->setText(QString("Seed atual: %1").arg(seedAtual));

    connect(
        botaoGerarSeed,
        &QPushButton::clicked,
        this,
        [this]()
        {
            unsigned int novaSeed;
            do {
                novaSeed = QRandomGenerator::global()->generate();
            } while (novaSeed == seedAtual);
            seedAtual = novaSeed;
            cubo->embaralharComSeed(seedAtual);
            labelSeedInfo->setText(QString("Seed atual: %1").arg(seedAtual));
            cubo->setFocus();
        }
    );

    connect(
        botaoEmbaralhar,
        &QPushButton::clicked,
        this,
        [this]()
        {
            cubo->embaralharComSeed(seedAtual);
            cubo->setFocus();
        }
    );
}

    void iniciarBuscaLargura(QPushButton *botao)
    {
        ThreadBuscaLargura *busca = new ThreadBuscaLargura(cubo->getEstadoAtual(), this);
        QProgressDialog *progresso = new QProgressDialog(
            "Buscando a solução do cubo atual...", "Cancelar", 0, 0, this);
        progresso->setWindowTitle("Busca em largura");
        progresso->setWindowModality(Qt::WindowModal);
        progresso->setMinimumDuration(0);
        progresso->setAutoClose(false);
        progresso->setAutoReset(false);
        botao->setEnabled(false);

        connect(progresso, &QProgressDialog::canceled,
                busca, &QThread::requestInterruption);

        // Ao fechar o programa, aguarda o encerramento cooperativo do cálculo.
        connect(qApp, &QCoreApplication::aboutToQuit, busca, [busca]() {
            busca->requestInterruption();
            busca->wait();
        });

        connect(busca, &QThread::finished, this, [this, busca, progresso, botao]() {
            progresso->hide();
            progresso->deleteLater();
            botao->setEnabled(true);

            QString texto;
            if (busca->cancelada) {
                texto = "Busca cancelada.";
            }
            else if (!busca->erro.isEmpty()) {
                texto = "Não foi possível concluir a busca.\n" + busca->erro;
            }
            else {
                const ResultadoBusca &resultado = busca->resultado;
                texto = QString("Estados visitados (incluindo o objetivo, se encontrado): %1\n\n")
                    .arg(static_cast<qulonglong>(resultado.estadosVisitados));

                if (!resultado.encontrou) {
                    texto += "Sem solução.";
                }
                else if (resultado.passos.empty()) {
                    texto += "O cubo já está resolvido.";
                }
                else {
                    texto += QString("Solução em %1 movimentos.\n"
                                     "Na tela do jogo, execute as teclas nesta ordem.\n"
                                     "Os sentidos são vistos de frente para a face girada.\n\n")
                        .arg(static_cast<qulonglong>(resultado.passos.size()));
                    for (std::size_t i = 0; i < resultado.passos.size(); i++) {
                        QString tecla = QString::fromStdString(resultado.passos[i]);
                        QString descricao;
                        if (tecla == "U") descricao = "Superior — horário";
                        else if (tecla == "I") descricao = "Superior — anti-horário";
                        else if (tecla == "R") descricao = "Direita — horário";
                        else if (tecla == "T") descricao = "Direita — anti-horário";
                        else if (tecla == "F") descricao = "Frontal — horário";
                        else if (tecla == "G") descricao = "Frontal — anti-horário";
                        else if (tecla == "L") descricao = "Esquerda — horário";
                        else if (tecla == "K") descricao = "Esquerda — anti-horário";
                        else if (tecla == "B") descricao = "Inferior — horário";
                        else if (tecla == "N") descricao = "Inferior — anti-horário";
                        else if (tecla == "A") descricao = "Traseira — horário";
                        else if (tecla == "S") descricao = "Traseira — anti-horário";
                        texto += QString("%1. %2: %3\n")
                            .arg(static_cast<qulonglong>(i + 1)).arg(tecla).arg(descricao);
                    }
                }
            }
            busca->deleteLater();
            QMessageBox mensagem(QMessageBox::Information, "Busca em largura",
                                 texto, QMessageBox::Ok, this);
            mensagem.setTextFormat(Qt::PlainText);
            mensagem.exec();
        });

        progresso->show();
        busca->start();
    }

    void iniciarBuscaProfundidade(QPushButton *botao)
    {
        ThreadBuscaProfundidade *busca =
            new ThreadBuscaProfundidade(
                cubo->getEstadoAtual(),
                10,
                this);

        QProgressDialog *progresso =
            new QProgressDialog(
                "Buscando a solução do cubo atual...",
                "Cancelar",
                0,
                0,
                this);

        progresso->setWindowTitle(
            "Busca em profundidade iterativa");

        progresso->setWindowModality(
            Qt::WindowModal);

        progresso->setMinimumDuration(0);
        progresso->setAutoClose(false);
        progresso->setAutoReset(false);

        botao->setEnabled(false);

        connect(
            progresso,
            &QProgressDialog::canceled,
            busca,
            &QThread::requestInterruption);

        connect(
            qApp,
            &QCoreApplication::aboutToQuit,
            busca,
            [busca]()
            {
                busca->requestInterruption();
                busca->wait();
            });

        connect(
            busca,
            &QThread::finished,
            this,
            [this, busca, progresso, botao]()
            {
                progresso->hide();
                progresso->deleteLater();

                botao->setEnabled(true);

                QString texto;

                if (busca->cancelada)
                {
                    texto = "Busca cancelada.";
                }
                else if (!busca->erro.isEmpty())
                {
                    texto =
                        "Não foi possível concluir a busca.\n" +
                        busca->erro;
                }
                else
                {
                    const ResultadoBusca &resultado =
                        busca->resultado;

                    texto =
                        QString("Estados visitados (incluindo o objetivo, se encontrado): %1\n\n")
                            .arg(static_cast<qulonglong>(
                                resultado.estadosVisitados));

                    if (!resultado.encontrou)
                    {
                        texto +=
                            "Nenhuma solução encontrada até o limite máximo.";
                    }
                    else if (resultado.passos.empty())
                    {
                        texto += "O cubo já está resolvido.";
                    }
                    else
                    {
                        texto += QString(
                            "Solução encontrada no limite: %1\n\n"
                            "Solução em %2 movimentos.\n"
                            "Na tela do jogo, execute as teclas nesta ordem.\n"
                            "Os sentidos são vistos de frente para a face girada.\n\n")
                            .arg(static_cast<qulonglong>(
                                resultado.limiteEncontrado))
                            .arg(static_cast<qulonglong>(
                                resultado.passos.size()));

                        for (std::size_t i = 0;
                            i < resultado.passos.size();
                            i++)
                        {
                            QString tecla =
                                QString::fromStdString(
                                    resultado.passos[i]);

                            QString descricao;

                            if (tecla == "U") descricao = "Superior — horário";
                            else if (tecla == "I") descricao = "Superior — anti-horário";
                            else if (tecla == "R") descricao = "Direita — horário";
                            else if (tecla == "T") descricao = "Direita — anti-horário";
                            else if (tecla == "F") descricao = "Frontal — horário";
                            else if (tecla == "G") descricao = "Frontal — anti-horário";
                            else if (tecla == "L") descricao = "Esquerda — horário";
                            else if (tecla == "K") descricao = "Esquerda — anti-horário";
                            else if (tecla == "B") descricao = "Inferior — horário";
                            else if (tecla == "N") descricao = "Inferior — anti-horário";
                            else if (tecla == "A") descricao = "Traseira — horário";
                            else if (tecla == "S") descricao = "Traseira — anti-horário";

                            texto += QString("%1. %2: %3\n")
                                .arg(static_cast<qulonglong>(i + 1))
                                .arg(tecla)
                                .arg(descricao);
                        }
                    }
                }

                busca->deleteLater();

                QMessageBox mensagem(
                    QMessageBox::Information,
                    "Busca em profundidade iterativa",
                    texto,
                    QMessageBox::Ok,
                    this);

                mensagem.setTextFormat(
                    Qt::PlainText);

                mensagem.exec();
            });

        progresso->show();

        busca->start();
    }

    void criarMenuBuscas()
    {
        menuBuscas = new QWidget();

        QVBoxLayout *layout =
            new QVBoxLayout(menuBuscas);

        QLabel *titulo =
            new QLabel("ESCOLHA UMA BUSCA");

            titulo->setObjectName("titulo");

        titulo->setAlignment(Qt::AlignCenter);

        QPushButton *busca1 =
            new QPushButton("Busca por largura");

        connect(busca1, &QPushButton::clicked, this, [this, busca1]() {
            iniciarBuscaLargura(busca1);
        });

        QPushButton *busca2 =
            new QPushButton("Busca por profundidade");
        connect(busca2, &QPushButton::clicked, this, [this, busca2]()
        {
            iniciarBuscaProfundidade(busca2);
        });

        QPushButton *busca3 =
            new QPushButton("Busca por estrela");

        QPushButton *comparacao =
            new QPushButton("Comparação");

        QPushButton *voltar =
            new QPushButton("Voltar");

        layout->addStretch();

        layout->addWidget(titulo);

        layout->addSpacing(40);

        layout->addWidget(busca1);
        layout->addWidget(busca2);
        layout->addWidget(busca3);

        layout->addSpacing(20);

        layout->addWidget(comparacao);

        layout->addStretch();

        layout->addWidget(voltar);

        telas->addWidget(menuBuscas);

        connect(
            voltar,
            &QPushButton::clicked,
            this,
            [this]()
            {
                telas->setCurrentWidget(menuPrincipal);
            }
        );
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    JanelaPrincipal janela;

    janela.show();

    return app.exec();
}