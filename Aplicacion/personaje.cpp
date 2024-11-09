#include "personaje.h"

Personaje::Personaje() : columnas(0), ancho(100), alto(100)
{
    pixmap = QPixmap(":/imagenes/Homero.png");
    setPixmap(pixmap.copy(columnas, 300, ancho, alto).scaled(70, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}


void Personaje::ActualizarImagen(int fila)
{
    columnas += 100;
    if (columnas >= 600) columnas = 0;
    setPixmap(pixmap.copy(columnas, fila, ancho, alto).scaled(70, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
