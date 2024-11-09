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

public slots:
    void llamarActualizarImagen();
    void crearLaberinto();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *escenaLaberinto;
    Personaje *Homero; // Personaje animado
    QTimer *timer;     // Temporizador para actualizar la animación

    QGraphicsRectItem* Rectangulo;

    QList<QGraphicsRectItem*> Bloques;
};

#endif // MAINWINDOW_H

