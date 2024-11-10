#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <QGraphicsPixmapItem>

class Personaje : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Personaje();

public slots:
    void ActualizarImagen(int fila);

private:
    QPixmap pixmap;
    int columnas;
    int ancho, alto;
};

#endif // PERSONAJE_H
