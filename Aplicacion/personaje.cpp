#include "personaje.h"

Personaje::Personaje() : columnas(0), ancho(100), alto(100)
{
    pixmap = QPixmap(":/imagenes/Homero.png");
    setPixmap(pixmap.copy(columnas, 300, ancho, alto).scaled(75, 75, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void Personaje::ActualizarImagen()
{
    columnas += 100;
    if (columnas >= 600) columnas = 0;
    setPixmap(pixmap.copy(columnas, 300, ancho, alto).scaled(75, 75, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
