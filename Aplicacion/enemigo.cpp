#include "enemigo.h"

Enemigo::Enemigo(QString nombreEnemigo)
    : Personaje(nombreEnemigo) // Llamamos al constructor de la clase base
{
    if (nombreEnemigo == "Policia1")setPos(1080, 225); // Ajustamos la posición inicial del Policia1
    if (nombreEnemigo == "Policia2")setPos(560, 173); // Ajustamos la posición inicial del Policia2
}


void Enemigo::moverPolicia(unsigned short policia)
{
    if (policia == 1){
        // Aquí debo verificar si debo caminar o disparar.
        static bool cambiarDireccionDelMovimiento;
        if (contador <= 0) cambiarDireccionDelMovimiento = true;
        else if (contador >= 375) cambiarDireccionDelMovimiento = false;

        if (cambiarDireccionDelMovimiento) { // Bajar
            ActualizarImagen(0); // Esto afecta al pixmap de la clase base
            contador += 5;
            setPos(x(), y() + 5);
        } else { //Subir
            ActualizarImagen(300);
            contador -= 5;
            setPos(x(), y() - 5);
        }
    }

    if (policia == 2){
        static bool cambiarDireccionDelMovimiento;
        if (contador <= 0) cambiarDireccionDelMovimiento = true;
        else if (contador >= 370) cambiarDireccionDelMovimiento = false;

        if (cambiarDireccionDelMovimiento) { // izquierda
            ActualizarImagen(100); // Esto afecta al pixmap de la clase base
            contador += 5;
            setPos(x() - 5, y());
        } else { //Derecha
            ActualizarImagen(200);
            contador -= 5;
            setPos(x() + 5, y());
        }
    }
}

void Enemigo::recetearCOntador()
{
    contador = 0;
}

