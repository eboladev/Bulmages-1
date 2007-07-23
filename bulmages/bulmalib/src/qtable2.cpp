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


/** Constructor de la clase estandar de delegacion para la edicion de elementos
en el QTable2 */
QTableItemTextDelegate::QTableItemTextDelegate(QObject *parent=0) : QItemDelegate(parent) {
    _depura("QTableItemTextDelegate::QTableItemTextDelegate", 0);
    _depura("END QTableItemTextDelegate::QTableItemTextDelegate", 0);
}


/** Destructor de la clase estandar de delegacion para la edicion de elementos en el Qtable2
*/
QTableItemTextDelegate::~QTableItemTextDelegate() {
    _depura("QTableItemTextDelegate::~QTableItemTextDelegate", 0);
    _depura("END QTableItemTextDelegate::~QTableItemTextDelegate", 0);
}


/** Creacion del editor basado en un QTextEdit para el caso de edicion de elementos
de Qtable2
*/
QWidget *QTableItemTextDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const {
    _depura("QTableItemTextDelegate::createEditor", 0);
    QTextEdit *textedit = new QTextEdit(parent);
    _depura("END QTableItemTextDelegate::createEditor", 0);
    return textedit;
}


/** Establecimiento de los datos que pasa entre el modelo de vista y el modelo
de edicion
*/
void QTableItemTextDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    _depura("QTableItemTextDelegate::setModelData", 0);
    QTextEdit *textedit = qobject_cast<QTextEdit *>(editor);
    model->setData(index, textedit->toPlainText());
    _depura("END QTableItemTextDelegate::setModelData", 0);

}


/** Establecimiento de los datos que pasa entre el modelo de vista y el modelo
de edicion
*/
void QTableItemTextDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
    _depura("QTableItemTextDelegate::setEditorData", 0);
    QString data = index.data(Qt::DisplayRole).toString();
    QTextEdit *textedit = qobject_cast<QTextEdit *>(editor);
    textedit->setPlainText(data);
    textedit->setGeometry(textedit->x(), textedit->y(), textedit->width()+150, textedit->height()+50);
    _depura("END QTableItemTextDelegate::setEditorData", 0);
}


/// ======================================================================


/** Constructor de QTableWidget2 clase derivada de QTableWidget con
un eventHandler especifico
*/
QTableWidget2::QTableWidget2(QWidget *parent) : QTableWidget(parent) {
    _depura("QTableWidget2::QTableWidget2", 0);
    installEventFilter(this);
    connect(this, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(sitemChanged(QTableWidgetItem *)));
    _depura("END QTableWidget2::QTableWidget2", 0);
}


/** Este m&eacute;todo no hace nada.
*/
void QTableWidget2::editItem(QTableWidgetItem *) {
    _depura("QTableWidget2::editItem", 0);
    _depura("END QTableWidget2::editItem", 0);
}


void QTableWidget2::sitemChanged(QTableWidgetItem *) {
    _depura("QTableWidget2::sitemChanged", 0);
    _depura("END QTableWidget2::sitemChanged", 0);
}


QTableWidgetItem2::QTableWidgetItem2(int type, int mode) : QTableWidgetItem(type) {
    modo = mode;
}


QTableWidgetItem2::QTableWidgetItem2(const QString &text, int type, int mode) : QTableWidgetItem(text, type) {
    modo = mode;
}


QTableWidgetItem2::~QTableWidgetItem2() {
    _depura("~QTableWidgetItem2", 1);
}


int QTableWidget2::tipoorden() {
    return m_tipoorden;
}


int QTableWidget2::colorden() {
    return m_colorden;
}


void QTableWidget2::settipoorden(int t) {
    m_tipoorden = t;
}


void QTableWidget2::setcolorden(int t) {
    m_colorden = t;
}


void QTableWidget2::columnMoved(int column, int oldIndex, int newIndex) {
    QTableWidget::columnMoved(column, oldIndex, newIndex);
}


QTableWidget2::~QTableWidget2() {
    _depura("END ~QTableWidget2", 0);
}

/// Habilita o inhabilita el ordenado de columnas mediante el pulsar sobre ellas.
void QTableWidget2::setSortingEnabled(bool sorting) {
    _depura("QTableWidget2::setSortingEnabled", 0);
    QTableWidget::setSortingEnabled(sorting);
    _depura("END QTableWidget2::setSortingEnabled", 0);
}



/** EventFilter para QTableWidget2, procesa los eventos recibidos por la
tabla y emite signals si lo considera adecuado.
*/
bool QTableWidget2::eventFilter(QObject *obj, QEvent *event) {
    _depura("QTableWidget2::eventFilter() :" + QString::number(event->type()), 1);
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int key = keyEvent->key();
        int row = currentRow();
        /// Algunas veces se produce un eventfilter pero la fila no existe (-1) en esos
        /// casos abortamos la ejecucion del eventFilter para que no de fallos en la
        /// busqueda de que celda es.
        if (row < 0) {
            return TRUE;
        } // end if
        Qt::KeyboardModifiers mod = keyEvent->modifiers();
        switch (key) {
        case Qt::Key_Return:
        case Qt::Key_Enter:
            m_teclasalida = key;
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
        m_teclasalida = 0;
        /// Algunas veces se produce un eventfilter pero la fila no existe (-1) en esos
        /// casos abortamos la ejecucion del eventFilter para que no de fallos en la
        /// busqueda de que celda es.
        if (row < 0) {
            return TRUE;
        } // end if
        Qt::KeyboardModifiers mod = keyEvent->modifiers();
        switch (key) {
        case Qt::Key_Return:
        case Qt::Key_Enter:
            m_teclasalida = key;
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
    _depura("END QTableWidget2::eventFilter()", 0);
    return QTableWidget::eventFilter(obj, event);
}


/** Se ha pulsado sobre las cabeceras en la tabla lo que invoca el
metodo de ordenacion.
*/
void QTableWidget2::ordenar() {
    _depura("QTableWidget2::ordenar ", 0, QString::number(m_colorden));
    /// Puede ocurrir que el parametro de ordenacion sea invalido por cualquier extranyo motivo.
    if (m_colorden < columnCount()) {
        sortByColumn(m_colorden);
    } // end if
    _depura("END QTableWidget2::ordenar", 0, QString::number(m_colorden));
}


/** Hace una asignacion de un elemento.
    @BUG: Parece que esta creando elementos de memoria sin intentar eliminar los antiguos.
*/
void QTableWidget2::setText(int x, int y, const QString & val) {
    _depura("QTableWidget::setText", 0);
    QTableWidgetItem2 *newitem = new QTableWidgetItem2(val);
    setItem(x, y, newitem);
    _depura("END QTableWidget::setText", 0);
}

void QTableWidget2::sortByColumn(int column, Qt::SortOrder order) {
    _depura("QTableWidget2::sortByColumn", 0);
    QTableWidget::sortByColumn(column, order);
    _depura("END QTableWidget2::sortByColumn", 0);
}

void QTableWidget2::sortItems(int column, Qt::SortOrder order) {
    _depura("QTableWidget2::sortItems", 0);
    QTableWidget::sortItems(column, order);
    _depura("END QTableWidget2::sortItems", 0);
}

