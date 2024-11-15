#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
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
#include <QList>
#include <QPair>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    Homero = new Personaje();

    escenaEscape = new QGraphicsScene();
    crearEscape();

    ui->graphicsView->setScene(escenaEscape);

    connect(this,SIGNAL(llamarJuegoAhorcado()),this,SLOT(juegoAhorcado()));
    /*
    do {
        ui->graphicsView->setScene(escenaEscape);
    } while (true);
*/
/*
    escenaLaberinto = new QGraphicsScene();
    crearLaberinto();
    ui->graphicsView->setScene(escenaLaberinto);

    //Crear y añadir el objeto sprite Donald a la escena
    //Homero = new Personaje();
    escenaLaberinto->addItem(Homero);
    Homero->setPos(1300, 330);
    Rectangulo = escenaLaberinto->addRect(75, 143, 50, 50);
*/
/*

*/
}





void MainWindow::llamarActualizarImagen()
{
    Homero->ActualizarImagen();
    bool moverse = true;
    for(auto iter = Bloques.begin(); iter < Bloques.end(); ++iter){
        if(Homero->collidesWithItem(*iter)){
            Rectangulo->setPen(QPen(Qt::red, 2)); // Aquí logica de no atravezar pared
            moverse = false;
        }
        else Rectangulo->setPen(QPen(Qt::green, 2));
    }

    if (moverse) Homero->setPos(Homero->x() + 60, Homero->y());
}





void MainWindow::crearLaberinto()
{
    escenaLaberinto->setBackgroundBrush(QBrush(QColor(200, 200, 255)));

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

void MainWindow::crearEscape() {
/*
    for (int i = 0; i<1340;i+=20){
        for (int j = 0; j<1340;j+=20){
            escenaEscape->addEllipse(j,i,1,1)->setZValue(10);
        }
    }
*/
    escenaEscape->setSceneRect(0,0,1330,670);
    escenaEscape->setBackgroundBrush(QPixmap(":/imagenes/fondo.png").scaled(1340,670,Qt::IgnoreAspectRatio));

    QGraphicsPixmapItem* puerta = new QGraphicsPixmapItem(QPixmap(":/imagenes/pared.png").scaled(40,110,Qt::IgnoreAspectRatio));
    puerta->setData(0,"puerta");
    puerta->setPos(1135,270);
    escenaEscape->addItem(puerta);

    Homero->setPos(210,300);
    escenaEscape->addItem(Homero);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout,this, [=]() {
        llamarActualizarImagen();
        if (Homero->collidesWithItem(puerta)){
            QEventLoop loop;
            QTimer::singleShot(1000, &loop, &QEventLoop::quit);
            loop.exec();
            timer->stop();
            emit llamarJuegoAhorcado();
        }
    });
    timer->start(200);


}

QPair<QString, QPixmap> elegirParAleatorio(const QMap<QString, QPixmap>& personajes) {
    // Convertir las claves a una lista para acceder a ellas por índice
    QList<QString> claves = personajes.keys();
    if (claves.isEmpty()) {
        return QPair<QString, QPixmap>();  // Retorna un par vacío si el mapa está vacío
    }

    // Elegir un índice aleatorio
    int indiceAleatorio = QRandomGenerator::global()->bounded(claves.size());

    // Obtener la clave y el valor correspondientes
    QString claveAleatoria = claves[indiceAleatorio];
    QPixmap valorAleatorio = personajes.value(claveAleatoria);

    return QPair<QString, QPixmap>(claveAleatoria, valorAleatorio);
}

void MainWindow::juegoAhorcado(){

    QMap<QString,QPixmap> personajes;

    QPixmap image(":/imagenes/milhouse.png");
    QPixmap image2(":/imagenes/barney.png");
    QPixmap image3(":/imagenes/edna.png");
    QPixmap image4(":/imagenes/burns.png");
    QPixmap image5(":/imagenes/clancy.png");

    personajes.insert("Milhouse",image);
    personajes.insert("Barney",image2);
    personajes.insert("Edna",image3);
    personajes.insert("Burns",image4);
    personajes.insert("Clancy",image5);

    QPair<QString,QPixmap> par = elegirParAleatorio(personajes);

    QString respuesta = par.first;
    QPixmap pixmap = par.second;
    QGraphicsPixmapItem* imagen = new QGraphicsPixmapItem(pixmap.scaled(220,340,Qt::KeepAspectRatio));

    QPainterPath path;
    path.addRoundedRect(320,130,660,400,20,20);
    QGraphicsPathItem* rect = escenaEscape->addPath(path);
    rect->setOpacity(0.8);
    rect->setBrush(QBrush(Qt::gray));
    rect->setPen(QPen(Qt::NoPen));

    QFont fuente("Goudy Stout",20);
    QFont fuente2("Cascadia Code Semibold",10);
    QFont fuente3("Goudy Stout",30);

    QLabel* title= new QLabel("Adivina");
    title->setFont(fuente);
    title->setStyleSheet("background-color: transparent;");
    QGraphicsProxyWidget* palabraLabelItem = escenaEscape->addWidget(title);
    palabraLabelItem->setPos(400, 170);

    QString palabra(QString("_ ").repeated(respuesta.length()).trimmed());
    QLabel* palabraParcial = new QLabel(palabra);
    palabraParcial->setFont(fuente3);
    palabraParcial->setStyleSheet("background-color: transparent;");
    QGraphicsProxyWidget* palabraLabel= escenaEscape->addWidget(palabraParcial);
    palabraLabel->setPos(360, 320);

    QLineEdit* letraInput = new QLineEdit();
    letraInput->setMaxLength(1);
    letraInput->setFont(fuente2);
    letraInput->setStyleSheet("QLineEdit { color: black; font-size: 10px; padding: 5px; border: 2px solid gray; border-radius: 5px; background-color: yellow; }");
    letraInput->setPlaceholderText("Introduce una letra");
    QGraphicsProxyWidget* letraInputItem = escenaEscape->addWidget(letraInput);
    letraInputItem->setPos(408, 435);

    QPushButton* verificarBoton = new QPushButton("Verificar Letra");
    verificarBoton->setFont(fuente2);
    QPalette paleta = verificarBoton->palette();
    paleta.setColor(QPalette::Button,Qt::yellow);
    paleta.setColor(QPalette::ButtonText,Qt::black);
    verificarBoton->setPalette(paleta);
    QGraphicsProxyWidget* verificarBotonItem = escenaEscape->addWidget(verificarBoton);
    verificarBotonItem->setPos(445, 470);

    imagen->setPos(700,170);
    escenaEscape->addItem(imagen);
    imagen->setZValue(10);


}

MainWindow::~MainWindow()
{
    delete ui;
}

