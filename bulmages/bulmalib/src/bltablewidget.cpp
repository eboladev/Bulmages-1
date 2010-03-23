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
#include <QBrush>
#include <QColor>

#include "bltablewidget.h"
#include "blconfiguration.h"


/** Constructor de BlTableWidget clase derivada de QTableWidget con
un eventHandler especifico
*/
/**
\param parent
**/
BlTableWidget::BlTableWidget ( QWidget *parent ) : QTableWidget ( parent )
{
    _depura ( "BlTableWidget::BlTableWidget", 0 );
    setcolorden ( -1 );
    settipoorden ( -1 );
    installEventFilter ( this );
    connect ( this, SIGNAL ( itemChanged ( QTableWidgetItem * ) ), this, SLOT ( sitemChanged ( QTableWidgetItem * ) ) );
    connect ( this, SIGNAL ( currentItemChanged ( QTableWidgetItem *, QTableWidgetItem * ) ), this, SLOT ( celdaChanged ( QTableWidgetItem *, QTableWidgetItem * ) ) );
    _depura ( "END BlTableWidget::BlTableWidget", 0 );
}


void BlTableWidget::moveRow ( int oldRow, int newRow )
{

    int column;
    QList<QTableWidgetItem *> rowItemsOld;

    /// Captura los items de las fila
    for ( column = 0; column < columnCount(); ++column ) {
        rowItemsOld << takeItem ( oldRow, column );
    } // end for

    removeRow ( oldRow );
    insertRow ( newRow );

    /// Aqui habria que averiguar si esta activado la ordenacion
    /// automatica para deshabilitarla antes y habilitarla despues.

    /// Establece los items de la fila
    for ( column = 0; column < columnCount(); ++column ) {
        setItem ( newRow, column, rowItemsOld.at ( column ) );
    } // end for

}


/** Este m&eacute;todo no hace nada.
*/
/**
**/
void BlTableWidget::editItem ( QTableWidgetItem * )
{
    _depura ( "BlTableWidget::editItem", 0 );
    _depura ( "END BlTableWidget::editItem", 0 );
}


///
/**
**/
void BlTableWidget::sitemChanged ( QTableWidgetItem * )
{
    _depura ( "BlTableWidget::sitemChanged", 0 );
    _depura ( "END BlTableWidget::sitemChanged", 0 );
}


///
/**
\param type
\param mode
**/
BlTableWidgetItem::BlTableWidgetItem ( int type, int mode ) : QTableWidgetItem ( type )
{
    _depura ( "BlTableWidgetItem::BlTableWidgetItem", 0 );
    modo = mode;
    _depura ( "END BlTableWidgetItem::BlTableWidgetItem", 0 );
}

///
/**
\param text
\param type
\param mode
**/
BlTableWidgetItem::BlTableWidgetItem ( const QString &text, int type, int mode ) : QTableWidgetItem ( text, type )
{
    _depura ( "BlTableWidgetItem::BlTableWidgetItem", 0 );
    modo = mode;
    _depura ( "END BlTableWidgetItem::BlTableWidgetItem", 0 );
}


///
/**
**/
BlTableWidgetItem::~BlTableWidgetItem()
{
    _depura ( "BlTableWidgetItem::~BlTableWidgetItem", 0 );
    _depura ( "END BlTableWidgetItem::~BlTableWidgetItem", 0 );
}


///
/**
\param other
\return
**/
bool BlTableWidgetItem::operator< ( const QTableWidgetItem & other ) const
{
    _depura ( "BlTableWidgetItem::operator<", 0, text() + " < " + other.text() );
    bool oknumero;
    bool oknumero1;
    QString cad = text();
    QString cad1 = other.text();

    if ( cad != "" && cad1 != "" ) {
        /// Comprobamos si es un n&uacute;mero.
        double ncad = cad.toDouble ( &oknumero );
        double ncad1 = cad1.toDouble ( &oknumero1 );
        if ( oknumero && oknumero1 ) {
            _depura ( "END BlTableWidgetItem::operator<", 0 );
            return ncad < ncad1;
        } // end if
        /// Comprobamos si es una fecha
        if ( cad[2] == '/' && cad[5] == '/' ) {
            QDate fcad = normalizafecha ( cad );
            QString acad = fcad.toString ( Qt::ISODate );
            QDate fcad1 = normalizafecha ( cad1 );
            QString acad1 = fcad1.toString ( Qt::ISODate );
            if ( acad[4] == '-' && acad1[4] == '-' && acad[7] == '-' && acad1[7] == '-' ) {
                _depura ( "END BlTableWidgetItem::operator<", 0 );
                return fcad < fcad1;
            } // end if
        } // end if
    } // end if
    _depura ( "END BlTableWidgetItem::operator<", 0 );
    return cad < cad1;
}


///
/**
\return
**/
int BlTableWidget::tipoorden()
{
    _depura ( "BlTableWidget::tipoorden", 0 );
    _depura ( "END BlTableWidget::tipoorden", 0 );
    return m_tipoorden;
}


///
/**
\return
**/
int BlTableWidget::colorden()
{
    _depura ( "BlTableWidget::colorden", 0 );
    _depura ( "END BlTableWidget::colorden", 0 );
    return m_colorden;
}


///
/**
\param t
**/
void BlTableWidget::settipoorden ( int t )
{
    _depura ( "BlTableWidget::settipoorden", 0 );
    m_tipoorden = t;
    _depura ( "END BlTableWidget::settipoorden", 0 );
}


///
/**
\param t
**/
void BlTableWidget::setcolorden ( int t )
{
    _depura ( "BlTableWidget::setcolorden", 0 );
    m_colorden = t;
    _depura ( "END BlTableWidget::setcolorden", 0 );
}

///
/**
\param column
\param oldIndex
\param newIndex
**/
void BlTableWidget::columnMoved ( int column, int oldIndex, int newIndex )
{
    _depura ( "BlTableWidget::columnMoved", 0 );
    QTableWidget::columnMoved ( column, oldIndex, newIndex );
    _depura ( "END BlTableWidget::columnMoved", 0 );
}


///
/**
**/
BlTableWidget::~BlTableWidget()
{
    _depura ( "BlTableWidget::~BlTableWidget", 0 );
    _depura ( "END BlTableWidget::~BlTableWidget", 0 );
}

/// Habilita o inhabilita el ordenado de columnas mediante el pulsar sobre ellas.
/**
\param sorting
**/
void BlTableWidget::setSortingEnabled ( bool sorting )
{
    _depura ( "BlTableWidget::setSortingEnabled", 0 );
    QTableWidget::setSortingEnabled ( sorting );
    _depura ( "END BlTableWidget::setSortingEnabled", 0 );
}


/** EventFilter para BlTableWidget, procesa los eventos recibidos por la
tabla y emite signals si lo considera adecuado.
*/
/**
\param obj
\param event
\return
**/
bool BlTableWidget::eventFilter ( QObject *obj, QEvent *event )
{
    if ( event->type() == QEvent::KeyPress ) {
//        _depura ( "BlTableWidget::eventFilter() :" + QString::number ( event->type() ), 1 );
        QKeyEvent *keyEvent = static_cast<QKeyEvent *> ( event );
        int key = keyEvent->key();
        int row = currentRow();
        /// Algunas veces se produce un eventfilter pero la fila no existe (-1) en esos
        /// casos abortamos la ejecucion del eventFilter para que no de fallos en la
        /// busqueda de que celda es.
        if ( row < 0 ) {
            return TRUE;
        } // end if
        Qt::KeyboardModifiers mod = keyEvent->modifiers();
        switch ( key ) {
        case Qt::Key_Return:
        case Qt::Key_Enter:
        case Qt::Key_Tab:
            m_teclasalida = key;
            return TRUE;
        } // end switch
    } // end if

    /// Si es un release de tecla se hace la funcionalidad especificada.
    if ( event->type() == QEvent::KeyRelease ) {
//        _depura ( "BlTableWidget::eventFilter() :" + QString::number ( event->type() ), 1 );

        QKeyEvent *keyEvent = static_cast<QKeyEvent *> ( event );
        int key = keyEvent->key();
        int col = currentColumn();
        int row = currentRow();

        m_teclasalida = 0;
        /// Algunas veces se produce un eventfilter pero la fila no existe (-1) en esos
        /// casos abortamos la ejecucion del eventFilter para que no de fallos en la
        /// busqueda de que celda es.
        if ( row < 0 ) {
            return TRUE;
        } // end if
        Qt::KeyboardModifiers mod = keyEvent->modifiers();
        switch ( key ) {
        case Qt::Key_Return:
        case Qt::Key_Enter:
        case Qt::Key_Tab:
            if ( ! ( mod & Qt::ShiftModifier ) ) {
                m_teclasalida = key;
                emit cellRePosition ( row, col );
                return TRUE;
                break;
            }// end if
        case Qt::Key_Slash:
            if ( ( mod & Qt::ControlModifier ) || ( mod & Qt::AltModifier ) ) {
                emit pressedSlash ( row, col );
                return TRUE;
            } // end if
            break;
        case Qt::Key_Minus:
            if ( ( mod & Qt::ControlModifier ) || ( mod & Qt::AltModifier ) ) {
                emit pressedMinus ( row, col );
                return TRUE;
            } // end if
            break;
        case Qt::Key_Plus:
            if ( ( mod & Qt::ControlModifier ) || ( mod & Qt::AltModifier ) ) {
                emit pressedPlus ( row, col );
                return TRUE;
            } // end if
            break;
        case Qt::Key_Asterisk:
            if ( ( mod & Qt::ControlModifier ) || ( mod & Qt::AltModifier ) ) {
                emit pressedAsterisk ( row, col );
                return TRUE;
            } // end if
            break;
        case Qt::Key_F2:
            emit pressedAsterisk ( row, col );
            return TRUE;
            break;
        case Qt::Key_Up:
            if ( ( mod & Qt::ControlModifier ) || ( mod & Qt::AltModifier ) ) {
                emit ctrlSubir ( row, col );
                return TRUE;
            } // end if
            /// Al pulsar la tecla 'arriba' se considera que es el fin de la edici&oacute;n de la celda.
        case Qt::Key_Down:
            if ( ( mod & Qt::ControlModifier ) || ( mod & Qt::AltModifier ) ) {
                emit ctrlBajar ( row, col );
                return TRUE;
            } // end if
            /// Al pulsar la tecla 'abajo' se considera que es el fin de la edici&oacute; de la celda.
            return TRUE;
        case Qt::Key_Left:
            if ( ( mod & Qt::ControlModifier ) || ( mod & Qt::AltModifier ) ) {
                emit ctrlIzquierda ( row, col );
                return TRUE;
            } // end if
        case Qt::Key_Right:
            if ( ( mod & Qt::ControlModifier ) || ( mod & Qt::AltModifier ) ) {
                emit ctrlDerecha ( row, col );
                return TRUE;
            } // end if
        } // end switch
    } // end if
//    _depura("END BlTableWidget::eventFilter()", 0);
    return QTableWidget::eventFilter ( obj, event );
//      return TRUE;
}


/** Se ha pulsado sobre las cabeceras en la tabla lo que invoca el
metodo de ordenacion.
*/
/**
**/
void BlTableWidget::ordenar()
{
    _depura ( "BlTableWidget::ordenar", 0, QString::number ( m_colorden ) );
    /// Puede ocurrir que el parametro de ordenacion sea invalido por cualquier extranyo motivo.
    if ( m_colorden < columnCount() && m_colorden >= 0 ) {
        sortByColumn ( m_colorden );
    } // end if
    _depura ( "END BlTableWidget::ordenar", 0, QString::number ( m_colorden ) );
}


/** Hace una asignacion de un elemento.
    @BUG: Parece que esta creando elementos de memoria sin intentar eliminar los antiguos.
*/
/**
\param x
\param y
\param val
**/
void BlTableWidget::setText ( int x, int y, const QString & val )
{
    _depura ( "QTableWidget::setText", 0 );
    BlTableWidgetItem *newitem = new BlTableWidgetItem ( val );
    setItem ( x, y, newitem );
    _depura ( "END QTableWidget::setText", 0 );
}


///
/**
\param column
\param order
**/
void BlTableWidget::sortByColumn ( int column, Qt::SortOrder order )
{
    _depura ( "BlTableWidget::sortByColumn", 0 );
    QTableWidget::sortByColumn ( column, order );
    _depura ( "END BlTableWidget::sortByColumn", 0 );
}


///
/**
\param column
\param order
**/
void BlTableWidget::sortItems ( int column, Qt::SortOrder order )
{
    _depura ( "BlTableWidget::sortItems", 0 );
    QTableWidget::sortItems ( column, order );
    _depura ( "END BlTableWidget::sortItems", 0 );
}


void BlTableWidget::showColumn ( int column )
{
    _depura ( "BlTableWidget::showColumn", 0 );
    QTableWidget::showColumn ( column );
    _depura ( "END BlTableWidget::showColumn", 0 );
}


void BlTableWidget::hideColumn ( int column )
{
    _depura ( "BlTableWidget::hideColumn", 0 );
    QTableWidget::hideColumn ( column );
    _depura ( "END BlTableWidget::hideColumn", 0 );
}
