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
#include <QtMath>
#include <QMediaPlayer>
#include <QUrl>
#include <QAudioOutput>
#include <QSoundEffect>
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
#include <cstdlib>
#include <ctime>
/*##################################*/

#include "personaje.h"
#include "enemigo.h"
#include "helicoptero.h"

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
    void juegoAhorcado(); // Juan
    void dispararBola(QString policia);
    void moverBalas(QGraphicsPixmapItem *policia, QList<QGraphicsEllipseItem *> &balas);
    void configurarElementosDeLaEscenaLaberinto();
    void anuncio(QString tipoAnuncio);
    void reiniciarVariables(QString motivoReinicio);
    void llamasDeFuego();
    void moverConResorte(QGraphicsPixmapItem* mano, double x, double y, double A, double beta, double omega);
    void iniciarResortes(bool primeraVez = true);
    void reproducirSonidos(QString sonidoAreproducir);
    void reinicializarLetras();
    void recogerLetra();
    void mostrarTextoParpadeante(QGraphicsTextItem* textoParpadeante, int duracionTotalMs, int intervaloParpadeoMs);
    ~MainWindow();

signals:
    void senalParaMoverElResorte(QGraphicsPixmapItem* mano); // Señal parametrizada con la mano que debe moverse
    //void llamarJuegoAhorcado();

protected:
    void keyPressEvent(QKeyEvent *e);

public slots:
    void controlDeTemporizadores(); // Cristian

private:
    Ui::MainWindow *ui;
    /* CRISTIAN */
    QGraphicsScene *escenaLaberinto;
    bool misionSuperada = false; // Si cambia a true configuramos la siguiente escena y la ponemos en el GraphicsView
    Personaje *Homero;
    QTimer *timer;

    QList<QGraphicsPixmapItem*> Bloques;

    QList<QGraphicsPixmapItem*> ruedas; // Lista de items de imagen para las ruedas
    QList<float> angulos;                 // Lista de ángulos para el movimiento circular
    float anguloHelice;                   // Ángulo de rotación de la hélice
    QPixmap imagenRueda;

    int escenaActual = 1;

    Enemigo *Policia1, *Policia2;
    QList<QGraphicsEllipseItem *> balasPolicia1; // Lista de balas disparadas por Policia1
    QList<QGraphicsEllipseItem *> balasPolicia2; // Lista de balas disparadas por Policia2
    int tiempoMoverBalas = 0, moverAgujaReloj = 0, Reloj = 120;
    QString ejeDisparoPolicia1, ejeDisparoPolicia2;
    QGraphicsTextItem *textRelojLaberinto, *textVidas, *textClave;
    QGraphicsRectItem *recuadro;
    QGraphicsPixmapItem *imagenFondo;
    QPushButton *botonReintentar;
    QPushButton *botonSalir;
    QGraphicsProxyWidget *proxyReintentar;
    QGraphicsProxyWidget *proxySalir;
    QGraphicsPixmapItem* fuego;
    QPixmap pixmapFuego;
    QGraphicsPixmapItem* puerta;
    // Manos
    QGraphicsPixmapItem* mano1, *mano2, *mano3;
    // Parámetros para cada mano
    QMap<QGraphicsPixmapItem*, QVector<double>> parametrosMovimientos;
    bool permitirEmision = true; // Controla si se emite la señal para mover el resorte
    QMediaPlayer *musicaFondoLaberinto, *sonidoDisparo;
    QAudioOutput *volumenFondoLaberinto, *volumenDisparo;
    QList<QGraphicsTextItem*> ListaLetras;
    QVector<QPoint> listaDePuntos;
    short int claveObtenida;
    /*############# FIN CRISTIAN ################*/


    /* JUAN LUIS */

    bool removido = false;

    QGraphicsScene *escenaEscape;
    void crearEscape();

    QGraphicsScene *escenaCarrera;
    void crearCarrera();
    Helicoptero* helicoptero;

    void colocarObstaculos(QGraphicsScene* scene, int dificultad);
    void moverObjetos(QGraphicsScene* scene, int velocidad);

    QList <QGraphicsPixmapItem*> obstaculos;
    int carrilAnterior;
    bool ablePower = true;

    QList <QGraphicsTextItem*> letras;
    QString clave = "LIBERTAD";
    int indexClave = 0;

    void colocarLetras(QGraphicsScene* scene);

    QGraphicsScene* escenaMenu;
    void crearMenu();

    void recuadroGameOver();

};

#endif // MAINWINDOW_H
