/***************************************************************************
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

#include <QWidget>
#include <QAction>

#include "qworkspace2.h"


void QWorkspace2::addWindow(QWidget * w) {
    int tamdispW;
    int tamdispH;
    int tamventanadecoW;
    int tamventanadecoH;
    int tamventanaW;
    int tamventanaH;
    int margen = 10;

    QWorkspace::addWindow(w);
    /// Se comprueba el tamanyo de la ventana que esta disenyada con Designer:
    /// Solo si la ventana es mas grande que el espacio de representacion se ajusta
    /// para caber dentro.

    /// Captura el tamanyo disponible. Restamos el margen que dejamos a cada lado (x2).
    tamdispW = this->width() - (margen * 2);
    tamdispH = this->height() - (margen * 2);

    /// Captura el tamanyo con decoracion de la ventana a insertar.
    tamventanadecoW = w->parentWidget()->frameGeometry().width();
    tamventanadecoH = w->parentWidget()->frameGeometry().height();
    /// Captura el tamanyo sin decoracion de la ventana a insertar.
    tamventanaW = w->geometry().width();
    tamventanaH = w->geometry().height();

    /// Comprobamos si es necesario cambiar el tamanyo a la ventana.
    if (tamventanadecoW > tamdispW)
        tamventanaW = tamdispW - (tamventanadecoW - tamventanaW);
    if (tamventanadecoH > tamdispH)
        tamventanaH = tamdispH - (tamventanadecoH - tamventanaH);

    w->setGeometry(0, 0, tamventanaW, tamventanaH);
    w->parentWidget()->move(margen, margen);

    /// Crea un QAction para manejar la tecla rapida ESC para cerrar la ventana.
    QAction *accionEsc = new QAction(w);
    accionEsc->setShortcut(tr("Esc"));
    connect(accionEsc, SIGNAL(triggered()), w, SLOT(close()));
    w->addAction(accionEsc);
}

