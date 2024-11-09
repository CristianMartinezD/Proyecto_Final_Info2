#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <QGraphicsRectItem>
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
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *e);

public slots:


private:
    Ui::MainWindow *ui;
    QGraphicsScene *escenaLaberinto;
    Personaje *Homero; // Personaje animado
    QTimer *timer;     // Temporizador para actualizar la animación

    QGraphicsRectItem* Rectangulo;

    QList<QGraphicsRectItem*> Bloques;
};

#endif // MAINWINDOW_H

