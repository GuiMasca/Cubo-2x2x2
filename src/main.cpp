#include <QApplication>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class CuboWidget : public QOpenGLWidget,
                   protected QOpenGLFunctions
{
public:
    CuboWidget(QWidget *parent = nullptr)
        : QOpenGLWidget(parent)
    {
    }

protected:

    void initializeGL() override
    {
        initializeOpenGLFunctions();

        glClearColor(
             0.1f,   // vermelho
        0.3f,   // verde
        0.8f,   // azul
        1.0f    // opacidade
        );
    }

    void resizeGL(int largura, int altura) override
    {
        glViewport(
            0,
            0,
            largura,
            altura
        );
    }

    void paintGL() override
    {
        glClear(
            GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT
        );
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    CuboWidget janela;

    janela.setWindowTitle("Cubo Magico 2x2x2");
    janela.resize(900, 700);
    janela.show();

    return app.exec();
}