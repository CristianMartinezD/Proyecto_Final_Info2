#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <QGraphicsPixmapItem>

class Personaje : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Personaje();
    Personaje(QString nombrePersonaje);
    void ActualizarImagen(int fila);
    void setVidas(short valor, QChar operacion);
    short getVidas();
    ~Personaje();

protected: // Cambiado de private a protected
    QPixmap pixmap;
    int columnas;
    int ancho, alto;
    QString nombrePersonaje;
    short vidas;
};

#endif // PERSONAJE_H
