#include "splashscreen.h"
#include "configuracion.h"
#include <qdialog.h>
#include <qlayout.h>
#include <qcolor.h>
#include <qtimer.h>
//Added by qt3to4:
#include <QPixmap>
#include <QLabel>
#include <QEvent>

/// Para la exploracion de iconos
#include <QDir>
#include <QIcon>
#include "funcaux.h"

Splash::Splash() : QDialog( 0, "", true, Qt::WStyle_NoBorder |Qt::WStyle_Customize ) {
    QPixmap image0;
    image0.load(confpr->valor(CONF_SPLASH).ascii());
    l = new QLabel( this );
    l->setPixmap( image0 );
    l->setGeometry ( 0, 0, image0.width(), image0.height() );

    QLabel *l1 = new QLabel( this );
    l1->setAlignment( Qt::AlignVCenter|Qt::AlignHCenter );
    l1->setFont( QFont( "Arial", 20, QFont::Bold ) );
    l1->setPaletteForegroundColor(QColor("#666688"));
    l1->setText( tr("BulmaCont") );
    l1->setGeometry(270,10,130,25);
    l1->setBackgroundOrigin(QWidget::ParentOrigin);
    l1->setBackgroundPixmap(image0);


    QLabel *l0 = new QLabel( this );
    l0->setPaletteForegroundColor(QColor("#CC6666"));
    l0->setAlignment( Qt::AlignVCenter );
    l0->setFont( QFont( "Arial", 20, QFont::Bold ) );
    l0->setText( tr("v 0.5.1") );
    l0->setGeometry(300,30,130,25);
    l0->setBackgroundOrigin(QWidget::ParentOrigin);
    l0->setBackgroundPixmap(image0);

    l2 = new Q3TextBrowser(this);
    l2->setVScrollBarMode(Q3ScrollView::AlwaysOff);
    l2->setAlignment( Qt::AlignBottom );
    l2->setFont( QFont( "helvetica", 11, QFont::Normal ) );
    l2->setText( tr("BULMACONT") );
    l2->setGeometry(0,image0.height(),image0.width(),58);
    l2->setPaletteForegroundColor(QColor("#000066"));
    l2->setPaletteBackgroundColor(QColor("#DDDDDD"));

    QTimer timer(this);
    connect( &timer, SIGNAL(timeout()), SLOT(close()) );
    timer.start(10000 );

    QTimer timer1(this);
    connect( &timer1, SIGNAL(timeout()), SLOT(paint()) );
    timer1.start( 1750 );
    exec();
    delete l1;
}// end splash

Splash::~Splash() {
    delete l;
    delete l2;
}

bool Splash::event( QEvent *evt) {
    if (evt->type() == QEvent::KeyPress) {
        close();
    }// end if
    if (evt->type() == QEvent::MouseButtonPress) {
        close();
    }// end if
    return QDialog::event( evt);
}


void Splash::paint() {
    static int a=0;
    static QString cad;
    static int b=0;
    QString mensajes[]={
                           tr("Comprobando nivel de combustible"),
                           tr("Calibrando los lasers del lector de CD"),
                           tr("Comprobando la disquetera y la Memoria Fisica"),
                           tr("Induciendo energía quantica, entre su RAM y su ROM"),
                           tr("Golpecitos de reajuste del HD"),
                           tr("Probando la Velocidad el Ventilador de la CPU y su Frecuencia"),
                           tr("Haciendo PING contra el servidor de la MetaBase"),
                           tr("Violando a Segmento"),
                           tr("Dejando tiempo libre al sistema"),
                           tr("Sincronizando fases Alfa Beta"),
                           tr("Flusheando Datos con vidas inteligentes superiores"),
                           tr("Permutando las tablas de partición del sistema operativo"),
                           tr("Crackeando BulmaGés")};
    if (a) {
        cad = cad + "<FONT COLOR='#FF0000'>....... <B>OK</B></FONT><BR>";
    }// end if

    /// Este trozo de codigo es para sacar los iconos del recurso.
    if (b ==  0) {
        b = 1;
//        lista(":/");
    } // end if

    cad= cad+" "+mensajes[a];
    if(a >10)
        a=0;
    a++;


    l2->setText( cad );
    l2->scrollBy(0,400);
}

void Splash::lista(QString dir) {
    _depura("Splash::lista", 0);
    static int x= 0;
    static int y = 0;
    /// Inicializamos los iconos para ver si se ven bien.AboutView
    QDir subdir(dir);
    QFileInfoList sublist = subdir.entryInfoList();
    for (int j =0; j < sublist.size(); ++j) {
        QFileInfo subfile = sublist.at(j);
        QString f = subfile.filePath();
        QPixmap icon;
        if (icon.load(f) ) {
            QLabel *l4 = new QLabel( this );
            l4->setPixmap( icon );
            l4->setGeometry ( x, y, 32, 32 );
	    l4->show();
            x = x + 40;
            if (x > 350) {
                x= 0;
                y = y + 40;
		if (y > 300) y = 0;
            } // end if
        } else {
            lista(f);
        }
    } // end for
    _depura("END Splash::lista", 0);
}
