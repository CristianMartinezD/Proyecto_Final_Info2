#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Homero = new Personaje("Homero");

    cambiarDeEscena(1);

}


void MainWindow::cambiarDeEscena(int escena)
{
    if (escena == 1) { /* CONFIGURAMOS TODO PARA LA ESCENA 1 (Abrir puerta - JUAN) */
        ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        escenaEscape = new QGraphicsScene();
        crearEscape();

        ui->graphicsView->setScene(escenaEscape);

        //connect(this,SIGNAL(llamarJuegoAhorcado()),this,SLOT(juegoAhorcado()));
    }

    else if (escena == 2){ /* CONFIGURAMOS TODO PARA LA ESCENA 2 (Laberinto - CRISTIAN) */
        //disconnect(this, SIGNAL(llamarJuegoAhorcado()), nullptr, nullptr);
        //disconnect(timer, &QTimer::timeout, this, nullptr);

        ui->graphicsView->setBackgroundBrush(QBrush(QColor(200, 200, 255)));

        escenaLaberinto = new QGraphicsScene();
        escenaActual = 2;
        crearLaberinto();
        ui->graphicsView->setScene(escenaLaberinto);
        escenaLaberinto->addItem(Homero);
        Homero->ActualizarImagen(100);
        Homero->setPos(1300, 340);
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

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout,this, [=]() {
        Homero->ActualizarImagen(0);
        Homero->setPos(Homero->x() + 30, Homero->y());
        if (Homero->collidesWithItem(puerta)){
            Homero->setPos(Homero->x() - 30, Homero->y());
            QEventLoop loop;
            QTimer::singleShot(500, &loop, &QEventLoop::quit);
            loop.exec();

            juegoAhorcado(); //PINTAR VENTANA "ADIVINA"

            disconnect(timer, &QTimer::timeout, this, nullptr);
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

QPair<qreal, qreal> centrar(QGraphicsTextItem* texto){

    QRectF areaCentro(380,350,265,40);
    QRectF textoRect = texto->boundingRect();
    qreal xCentrado = areaCentro.x() + (areaCentro.width() - textoRect.width()) / 2;
    qreal yCentrado = areaCentro.y() + (areaCentro.height() - textoRect.height()) / 2;
    return QPair<qreal, qreal> (xCentrado, yCentrado);

}

void MainWindow::juegoAhorcado(){

    QMap<QString,QPixmap> personajes;

    QPixmap image(":/imagenes/milhouse.png");
    QPixmap image2(":/imagenes/barney.png");
    QPixmap image3(":/imagenes/nelson.png");
    QPixmap image4(":/imagenes/burns.png");
    QPixmap image5(":/imagenes/clancy.png");
    QPixmap image6(":/imagenes/seymour.png");
    QPixmap image7(":/imagenes/jeff.png");


    personajes.insert("MILHOUSE",image);
    personajes.insert("BARNEY",image2);
    personajes.insert("NELSON",image3);
    personajes.insert("BURNS",image4);
    personajes.insert("CLANCY",image5);
    personajes.insert("SEYMOUR",image6);
    personajes.insert("JEFF",image7);

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
    QFont fuente3("Goudy Stout",15);

    QGraphicsTextItem* title = new QGraphicsTextItem("Adivina");
    title->setFont(fuente);
    title->setPos(400,170);
    escenaEscape->addItem(title);

    unsigned short int contadorTiempo = 30;
    QGraphicsTextItem* tiemporestante = new QGraphicsTextItem("Time: "+QString::number(contadorTiempo));
    tiemporestante->setFont(fuente3);
    tiemporestante->setPos(435,210);
    escenaEscape->addItem(tiemporestante);

    QString espacios = QString("_ ").repeated(respuesta.length()).trimmed();
    QGraphicsTextItem* palabraParcial = new QGraphicsTextItem(espacios);
    palabraParcial->setFont(fuente3);
    QPair<qreal,qreal> coordenadas = centrar(palabraParcial);
    palabraParcial->setPos(coordenadas.first,coordenadas.second);
    escenaEscape->addItem(palabraParcial);

    QLineEdit* letraInput = new QLineEdit();
    letraInput->setMaxLength(1);
    letraInput->setFont(fuente2);
    letraInput->setPlaceholderText("Introduce una letra");
    letraInput->setStyleSheet("QLineEdit { color: black; font-size: 10px; padding: 5px; border: 2px solid gray; border-radius: 5px; background-color: yellow; }");
    QGraphicsProxyWidget* letraInputItem = escenaEscape->addWidget(letraInput);
    letraInputItem->setPos(412, 435);

    QPushButton* verificarBoton = new QPushButton("Verificar Letra");
    verificarBoton->setFont(fuente2);
    QPalette paleta = verificarBoton->palette();
    paleta.setColor(QPalette::Button,Qt::yellow);
    paleta.setColor(QPalette::ButtonText,Qt::black);
    verificarBoton->setPalette(paleta);
    QGraphicsProxyWidget* verificarBotonItem = escenaEscape->addWidget(verificarBoton);
    verificarBotonItem->setPos(448, 470);

    imagen->setPos(700,170);
    escenaEscape->addItem(imagen);
    imagen->setZValue(10);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout,this, [=]() mutable {
        if (contadorTiempo > 0){
            contadorTiempo--;
            tiemporestante->setPlainText("Time: "+QString::number(contadorTiempo));
        }else{
            timer->stop();
            escenaEscape->removeItem(rect);
            escenaEscape->removeItem(title);
            escenaEscape->removeItem(tiemporestante);
            escenaEscape->removeItem(palabraParcial);
            escenaEscape->removeItem(letraInputItem);
            escenaEscape->removeItem(verificarBotonItem);
            escenaEscape->removeItem(imagen);
        }
    timer->start(1000);

    connect(verificarBoton, &QPushButton::clicked, this, [=]() mutable {

        QString letra = letraInput->text().toUpper();
        letraInput->clear();
        if (letra.isEmpty() || letra.length() != 1) {
            return;
        }
        for (int i = 0; i < respuesta.length(); ++i) {
            if (respuesta[i] == letra[0]) {
                espacios[i * 2] = letra[0];
            }
        }
        palabraParcial->setPlainText(espacios);
        QPair<qreal,qreal> coordenadas = centrar(palabraParcial);
        palabraParcial->setPos(coordenadas.first,coordenadas.second);

        if (!espacios.contains('_')){

            escenaEscape->removeItem(rect);
            escenaEscape->removeItem(title);
            escenaEscape->removeItem(tiemporestante);
            escenaEscape->removeItem(palabraParcial);
            escenaEscape->removeItem(letraInputItem);
            escenaEscape->removeItem(verificarBotonItem);
            escenaEscape->removeItem(imagen);

            for (auto item : escenaEscape->items()){
                if (item->data(0).toString() == "puerta"){
                    escenaEscape->removeItem(item);
                }
            }
            QTimer* timer2 = new QTimer(this);
            connect(timer2,&QTimer::timeout,this,[=](){
                Homero->ActualizarImagen(0);
                Homero->setPos(Homero->x()+30,Homero->y());
                if (Homero->x()>1330){
                    timer2->stop();
                    cambiarDeEscena(2);
                }
            });
            timer2->start(200);
        }
    });
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
    for (int i = 0; i < 3; ++i) {
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

    // Establecer el punto de origen de rotación para la hélice en su centro
    ruedas[0]->setTransformOriginPoint(ruedas[0]->boundingRect().center());
    ruedas[2]->setTransformOriginPoint(ruedas[2]->boundingRect().center());
    anguloHelice = 0; // Ángulo inicial de rotación para la hélice


    connect(timer, SIGNAL(timeout()), this, SLOT(controlDeTemporizadores()));
    timer->start(50); // Reloj Base actualizado
}




void MainWindow::controlDeTemporizadores()
{
    if (escenaActual == 1){
        // Llamar a la función que sea necesario invocar cada 200 mls (con un contador podemos modificar ese tiempo)
    }

    else if (escenaActual == 2){
        moverRuedas(); // Esta función esta siendo invocada cada 50 mls
    }

}








// MÉTODO QUE DETECTA CUANDO SE PRECIONAN TECLAS //
void MainWindow::keyPressEvent(QKeyEvent *e)
{
    // Guardamos la posición actual del personaje antes de moverlo
    qreal posXAnterior = Homero->x();
    qreal posYAnterior = Homero->y();

    switch (e->key()) {
        case Qt::Key_D: {  // Mover a la derecha
            Homero->ActualizarImagen(0);
            Homero->setPos(Homero->x() + 5, Homero->y());  // Intentar mover a la derecha
            if (tocarPared()) {  // Si colisiona, restauramos a la posición anterior
                Homero->setPos(posXAnterior, posYAnterior);
            }
            break;
        }
        case Qt::Key_A: {  // Mover a la izquierda
            Homero->ActualizarImagen(100);
            Homero->setPos(Homero->x() - 5, Homero->y());  // Intentar mover a la izquierda
            if (tocarPared()) {
                Homero->setPos(posXAnterior, posYAnterior);
            }
            break;
        }
        case Qt::Key_W: {  // Mover hacia arriba
            Homero->ActualizarImagen(300);
            Homero->setPos(Homero->x(), Homero->y() - 5);  // Intentar mover hacia arriba
            if (tocarPared()) {
                Homero->setPos(posXAnterior, posYAnterior);
            }
            break;
        }
        case Qt::Key_Z: {  // Mover hacia abajo
            Homero->ActualizarImagen(200);
            Homero->setPos(Homero->x(), Homero->y() + 5);  // Intentar mover hacia abajo
            if (tocarPared()) {
                Homero->setPos(posXAnterior, posYAnterior);
            }
            break;
        }
    }
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

            // Mantenemos el ángulo en un rango de 0 a 360 grados
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

            // Mantenemos el ángulo en un rango de 0 a 2π radianes
            if (angulos[i] >= 2 * M_PI) angulos[i] -= 2 * M_PI;
        }
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}
