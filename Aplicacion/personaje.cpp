#include "personaje.h"

Personaje::Personaje(){}

Personaje::Personaje(QString _nombrePersonaje) : columnas(0), ancho(100), alto(100)
{
    nombrePersonaje = _nombrePersonaje;
    if(_nombrePersonaje == "Homero"){
        pixmap = QPixmap(":/imagenes/Homero.png");
        setPixmap(pixmap.copy(columnas, 100, ancho, alto).scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        vidas = 7;
    }

    else if(_nombrePersonaje == "Policia1" || _nombrePersonaje == "Policia2"){
        pixmap = QPixmap(":/imagenes/policia.png");
        setPixmap(pixmap.copy(columnas, 0, ancho, alto).scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        vidas = 1;
    }
}


void Personaje::ActualizarImagen(int fila)
{
    columnas += 100;
    if(nombrePersonaje == "Homero"){
        if (columnas >= 600) columnas = 0;
    }
    else if(nombrePersonaje == "Policia1" || nombrePersonaje == "Policia2"){
        if (columnas >= 300) columnas = 0;
    }
    setPixmap(pixmap.copy(columnas, fila, ancho, alto).scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void Personaje::setVidas(short valor, QChar operacion)
{
    if (operacion == '-') vidas -= valor;
    else vidas += valor;
}

short Personaje::getVidas()
{
    return vidas;
}

Personaje::~Personaje(){}
