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

#include "busquedafecha.h"


///  Inicializa el objeto y hace todas las conexiones necesarias.
/**
\param parent
**/
BusquedaFecha::BusquedaFecha ( QWidget *parent ) : BlWidget ( parent )
{
    _depura ( "BusquedaFecha::BusquedaFecha", 0 );
    setupUi ( this );
    QObject::connect ( mui_busquedaFecha, SIGNAL ( clicked ( bool ) ), this, SLOT ( s_searchFecha() ) );
    QObject::connect ( mui_textoFecha, SIGNAL ( returnPressed() ), this, SLOT ( s_returnPressed() ) );
    QObject::connect ( mui_textoFecha, SIGNAL ( editingFinished() ), this, SLOT ( s_fechalostFocus() ) );
    QObject::connect ( mui_textoFecha, SIGNAL ( editingFinished() ), this, SIGNAL ( editingFinished() ) );
    _depura ( "END BusquedaFecha::BusquedaFecha", 0 );
}


/// No requiere de acciones especiales en el destructor.
/**
**/
BusquedaFecha::~BusquedaFecha()
{
    _depura ( "BusquedaFecha::~BusquedaFecha", 0 );
    _depura ( "END BusquedaFecha::~BusquedaFecha", 0 );
}


///
/**
\param val
**/
void BusquedaFecha::setfecha ( QString val )
{
    _depura ( "BusquedaFecha::setfecha", 0 );
    mui_textoFecha->setText ( val );
    _depura ( "END BusquedaFecha::setfecha", 0 );
}

///
/**
\param val
**/
void BusquedaFecha::setText ( QString val )
{
    _depura ( "BusquedaFecha::setText", 0 );
    s_fechalostFocus();
    mui_textoFecha->setText ( val );
    _depura ( "END BusquedaFecha::setText", 0 );
}


///
/**
\param val
**/
void BusquedaFecha::setValorCampo ( QString val )
{
    _depura ( "BusquedaFecha::setValorCampo", 0 );
    mui_textoFecha->setText ( val );
    _depura ( "END BusquedaFecha::setValorCampo", 0 );
}


///
/**
\return
**/
QString BusquedaFecha::fecha()
{
    _depura ( "BusquedaFecha::fecha", 0 );
    _depura ( "END BusquedaFecha::fecha", 0 );
    s_fechalostFocus();
    return mui_textoFecha->text();
}


///
/**
\return
**/
QString BusquedaFecha::text()
{
    _depura ( "BusquedaFecha::text", 0 );
    _depura ( "END BusquedaFecha::text", 0 );
    s_fechalostFocus();
    return mui_textoFecha->text();
}


///
/**
\return
**/
QString BusquedaFecha::valorCampo()
{
    _depura ( "BusquedaFecha::valorCampo", 0 );
    s_fechalostFocus();
    _depura ( "END BusquedaFecha::valorCampo", 0 );
    return mui_textoFecha->text();
}


///
/**
**/
void BusquedaFecha::s_returnPressed()
{
    _depura ( "BusquedaFecha::s_enterPressed", 0 );
    s_fechalostFocus();
    emit returnPressed();
    _depura ( "END BusquedaFecha::s_enterPressed", 0 );
}


///
/**
**/
void BusquedaFecha::selectAll()
{
    _depura ( "BusquedaFecha::selectAll", 0 );
    mui_textoFecha->selectAll();
    _depura ( "END BusquedaFecha::selectAll", 0 );
}


///
/**
**/
void BusquedaFecha::setFocus()
{
    _depura ( "BusquedaFecha::setFocus", 0 );
    mui_textoFecha->setFocus ( Qt::OtherFocusReason );
    _depura ( "END BusquedaFecha::setFocus", 0 );
}


///
/**
**/
void BusquedaFecha::s_searchFecha()
{
    _depura ( "BusquedaFecha::s_searchFecha", 0 );

    QDialog *diag = new QDialog ( 0 );
    diag->setModal ( true );
    QCalendarWidget *calend = new QCalendarWidget ( diag );
    /// Se pone el 1er dia del calendario a lunes.
    calend->setFirstDayOfWeek ( Qt::Monday );
    connect ( calend, SIGNAL ( activated ( const QDate & ) ), diag, SLOT ( accept() ) );

    /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
    /// para que sea redimensionable y aparezca el titulo de la ventana.
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget ( calend );
    layout->setMargin ( 0 );
    layout->setSpacing ( 0 );
    diag->setLayout ( layout );
    diag->setWindowTitle ( _( "Seleccione fecha" ) );
    diag->exec();

    mui_textoFecha->setText ( calend->selectedDate().toString ( "dd/MM/yyyy" ) );

    delete calend;
    delete diag;

    emit ( valueChanged ( mui_textoFecha->text() ) );
    _depura ( "END BusquedaFecha::s_searchFecha", 0 );
}


///
/**
\param texto
\return
**/
void BusquedaFecha::s_fechatextChanged ( const QString &texto )
{
    _depura ( "BusquedaFecha::s_fechatextChanged", 0 );
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
    _depura ( "END BusquedaFecha::s_fechatextChanged", 0 );
}


///
/**
**/
void BusquedaFecha::s_fechalostFocus()
{
    _depura ( "BusquedaFecha::s_fechalostFocus", 0 );
    QString fech = mui_textoFecha->text();
    if ( fech != "" )
        s_fechatextChanged ( fech );
    _depura ( "END BusquedaFecha::s_fechalostFocus", 0 );
}

