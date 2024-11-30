#ifndef HELICOPTERO_H
#define HELICOPTERO_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

class Helicoptero : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

private:

    QGraphicsPixmapItem* misil;
    bool activeMisil = false;

public:
    Helicoptero();
    void lanzarMisil(qreal x, qreal y, QGraphicsScene* scene, bool activePower);
    void mover();
//    QGraphicsPixmapItem* getMisil();
signals:
    int posExplosion(int y);

};

#endif // HELICOPTERO_H

