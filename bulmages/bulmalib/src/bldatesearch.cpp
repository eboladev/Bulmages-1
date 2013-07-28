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

#include <QtWidgets/QCalendarWidget>

#include "bldatesearch.h"


///  Inicializa el objeto y hace todas las conexiones necesarias.
/**
\param parent
**/
BlDateSearch::BlDateSearch ( QWidget *parent ) : BlWidget ( parent )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    QObject::connect ( m_busquedaFecha, SIGNAL ( clicked ( bool ) ), this, SLOT ( s_searchFecha() ) );
    QObject::connect ( m_textoFecha, SIGNAL ( returnPressed() ), this, SLOT ( s_returnPressed() ) );
    QObject::connect ( m_textoFecha, SIGNAL ( editingFinished() ), this, SLOT ( s_fechalostFocus() ) );
    QObject::connect ( m_textoFecha, SIGNAL ( editingFinished() ), this, SIGNAL ( editingFinished() ) );

    /// Establecemos la delegacion del foco en el texto
    setFocusProxy(m_textoFecha);
}


/// No requiere de acciones especiales en el destructor.
/**
**/
BlDateSearch::~BlDateSearch()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param val
**/
void BlDateSearch::setDate ( QString val )
{
    BL_FUNC_DEBUG
    m_textoFecha->setText ( val.left(10) );
    
}

///
/**
\param val
**/
void BlDateSearch::setText ( QString val )
{
    BL_FUNC_DEBUG
    s_fechalostFocus();
    m_textoFecha->setText ( val.left(10) );
    
}


///
/**
\param val
**/
void BlDateSearch::setFieldValue ( QString val )
{
    BL_FUNC_DEBUG
    m_textoFecha->setText ( val );
    
}


///
/**
\return
**/
QString BlDateSearch::fecha()
{
    BL_FUNC_DEBUG
    s_fechalostFocus();
    
    return m_textoFecha->text();
}


///
/**
\return
**/
QString BlDateSearch::text()
{
    BL_FUNC_DEBUG
    s_fechalostFocus();
    
    return m_textoFecha->text();
}


///
/**
\return
**/
QString BlDateSearch::fieldValue()
{
    BL_FUNC_DEBUG
    s_fechalostFocus();
    
    return m_textoFecha->text();
}


///
/**
**/
void BlDateSearch::s_returnPressed()
{
    BL_FUNC_DEBUG
    s_fechalostFocus();
    emit returnPressed();
    
}


///
/**
**/
void BlDateSearch::selectAll()
{
    BL_FUNC_DEBUG
    m_textoFecha->selectAll();
    
}





///
/**
**/
void BlDateSearch::s_searchFecha()
{
    BL_FUNC_DEBUG

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

    /// Liberamos la memoria
    delete layout;
    delete calend;
    delete diag;

    emit ( valueChanged ( m_textoFecha->text() ) );
    emit ( editingFinished () );

    
}


///
/**
\param texto
\return
**/
void BlDateSearch::s_fechatextChanged ( const QString &texto )
{
    BL_FUNC_DEBUG
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
    
}


///
/**
**/
void BlDateSearch::s_fechalostFocus()
{
    BL_FUNC_DEBUG
    QString fech = m_textoFecha->text();
    if ( fech != "" )
        s_fechatextChanged ( fech );
    
}


///
/**
\return
**/
QDate BlDateSearch::date()
{
    BL_FUNC_DEBUG
    
    return QDate::fromString(m_textoFecha->text(), "dd/MM/yyyy");
}
