#include "helicoptero.h"
#include <cmath>
#include <QTimer>

Helicoptero::Helicoptero() {

    setPixmap(QPixmap(":/imagenes/helicoptero.png").scaled(220,180,Qt::KeepAspectRatio));

}

void Helicoptero::lanzarMisil(qreal xFin, qreal yFin, QGraphicsScene *scene, bool activePower){

    if (activeMisil) return;
    if (activePower) return;

    activeMisil = true;

    xFin += 30;//Ajustes graficos
    yFin -= 60;//Ajustes graficos
    qreal xIni = x() + 180 ; //Ajustes graficos
    qreal yIni = y() + 100; //Ajustes graficosx

    misil = new QGraphicsPixmapItem();
    misil->setPixmap(QPixmap(":/imagenes/misil.png").scaled(50,20,Qt::KeepAspectRatio));
    misil->setPos(xIni,yIni);
    scene->addItem(misil);

    float velIniX = (xFin - xIni)/sqrt((2*(yFin - yIni))/(9.81));//Velocidad incial necesaria para alcanzar la posicion requerida
    float velIniY = 0.0;
    float t = 0.0f;
    QTimer* timer = new QTimer(this);
    connect(timer,&QTimer::timeout,[=]() mutable {
        //Movimiento parabólico
        t+=0.01f;
        float x = misil->x() + velIniX * t;
        float y = misil->y() + velIniY*t - 0.5 * 9.81 * t * t;

        velIniY += 9.81 * t ;

        misil->setPos(x,y);
        misil->setRotation(90*t);

        if (y>=yFin) {
            emit posExplosion(y);
            int columnas = 0;
            QPixmap pixmapFuego = QPixmap(":/imagenes/fuego.png");
            QTimer*timer2 = new QTimer(this);//Animacion de la explosion
            connect(timer2,&QTimer::timeout,[=]()mutable{
                misil->setPixmap(pixmapFuego.copy(columnas,0,100,100).scaled(100,100,Qt::KeepAspectRatio));
                columnas+= 100;
                if(columnas>500){
                    timer2->stop();
                    scene->removeItem(misil);
                    delete misil;
                    activeMisil = false;
                }
            });
            timer2->start(70);
            timer->stop();
        }
    });
    timer->start(50);
}

void Helicoptero::mover() {

    int xMin = 20;
    int xMax = 100;

    int yMin = 40;
    int yMax = 80;

    static bool adelante = true;
    static bool arriba = false;

    if (adelante) {
        // Si está yendo hacia adelante y alcanza el límite, cambia de dirección
        if (x() + 1 > xMax) {
            adelante = false;
        } else {
            setX(x() + 1); // Mueve hacia adelante
        }
    } else {
        // Si está yendo hacia atrás y alcanza el límite, cambia de dirección
        if (x() - 1 < xMin) {
            adelante = true;
        } else {
            setX(x() - 1); // Mueve hacia atrás
        }
    }

    if (arriba) {
        if (y() - 1 < yMin) {
            arriba = false;
        } else {
            setY(y() - 1);
        }
    } else {
        if (y() + 1 > yMax) {
            arriba = true;
        } else {
            setY(y() + 1);
        }
    }
}
