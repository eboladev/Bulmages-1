/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
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

#include "qtexteditdelegate.h"
#include "funcaux.h"

QTextEditDelegate::QTextEditDelegate(QWidget *parent) : QTextEdit(parent) {
    installEventFilter(this);
}


QTextEditDelegate::~QTextEditDelegate() {
}



bool QTextEditDelegate::eventFilter(QObject *obj, QEvent *event) {
    _depura("QTextEditDelegate::eventFilter", 0,   QString::number(event->type()));

    /// Si es un release de tecla se hace la funcionalidad especificada.
    if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int key = keyEvent->key();
    	_depura("QTextEditDelegate::key = :", 0, QString::number(key));
        Qt::KeyboardModifiers mod = keyEvent->modifiers();
        /// ------------------ EL CAMBIO ------------------------------
        switch (key) {

        case Qt::Key_Return:
        case Qt::Key_Enter:
            return TRUE;
            break;
        } // end switch
    } // end if
    _depura("END QTextEditDelegate::eventFilter()", 0);
    return QTextEdit::eventFilter(obj, event);
//	return TRUE;
}
