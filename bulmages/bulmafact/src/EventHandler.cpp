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

#include <QWidget>
#include <Q3Table>
#include <QLineEdit>
#include <QKeyEvent>
#include <QEvent>

#include "EventHandler.h"
#include "funcaux.h"


EventHandler::EventHandler(QWidget *, const int textEditType = 0) {
    m_TextEditType=textEditType;
}


EventHandler::EventHandler(QWidget *, Q3Table *) {}


EventHandler::~EventHandler() {}


bool EventHandler::eventFilter(QObject *obj, QEvent *ev) {
    if (obj->isA("QTable")) {
        if (ev->type() == QEvent::KeyPress) {
            QKeyEvent *k = (QKeyEvent *)ev;
            switch (k->key()) {
            case Qt::Key_Enter: {
                    nextCell(obj);
                    return TRUE;
                }
            case Qt::Key_Return: {
                    nextCell(obj);
                    return TRUE;
                }
            case Qt::Key_Asterisk: {
                    qDebug("ASTERISCO");
                    duplicateCell(obj);
                    return TRUE;
                }
            }
        } // end if
    } // end if
    if (obj->isA("QLineEdit")) {
        if (ev->type() == QEvent::FocusOut) {
            switch (m_TextEditType) {
            case QUANTITY: {
                    return TRUE;
                }
            case DATE: {
                    manageDate(obj);
                    return TRUE;
                }
            }
        } // end if
    } // end if
    return FALSE;
}


void EventHandler::nextCell(QObject *obj) {
    Q3Table *t = (Q3Table *)obj;
    int row = t->currentRow();
    int col = t->currentColumn();
    int lastCol = t->numCols() - 1;
    if (t->isReadOnly() == FALSE) {
        //qDebug( "Fila, Columna = %d, %d", row, col);
        col++;
        while (true) {
            qDebug("Fila, Columna = %d, %d", row, col);
            if (col > lastCol) {
                col = 0;
                row++;
                if (row == (t->numRows())) {
                    t->setNumRows(row + 1);
                }
            } else {
                if (t->isColumnHidden(col) || t->isColumnReadOnly(col) || t->isRowHidden(row) || t->isRowReadOnly(row)) {
                    col++;
                } else {
                    t->setCurrentCell(row, col);
                    //t->editCell(row, col);
                    break;
                }
            } // end if
        } // end while
    } // end if
}


void EventHandler::duplicateCell(QObject *obj) {
    Q3Table *t = (Q3Table *)obj;
    int row = t->currentRow();
    int col = t->currentColumn();
    if (t->text(row, col) == "" && row > 0) {
        t->setText(row, col, t->text(row - 1, col));
    }
}


void EventHandler::manageDate(QObject *obj) {
    QLineEdit *t = (QLineEdit *)obj;
    t->setText(normalizafecha(t->text()).toString("dd/MM/yyyy"));
}


void EventHandler::manageQuantity(QObject *obj) {
    QLineEdit *t = (QLineEdit *)obj;
    t->setText(t->text().replace(",", "."));
}

