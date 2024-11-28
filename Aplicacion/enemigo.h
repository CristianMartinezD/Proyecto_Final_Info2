#ifndef ENEMIGO_H
#define ENEMIGO_H
#include "personaje.h"
#include <QString>

class Enemigo : public Personaje
{
    Q_OBJECT
public:
    Enemigo(QString nombreEnemigo);
    void moverPolicia(unsigned short policia);
    void recetearCOntador();
private:
    int contador = 0;
};


#endif // ENEMIGO_H
