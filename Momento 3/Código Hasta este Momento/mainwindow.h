#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QList>
#include <QKeyEvent>

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
    void crearLaberinto();
    bool tocarPared();
    void moverRuedas();
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *e);

public slots:


private:
    Ui::MainWindow *ui;
    QGraphicsScene *escenaLaberinto;
    Personaje *Homero; // Personaje animado
    QTimer *timer;     // Temporizador para actualizar la animación

    QList<QGraphicsRectItem*> Bloques;



    QList<QGraphicsPixmapItem*> ruedas; // Lista de items de imagen para las ruedas
    QList<float> angulos;                 // Lista de ángulos para el movimiento circular
    float anguloHelice;                   // Ángulo de rotación de la hélice
    QTimer* timerRuedas;                  // Temporizador para mover las elipses
    QPixmap imagenRueda;

};

#endif // MAINWINDOW_H

