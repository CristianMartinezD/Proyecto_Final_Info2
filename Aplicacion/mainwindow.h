#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <QGraphicsRectItem>
#include <QList>

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
    ~MainWindow();

signals:
    void llamarJuegoAhorcado();

public slots:
    void llamarActualizarImagen();
    void crearLaberinto();
    void juegoAhorcado();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *escenaEscape;
    QGraphicsScene *escenaLaberinto;
    Personaje *Homero; // Personaje animado
    QTimer *timer;     // Temporizador para actualizar la animación

    QGraphicsRectItem* Rectangulo;

    QList<QGraphicsRectItem*> Bloques;

    void crearEscape();

};

#endif // MAINWINDOW_H

