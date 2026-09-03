#include "interface/VisualizadorCubo.hpp"
#include <QMouseEvent>
#include <QPainter>
#include <QVector3D>
#include <algorithm>
#include <array>
#include <cmath>
#include <vector>

namespace {
QColor corQt(cubo::Cor c) {
    switch(c) {
        case cubo::Cor::Branco:return QColor("#f7f7f7"); case cubo::Cor::Vermelho:return QColor("#d62828");
        case cubo::Cor::Verde:return QColor("#20a05a"); case cubo::Cor::Amarelo:return QColor("#ffd60a");
        case cubo::Cor::Laranja:return QColor("#f77f00"); case cubo::Cor::Azul:return QColor("#277da1");
    } return Qt::black;
}
QVector3D girar(QVector3D p,double ax,double ay) {
    const double rx=ax*3.141592653589793/180.0, ry=ay*3.141592653589793/180.0;
    const double cy=std::cos(ry),sy=std::sin(ry),cx=std::cos(rx),sx=std::sin(rx);
    QVector3D q(p.x()*cy+p.z()*sy,p.y(),-p.x()*sy+p.z()*cy);
    return {q.x(),static_cast<float>(q.y()*cx-q.z()*sx),
            static_cast<float>(q.y()*sx+q.z()*cx)};
}
std::array<QVector3D,4> quadradoFace(int f,int i) {
    const float a=(i%2==0)?-0.92f:0.04f,b=(i/2==0)?0.92f:-0.04f,c=a+0.88f,d=b-0.88f;
    switch(f) {
        case 0:return {{{a,1,b},{c,1,b},{c,1,d},{a,1,d}}};
        case 1:return {{{1,b,-a},{1,b,-c},{1,d,-c},{1,d,-a}}};
        case 2:return {{{a,b,1},{c,b,1},{c,d,1},{a,d,1}}};
        case 3:return {{{a,-1,-b},{c,-1,-b},{c,-1,-d},{a,-1,-d}}};
        case 4:return {{{-1,b,a},{-1,b,c},{-1,d,c},{-1,d,a}}};
        default:return {{{-a,b,-1},{-c,b,-1},{-c,d,-1},{-a,d,-1}}};
    }
}
std::array<QVector3D,4> faceCompleta(int f) {
    switch(f) {
        case 0:return {{{-1,1,1},{1,1,1},{1,1,-1},{-1,1,-1}}};
        case 1:return {{{1,1,1},{1,1,-1},{1,-1,-1},{1,-1,1}}};
        case 2:return {{{-1,1,1},{1,1,1},{1,-1,1},{-1,-1,1}}};
        case 3:return {{{-1,-1,-1},{1,-1,-1},{1,-1,1},{-1,-1,1}}};
        case 4:return {{{-1,1,-1},{-1,1,1},{-1,-1,1},{-1,-1,-1}}};
        default:return {{{1,1,-1},{-1,1,-1},{-1,-1,-1},{1,-1,-1}}};
    }
}
QVector3D normalFace(int f) {
    static const QVector3D normais[]={{0,1,0},{1,0,0},{0,0,1},{0,-1,0},{-1,0,0},{0,0,-1}};
    return normais[f];
}
}

VisualizadorCubo::VisualizadorCubo(QWidget* p):QWidget(p) { setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding); setCursor(Qt::OpenHandCursor); }
void VisualizadorCubo::definirEstado(const cubo::EstadoCubo& e) { estado_=e; update(); }
QSize VisualizadorCubo::minimumSizeHint() const { return {560,500}; }
void VisualizadorCubo::mousePressEvent(QMouseEvent* e) { ultimoMouse_=e->pos(); setCursor(Qt::ClosedHandCursor); }
void VisualizadorCubo::mouseMoveEvent(QMouseEvent* e) {
    if (!(e->buttons()&Qt::LeftButton)) { setCursor(Qt::OpenHandCursor); return; }
    const QPoint d=e->pos()-ultimoMouse_; ultimoMouse_=e->pos(); anguloY_+=d.x()*0.6;
    anguloX_=std::clamp(anguloX_+d.y()*0.6,-89.0,89.0); update();
}
void VisualizadorCubo::mouseReleaseEvent(QMouseEvent*) { setCursor(Qt::OpenHandCursor); }
void VisualizadorCubo::paintEvent(QPaintEvent*) {
    QPainter p(this); p.setRenderHint(QPainter::Antialiasing); p.fillRect(rect(),QColor("#171a21"));
    const auto cores=estado_.adesivos();
    struct FaceVisivel { int indice; std::array<QVector3D,4> corpo; double profundidade; };
    std::vector<FaceVisivel> faces;
    const QVector3D camera(0,0,5);
    for(int f=0;f<6;++f) {
        auto corpo=faceCompleta(f); for(auto& v:corpo)v=girar(v,anguloX_,anguloY_);
        const auto centro=(corpo[0]+corpo[1]+corpo[2]+corpo[3])/4.0f;
        const auto normal=girar(normalFace(f),anguloX_,anguloY_);
        if(QVector3D::dotProduct(normal,camera-centro)>0) faces.push_back({f,corpo,centro.z()});
    }
    std::sort(faces.begin(),faces.end(),[](const auto&a,const auto&b){return a.profundidade<b.profundidade;});
    const double escala=std::min(width(),height())*0.30;
    auto proj=[&](const QVector3D& v){const double k=4.5/(4.5-v.z());return QPointF(width()/2.0+v.x()*escala*k,height()/2.0-v.y()*escala*k);};
    for(const auto& face:faces) {
        QPolygonF corpo; for(const auto& v:face.corpo)corpo<<proj(v);
        p.setPen(QPen(QColor("#050607"),5)); p.setBrush(QColor("#090b0e")); p.drawPolygon(corpo);
        for(int i=0;i<4;++i) {
            auto pts=quadradoFace(face.indice,i); for(auto& v:pts)v=girar(v,anguloX_,anguloY_);
            QPolygonF adesivo; for(const auto& v:pts)adesivo<<proj(v);
            p.setPen(QPen(QColor("#08090b"),3,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
            p.setBrush(corQt(cores[face.indice*4+i])); p.drawPolygon(adesivo);
        }
    }
    p.setPen(QColor("#bdc7d5"));p.drawText(QRect(0,height()-35,width(),25),Qt::AlignCenter,"Arraste com o botão esquerdo para girar o cubo");
}
