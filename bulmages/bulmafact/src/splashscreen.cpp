#include "splashscreen.h"
#include "configuracion.h"
#include "qdialog.h"
#include "qlayout.h"
#include "qcolor.h"

Splash::Splash() : QDialog( 0, "", true, WStyle_NoBorder |WStyle_Customize ) {
//  QPixmap image0( logofx );
	QPixmap image0;
	image0.load(confpr->valor(CONF_SPLASH_BULMAFACT).c_str());
   l = new QLabel( this );
   l->setPixmap( image0 );
//	fprintf(stderr,"ancho: %d, alto: %d\n", image0.width(), image0.height());
   l->setGeometry ( 0, 0, image0.width(), image0.height() );

// QVBoxLayout *vb = new QVBoxLayout(this);
// this->addLayout( vb );


 // Add Product name/Main message
// if(mmsg != 0) {
  QLabel *l1 = new QLabel( this );
  l1->setAlignment( AlignVCenter|AlignHCenter );
  l1->setFont( QFont( "Arial", 20, QFont::Bold ) );
  l1->setPaletteForegroundColor(QColor("#333388"));
  l1->setText( tr("BulmaFact") );
  l1->setGeometry(10,10,150,25);
  l1->setBackgroundOrigin(QWidget::ParentOrigin);
  l1->setBackgroundPixmap(image0);

 
  QLabel *l0 = new QLabel( this );
  l0->setPaletteForegroundColor(QColor("#FF0000"));
  l0->setAlignment( AlignVCenter );
  l0->setFont( QFont( "Arial", 20, QFont::Bold ) );
  l0->setText( tr("v 0.0.1") );
  l0->setGeometry(100,35,150,25);
  l0->setBackgroundOrigin(QWidget::ParentOrigin);
  l0->setBackgroundPixmap(image0);


  QLabel *l21 = new QLabel( this );
  l21->setAlignment( AlignVCenter );
  l21->setFont( QFont( "helvetica", 7, QFont::Normal ) );
  l21->setText( "" );
  l21->setGeometry(0,image0.height()-38,image0.width(),58);
  l21->setPaletteForegroundColor(QColor("#000066"));
  l21->setPaletteBackgroundColor(QColor("#000000"));
    
  l2 = new QTextBrowser(this);
  l2->setVScrollBarMode(QScrollView::AlwaysOff);
  l2->setAlignment( AlignBottom );
  l2->setFont( QFont( "helvetica", 11, QFont::Normal ) );
  l2->setText( tr("BULMAGÉS") );
  l2->setGeometry(0,image0.height()-38,image0.width(),58);
  l2->setPaletteForegroundColor(QColor("#000066"));
  l2->setPaletteBackgroundColor(QColor("#DDDDDD"));

  QTimer *timer = new QTimer(this);
  connect( timer, SIGNAL(timeout()), SLOT(close()) );
  timer->start( 10000 ); //timer->start( 10000);

  QTimer *timer1 = new QTimer(this);
  connect( timer1, SIGNAL(timeout()), SLOT(paint()) );
  timer1->start( 1750 ); //timer1->start( 1750);
  exec();
  delete l1;
}// end splash

Splash::~Splash() {
 delete l;
 delete l2;
}


void Splash::paint() {
  static int a=0;
  static QString cad;
  QString mensajes[]={
                    tr("Comprobando nivel de combustible"),
                    tr("Calibrando los lasers del lector de CD"),
                    tr("Comprobando la disquetera y la Memoria Fisica"),
                    tr("Induciendo energía quantica, entre su RAM y su ROM"),
                    tr("Pequeños golpecitos de reajuste del HD"),
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
   cad= cad+" "+mensajes[a];
  if(a >10) a=0;
  a++;
  l2->setText( cad );
  l2->scrollBy(0,400);
}
