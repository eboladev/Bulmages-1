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
#include <QLineEdit>
#include <QTextEdit>

#include "qtable2.h"
#include "configuracion.h"
#include "funcaux.h"


QTableItemTextDelegate::QTableItemTextDelegate(QObject *parent=0) : QItemDelegate(parent) {}

QTableItemTextDelegate::~QTableItemTextDelegate() {}


QWidget *QTableItemTextDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
	QTextEdit *textedit = new QTextEdit(parent);
	return textedit;
/*
    if (index.column() == durationColumn) {
        QTimeEdit *timeEdit = new QTimeEdit(parent);
        timeEdit->setDisplayFormat("mm:ss");
        connect(timeEdit, SIGNAL(editingFinished()),
                this, SLOT(commitAndCloseEditor()));
        return timeEdit;
    } else {
        return QItemDelegate::createEditor(parent, option, index);
    }
*/
}

void QTableItemTextDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
        QTextEdit *textedit = qobject_cast<QTextEdit *>(editor);
	model->setData(index, textedit->toPlainText());
/*
    if (index.column() == durationColumn) {
        QTimeEdit *timeEdit = qobject_cast<QTimeEdit *>(editor);
        QTime time = timeEdit->time();
        int secs = (time.minute() * 60) + time.second();
        model->setData(index, secs);
    } else {
        QItemDelegate::setModelData(editor, model, index);
    }
*/
}


void QTableItemTextDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {

//    QString data = index.model()->data(index, Qt::DisplayRole);
    QString data = index.data(Qt::DisplayRole).toString();
    QTextEdit *textedit = qobject_cast<QTextEdit *>(editor);
    textedit->setText(data);
    textedit->setGeometry(textedit->x(), textedit->y(), textedit->width()+150, textedit->height()+50);

/*
    if (index.column() == durationColumn) {
        int secs = index.model()->data(index, Qt::DisplayRole).toInt();
        QTimeEdit *timeEdit = qobject_cast<QTimeEdit *>(editor);
        timeEdit->setTime(QTime(0, secs / 60, secs % 60));
    } else {
        QItemDelegate::setEditorData(editor, index);
    }
*/
/*
    // let base class fill the editor
    QItemDelegate::setEditorData(editor, index);

    // select all if it's a line edit
    QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor);
    if (lineEdit)
        lineEdit->selectAll();
*/
}


QTableWidget2::QTableWidget2(QWidget *parent) : QTableWidget(parent) {
    _depura("QTableWidget2::QTableWidget2", 0);
//    setItemDelegate(new QTableItemDelegate(this));
    installEventFilter(this);
    connect(this, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(sitemChanged(QTableWidgetItem *)));
    _depura("END QTableWidget2::QTableWidget2", 0);
}

void QTableWidget2::editItem(QTableWidgetItem *it) {
    _depura("edicion", 2);
}

void QTableWidget2::sitemChanged(QTableWidgetItem *it) {
    //	_depura("item cambiado", 2);
}

/// Esta funcion ya es obsoleta y no se utiliza.
bool QTableWidgetItem2::operator< (const QTableWidgetItem & other) const {
    _depura("QTableWidgetItem2::operator<", 2);
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
    _depura("END QTableWidgetItem2::operator<", 2);
    return TRUE;
}


bool QTableWidget2::eventFilter(QObject *obj, QEvent *event) {
    _depura("QTableWidget2::eventFilter() :" + QString::number(event->type()), 1);
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int key = keyEvent->key();
        int col = currentColumn();
        int row = currentRow();
        /// Algunas veces se produce un eventfilter pero la fila no existe (-1) en esos
        /// casos abortamos la ejecucion del eventFilter para que no de fallos en la
        /// busqueda de que celda es.
        if (row < 0) {
            return TRUE;
        } // end if
        Qt::KeyboardModifiers mod = keyEvent->modifiers();
        /// ------------------ EL CAMBIO ------------------------------
        switch (key) {

        case Qt::Key_Return:
        case Qt::Key_Enter:
            emit editFinished(row, col, key);
            return TRUE;
            break;
	} // end switch
    } // end if

    /// Si es un release de tecla se hace la funcionalidad especificada.
    if (event->type() == QEvent::KeyRelease) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int key = keyEvent->key();
        int col = currentColumn();
        int row = currentRow();
        /// Algunas veces se produce un eventfilter pero la fila no existe (-1) en esos
        /// casos abortamos la ejecucion del eventFilter para que no de fallos en la
        /// busqueda de que celda es.
        if (row < 0) {
            return TRUE;
        } // end if
        Qt::KeyboardModifiers mod = keyEvent->modifiers();
        /// ------------------ EL CAMBIO ------------------------------
        switch (key) {

        case Qt::Key_Return:
        case Qt::Key_Enter:
	    _depura("Se ha pulsado el Return", 2);
//            emit editFinished(row, col, key);
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
            } // end if
            /// Al pulsar la tecla 'arriba' se considera que es el fin de la edici&oacute;n de la celda.
        case Qt::Key_Down:
            if ((mod & Qt::ControlModifier) || (mod & Qt::AltModifier)) {
                emit ctrlBajar(row, col);
                return TRUE;
            } // end if
            /// Al pulsar la tecla 'abajo' se considera que es el fin de la edici&oacute; de la celda.
            return TRUE;
        case Qt::Key_Left:
            if ((mod & Qt::ControlModifier) || (mod & Qt::AltModifier)) {
                emit ctrlIzquierda(row, col);
                return TRUE;
            } // end if
        case Qt::Key_Right:
            if ((mod & Qt::ControlModifier) || (mod & Qt::AltModifier)) {
                emit ctrlDerecha(row, col);
                return TRUE;
            } // end if
        } // end switch
    } // end if
    _depura("END QTableWidget2::eventFilter()\n", 1);
    return QTableWidget::eventFilter(obj, event);
}


void QTableWidget2::ordenar() {
    _depura("QTableWidget2::ordenar ", 0);
    sortByColumn(m_colorden);
    _depura("END QTableWidget2::ordenar", 0);
}



void QTableWidget2::setText(int x, int y, const QString & val) {
    _depura("QTableWidget::setText", 0);
    QTableWidgetItem2 *newitem = new QTableWidgetItem2(val);
    setItem(x, y, newitem);
    _depura("END QTableWidget::setText", 0);
}

