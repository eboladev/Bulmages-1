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
    _depura ( "BlDateSearch::BlDateSearch", 0 );
    setupUi ( this );
    QObject::connect ( mui_busquedaFecha, SIGNAL ( clicked ( bool ) ), this, SLOT ( s_searchFecha() ) );
    QObject::connect ( mui_textoFecha, SIGNAL ( returnPressed() ), this, SLOT ( s_returnPressed() ) );
    QObject::connect ( mui_textoFecha, SIGNAL ( editingFinished() ), this, SLOT ( s_fechalostFocus() ) );
    QObject::connect ( mui_textoFecha, SIGNAL ( editingFinished() ), this, SIGNAL ( editingFinished() ) );
    /// Establecemos la delegacion del foco en el texto
    setFocusProxy(mui_textoFecha);
    _depura ( "END BlDateSearch::BlDateSearch", 0 );
}


/// No requiere de acciones especiales en el destructor.
/**
**/
BlDateSearch::~BlDateSearch()
{
    _depura ( "BlDateSearch::~BlDateSearch", 0 );
    _depura ( "END BlDateSearch::~BlDateSearch", 0 );
}


///
/**
\param val
**/
void BlDateSearch::setfecha ( QString val )
{
    _depura ( "BlDateSearch::setfecha", 0 );
    mui_textoFecha->setText ( val );
    _depura ( "END BlDateSearch::setfecha", 0 );
}

///
/**
\param val
**/
void BlDateSearch::setText ( QString val )
{
    _depura ( "BlDateSearch::setText", 0 );
    s_fechalostFocus();
    mui_textoFecha->setText ( val );
    _depura ( "END BlDateSearch::setText", 0 );
}


///
/**
\param val
**/
void BlDateSearch::setFieldValue ( QString val )
{
    _depura ( "BlDateSearch::setFieldValue", 0 );
    mui_textoFecha->setText ( val );
    _depura ( "END BlDateSearch::setFieldValue", 0 );
}


///
/**
\return
**/
QString BlDateSearch::fecha()
{
    _depura ( "BlDateSearch::fecha", 0 );
    _depura ( "END BlDateSearch::fecha", 0 );
    s_fechalostFocus();
    return mui_textoFecha->text();
}


///
/**
\return
**/
QString BlDateSearch::text()
{
    _depura ( "BlDateSearch::text", 0 );
    _depura ( "END BlDateSearch::text", 0 );
    s_fechalostFocus();
    return mui_textoFecha->text();
}


///
/**
\return
**/
QString BlDateSearch::fieldValue()
{
    _depura ( "BlDateSearch::fieldValue", 0 );
    s_fechalostFocus();
    _depura ( "END BlDateSearch::fieldValue", 0 );
    return mui_textoFecha->text();
}


///
/**
**/
void BlDateSearch::s_returnPressed()
{
    _depura ( "BlDateSearch::s_enterPressed", 0 );
    s_fechalostFocus();
    emit returnPressed();
    _depura ( "END BlDateSearch::s_enterPressed", 0 );
}


///
/**
**/
void BlDateSearch::selectAll()
{
    _depura ( "BlDateSearch::selectAll", 0 );
    mui_textoFecha->selectAll();
    _depura ( "END BlDateSearch::selectAll", 0 );
}


///
/**
**/
void BlDateSearch::setFocus()
{
    _depura ( "BlDateSearch::setFocus", 0 );
    mui_textoFecha->setFocus ( Qt::OtherFocusReason );
    _depura ( "END BlDateSearch::setFocus", 0 );
}


///
/**
**/
void BlDateSearch::s_searchFecha()
{
    _depura ( "BlDateSearch::s_searchFecha", 0 );

    QDialog *diag = new QDialog ( 0 );
    diag->setModal ( true );
    QCalendarWidget *calend = new QCalendarWidget ( diag );
    /// Se pone el 1er dia del calendario a lunes.
    calend->setFirstDayOfWeek ( Qt::Monday );

    /// Si el campo estaba vac&iacute;o, seleccionar una fecha imposible, pero mostrar el mes actual
    if ( mui_textoFecha->text().isEmpty() ) {
        calend->setSelectedDate ( QDate ( 1900, 1, 1 ) );
        calend->setCurrentPage ( QDate::currentDate().year(), QDate::currentDate().month() );
    }

    /// Si ya hay una fecha en el campo, abrir el calendario con ese d&iacute;a seleccionado inicialmente
    else {
        calend->setSelectedDate ( normalizafecha ( mui_textoFecha->text() ) );
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
        mui_textoFecha->setText ( calend->selectedDate().toString ( "dd/MM/yyyy" ) );
    }

    delete calend;
    delete diag;

    emit ( valueChanged ( mui_textoFecha->text() ) );
    _depura ( "END BlDateSearch::s_searchFecha", 0 );
}


///
/**
\param texto
\return
**/
void BlDateSearch::s_fechatextChanged ( const QString &texto )
{
    _depura ( "BlDateSearch::s_fechatextChanged", 0 );
    if ( texto == "+" )
        s_searchFecha();
    if ( texto == "*" )
        mui_textoFecha->setText ( QDate::currentDate().toString ( "dd/MM/yyyy" ) );
        mui_textoFecha->setText ( normalizafecha ( texto ).toString ( "dd/MM/yyyy" ) );
    if ( texto == "" ) {
        mui_textoFecha->setText ( "" );
        return;
    } // end if
    emit ( valueChanged ( mui_textoFecha->text() ) );
    _depura ( "END BlDateSearch::s_fechatextChanged", 0 );
}


///
/**
**/
void BlDateSearch::s_fechalostFocus()
{
    _depura ( "BlDateSearch::s_fechalostFocus", 0 );
    QString fech = mui_textoFecha->text();
    if ( fech != "" )
        s_fechatextChanged ( fech );
    _depura ( "END BlDateSearch::s_fechalostFocus", 0 );
}


///
/**
\return
**/
QDate BlDateSearch::date()
{
    _depura ( "BlDateSearch::fecha", 0 );
    _depura ( "END BlDateSearch::fecha", 0 );
    return QDate::fromString(mui_textoFecha->text(), "dd/MM/yyyy");
}
