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
        {1,0,1,1,0,1,0,0,0,0,1,0,1,0,1,0,1,1,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {1,0,1,1,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1},
        {1,0,0,0,1,0,0,1,0,0,1,1,1,0,1,0,1,1,0,1},
        {1,0,1,0,1,1,0,1,1,0,1,0,1,0,1,0,0,0,0,1},
        {0,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };

    for (int i = 0; i < 11; ++i){
        for (int j = 0; j < 20; ++j){
            if (configLab[i][j] == 1) Bloques.append(escenaLaberinto->addRect(j*67,i*67,67,67,QPen(Qt::black,2),QBrush(Qt::green)));
        }
    }


    // Crear items de imagen para las ruedas
    for (int i = 0; i < 3; ++i) {  // Ajusta la cantidad de ruedas si es necesario
        QGraphicsPixmapItem* rueda = new QGraphicsPixmapItem();

        QPixmap pixmap(":/imagenes/rueda.png");
        if (i == 0){
            rueda->setPixmap(pixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            rueda->setPos(90, 344);
        }
        if (i == 1){
            rueda->setPixmap(pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            rueda->setPos(510, 210);
        }
        if (i == 2){
            rueda->setPixmap(pixmap.scaled(45, 45, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            rueda->setPos(745, 267);
        }

        escenaLaberinto->addItem(rueda);
        ruedas.append(rueda);
        angulos.append(0);  // Inicializamos el ángulo de cada rueda
    }

    // Establece el punto de origen de rotación para la hélice en su centro
    ruedas[0]->setTransformOriginPoint(ruedas[0]->boundingRect().center());
    ruedas[2]->setTransformOriginPoint(ruedas[2]->boundingRect().center());
    anguloHelice = 0; // Ángulo inicial de rotación para la hélice

    // Configurar el temporizador
    timerRuedas = new QTimer(this);
    connect(timerRuedas, &QTimer::timeout, this, &MainWindow::moverRuedas);
    timerRuedas->start(50);  // Ajusta según la velocidad deseada
}





bool MainWindow::tocarPared()
{
    bool tocoLaPared = false;
    for(auto iter = Bloques.begin(); iter < Bloques.end(); ++iter){
        if(Homero->collidesWithItem(*iter)){
            tocoLaPared = true;
        }
    }

    return tocoLaPared;
}



int contador = 0;
void MainWindow::moverRuedas()
{
    const float radio = 8.0f;  // Radio del movimiento circular
    const float velocidadAngular = 0.2f;  // Velocidad de rotación
    const float velocidadRotacionHelice = 20.0f; // Velocidad de rotación de la hélice

    for (int i = 0; i < ruedas.size(); ++i) {
        if (i == 0 || i == 2) {  // La primera rueda es la hélice
            anguloHelice += velocidadRotacionHelice;
            ruedas[i]->setRotation(anguloHelice);

            // Mantener el ángulo en un rango de 0 a 360 grados
            if (anguloHelice >= 360.0f) anguloHelice -= 360.0f;

            static bool cambiarDireccionDelMovimiento;
            if (contador <= 0) cambiarDireccionDelMovimiento = true;
            else if (contador >= 300) cambiarDireccionDelMovimiento = false;
            if (cambiarDireccionDelMovimiento){
                contador += 5;
                ruedas[0]->setPos(ruedas[0]->x()+5, ruedas[0]->y());
                ruedas[2]->setPos(ruedas[2]->x(), ruedas[2]->y()+2.6);
            }
            else{
                contador -= 5;
                ruedas[0]->setPos(ruedas[0]->x()-5, ruedas[0]->y());
                ruedas[2]->setPos(ruedas[2]->x(), ruedas[2]->y()-2.6);
            }

        } else {
            angulos[i] += velocidadAngular;

            float x = ruedas[i]->x() + radio * cos(angulos[i]);
            float y = ruedas[i]->y() + radio * sin(angulos[i]);

            ruedas[i]->setPos(x, y);

            // Mantener el ángulo en un rango de 0 a 2π radianes
            if (angulos[i] >= 2 * M_PI) angulos[i] -= 2 * M_PI;
        }
    }
}





MainWindow::~MainWindow()
{
    delete ui;
}
