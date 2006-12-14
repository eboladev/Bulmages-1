/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#include <QLayout>
#include <QColor>
#include <QTimer>
#include <QPixmap>
#include <QDir>
#include <QIcon>

#include "splashscreen.h"
#include "configuracion.h"
#include "funcaux.h"

/// Constructor de la clase
/** Genera el splash y pinta los elementos iniciales. */
Splash::Splash() : QDialog(0, Qt::FramelessWindowHint) {
    QPixmap image0;
    image0.load(confpr->valor(CONF_SPLASH).toAscii());
    l = new QLabel(this);
    l->setPixmap(image0);
    l->setGeometry(0, 0, image0.width(), image0.height());


    QLabel *l0 = new QLabel(this);
    l0->setTextFormat(Qt::RichText);
    l0->setGeometry(0, 0, image0.width(), image0.height());

    l0->setAlignment(Qt::AlignTop);
    l0->setFont(QFont("Arial", 20, QFont::Bold));
    l0->setText(tr("<center><font size=+1 color=\"#666666\">BulmaCont</font>&nbsp;<font color=\"#333333\">0.5.9</font></center>"));

    l2 = new QTextBrowser(this);
    l2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    l2->setAlignment(Qt::AlignBottom);
    l2->setFont(QFont("helvetica", 9, QFont::Normal));
    l2->setGeometry(0, image0.height(), image0.width(), 58);

    QTimer timer(this);
    connect(&timer, SIGNAL(timeout()), SLOT(close()));
    timer.start(10000);

    QTimer timer1(this);
    connect(&timer1, SIGNAL(timeout()), SLOT(paint()));
    timer1.start(1750);
    exec();
}

/// Destructor de la clase.
/** Libera memoria. */
Splash::~Splash() {
    delete l;
    delete l2;
}

/// Evento que se dispara cada cierto tiempo.
/** El constructor crea un evento temporal que dispara este metodo. */
bool Splash::event(QEvent *evt) {
    if (evt->type() == QEvent::KeyPress) {
        close();
    } // end if
    if (evt->type() == QEvent::MouseButtonPress) {
        close();
    } // end if
    return QDialog::event(evt);
}

/// Pintado de la pantalla.
/** Actualiza el widget. */
void Splash::paint() {
    static int a = 0;
    int cantidadmensajes;
    QString cad = "";
    QString mensajes[] = {
                             tr("Comprobando nivel de combustible"),
                             tr("Calibrando los lasers del lector de CD"),
                             tr("Comprobando la disquetera y la memoria fisica"),
                             tr("Induciendo energia quantica, entre su RAM y su ROM"),
                             tr("Golpecitos de reajuste del HD"),
                             tr("Probando la velocidad del ventilador de la CPU y su frecuencia"),
                             tr("Haciendo PING contra el servidor de la MetaBase"),
                             tr("Dejando tiempo libre al sistema"),
                             tr("Sincronizando fases Alfa Beta"),
                             tr("Flusheando datos con vidas inteligentes superiores"),
                             tr("Permutando las tablas de particion del Sistema Operativo"),
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
    l2->insertHtml(cad);
    /// Asegura que los ultimos mensajes son visibles haciendo el desplazamiento necesario.
    l2->ensureCursorVisible();
}

