#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Homero = new Personaje("Homero");

    cambiarDeEscena(3);

}

void MainWindow::crearMenu(){
/*
    for (int i = 0; i<1340;i+=20){
        for (int j = 0; j<1340;j+=20){
            escenaMenu->addEllipse(j,i,1,1)->setZValue(10);
        }
    }
*/
    escenaMenu->setSceneRect(0,0,1330,670);
    escenaMenu->setBackgroundBrush(QPixmap(":/imagenes/fondoMenu2.png").scaled(1330,670,Qt::IgnoreAspectRatio));

    QPushButton* nuevaPartidaButton = new QPushButton("Nueva Partida");
    QPushButton* continuarPartidaButton = new QPushButton("Continuar");

    QFont fuente = QFont("Upheaval TT (BRK)",30);

    QPalette paleta;
    paleta.setColor(QPalette::Button,QColorConstants::Cyan);
    paleta.setColor(QPalette::ButtonText,Qt::black);
    nuevaPartidaButton->setFont(fuente);
    continuarPartidaButton->setFont(fuente);
    nuevaPartidaButton->setPalette(paleta);
    continuarPartidaButton->setPalette(paleta);

    QGraphicsProxyWidget* proxyNuevaPartida = escenaMenu->addWidget(nuevaPartidaButton);
    QGraphicsProxyWidget* proxyContinuar = escenaMenu->addWidget(continuarPartidaButton);

    proxyNuevaPartida->setPos(295,500);
    proxyContinuar->setPos(720,500);

    connect(nuevaPartidaButton, &QPushButton::clicked, this, [=]() mutable {
/*
 *  LIBERAR MEMORIA
        escenaMenu->removeItem(proxyContinuar);
        escenaMenu->removeItem(proxyNuevaPartida);
        delete proxyContinuar;
        delete proxyNuevaPartida;
*/
        cambiarDeEscena(1);
    });

    connect(continuarPartidaButton, &QPushButton::clicked, this, [=] () mutable {
    /*
     * LIBERAR MEMORIA ESCENA ACTUAL
     *
     *  FUNCION CARGAR ESCENA Y DATOS DESDE UN ARCHIVO;
    */
    });
}

void MainWindow::cambiarDeEscena(int escena)
{
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    if (escena == 0){
        escenaMenu = new QGraphicsScene();
        crearMenu();
        ui->graphicsView->setScene(escenaMenu);
    }
    else if  (escena == 1) { /* CONFIGURAMOS TODO PARA LA ESCENA 1 (Abrir puerta - JUAN) */


        escenaEscape = new QGraphicsScene();
        crearEscape();

        ui->graphicsView->setScene(escenaEscape);
    }

    else if (escena == 2){ /* CONFIGURAMOS TODO PARA LA ESCENA 2 (Laberinto - CRISTIAN) */

        QImage imagen(":/imagenes/fondoLaberinto.png");
        escenaLaberinto = new QGraphicsScene();

        escenaLaberinto->setBackgroundBrush(imagen.scaled(1516,750));

        escenaLaberinto->setSceneRect(0, 0, 1917, 917); //Camara.

        escenaActual = 2;
        crearLaberinto();
        ui->graphicsView->setScene(escenaLaberinto);
        escenaLaberinto->addItem(Homero);
        Homero->ActualizarImagen(100);
        Homero->setPos(1325, 413);

        Policia1 = new Enemigo("Policia1");
        escenaLaberinto->addItem(Policia1);
        Policia2 = new Enemigo("Policia2");
        escenaLaberinto->addItem(Policia2);
        configurarElementosDeLaEscenaLaberinto();
        iniciarResortes();

        musicaFondoLaberinto = new QMediaPlayer(this);
        musicaFondoLaberinto->setSource(QUrl("qrc:/audios/fondoLaberinto.mp3"));
        volumenFondoLaberinto = new QAudioOutput(this);
        volumenFondoLaberinto->setVolume(0.6);
        musicaFondoLaberinto->setAudioOutput(volumenFondoLaberinto);
        musicaFondoLaberinto->play();
        sonidoDisparo = new QMediaPlayer(this);
        sonidoDisparo->setSource(QUrl("qrc:/audios/disparo.mp3"));
        volumenDisparo = new QAudioOutput(this);
        volumenDisparo->setVolume(0.8);
        sonidoDisparo->setAudioOutput(volumenDisparo);
    }
    else if (escena == 3) {

        escenaCarrera = new QGraphicsScene();
        crearCarrera();
        ui->graphicsView->setScene(escenaCarrera);
    }
}




void MainWindow::crearEscape() {

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

    unsigned short int contadorTiempo = 50;
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
        }else{ /* HAS PERRDIDO */
            /* Preguntamos al usuario si desea reintentar la partida, ir al nivel 2 o Salir */
            // Si elige ir al nivel 2, hacemos lo siguiente:
            timer->disconnect(); // Desconectamos todas las señales conectadas a este timer.
            timer->stop();
            if (!removido) {
            /* LIBERAMOS MEMORIA */
                escenaEscape->removeItem(rect);
                escenaEscape->removeItem(title); delete title;
                escenaEscape->removeItem(tiemporestante); delete tiemporestante;
                escenaEscape->removeItem(palabraParcial); delete palabraParcial;
                escenaEscape->removeItem(letraInputItem);
                escenaEscape->removeItem(verificarBotonItem);
                escenaEscape->removeItem(imagen); delete imagen;
                delete letraInput;
                delete verificarBoton;
                removido = !removido;
            }
        }
    });
    timer->start(1000);

    connect(verificarBoton, &QPushButton::clicked, this, [=]() mutable {

        QString letra = letraInput->text().toUpper();
        letraInput->clear();
        if (letra.isEmpty() || letra.length() != 1) {
            return;
        }
        bool acierto = false;
        for (int i = 0; i < respuesta.length(); ++i) {
            if (respuesta[i] == letra[0]) {
                espacios[i * 2] = letra[0];
                acierto = true;
            }
        }
        if (!acierto) {
            QColor originalColor = palabraParcial->defaultTextColor();
            palabraParcial->setDefaultTextColor(Qt::red);
            QTimer::singleShot(300,this,[palabraParcial,originalColor](){
                palabraParcial->setDefaultTextColor(originalColor);
            });
        }
        palabraParcial->setPlainText(espacios);
        QPair<qreal,qreal> coordenadas = centrar(palabraParcial);
        palabraParcial->setPos(coordenadas.first,coordenadas.second);

        if (!espacios.contains('_')){ /* NIVEL SUPERADO */
            timer->disconnect(); // Desconectamos todas las señales conectadas a este timer.
            timer->stop();
            if (!removido) {
            /* LIBERAMOS MEMORIA */
                escenaEscape->removeItem(rect);
                escenaEscape->removeItem(title); delete title;
                escenaEscape->removeItem(tiemporestante); delete tiemporestante;
                escenaEscape->removeItem(palabraParcial); delete palabraParcial;
                escenaEscape->removeItem(letraInputItem);
                escenaEscape->removeItem(verificarBotonItem);
                escenaEscape->removeItem(imagen); delete imagen;
                delete letraInput;
                delete verificarBoton;
                removido = !removido;
            }
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
    escenaActual = 2;
    int configLab[12][23] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,2,0,0,1,1,1,1,1,1,2,1,1,0,0,0,0,1,0,0,2,0,1},
        {1,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0,1,1,0,1},
        {1,0,1,0,1,1,0,1,1,0,1,0,1,0,1,0,0,0,0,1,2,0,1},
        {1,0,0,0,2,0,0,0,0,0,2,1,0,0,1,0,1,1,0,0,1,0,1},
        {1,0,1,0,1,1,2,0,0,0,1,0,1,0,1,0,1,1,0,1,1,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {1,0,1,0,1,1,0,1,1,1,1,0,1,0,1,0,1,1,0,1,1,0,1},
        {1,0,2,0,1,2,0,0,0,2,1,0,1,0,1,0,1,1,0,1,0,0,1},
        {1,0,1,0,1,1,0,1,0,1,1,0,1,0,1,0,0,0,0,1,1,0,1},
        {0,0,1,0,2,0,0,0,2,1,2,0,0,0,2,1,0,1,2,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };


    for (int i = 0; i < 12; ++i){
        for (int j = 0; j < 23; ++j){
            if (configLab[i][j] == 1){
                QGraphicsPixmapItem* bloque = new QGraphicsPixmapItem(QPixmap(":/imagenes/bloque.png").scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                escenaLaberinto->addItem(bloque);
                bloque->setPos(j*60, 49 + i*60);
                Bloques.append(bloque);
                if(i == 0 || i == 11) bloque->setVisible(false);
                if((i == 11 && j == 22) || (i == 11 && j == 0)) bloque->setVisible(true);
            }
            else if (configLab[i][j] == 2) {
                // Agregar puntos disponibles para letras
                listaDePuntos.append(QPoint(j * 60, 49 + i * 60));
            }
        }
    }
    // Inicializar letras en posiciones aleatorias
    reinicializarLetras();
    QGraphicsPixmapItem* bloque = new QGraphicsPixmapItem(QPixmap(":/imagenes/bloque.png").scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    escenaLaberinto->addItem(bloque);
    bloque->setPos(1385, 413);
    Bloques.append(bloque);
    bloque->setVisible(false);
    puerta = new QGraphicsPixmapItem(QPixmap(":/imagenes/pared.png").scaled(100, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    escenaLaberinto->addItem(puerta);
    puerta->setPos(45, 11*60-20);
    Bloques.append(puerta);

    // Crear items de imagen para las ruedas
    for (int i = 0; i < 3; ++i) {
        QGraphicsPixmapItem* rueda = new QGraphicsPixmapItem();

        QPixmap pixmap(":/imagenes/rueda.png");
        if (i == 0){
            rueda->setPixmap(pixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            rueda->setPos(185, 670);
        }
        if (i == 1){
            rueda->setPixmap(pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            rueda->setPos(440, 287);
        }
        if (i == 2){
            rueda->setPixmap(pixmap.scaled(45, 45, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            rueda->setPos(785, 170);
        }

        rueda->setZValue(1);
        escenaLaberinto->addItem(rueda);
        ruedas.append(rueda);
        angulos.append(0);  // Inicializamos el ángulo de cada rueda
    }

    // Establecer el punto de origen de rotación para la hélice en su centro
    ruedas[0]->setTransformOriginPoint(ruedas[0]->boundingRect().center());
    ruedas[1]->setTransformOriginPoint(ruedas[1]->boundingRect().center());
    ruedas[2]->setTransformOriginPoint(ruedas[2]->boundingRect().center());
    anguloHelice = 0; // Ángulo inicial de rotación para la hélice


    connect(timer, SIGNAL(timeout()), this, SLOT(controlDeTemporizadores()));
    timer->start(50); // Reloj Base actualizado
}


void MainWindow::colocarObstaculos(QGraphicsScene* scene, int dificultad) {

    srand(time(0));
    int carril;
    do {
        carril = rand() % 3 + 1;
    }while (carril == carrilAnterior);
    carrilAnterior = carril;
    qreal x = 0, y = 0;
    if (carril == 1) { x = 1450; y = 550; }
    if (carril == 2) { x = 1400; y = 600; }
    if (carril == 3) { x = 1350; y = 660; }

    QGraphicsPixmapItem* obstaculo = new QGraphicsPixmapItem();
    obstaculo->setPixmap(QPixmap(":/imagenes/pared.png").scaled(100,50));
    obstaculo->setPos(x,y);
    obstaculo->setRotation(180);
    QTransform transform;
    transform.scale(-1, 1);
    obstaculo->setTransform(transform);
    scene->addItem(obstaculo);
    obstaculos.append(obstaculo);

    if (carril == 2) obstaculo->setZValue(4);
    if (carril == 3) obstaculo->setZValue(5);

    if (dificultad == 2){
        int carril2;
        do {
            carril2 = rand() % 3 + 1;
        } while(carril2 == carril);

        if (carril2 == 1) { x = 1450; y = 550; }
        if (carril2 == 2) { x = 1400; y = 600; }
        if (carril2 == 3) { x = 1350; y= 660; }

        QGraphicsPixmapItem* obstaculo2 = new QGraphicsPixmapItem();
        obstaculo2->setPixmap(QPixmap(":/imagenes/pared.png").scaled(100,50));
        obstaculo2->setPos(x,y);
        obstaculo2->setRotation(180);
        obstaculo2->setTransform(transform);
        scene->addItem(obstaculo2);
        obstaculos.append(obstaculo2);
        if (carril2 == 2) obstaculo2->setZValue(4);
        if (carril2 == 3) obstaculo2->setZValue(5);
    }
}

void MainWindow::colocarLetras(QGraphicsScene *scene){

    srand(time(0));
    int carril = rand()%3 + 1;

    qreal x = 0 , y = 0;
    if (carril == 1) {x = 1450, y = 505; }
    if (carril == 2) { x = 1400; y = 560; }
    if (carril == 3) { x = 1350; y = 615; }

    QGraphicsTextItem* letra = new QGraphicsTextItem(QString(clave[indexClave]));
    QFont font ("Cooper Black",24,QFont::Bold);
    letra->setFont(font);
    letra->setDefaultTextColor(Qt::red);
    letra->setPos(x,y);
    scene->addItem(letra);
    qDebug()<<"LETRA AÑADIDA";
    letras.append(letra);

}

void MainWindow::moverObjetos(QGraphicsScene *scene, int velocidad){

    for (auto obstaculo : obstaculos){
        obstaculo->setX(obstaculo->x()-velocidad);

        if (obstaculo->x() < - 100) {
            scene->removeItem(obstaculo);
            obstaculos.removeOne(obstaculo);
            delete obstaculo;
        }
    }

    for (auto letra : letras ){
        letra->setX(letra->x()-velocidad);
        if (letra->x()< -100 ){
            scene->removeItem(letra);
            letras.removeOne(letra);
            delete letra;
        }
    }
}

void MainWindow::crearCarrera(){
/*
    for (int i = 0; i<1340;i+=20){
        for (int j = 0; j<1340;j+=20){
            escenaCarrera->addEllipse(j,i,1,1)->setZValue(100);
        }
    }
*/
    escenaActual = 3;
    escenaCarrera->setSceneRect(0,0,1330,670);
    QPixmap pixmap (":/imagenes/fondocarretera.png");
    int move = 0;
    escenaCarrera->setBackgroundBrush(pixmap.copy(move,0,400,147).scaled(1330,670));
    QTimer* timer = new QTimer(this);
    int cont = 0;//Para mover la escena cada 50ms
    int cont2 = 0;//Para lanzar misiles cada 5s
    int cont3 = 0; //Para poner obstaculos cada 10s
    int cont4 = 0; //Para mover helocptero y move obstaculos;
    int cont5 = 0; //Colocar las letras
    int velocidadObstaculos = 2;//x-2
    int velocidadFondo = 6;//30 ms
    int intervaloObstaculos = 500;
    int aumentoVelocidad = 0;
    int numObstaculos = 1;
    connect(timer,&QTimer::timeout,this, [=]() mutable {
        cont4++;
        if (cont4 == 2){
            helicoptero->mover();
            moverObjetos(escenaCarrera, velocidadObstaculos);
            cont4 = 0;
        }

        for (auto obstaculo : obstaculos){
            if (Homero->collidesWithItem(obstaculo)){
                timer->stop();
                recuadroGameOver();
            }
        }

        for (auto letra : letras) {
            if (Homero->collidesWithItem(letra)){
                indexClave++;
                letras.removeOne(letra);
                escenaCarrera->removeItem(letra);
                delete letra;

                if (indexClave == clave.size()){
                    qDebug()<<"GANO";
                    /*
                        PASO EL NIVEL
                    */
                }
            }
        }

        cont++;
        cont2++;
        cont3++;
        aumentoVelocidad++;

        if (cont == velocidadFondo || velocidadFondo == 0){
            move++;
            escenaCarrera->setBackgroundBrush(pixmap.copy(move,0,400,147).scaled(1330,670));
            if (move == 800) move = 0;
            cont = 0;
        }

        if(cont2 == 1040) {
            helicoptero->lanzarMisil(Homero->x(),Homero->y(),escenaCarrera);
            cont2 = 0;
        }

        if (cont3 >= intervaloObstaculos){
            colocarObstaculos(escenaCarrera,numObstaculos);
            cont3 = 0;
        }

        cont5++;
        if (cont5 == 1800){
            if (indexClave < clave.size()){
                colocarLetras(escenaCarrera);
            }
            cont5 = 0;
        }

        if (aumentoVelocidad == 2400){
            velocidadObstaculos++;
            if (velocidadFondo > 1) velocidadFondo--;
            intervaloObstaculos = std::max(200, intervaloObstaculos-50);
            if (numObstaculos < 2) numObstaculos++;
            aumentoVelocidad = 0;
        }
    });
    timer->start(5);

    helicoptero = new Helicoptero ();
    helicoptero->setPos(20,40);
    escenaCarrera->addItem(helicoptero);

    Homero = new Personaje("HomeroEnCarro");
    Homero->setPos(470,555);
    escenaCarrera->addItem(Homero);

    connect(helicoptero,&Helicoptero::posExplosion, this, [=] (int y) mutable {
        if (y>430 && y < 475 && Homero->y()==500 && Homero->x() == 470){//Explosion en carril 1
            timer->stop();
            recuadroGameOver();
        }else if (y>485 && y<535 && Homero->y() == 555  && Homero->x() == 470){ //Explosion en carril 2
            timer->stop();
            recuadroGameOver();
        }else if (y>550 && y < 595 && Homero->y() == 610  && Homero->x() == 470){ //Explosion en carril 3
            timer->stop();
            recuadroGameOver();
        }
    });

}

void MainWindow::recuadroGameOver(){

    QGraphicsPixmapItem* fondo = new QGraphicsPixmapItem(QPixmap(":/imagenes/gameover3.png").scaled(665,185,Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    fondo->setPos(332,182);
    fondo->setZValue(10);
    escenaCarrera->addItem(fondo);

    QPushButton* reiniciarButton = new QPushButton("Reiniciar");
    QPushButton* salirButton = new QPushButton("Salir");

    QFont fuente = QFont("Upheaval TT (BRK)",20);
    QPalette paleta;
    paleta.setColor(QPalette::Button,QColorConstants::Green);
    paleta.setColor(QPalette::ButtonText,Qt::black);
    reiniciarButton->setFont(fuente);
    salirButton->setFont(fuente);
    reiniciarButton->setPalette(paleta);
    salirButton->setPalette(paleta);

    QGraphicsProxyWidget* proxyReiniciar = escenaCarrera->addWidget(reiniciarButton);
    QGraphicsProxyWidget* proxySalir = escenaCarrera->addWidget(salirButton);
    proxyReiniciar->setZValue(11);
    proxySalir->setZValue(11);


    proxyReiniciar->setPos(480,455);
    proxySalir->setPos(720,455);

    connect(reiniciarButton, &QPushButton::clicked, this, [=] () mutable {
        // MOVER LAS VARIABLES AL .H
    });
    connect(salirButton, &QPushButton::clicked, this, [=] () mutable {
        /*
        LIBERAR MEMORIA
        */
        cambiarDeEscena(0);
    });

}

void MainWindow::controlDeTemporizadores()
{
    if (escenaActual == 1){
        // Llamar a la función que sea necesario invocar cada 200 mls (con un contador podemos modificar ese tiempo)
    }

    else if (escenaActual == 2){
        moverRuedas();           /* Mover ruedas cada 50 mls */

        /*MOVER POLICIA1*/
        if (Homero->scenePos().x() <= 1100 && Homero->scenePos().x() >= 1060) {
            QString direccion = (Homero->y() > Policia1->y()) ? "abajo" : "arriba";
            if (direccion == "arriba") Policia1->setPixmap(QPixmap(":/imagenes/pistolas.png").copy(100, 0, 100, 100).scaled(70, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            else Policia1->setPixmap(QPixmap(":/imagenes/pistolas.png").copy(0, 0, 100, 100).scaled(70, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            ejeDisparoPolicia1 = "y";
            dispararBola("Policia1");
        }
        else if ((Homero->scenePos().y() <= 433 && Homero->scenePos().y() >= 393) && Policia1->scenePos().y() <= 415 && Policia1->scenePos().y() >= 411) {
            QString direccion = (Homero->x() > Policia1->x()) ? "derecha" : "izquierda";
            if (direccion == "derecha") Policia1->setPixmap(QPixmap(":/imagenes/pistolas.png").copy(300, 0, 100, 100).scaled(70, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            else Policia1->setPixmap(QPixmap(":/imagenes/pistolas.png").copy(200, 0, 100, 100).scaled(70, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            ejeDisparoPolicia1 = "x";
            dispararBola("Policia1");
        }
        else if ((qAbs(Policia1->scenePos().y() - Homero->scenePos().y()) < 20) &&
                   Homero->scenePos().x() < Policia1->scenePos().x() &&
                   (Policia1->scenePos().y() <= 260 || Policia1->scenePos().y() >= 580) &&
                   Homero->scenePos().x() > 800) {
            QString direccion = (Homero->x() > Policia1->x()) ? "derecha" : "izquierda";
            if (direccion == "derecha") Policia1->setPixmap(QPixmap(":/imagenes/pistolas.png").copy(300, 0, 100, 100).scaled(70, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            else Policia1->setPixmap(QPixmap(":/imagenes/pistolas.png").copy(200, 0, 100, 100).scaled(70, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            ejeDisparoPolicia1 = "x";
            dispararBola("Policia1");
        }
        else Policia1->moverPolicia(1); /* Mover policia1 cada 50 mls*/

        /*MOVER POLICIA2*/
        if (Homero->scenePos().y() <= 193 && Homero->scenePos().y() >= 153 && Homero->scenePos().x() <= 785 && Homero->scenePos().x() >= 170) { // Si Homero esta a la altura de Policia2
            QString direccion = (Homero->x() > Policia2->x()) ? "derecha" : "izquierda";
            if (direccion == "derecha") Policia2->setPixmap(QPixmap(":/imagenes/pistolas.png").copy(300, 0, 100, 100).scaled(70, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            else Policia2->setPixmap(QPixmap(":/imagenes/pistolas.png").copy(200, 0, 100, 100).scaled(70, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            ejeDisparoPolicia2 = "x";
            dispararBola("Policia2");
        }
        else if (((Homero->scenePos().x() <= 565 && Homero->scenePos().x() >= 520 && qAbs(Policia2->scenePos().x() - Homero->scenePos().x()) < 10 && Homero->scenePos().y() <= 415) ||
            (Homero->scenePos().x() <= 385 && Homero->scenePos().x() >= 340 && qAbs(Policia2->scenePos().x() - Homero->scenePos().x()) < 10))) {
            Policia2->setPixmap(QPixmap(":/imagenes/pistolas.png").copy(0, 0, 100, 100).scaled(70, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            ejeDisparoPolicia2 = "y";
            dispararBola("Policia2");
        }
        else Policia2->moverPolicia(2); /* Mover policia2 cada 50 mls*/



        if (tiempoMoverBalas == 2){
            moverBalas(Policia1, balasPolicia1);
            moverBalas(Policia2, balasPolicia2);
            tiempoMoverBalas = 0;
        }
        tiempoMoverBalas += 1;



        /* RELOJ LABERINTO */
        ++moverAgujaReloj;
        if (moverAgujaReloj == 4 || moverAgujaReloj == 8 || moverAgujaReloj == 12|| moverAgujaReloj == 16 || moverAgujaReloj == 12) llamasDeFuego();
        if (moverAgujaReloj == 20){
            --Reloj;
            if (Reloj == 0){
                timer->stop();
                /* FUNCION QUE UNUNCIA LA PERDIDA DE LA PARTIDA! */
                anuncio("Derrota");
            }
            textRelojLaberinto->setPlainText("Reloj " + QString::number(Reloj));
            moverAgujaReloj = 0;
            if (Reloj == 22) musicaFondoLaberinto->play();
        }
    }

    if (claveObtenida < 8) recogerLetra();

}




// MÉTODO QUE DETECTA CUANDO SE PRECIONAN TECLAS //
void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (escenaActual == 1) return;

    if (escenaActual == 2) {
        // Guardamos la posición actual del personaje antes de moverlo
        qreal posXAnterior = Homero->x();
        qreal posYAnterior = Homero->y();

        switch (e->key()) {
            case Qt::Key_D: {  // Mover a la derecha
                Homero->ActualizarImagen(0);
                Homero->setPos(Homero->x() + 10, Homero->y());  // Intentar mover a la derecha
                if (tocarPared()) {  // Si colisiona, restauramos a la posición anterior
                    Homero->setPos(posXAnterior, posYAnterior);
                }
                break;
            }
            case Qt::Key_A: {  // Mover a la izquierda
                Homero->ActualizarImagen(100);
                Homero->setPos(Homero->x() - 10, Homero->y());  // Intentar mover a la izquierda
                if (tocarPared()) {
                    Homero->setPos(posXAnterior, posYAnterior);
                }
                break;
            }
            case Qt::Key_W: {  // Mover hacia arriba
                Homero->ActualizarImagen(300);
                Homero->setPos(Homero->x(), Homero->y() - 10);  // Intentar mover hacia arriba
                if (tocarPared()) {
                    Homero->setPos(posXAnterior, posYAnterior);
                }
                break;
            }
            case Qt::Key_S: {  // Mover hacia abajo
                Homero->ActualizarImagen(200);
                Homero->setPos(Homero->x(), Homero->y() + 10);  // Intentar mover hacia abajo
                if (tocarPared()) {
                    Homero->setPos(posXAnterior, posYAnterior);
                }
                break;
            }
        }

        if (Homero->x() <= 80 && Homero->y() >= 11*60-60 && claveObtenida == 8){
            if (escenaLaberinto->items().contains(puerta)){
                escenaLaberinto->removeItem(puerta);
                // Eliminar de la lista
                Bloques.removeOne(puerta);
                // Liberar la memoria de la puerta.
                delete puerta;
                puerta = nullptr;
            }
            static bool anunciarVictoria = true;
            if (Homero->x() <= 50 && anunciarVictoria){
                anunciarVictoria = false;
                Homero->setPos(Homero->x()-65, Homero->y());
                anuncio("Victoria"); /* LLAMAR FUNCIÓN QUE ANUNCIA HABER SUPERADO EL NIVEL */
            }
        }
    }
    else if (escenaActual == 3){

        int posMax = 610;
        int posMin = 500;

        switch (e->key()){

            case Qt::Key_W: {

                if (Homero->y()-55 < posMin) return;
                Homero->setY(Homero->y()-55);
                break;
            }

            case Qt::Key_S: {
                if (Homero->y()+55 > posMax) return;
                Homero->setY(Homero->y()+55);
                break;
            }

            case Qt::Key_Space: {
                if (Homero->x() == 570) return;
                if (!ablePower) return;
                Homero->setX(Homero->x()+100);
                QEventLoop loop;
                QTimer::singleShot(600, &loop, &QEventLoop::quit);
                loop.exec();
                Homero->setX(Homero->x()-100);
                ablePower = false;
                QTimer* timer = new QTimer();
                QObject::connect(timer, &QTimer::timeout, this, [=]() mutable {
                    ablePower = true;
                    timer->stop();
                });
                timer->start(10000);

                break;
            }
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



int contador = 0, contador1 = 0;
void MainWindow::moverRuedas()
{
    const float radio = 8.0f;  // Radio del movimiento circular
    const float velocidadAngular = 0.2f;  // Velocidad de rotación
    const float velocidadRotacionHelice = 20.0f; // Velocidad de rotación de la hélice

    for (int i = 0; i < ruedas.size(); ++i) {
        if (i == 0 || i == 2) {
            // Rotación sobre su propio eje
            anguloHelice += velocidadRotacionHelice;
            ruedas[i]->setRotation(anguloHelice);
            // Mantenemos el ángulo en un rango de 0 a 360 grados
            if (anguloHelice >= 360.0f) anguloHelice -= 360.0f;

            //Movimiento lineal periodico rueda0
            static bool cambiarDireccionDelMovimiento1;
            if (contador <= 0) cambiarDireccionDelMovimiento1 = true;
            else if (contador >= 450) cambiarDireccionDelMovimiento1 = false;
            if (cambiarDireccionDelMovimiento1){
                contador += 5;
                ruedas[0]->setPos(ruedas[0]->x(), ruedas[0]->y() -5 );
            }
            else{
                contador -= 5;
                ruedas[0]->setPos(ruedas[0]->x(), ruedas[0]->y() + 5);
            }

            //Movimiento lineal periodico rueda2
            static bool cambiarDireccionDelMovimiento2;
            if (contador1 <= 0) cambiarDireccionDelMovimiento2 = true;
            else if (contador1 >= 500) cambiarDireccionDelMovimiento2 = false;
            if (cambiarDireccionDelMovimiento2){
                contador1 += 5;
                ruedas[2]->setPos(ruedas[2]->x(), ruedas[2]->y()+5);
            }
            else{
                contador1 -= 5;
                ruedas[2]->setPos(ruedas[2]->x(), ruedas[2]->y()-5);
            }

        } else {
            // Rotación sobre su propio eje
            anguloHelice += velocidadRotacionHelice;
            ruedas[i]->setRotation(anguloHelice);
            // Mantenemos el ángulo en un rango de 0 a 360 grados
            if (anguloHelice >= 360.0f) anguloHelice -= 360.0f;


            //Movimiento circular
            angulos[i] += velocidadAngular;
            float x = ruedas[i]->x() + radio * cos(angulos[i]);
            float y = ruedas[i]->y() + radio * sin(angulos[i]);
            ruedas[i]->setPos(x, y);
            // Mantenemos el ángulo en un rango de 0 a 2π radianes
            if (angulos[i] >= 2 * M_PI) angulos[i] -= 2 * M_PI;
        }

        if (ruedas[i]->collidesWithItem(Homero)) {
            reproducirSonidos("au");
            Homero->setVidas(1, '-');
            textVidas->setPlainText("Vidas " + QString::number(Homero->getVidas()));
            if (Homero->getVidas() <= 0){
                /* FUNCION QUE UNUNCIA LA PERDIDA DE LA PARTIDA! */
                anuncio("Derrota");
            }
        }
    }
}




void MainWindow::dispararBola(QString policia)
{
    reproducirSonidos("Disparo");

    if (policia == "Policia1"){
        QGraphicsEllipseItem *bala = new QGraphicsEllipseItem(Policia1->x(), Policia1->y(), 10, 10);
        if (ejeDisparoPolicia1 == "y"){
            int direccion = (Homero->y() > Policia1->y()) ? 65 : -5;
            if (direccion == -5) bala->setPos(35, direccion);
            else bala->setPos(22, direccion);
        }
        else {
            int direccion = (Homero->x() > Policia1->x()) ? 65 : -5;
            bala->setPos(direccion, 15);
        }
        bala->setBrush(Qt::green);
        escenaLaberinto->addItem(bala);
        balasPolicia1.append(bala);
    }
    else {
        QGraphicsEllipseItem *bala = new QGraphicsEllipseItem(Policia2->x(), Policia2->y(), 10, 10);
        if (ejeDisparoPolicia2 == "y"){
            int direccion = (Homero->y() > Policia2->y()) ? 65 : -5;
            if (direccion == -5) bala->setPos(35, direccion);
            else bala->setPos(22, direccion);
        }
        else {
            int direccion = (Homero->x() > Policia2->x()) ? 65 : -5;
            bala->setPos(direccion, 15);
        }
        bala->setBrush(Qt::green);
        escenaLaberinto->addItem(bala);
        balasPolicia2.append(bala);
    }
}




void MainWindow::moverBalas(QGraphicsPixmapItem *policia, QList<QGraphicsEllipseItem *> &balas)
{
    QString ejeDisparo;
    if (policia == Policia1) ejeDisparo = ejeDisparoPolicia1;
    else if (policia == Policia2) ejeDisparo = ejeDisparoPolicia2;

    // Iterar sobre las balas y moverlas
    for (auto it = balas.begin(); it != balas.end();) {
        QGraphicsEllipseItem *bala = *it;

        // Determinar la dirección de la bala basada en la posición de Homero
        int direccion = 0;
        if (ejeDisparo == "y") {
            direccion = (Homero->y() > policia->y()) ? 20 : -20;
            bala->moveBy(0, direccion); // Mover la bala hacia arriba o abajo
        } else {
            direccion = (Homero->x() > policia->x()) ? 20 : -20;
            bala->moveBy(direccion, 0); // Mover la bala hacia derecha o izquierda
        }

        bool eliminarBala = false;
        if (bala->collidesWithItem(Homero)) { // Verificar si la bala colisiona con Homero
            reproducirSonidos("au");
            Homero->setVidas(1, '-');
            textVidas->setPlainText("Vidas " + QString::number(Homero->getVidas()));
            if (Homero->getVidas() <= 0){
                /* FUNCION QUE UNUNCIA LA PERDIDA DE LA PARTIDA! */
                anuncio("Derrota");
            }
            eliminarBala = true;
        }

        // Verificar colisión con las paredes
        for (QGraphicsPixmapItem *bloque : Bloques) { // Bloques es una variable miembro
            if (bala->collidesWithItem(bloque)) {
                eliminarBala = true;
                break;
            }
        }

        // Eliminar la bala si es necesario
        if (eliminarBala) {
            escenaLaberinto->removeItem(bala);
            delete bala;
            it = balas.erase(it);
        } else {
            ++it;
        }
    }
}





void MainWindow::configurarElementosDeLaEscenaLaberinto()
{
    /* TEXTOS */
    textRelojLaberinto = escenaLaberinto->addText("Reloj 120");
    textRelojLaberinto->setFont(QFont("Arial", 18, QFont::Bold));
    textRelojLaberinto->setDefaultTextColor(Qt::white);
    textRelojLaberinto->setPos(1390, 150);

    textVidas = escenaLaberinto->addText("Vidas 7");
    textVidas->setFont(QFont("Arial", 18, QFont::Bold));
    textVidas->setDefaultTextColor(Qt::white);
    textVidas->setPos(1390, 200);

    textClave = escenaLaberinto->addText("Clave No\nObtenida");
    textClave->setFont(QFont("Arial", 18, QFont::Bold));
    textClave->setDefaultTextColor(Qt::white);
    textClave->setPos(1390, 480);


    /* RECUADRO GAME OVER */
    // Tamaño del recuadro
    int width = 700;
    int height = 500;

    // Crear un rectángulo de fondo
    recuadro = new QGraphicsRectItem(0, 0, width, height);
    recuadro->setZValue(2);
    recuadro->setPen(QPen(Qt::NoPen));

    // Ajustamos posicion del recuadro para centrarlo en la escena
    recuadro->setPos(escenaLaberinto->width() / 2 - (width / 2) - 240, escenaLaberinto->height() / 2 - height / 2);

    // Crear una imagen de fondo para el recuadro
    imagenFondo = new QGraphicsPixmapItem(QPixmap(":/imagenes/GameOver.png").scaled(700, 500, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation),recuadro);
    imagenFondo->setPos(0, 0); // Alinear la imagen con el recuadro
    // Botón "Reintentar"
    botonReintentar = new QPushButton("Reintentar");
    // Botón "Salir"
    botonSalir = new QPushButton("Salir");

    proxyReintentar = escenaLaberinto->addWidget(botonReintentar);
    proxySalir = escenaLaberinto->addWidget(botonSalir);

    // Estilo y tamaño del botón "Reintentar"
    botonReintentar->setFixedSize(100, 40);  // Tamaño del botón
    botonReintentar->setStyleSheet("QPushButton {"
                                   "background-color: #4CAF50; "  // Color de fondo
                                   "color: white; "              // Color del texto
                                   "font-size: 16px; "           // Tamaño de la fuente
                                   "padding: 10px; "             // Espaciado interno
                                   "}");
    // Aplicar negrita al texto
    QFont font = botonReintentar->font();
    font.setBold(true);  // Poner el texto en negrita
    botonReintentar->setFont(font);

    // Estilo y tamaño del botón "Salir"
    botonSalir->setFixedSize(100, 40);  // Tamaño del botón
    botonSalir->setStyleSheet("QPushButton {"
                              "background-color: #f44336; "  // Color de fondo
                              "color: white; "              // Color del texto
                              "font-size: 16px; "           // Tamaño de la fuente
                              "padding: 10px; "             // Espaciado interno
                              "}");
    // Aplicar negrita al texto
    QFont fontSalir = botonSalir->font();
    fontSalir.setBold(true);  // Poner el texto en negrita
    botonSalir->setFont(fontSalir);

    escenaLaberinto->addItem(recuadro);

    proxyReintentar->setParentItem(recuadro);

    proxyReintentar->setPos(700 / 2 - botonReintentar->width()+30, 500-180);

    proxySalir->setParentItem(recuadro);

    proxySalir->setPos(700 / 2 + 50, 500-180);

    recuadro->setVisible(false); // Ocultar
}

void MainWindow::anuncio(QString tipoAnuncio)
{
    timer->stop();
    permitirEmision = false;
    if (tipoAnuncio == "Derrota"){
        recuadro->setVisible(true); // Mostrar
    }
    else if (tipoAnuncio == "Victoria"){
        musicaFondoLaberinto->stop();
        reproducirSonidos("Victoria");
        int width = 900;
        int height = 600;
        // Ajustamos posicion del recuadro para centrarlo en la escena
        recuadro->setPos(escenaLaberinto->width() / 2 - (width / 2) - 225, escenaLaberinto->height() / 2 - (50 + height / 2));

        imagenFondo->setPixmap(QPixmap(":/imagenes/NivelSuperado.png").scaled(900, 600, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        botonReintentar->setText("Nivel 3");
        proxyReintentar->setPos(700 / 2 - botonReintentar->width()+100, 510);
        proxySalir->setPos(700 / 2 + 120, 510);
        recuadro->setVisible(true); // Mostrar
    }

    // Conectar señales de los botones
    QObject::connect(botonReintentar, &QPushButton::clicked, this, [=]() {
        // Desconectar las señales de ambos botones
        botonReintentar->disconnect();
        botonSalir->disconnect();

        if (tipoAnuncio == "Derrota") reiniciarVariables("Derrota");
        else if (tipoAnuncio == "Victoria"){
            reiniciarVariables("Victoria");
        }
    });

    QObject::connect(botonSalir, &QPushButton::clicked, [=]() {
        // Lógica para salir
        QApplication::quit();
    });
}




void MainWindow::reiniciarVariables(QString motivoReinicio)
{
    // Iterar sobre las balas y removerlas
    for (auto it = balasPolicia1.begin(); it != balasPolicia1.end();) {
        QGraphicsEllipseItem *bala = *it;
        escenaLaberinto->removeItem(bala);
        delete bala;
        it = balasPolicia1.erase(it);
    }
    for (auto it = balasPolicia2.begin(); it != balasPolicia2.end();) {
        QGraphicsEllipseItem *bala = *it;
        escenaLaberinto->removeItem(bala);
        delete bala;
        it = balasPolicia2.erase(it);
    }
    recuadro->setVisible(false); // Ocultar

    if (motivoReinicio == "Derrota"){
        Homero->setVidas(7, '+');
        Homero->setPos(1325, 413);
        Homero->ActualizarImagen(100);
        tiempoMoverBalas = 0, moverAgujaReloj = 0, Reloj = 120;
        textRelojLaberinto->setPlainText("Reloj 120");
        textVidas->setPlainText("Vidas 7");

        Policia1->setPos(1080, 225);
        Policia1->recetearCOntador();
        Policia2->setPos(560, 173);
        Policia2->recetearCOntador();
        permitirEmision = true;

        iniciarResortes(false);
        musicaFondoLaberinto->stop();
        musicaFondoLaberinto->play();
        reinicializarLetras();
        timer->start(50);
    }
    else if (motivoReinicio == "Victoria"){
        for (auto it = Bloques.begin(); it != Bloques.end();) {
            QGraphicsPixmapItem *bloque = *it;
            escenaLaberinto->removeItem(bloque);
            delete bloque;
            it = Bloques.erase(it);
        }
        for (auto it = ruedas.begin(); it != ruedas.end();) {
            QGraphicsPixmapItem *rueda = *it;
            escenaLaberinto->removeItem(rueda);
            delete rueda;
            it = ruedas.erase(it);
        }

        ListaLetras.clear();
        listaDePuntos.clear();
        escenaLaberinto->removeItem(mano1);
        delete mano1;
        escenaLaberinto->removeItem(mano2);
        delete mano2;
        escenaLaberinto->removeItem(mano3);
        delete mano3;
        escenaLaberinto->removeItem(fuego);
        delete fuego;
        escenaLaberinto->removeItem(Policia1);
        delete Policia1;
        escenaLaberinto->removeItem(Policia2);
        delete Policia2;
        escenaLaberinto->clear();
        delete escenaLaberinto;
        escenaLaberinto = nullptr;
        //cambiarDeEscena(3);
    }

}

void MainWindow::llamasDeFuego()
{
    int x; static bool yaFueCreadoGraphicsPixmap = false; static int columnas = 0;

    if (yaFueCreadoGraphicsPixmap){
        if (columnas >= 500){
            columnas = 0;
            x = QRandomGenerator::global()->bounded(1, 23); // Generar un valor aleatorio para x entre 1 y 22
            fuego->setPos(x*60, 10*60 + 30);
        }
        fuego->setPixmap(pixmapFuego.copy(columnas, 0, 100, 100));
    }
    else{
        pixmapFuego = QPixmap(":/imagenes/fuego.png");
        fuego = new QGraphicsPixmapItem();
        fuego->setPixmap(pixmapFuego.copy(columnas, 0, 100, 100));
        escenaLaberinto->addItem(fuego);
        x = QRandomGenerator::global()->bounded(1, 23); // Generar un valor aleatorio para x entre 1 y 22
        fuego->setPos(x*60, 10*60 + 30);
        yaFueCreadoGraphicsPixmap = true;
    }
    columnas += 100;
    if (columnas >= 99){ /* SONIDO DE FUEGO */ }
    if (Homero->collidesWithItem(fuego)){
        reproducirSonidos("au");
        Homero->setVidas(1, '-');
        textVidas->setPlainText("Vidas " + QString::number(Homero->getVidas()));
        if (Homero->getVidas() <= 0){
            /* FUNCION QUE UNUNCIA LA PERDIDA DE LA PARTIDA! */
            anuncio("Derrota");
        }
    }

}

void MainWindow::iniciarResortes(bool primeraVez)
{
    if (primeraVez){
        // Crear objetos gráficos
        mano1 = new QGraphicsPixmapItem(QPixmap(":/imagenes/mano.png").scaled(45, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        mano2 = new QGraphicsPixmapItem(QPixmap(":/imagenes/mano2.png").scaled(50, 45, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        mano3 = new QGraphicsPixmapItem(QPixmap(":/imagenes/mano3.png").scaled(50, 45, Qt::KeepAspectRatio, Qt::SmoothTransformation));

        // Añadir objetos a la escena
        escenaLaberinto->addItem(mano1);
        escenaLaberinto->addItem(mano2);
        escenaLaberinto->addItem(mano3);

        // Posiciones iniciales
        mano1->setPos(65, 10*60 - 130);
        mano2->setPos(365, 10*60);
        mano3->setPos(21*60+10, 150);

        // Asociar parámetros de movimiento para cada objeto
        parametrosMovimientos[mano1] = {70, 0.2, 3 * M_PI}; // {A, beta, omega}
        parametrosMovimientos[mano2] = {50, 0.2, 3 * M_PI};
        parametrosMovimientos[mano3] = {-80, 0.3, 3 * M_PI};

        // Conectar la señal para reiniciar el movimiento de los resortes
        connect(this, &MainWindow::senalParaMoverElResorte, [=](QGraphicsPixmapItem* mano) {
            auto params = parametrosMovimientos[mano];
            moverConResorte(mano, mano->x(), mano->y(), params[0], params[1], params[2]);
        });

        // Iniciar el movimiento de ambos resortes
        auto params1 = parametrosMovimientos[mano1];
        moverConResorte(mano1, mano1->x(), mano1->y(), params1[0], params1[1], params1[2]);

        auto params2 = parametrosMovimientos[mano2];
        moverConResorte(mano2, mano2->x(), mano2->y(), params2[0], params2[1], params2[2]);

        auto params3 = parametrosMovimientos[mano3];
        moverConResorte(mano3, mano3->x(), mano3->y(), params3[0], params3[1], params3[2]);
    } else{
        // Posiciones iniciales
        mano1->setPos(65, 10*60 - 135);
        mano2->setPos(365, 10*60);
        mano3->setPos(21*60+10, 150);
        // Iniciar el movimiento de ambos resortes
        auto params1 = parametrosMovimientos[mano1];
        moverConResorte(mano1, mano1->x(), mano1->y(), params1[0], params1[1], params1[2]);

        auto params2 = parametrosMovimientos[mano2];
        moverConResorte(mano2, mano2->x(), mano2->y(), params2[0], params2[1], params2[2]);

        auto params3 = parametrosMovimientos[mano3];
        moverConResorte(mano3, mano3->x(), mano3->y(), params3[0], params3[1], params3[2]);
    }
}


void MainWindow::moverConResorte(QGraphicsPixmapItem *mano, double x, double y, double A, double beta, double omega)
{
    // Configuración inicial
    const int intervalo = 16; // Intervalo de tiempo (ms) para actualizar
    double tiempo = 0;        // Tiempo inicial
    const double dt = intervalo / 1000.0; // Incremento de tiempo en segundos
    const double y_inicial = y; // Posición inicial

    QTimer* timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout, this, [=]() mutable {
        // Ecuación del movimiento
        double desplazamiento = A * qExp(-beta * tiempo) * qCos(omega * tiempo);
        double nueva_y = y_inicial - desplazamiento; // Movimiento hacia arriba

        // Actualizar posición del objeto
        mano->setPos(x, nueva_y);
        if (mano->collidesWithItem(Homero)) {
            reproducirSonidos("au");
            if (mano == mano3 && Homero->x() >= 21*60-10) Homero->setPos(Homero->x(), Homero->y()+30);
            else if (mano == mano2 && Homero->x() >= 355 && Homero->x() <= 375) Homero->setPos(Homero->x(), Homero->y()-30);
            else if (mano == mano1 && Homero->x() <= 75) Homero->setPos(Homero->x(), Homero->y()-30);

            Homero->setVidas(1, '-');
            textVidas->setPlainText("Vidas " + QString::number(Homero->getVidas()));
            if (Homero->getVidas() <= 0){
                /* FUNCION QUE UNUNCIA LA PERDIDA DE LA PARTIDA! */
                anuncio("Derrota");
            }
        }

        // Aumentar tiempo
        tiempo += dt;

        // Detener cuando el desplazamiento es muy pequeño y el objeto está cerca de la posición inicial
        if ((qFabs(desplazamiento) < 0.1 && qFabs(nueva_y - y_inicial) < 0.1) || !permitirEmision) {
            timer->stop();             // Detener el timer
            timer->deleteLater();      // Liberar memoria del timer
            if (permitirEmision) emit senalParaMoverElResorte(mano); // Emitir la señal con el objeto correspondiente
        }
    });

    // Iniciar el timer
    timer->start(intervalo);
}




void MainWindow::reproducirSonidos(QString sonidoAreproducir)
{
    auto* sonido = new QSoundEffect(this);
    if (sonidoAreproducir == "Disparo"){
        sonido->setSource(QUrl("qrc:/audios/disparo.wav"));
    }
    else if (sonidoAreproducir == "Victoria"){
        sonido->setSource(QUrl("qrc:/audios/victoria.WAV"));
    }
    else if (sonidoAreproducir == "au"){
        sonido->setSource(QUrl("qrc:/audios/au.WAV"));
    }
    sonido->setVolume(0.8);
    sonido->play();

    // Liberar memoria después de reproducir
    connect(sonido, &QSoundEffect::playingChanged, this, [sonido]() {
        if (!sonido->isPlaying()) {
            sonido->deleteLater();
        }
    });
}




void MainWindow::reinicializarLetras()
{
    // Reiniciar el contador de clave obtenida
    claveObtenida = 0;

    // Verificar si ListaLetras contiene elementos
    if (!ListaLetras.isEmpty()) {
        // Recorrer los elementos de ListaLetras
        for (auto letra : ListaLetras) {
            // Verificar si el elemento está en la escena
            if (escenaLaberinto->items().contains(letra)) {
                escenaLaberinto->removeItem(letra); // Remover de la escena si está presente
            }
            delete letra; // Liberar memoria
        }
        // Vaciar la lista una vez que todos los elementos han sido eliminados
        ListaLetras.clear();
    }

    // Mezclar las posiciones disponibles
    std::random_shuffle(listaDePuntos.begin(), listaDePuntos.end());

    // Crear las nuevas letras
    QString palabra = "LIBERTAD";
    for (int i = 0; i < palabra.size(); ++i) {
        QGraphicsTextItem* letra = new QGraphicsTextItem(QString(palabra[i]));
        letra->setDefaultTextColor(Qt::yellow);
        letra->setFont(QFont("Arial", 18, QFont::Bold));

        // Posicionar la letra en una posición aleatoria
        QPoint posicion = listaDePuntos[i];
        letra->setPos(posicion);
        escenaLaberinto->addItem(letra);

        // Agregar la letra a la lista
        ListaLetras.append(letra);
    }
}


void MainWindow::recogerLetra()
{
    for (int i = 0; i < ListaLetras.size(); ++i) {
        if (escenaLaberinto->items().contains(ListaLetras[i])){
            if (ListaLetras[i]->collidesWithItem(Homero)) {
                // Incrementar claveObtenida
                claveObtenida++;

                // Remover el elemento de la escena
                escenaLaberinto->removeItem(ListaLetras[i]);

                // Terminar el bucle, ya que una letra fue procesada
                break;
            }
        }
    }
    if (claveObtenida == 8) {
        textClave->setPlainText("Clave\nObtenida");
        mostrarTextoParpadeante(textClave, 7000, 500);
    }
}




void MainWindow::mostrarTextoParpadeante(QGraphicsTextItem* textoParpadeante, int duracionTotalMs, int intervaloParpadeoMs)
{
    // Temporizador para alternar visibilidad
    QTimer* temporizadorParpadeo = new QTimer(this);
    connect(temporizadorParpadeo, &QTimer::timeout, this, [textoParpadeante]() {
        textoParpadeante->setVisible(!textoParpadeante->isVisible()); // Alternar visibilidad
    });

    // Iniciar el temporizador de parpadeo
    temporizadorParpadeo->start(intervaloParpadeoMs);

    // Temporizador para detener el parpadeo después de duracionTotalMs
    QTimer::singleShot(duracionTotalMs, this, [temporizadorParpadeo, textoParpadeante, this]() {
        temporizadorParpadeo->stop(); // Detener el parpadeo
        temporizadorParpadeo->deleteLater(); // Liberar el temporizador
        textoParpadeante->setVisible(isVisible());
    });
}


MainWindow::~MainWindow()
{
    delete ui;
}
