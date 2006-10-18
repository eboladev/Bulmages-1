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

#include <QKeyEvent>
#include <QEvent>

#include "qtable2.h"
#include "configuracion.h"
#include "funcaux.h"


QTableWidget2::QTableWidget2(QWidget *parent) : QTableWidget(parent) {
    installEventFilter(this);
}


/// Esta funcion ya es obsoleta y no se utiliza.
bool QTableWidgetItem2::operator < (const QTableWidgetItem & other) {
    _depura("QTableWidgetItem2::operator<", 0);
    bool oknumero;
    bool oknumero1;
    QString cad = text();
    QString cad1 = other.text();

    if (cad != "") {
        /// Comprobamos si es un número.
        double ncad = cad.toDouble(&oknumero);
        double ncad1 = cad1.toDouble(&oknumero1);
        if (oknumero && oknumero1) {
            return ncad < ncad1;
        } // end if
        QDate fcad = normalizafecha(cad);
        QString acad = fcad.toString(Qt::ISODate);
        QDate fcad1 = normalizafecha(cad1);
        QString acad1 = fcad1.toString(Qt::ISODate);

        if (acad[2] == '/' && acad1[2]== '/') {
            return fcad < fcad1;
        } // end if
        return cad < cad1;
    } // end if
    _depura("END QTableWidgetItem2::operator<", 0);
    return TRUE;
}


bool QTableWidget2::eventFilter(QObject *obj, QEvent *event) {
    _depura("QTableWidget2::eventFilter() :" + QString::number(event->type()), 0);

    /// Si es una pulsaci&oacute;n de tecla que esta capturada con el release salimos
    /// sin hacer nada.
/*
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int key = keyEvent->key();
        switch (key) {
        case Qt::Key_Return:
        case Qt::Key_Enter:
            printf("pulsado intro\n");
            return TRUE;
        case Qt::Key_Up:
            printf("pulsado flecha arriba\n");
            return TRUE;
        case Qt::Key_Down:
            printf("pulsado flecha abajo\n");
            return TRUE;
        } // end switch
    } // end if
*/

    /// Si es un release de tecla se hace la funcionalidad especificada.
    if (event->type() == QEvent::KeyRelease) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int key = keyEvent->key();
        int col = currentColumn();
        int row = currentRow();
        /// Algunas veces se produce un eventfilter pero la fila no existe (-1) en esos
        /// casos abortamos la ejecucion del eventFilter para que no de fallos en la
        /// busqueda de que celda es.
        if (row < 0)
            return TRUE;
        Qt::KeyboardModifiers mod = keyEvent->modifiers();
        /// ------------------ EL CAMBIO ------------------------------
        switch (key) {
        case Qt::Key_Return:
        case Qt::Key_Enter:
            emit editFinished(row, col, key);
            return TRUE;
            break;
        case Qt::Key_Slash:
            if ((mod & Qt::ControlModifier) || (mod & Qt::AltModifier)) {
                emit pressedSlash(row, col);
                return TRUE;
            } // end if
            break;
        case Qt::Key_Minus:
            if ((mod & Qt::ControlModifier) || (mod & Qt::AltModifier)) {
                emit pressedMinus(row, col);
                return TRUE;
            } // end if
            break;
        case Qt::Key_Plus:
            emit pressedPlus(row, col);
            return TRUE;
        case Qt::Key_Asterisk:
            if ((mod & Qt::ControlModifier) || (mod & Qt::AltModifier)) {
                emit pressedAsterisk(row, col);
                return TRUE;
            } // end if
            break;
        case Qt::Key_Up:
            if ((mod & Qt::ControlModifier) || (mod & Qt::AltModifier)) {
                emit ctrlSubir(row, col);
            return TRUE;
 //           } else {
//                emit editFinished(row, col, key);
            } // end if
            /// Al pulsar la tecla 'arriba' se considera que es el fin de la edici&oacute;n de la celda.
        case Qt::Key_Down:
            if ((mod & Qt::ControlModifier) || (mod & Qt::AltModifier)) {
                emit ctrlBajar(row, col);
                return TRUE;
            } // end if
            /// Al pulsar la tecla 'abajo' se considera que es el fin de la edici&oacute; de la celda.
            emit editFinished(row, col, key);
            return TRUE;
        case Qt::Key_Left:
            if ((mod & Qt::ControlModifier) || (mod & Qt::AltModifier)) {
                emit ctrlIzquierda(row, col);
                return TRUE;
            } // end if
 //           emit editFinished(row, col, key);
 //           return TRUE;
        case Qt::Key_Right:
            if ((mod & Qt::ControlModifier) || (mod & Qt::AltModifier)) {
                emit ctrlDerecha(row, col);
                return TRUE;
            } // end if
//            emit editFinished(row, col, key);
//            return TRUE;
        } // end switch
    } // end if
    _depura("END QTableWidget2::eventFilter()\n", 1);
    return QTableWidget::eventFilter(obj, event);
}


void QTableWidget2::ordenar() {
    _depura("QTableWidget2::ordenar ", 0);
    sortColumn(m_colorden, (Qt::SortOrder) m_tipoorden);
    _depura("END QTableWidget2::ordenar", 0);
}


void QTableWidget2::sortByColumn(int col) {
    _depura("QTableWidget2::sortByColumn ", 0);
    if (m_tipoorden == 0)
        m_tipoorden = 1;
    else
        m_tipoorden = 0;
    sortColumn(col, (Qt::SortOrder) m_tipoorden);
    _depura("END QTableWidget2::sortByColumn", 0);
}


void QTableWidget2::sortColumn(int col, Qt::SortOrder tipoorden) {
    _depura("QTableWidget2::sortColumn", 0);

    m_tipoorden = tipoorden;
    m_colorden = col;
    if (m_colorden > columnCount() | m_colorden < 0) {
        m_colorden = 0;
        col = 0;
    } // end if
    if (m_tipoorden >1) {
        m_tipoorden = 1;
        tipoorden = (Qt::SortOrder) 1;
    } // end if
    int lastcol = columnCount();
    _depura("insercion de columnas", 0);
    insertColumn(lastcol);
    insertColumn(lastcol + 1);
    insertColumn(lastcol + 2);
    _depura("ocultacion de columnas", 0);
    hideColumn(lastcol);
    hideColumn(lastcol + 1);
    hideColumn(lastcol + 2);
    bool oknumero = TRUE;
    bool okfecha = TRUE;

    _depura("comienza el bucle", 0);
    for (int x = 0; x < rowCount(); x++) {
        _depura("tratamos un elemento " + QString::number(x) + " " + QString::number(col), 0);
        QString cad = item(x, col)->text();
        _depura("comprobamos " + cad, 0);
        if (cad != "") {
            setText(x, lastcol + 0, cad);
            /// Comprobamos si es un numero.
            cad.toDouble(&oknumero);
            if (oknumero) {
                while (cad.length() < 10)
                    cad.insert(0, "0");
                setText(x,lastcol + 1, cad);
            } // end if
            if (okfecha) {
                if (cad[2] == '/') {
                    QDate fech = normalizafecha(cad);
                    cad = fech.toString(Qt::ISODate);
                } else {
                    okfecha = FALSE;
                } // end if
                setText(x, lastcol + 2, cad);
            } // end if
        } // end if
    } // end for

    if (oknumero)
        QTableWidget::sortItems(lastcol + 1, tipoorden);
    else if (okfecha)
        QTableWidget::sortItems(lastcol + 2, tipoorden);
    else
        QTableWidget::sortItems(lastcol + 0, tipoorden);

    removeColumn(lastcol + 2);
    removeColumn(lastcol + 1);
    removeColumn(lastcol + 0);
    _depura("END QTableWidget2::sortColumn", 0);
}


void QTableWidget2::setText(int x, int y, const QString & val) {
    _depura("QTableWidget::setText", 0);
    QTableWidgetItem2 *newitem = new QTableWidgetItem2(val);
    setItem(x, y, newitem);
    _depura("END QTableWidget::setText", 0);
}

