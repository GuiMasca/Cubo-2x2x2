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
#include <iostream>
#include "CubeState.h"

class CuboWidget : public QOpenGLWidget,
                   protected QOpenGLFunctions
{
public:
    CuboWidget(QWidget *parent = nullptr)
        : QOpenGLWidget(parent), m_program(nullptr), m_vbo(QOpenGLBuffer::VertexBuffer)
    {
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
            std::cout << "Movimento R executado!" << std::endl;
        } 
        else if (event->key() == Qt::Key_U) {
            meuCubo = meuCubo.moveU();
            std::cout << "Movimento U executado!" << std::endl;
        }
        else if (event->key() == Qt::Key_F) {
            meuCubo = meuCubo.moveF();
            std::cout << "Movimento F executado!" << std::endl;
        }
        else if (event->key() == Qt::Key_L) {
            meuCubo = meuCubo.moveL();
            std::cout << "Movimento L executado!" << std::endl;
        }
        else if (event->key() == Qt::Key_A) {
            meuCubo = meuCubo.moveA();
            std::cout << "Movimento A executado!" << std::endl;
        }
        else if (event->key() == Qt::Key_B) {
            meuCubo = meuCubo.moveB();
            std::cout << "Movimento B executado!" << std::endl;
        }
        else if (event->key() == Qt::Key_N) {
            meuCubo = meuCubo.moveN();
            std::cout << "Movimento de baixo anti-horario (N) executado!" << std::endl;
        }
        else if (event->key() == Qt::Key_I) {
            meuCubo = meuCubo.moveI();
            std::cout << "Movimento de cima anti-horario (I) executado!" << std::endl;
        }
        else if (event->key() == Qt::Key_K) {
            meuCubo = meuCubo.moveK();
            std::cout << "Movimento da esquerda anti-horario (K) executado!" << std::endl;
        }
        else if (event->key() == Qt::Key_S) {
            meuCubo = meuCubo.moveS();
            std::cout << "Movimento de tras anti-horario (S) executado!" << std::endl;
        }
        else if (event->key() == Qt::Key_G) {
            meuCubo = meuCubo.moveG();
            std::cout << "Movimento da frente anti-horario (G) executado!" << std::endl;
        }
        else if (event->key() == Qt::Key_T) {
            meuCubo = meuCubo.moveT();
            std::cout << "Movimento da direita anti-horario (T) executado!" << std::endl;
        }
        
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
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    CuboWidget janela;
    janela.setWindowTitle("Cubo Mágico 2x2x2 - Jogável (Teclas U, B, F, A, R, L)");
    janela.resize(900, 700);
    janela.show();
    return app.exec();
}