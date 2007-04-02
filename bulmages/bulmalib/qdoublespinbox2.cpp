/***************************************************************************
 *   Copyright (C) 2007 by Fco. Javier M. C.                               *
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

#include "qdoublespinbox2.h"


QDoubleSpinBox2::QDoubleSpinBox2(QWidget *parent) : QDoubleSpinBox(parent) {
    setAlignment(Qt::AlignRight);
}


QDoubleSpinBox2::~QDoubleSpinBox2() {
}


QString const QDoubleSpinBox2::text() {
    _depura("QDoubleSpinBox2::text", 0);
    QString a = QDoubleSpinBox::text();
    //a = a.replace(",", ".");
    _depura("END QDoubleSpinBox2::text", 0);
    return a;
}

void QDoubleSpinBox2::setValue(double valor) {
    QDoubleSpinBox::setValue(valor);
}


bool QDoubleSpinBox2::eventFilter(QObject *obj, QEvent *event) {
    /*
    /// Si es un release de tecla se hace la funcionalidad especificada.
    if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) {
        _depura("se pulsa una tecla", 2);
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int key = keyEvent->key();
        Qt::KeyboardModifiers mod = keyEvent->modifiers();
        switch (key) {
            case Qt::Key_Period:
                _depura("estoy", 2);
 //               QCoreApplication::sendEvent(obj, new QKeyEvent(QEvent::KeyPress, Qt::Key_Comma, 0, 0));
                return TRUE;
        } // end switch
    } // end if
    */
    return QDoubleSpinBox2::eventFilter(obj, event);
}

