#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/* LIBRERIAS AGREGADAS POR CRISTIAN */
#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QList>
#include <QKeyEvent>
/*##################################*/


/* LIBRERIAS AGREGADAS POR JUAN LUIS */
#include <QDebug>
#include <QApplication>
#include <QWidget>
#include <QPainter>
//Ahorcado
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QGraphicsTextItem>
#include <QGraphicsProxyWidget>
#include <QMap>
#include <QPixmap>
#include <QString>
#include <QRandomGenerator>
#include <QPair>
/*##################################*/

#include "personaje.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void crearLaberinto(); // Cristian
    bool tocarPared();     // Cristian
    void moverRuedas();    // Cristian
    void cambiarDeEscena(int escena); // Cristian
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *e);

public slots:
    void controlDeTemporizadores(); // Cristian
    void juegoAhorcado(); // Juan

private:
    Ui::MainWindow *ui;
    /* CRISTIAN */
    QGraphicsScene *escenaLaberinto;
    bool misionSuperada = false; // Si cambia a true configuramos la siguiente escena y la ponemos en el GraphicsView
    Personaje *Homero;
    QTimer *timer, *timer1;     // timer para invocar cada 50 mls a controlDeTemporizadores();
    unsigned short Temporizadores[5] = {0, 0, 0, 0, 0};

    QList<QGraphicsRectItem*> Bloques;

    QList<QGraphicsPixmapItem*> ruedas; // Lista de items de imagen para las ruedas
    QList<float> angulos;                 // Lista de ángulos para el movimiento circular
    float anguloHelice;                   // Ángulo de rotación de la hélice
    QPixmap imagenRueda;

    int escenaActual = 1;
    /*############# FIN CRISTIAN ################*/


    /* JUAN LUIS */
    QGraphicsScene *escenaEscape;
    void crearEscape();

};

#endif // MAINWINDOW_H

