#include "personaje.h"

Personaje::Personaje(QString personaje) : columnas(0), ancho(100), alto(100)
{
    if(personaje == "Homero") pixmap = QPixmap(":/imagenes/Homero.png");
    setPixmap(pixmap.copy(columnas, 100, ancho, alto).scaled(56, 56, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    /*
    if(personaje == "Enemigo") pixmap = QPixmap(":/imagenes/Enemigo.png");
    setPixmap(pixmap.copy(columnas, 100, ancho, alto).scaled(56, 56, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    */
}


void Personaje::ActualizarImagen(int fila)
{
    columnas += 100;
    if (columnas >= 600) columnas = 0;
    setPixmap(pixmap.copy(columnas, fila, ancho, alto).scaled(56, 56, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

Personaje::~Personaje(){}
