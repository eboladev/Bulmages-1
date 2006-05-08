/***************************************************************************
*   Copyright (C) 2005 by Tomeu Borras Riera                              *
*   tborras@conetxia.com                                                  *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/

#include "splashscreen.h"
#include "configuracion.h"
#include "qdialog.h"
#include "qlayout.h"
#include "qcolor.h"
#include <QObject>
#include <qevent.h>
//Added by qt3to4:
#include <QPixmap>
#include <QLabel>

Splash::Splash() : QDialog( 0, "", true, Qt::WStyle_NoBorder |Qt::WStyle_Customize ) {
   QPixmap image0;
   image0.load(confpr->valor(CONF_SPLASH_BULMAFACT));
   setBackgroundPixmap(image0);

   l = new QLabel( this );
  QLabel *l0 = new QLabel( this );
  l0->setPaletteForegroundColor(QColor("#FF0000"));
  l0->setAlignment( Qt::AlignVCenter );
  l0->setFont( QFont( "Arial", 20, QFont::Bold ) );
  l0->setText( tr("\nv 0.5.9") );
  l0->setGeometry(0,0,350,55);


  QLabel *l1 = new QLabel( this );
  l1->setGeometry(0,0,150,55);
  l1->setAlignment( Qt::AlignVCenter | Qt::AlignHCenter );
  l1->setFont( QFont( "Arial", 20, QFont::Bold ) );
  l1->setPaletteForegroundColor(QColor("#333388"));
  l1->setText( tr("\nBulmaFact") );

  l2 = new Q3TextBrowser(this);
  l2->setVScrollBarMode(Q3ScrollView::AlwaysOff);
  l2->setAlignment( Qt::AlignBottom );
  l2->setFont( QFont( "helvetica", 11, QFont::Normal ) );
  l2->setText( tr("BULMAGES") );
  l2->setGeometry(0,image0.height()-38,image0.width(),58);
  l2->setPaletteForegroundColor(QColor("#000066"));
  l2->setPaletteBackgroundColor(QColor("#DDDDDD"));


  QTimer timer(this); // = new QTimer(this);
  connect( &timer, SIGNAL(timeout()), SLOT(close()) );
  timer.start( 10000 ); //timer->start( 10000);

  QTimer timer1(this); // = new QTimer(this);
  connect( &timer1, SIGNAL(timeout()), SLOT(paint()) );
  timer1.start( 1750 ); //timer1->start( 1750);  
  exec();
  delete l1;
}

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
  QString mensajes[]={
                    tr("Calibrando los lasers del lector de CD."),
                    tr("Comprobando la disquetera y la memoria RAM."),
                    tr("Induciendo energia cuantica, entre su RAM y su ROM."),
                    tr("Pequenyos golpecitos de reajuste del HD."),
                    tr("Probando la velocidad del ventilador de la CPU y su frecuencia."),
                    tr("Haciendo PING contra el servidor de la MetaBase."),
                    tr("Violando a Segmento."),
                    tr("Dejando tiempo libre al sistema."),
                    tr("Sincronizando fases Alfa Beta."),
                    tr("Flusheando datos con vidas inteligentes superiores."),
                    tr("Permutando las tablas de particiones del Sistema Operativo."),
		    tr("Crackeando BulmaGes.")};
  if (a) {
   cad = cad + "<FONT COLOR='#FF0000'>....... <B>OK</B></FONT><BR>";
  }// end if
   cad= cad+" "+mensajes[a];
  if(a >10) a=0;
  a++;
  l2->setText( cad );
  l2->scrollBy(0,400);
}



