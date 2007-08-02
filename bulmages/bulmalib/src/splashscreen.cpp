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
#include "funcaux.h"


Splash::Splash(QString appSplash, QString appName, QString appVersion) : QDialog(0, Qt::FramelessWindowHint) {
    image0 = new QPixmap();
    image0->load(appSplash);

    /// Se modifica la paleta para que utilize la imagen como fondo.
    QPalette p = this->palette();
    p.setBrush(QPalette::Window, *image0);
    this->setPalette(p);

    /// Centramos la ventana en la pantalla.
    QDesktopWidget *pantalla = new QDesktopWidget();
    setGeometry((pantalla->screenGeometry().width() / 2) - (image0->width() / 2), (pantalla->screenGeometry().height() / 2) - ((image0->height() + 58) / 2), image0->width(), image0->height());
    delete pantalla;

    l0 = new QLabel(this);
    l0->setTextFormat(Qt::RichText);
    l0->setGeometry(0, 0, image0->width(), image0->height());
    l0->setAlignment(Qt::AlignTop);
    l0->setFont(QFont("Arial", 20, QFont::Bold));
    l0->setText("<center><font size=+1 color=\"#a3ffa3\">" + appName + "</font>&nbsp;<font color=\"#0000ff\">" + appVersion + "</font></center>");

    m_label = new QLabel(this);
    m_label->setTextFormat(Qt::RichText);
    m_label->setGeometry(0, image0->height() - 80, image0->width(), 15);
    m_label->setAlignment(Qt::AlignTop);
    m_label->setFont(QFont("helvetica", 8, QFont::Bold));
    m_label->show();


    m_label1 = new QLabel(this);
    m_label1->setAlignment(Qt::AlignBottom);
    m_label1->setFont(QFont("helvetica", 9, QFont::Normal));
    m_label1->setGeometry(0, image0->height() - 50, image0->width(), 50);



    barra = new QProgressBar(this);
    barra->setTextVisible(FALSE);
    /// Poniendo el minimo y maximo a 0 hace el efecto especial.
    barra->setRange(0, 100);
    barra->setGeometry(0, image0->height() - 65, image0->width(), 15);
    QPalette pbarra = barra->palette();
    QColor colorfondobarra = QColor("#000000");
    colorfondobarra.setAlpha(100);
    pbarra.setBrush(QPalette::Base, colorfondobarra);
    barra->setPalette(pbarra);

}


Splash::~Splash() {
    delete l0;
    delete m_label;
    delete m_label1;
    delete image0;
}




void Splash::mensaje(QString mens) {
    _depura(mens, 0);
    static int a = 0;
    static QString cadant="";
    QString cad = "";
    QString cad1 = "";
    if (a != 0)
            cad1 = "<FONT SIZE=\"-3\" COLOR='#FF0000'>&nbsp;.......&nbsp;<B>OK</B></FONT><BR>";
    cad = cad + "<FONT SIZE=\"-3\" COLOR='#000066'>" + mens + "</FONT>";
    a++;
    m_label1->setText(cadant+cad1+cad);
    m_label1->repaint();
    m_label->setText(mens);
    m_label->repaint();
    repaint();
    cadant = cad;
}

void Splash::setBarraProgreso(int progreso) {
        barra->setValue(progreso);
}

void Splash::barraprogreso() {
    if (barra->value() < 10) {
        barra->setValue(barra->value() + 1);
    } else {
        barra->setValue(0);
    } // end if
}

