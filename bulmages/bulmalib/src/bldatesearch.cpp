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

#include <QCalendarWidget>

#include "bldatesearch.h"


///  Inicializa el objeto y hace todas las conexiones necesarias.
/**
\param parent
**/
BlDateSearch::BlDateSearch ( QWidget *parent ) : BlWidget ( parent )
{
    blDebug ( "BlDateSearch::BlDateSearch", 0 );
    setupUi ( this );
    QObject::connect ( m_busquedaFecha, SIGNAL ( clicked ( bool ) ), this, SLOT ( s_searchFecha() ) );
   
    QObject::connect ( m_textoFecha, SIGNAL ( returnPressed() ), this, SLOT ( s_returnPressed() ) );
    QObject::connect ( m_textoFecha, SIGNAL ( editingFinished() ), this, SLOT ( s_fechalostFocus() ) );
    QObject::connect ( m_textoFecha, SIGNAL ( editingFinished() ), this, SIGNAL ( editingFinished() ) );

    /// Establecemos la delegacion del foco en el texto
    setFocusProxy(m_textoFecha);
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


/// No requiere de acciones especiales en el destructor.
/**
**/
BlDateSearch::~BlDateSearch()
{
    blDebug ( "BlDateSearch::~BlDateSearch", 0 );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\param val
**/
void BlDateSearch::setDate ( QString val )
{
    blDebug ( "BlDateSearch::setDate", 0 );
    m_textoFecha->setText ( val.left(10) );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}

///
/**
\param val
**/
void BlDateSearch::setText ( QString val )
{
    blDebug ( "BlDateSearch::setText", 0 );
    s_fechalostFocus();
    m_textoFecha->setText ( val.left(10) );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\param val
**/
void BlDateSearch::setFieldValue ( QString val )
{
    blDebug ( "BlDateSearch::setFieldValue", 0 );
    m_textoFecha->setText ( val );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\return
**/
QString BlDateSearch::fecha()
{
    blDebug ( "BlDateSearch::fecha", 0 );
    s_fechalostFocus();
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
    return m_textoFecha->text();
}


///
/**
\return
**/
QString BlDateSearch::text()
{
    blDebug ( "BlDateSearch::text", 0 );
    s_fechalostFocus();
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
    return m_textoFecha->text();
}


///
/**
\return
**/
QString BlDateSearch::fieldValue()
{
    blDebug ( "BlDateSearch::fieldValue", 0 );
    s_fechalostFocus();
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
    return m_textoFecha->text();
}


///
/**
**/
void BlDateSearch::s_returnPressed()
{
    blDebug ( "BlDateSearch::s_enterPressed", 0 );
    s_fechalostFocus();
    emit returnPressed();
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
**/
void BlDateSearch::selectAll()
{
    blDebug ( "BlDateSearch::selectAll", 0 );
    m_textoFecha->selectAll();
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}





///
/**
**/
void BlDateSearch::s_searchFecha()
{
    blDebug ( "BlDateSearch::s_searchFecha", 0 );

    QDialog *diag = new QDialog ( 0 );
    diag->setModal ( true );
    QCalendarWidget *calend = new QCalendarWidget ( diag );
    /// Se pone el 1er dia del calendario a lunes.
    calend->setFirstDayOfWeek ( Qt::Monday );

    /// Evitar fechas demasiado antiguas
    calend->setMinimumDate( QDate ( 1900, 1, 1 ) );

    /// Si el campo estaba vac&iacute;o, seleccionar una fecha imposible, pero mostrar el mes actual
    if ( m_textoFecha->text().isEmpty() ) {
        calend->setSelectedDate ( calend->minimumDate() );
        calend->setCurrentPage ( QDate::currentDate().year(), QDate::currentDate().month() );
    }

    /// Si ya hay una fecha en el campo, abrir el calendario con ese d&iacute;a seleccionado inicialmente
    else {
        calend->setSelectedDate ( blNormalizeDate ( m_textoFecha->text() ) );
    }

    connect ( calend, SIGNAL ( activated ( const QDate & ) ), diag, SLOT ( accept() ) );

    /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
    /// para que sea redimensionable y aparezca el titulo de la ventana.
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget ( calend );
    layout->setMargin ( 0 );
    layout->setSpacing ( 0 );
    diag->setLayout ( layout );
    diag->setWindowTitle ( _ ( "Seleccione fecha" ) );
    diag->exec();

    /// Si la fecha es imposible, significa que el usuario no ha seleccionado una fecha
    /// y su campo debe quedarse como estaba: vac&iacute;o
    if ( calend->selectedDate() != QDate ( 1900, 1, 1 ) ) {
        m_textoFecha->setText ( calend->selectedDate().toString ( "dd/MM/yyyy" ) );
    }

    delete calend;
    delete diag;

    emit ( valueChanged ( m_textoFecha->text() ) );
    emit ( editingFinished () );

    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\param texto
\return
**/
void BlDateSearch::s_fechatextChanged ( const QString &texto )
{
    blDebug ( "BlDateSearch::s_fechatextChanged", 0 );
    if ( texto == "+" )
        s_searchFecha();
    if ( texto == "*" )
        m_textoFecha->setText ( QDate::currentDate().toString ( "dd/MM/yyyy" ) );
        m_textoFecha->setText ( blNormalizeDate ( texto ).toString ( "dd/MM/yyyy" ) );
    if ( texto == "" ) {
        m_textoFecha->setText ( "" );
        return;
    } // end if
    emit ( valueChanged ( m_textoFecha->text() ) );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
**/
void BlDateSearch::s_fechalostFocus()
{
    blDebug ( "BlDateSearch::s_fechalostFocus", 0 );
    QString fech = m_textoFecha->text();
    if ( fech != "" )
        s_fechatextChanged ( fech );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\return
**/
QDate BlDateSearch::date()
{
    blDebug ( "BlDateSearch::fecha", 0 );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
    return QDate::fromString(m_textoFecha->text(), "dd/MM/yyyy");
}
