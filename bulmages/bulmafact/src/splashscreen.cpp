/**************************************************************************
*   Copyright (C) 2005 by Tomeu Borras Riera                              *
*   tborras@conetxia.com                                                  *
*   Copyright (C) 2006 by Fco. Javier M. C.                               *
*   fcojavmc@todo-redes.com                                               *
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


Splash::Splash() : QDialog(0, Qt::FramelessWindowHint) {
    QPixmap image0;
    image0.load(confpr->valor(CONF_SPLASH_BULMAFACT));
    /// Se modifica la paleta para que utilize la imagen como fondo.
    QPalette p = this->palette();
    p.setBrush(QPalette::Window, image0);
    this->setPalette(p);

    QLabel *l0 = new QLabel(this);
    l0->setTextFormat(Qt::RichText);
    l0->setGeometry(0, 0, image0.width(), image0.height());

    l0->setAlignment(Qt::AlignTop);
    l0->setFont(QFont("Arial", 20, QFont::Bold));
    l0->setText(tr("<center><font size=+1 color=\"#0000ff\">BulmaFact</font>&nbsp;<font color=\"#ffcc00\">0.5.9</font></center>"));

    l1 = new QTextBrowser(this);
    l1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    l1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    l1->setAlignment(Qt::AlignBottom);
    l1->setFont(QFont("helvetica", 9, QFont::Normal));
    l1->setGeometry(0, image0.height() - 23, image0.width(), 58);

    QPalette pl1 = l1->palette();
    pl1.setBrush(QPalette::Base, QColor("#DDDDDD"));
    l1->setPalette(pl1);

    QProgressBar *barra = new QProgressBar(this);
    /// Poniendo el minimo y maximo a 0 hace el efecto especial.
    barra->setRange(0, 0);
    barra->setGeometry(0, image0.height() - 38, image0.width(), 15);
    QPalette pbarra = barra->palette();
    QColor colorfondobarra = QColor("#000000");
    colorfondobarra.setAlpha(100);
    pbarra.setBrush(QPalette::Base, colorfondobarra);
    barra->setPalette(pbarra);

    this->paint();

    QTimer timer1(this);
    connect(&timer1, SIGNAL(timeout()), SLOT(paint()));
    timer1.start(1750);
    /// Nos muestra la ventana en modo MODAL.
    exec();
}


Splash::~Splash() {
    delete l1;
}


bool Splash::event(QEvent *evt) {
    if (evt->type() == QEvent::KeyPress) {
        close();
    } // end if

    if (evt->type() == QEvent::MouseButtonRelease) {
        close();
    } // end if

    return QDialog::event(evt);
}


void Splash::paint() {
    static int a = 0;
    int cantidadmensajes;
    QString cad = "";
    QString mensajes[] = {
                             tr("Calibrando los lasers del lector de CD"),
                             tr("Comprobando la disquetera y la memoria RAM"),
                             tr("Induciendo energia cuantica, entre su RAM y su ROM"),
                             tr("Pequenyos golpecitos de reajuste del HD"),
                             tr("Probando la velocidad del ventilador de la CPU"),
                             tr("Haciendo PING contra el servidor de la MetaBase"),
                             tr("Fallando a Segmento"),
                             tr("Dejando tiempo libre al sistema"),
                             tr("Sincronizando fases Alfa Beta"),
                             tr("Flusheando datos con vidas inteligentes superiores"),
                             tr("Permutando las particiones del Sistema Operativo"),
                             tr("Crackeando BulmaGes")};

    /// Cuenta el numero de mensajes.
    cantidadmensajes = sizeof(mensajes) / sizeof(mensajes[0]);

    if (a) {
        cad = cad + "<FONT COLOR='#FF0000'>&nbsp;.......&nbsp;<B>OK</B></FONT><BR>";
    } // end if

    /// Recorre todos los elementos del array de mensajes cada vez que se llama a la funcion.
    /// Cuando termina de recorrerlos todos cierra la ventana y el programa continua.
    if (a >= cantidadmensajes)
        this->close();

    cad = cad + "<FONT COLOR='#000066'>" + mensajes[a] + "</FONT>";
    a++;
    l1->insertHtml(cad);
    /// Asegura que los ultimos mensajes son visibles haciendo el desplazamiento necesario.
    l1->ensureCursorVisible();
}

