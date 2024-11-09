#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setBackgroundBrush(QBrush(QColor(200, 200, 255)));

    escenaLaberinto = new QGraphicsScene();
    crearLaberinto();
    ui->graphicsView->setScene(escenaLaberinto);

    //Crear y añadir el objeto sprite Donald a la escena
    Homero = new Personaje();
    escenaLaberinto->addItem(Homero);
    Homero->setPos(1300, 330);
    Rectangulo = escenaLaberinto->addRect(75, 143, 50, 50);

    timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()), this, SLOT(llamarActualizarImagen()));
    timer->start(100); // modifica la velocidad en que itera entre las imágenes

}





// MÉTODO QUE DETECTA CUANDO SE PRECIONAN TECLAS //
void MainWindow::keyPressEvent(QKeyEvent *e)
{
    // Guardamos la posición actual del personaje antes de moverlo
    qreal posXAnterior = Homero->x();
    qreal posYAnterior = Homero->y();

    switch (e->key()) {
        case Qt::Key_D: {  // Mover a la derecha
            Homero->ActualizarImagen(100);
            Homero->setPos(Homero->x() + 5, Homero->y());  // Intentar mover a la derecha
            if (tocarPared()) {  // Si colisiona, restauramos a la posición anterior
                Homero->setPos(posXAnterior, posYAnterior);
            }
            break;
        }
        case Qt::Key_A: {  // Mover a la izquierda
            Homero->ActualizarImagen(300);
            Homero->setPos(Homero->x() - 5, Homero->y());  // Intentar mover a la izquierda
            if (tocarPared()) {
                Homero->setPos(posXAnterior, posYAnterior);
            }
            break;
        }
        case Qt::Key_W: {  // Mover hacia arriba
            Homero->ActualizarImagen(200);
            Homero->setPos(Homero->x(), Homero->y() - 5);  // Intentar mover hacia arriba
            if (tocarPared()) {
                Homero->setPos(posXAnterior, posYAnterior);
            }
            break;
        }
        case Qt::Key_Z: {  // Mover hacia abajo
            Homero->ActualizarImagen(0);
            Homero->setPos(Homero->x(), Homero->y() + 5);  // Intentar mover hacia abajo
            if (tocarPared()) {
                Homero->setPos(posXAnterior, posYAnterior);
            }
            break;
        }
    }
}






void MainWindow::crearLaberinto()
{
    int configLab[11][20] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,1,1,1,1,0,0,0,0,0,0,0,0,1,0,1,1,1,1},
        {1,0,0,0,0,0,0,1,1,0,1,0,1,0,1,0,0,0,0,1},
        {1,0,1,1,1,1,0,0,0,0,1,1,1,0,1,0,1,1,0,1},
        {1,0,1,1,0,1,0,1,1,0,1,1,1,0,1,0,1,1,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {1,0,1,1,1,1,0,1,1,0,1,1,1,0,1,0,1,1,0,1},
        {1,0,0,0,1,1,0,1,0,0,1,1,1,0,1,0,1,1,0,1},
        {1,0,1,0,1,1,0,1,1,0,1,0,1,0,1,0,0,0,0,1},
        {0,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };

    for (int i = 0; i < 11; ++i){
        for (int j = 0; j < 20; ++j){
            if (configLab[i][j] == 1) Bloques.append(escenaLaberinto->addRect(j*67,i*67,67,67,QPen(Qt::black,2),QBrush(Qt::green)));
        }
    }
}





bool MainWindow::tocarPared()
{
    bool tocoLaPared = false;
    for(auto iter = Bloques.begin(); iter < Bloques.end(); ++iter){
        if(Homero->collidesWithItem(*iter)){
            Rectangulo->setPen(QPen(Qt::red, 2)); // Aquí logica de no atravezar pared
            tocoLaPared = true;
        }
        else Rectangulo->setPen(QPen(Qt::green, 2));
    }

    return tocoLaPared;
}



MainWindow::~MainWindow()
{
    delete ui;
}
