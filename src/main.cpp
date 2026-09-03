#include "interface/JanelaPrincipal.hpp"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication aplicacao(argc, argv);
    aplicacao.setApplicationName("Cubo Mágico 2x2x2");
    aplicacao.setStyle("Fusion");
    JanelaPrincipal janela;
    janela.show();
    return aplicacao.exec();
}
