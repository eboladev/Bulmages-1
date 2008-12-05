/***************************************************************************
 *   Copyright (C) 2008 by Tomeu Borras Riera                              *
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

#include "blbancoedit.h"
#include "funcaux.h"


///  Inicializa el objeto y hace todas las conexiones necesarias.
/**
\param parent
**/
BLBancoEdit::BLBancoEdit ( QWidget *parent ) : BLWidget ( parent )
{
    _depura ( "BLBancoEdit::BLBancoEdit", 0 );
    setupUi ( this );
    QObject::connect ( m_cuenta, SIGNAL ( returnPressed() ), this, SLOT ( s_returnPressed() ) );
    QObject::connect ( m_cuenta, SIGNAL ( editingFinished() ), this, SLOT ( s_cuentalostFocus() ) );
    QObject::connect ( m_cuenta, SIGNAL ( editingFinished() ), this, SIGNAL ( editingFinished() ) );
    _depura ( "END BLBancoEdit::BLBancoEdit", 0 );
}


/// No requiere de acciones especiales en el destructor.
/**
**/
BLBancoEdit::~BLBancoEdit()
{
    _depura ( "BLBancoEdit::~BLBancoEdit", 0 );
    _depura ( "END BLBancoEdit::~BLBancoEdit", 0 );
}


///
/**
\param val
**/
void BLBancoEdit::setText ( QString val )
{
    _depura ( "BLBancoEdit::setText", 0 );
    s_cuentalostFocus();
    m_entidad->setText(val.left(4));
    m_oficina->setText(val.left(8).right(4));
    m_dc->setText(val.left(10).right(2));
    m_cuenta->setText ( val.left(20).right(10) );
    _depura ( "END BLBancoEdit::setText", 0 );
}


///
/**
\param val
**/
void BLBancoEdit::setValorCampo ( QString val )
{
    _depura ( "BLBancoEdit::setValorCampo", 0 );
    setText ( val );
    _depura ( "END BLBancoEdit::setValorCampo", 0 );
}




///
/**
\return
**/
QString BLBancoEdit::text()
{
    _depura ( "BLBancoEdit::text", 0 );
    _depura ( "END BLBancoEdit::text", 0 );
    s_cuentalostFocus();
    QString val = m_entidad->text() + m_oficina->text() + m_dc->text() + m_cuenta->text();
    return val;
}


///
/**
\return
**/
QString BLBancoEdit::valorCampo()
{
    _depura ( "BLBancoEdit::valorCampo", 0 );
    _depura ( "END BLBancoEdit::valorCampo", 0 );
    return text();
}


///
/**
**/
void BLBancoEdit::s_returnPressed()
{
    _depura ( "BLBancoEdit::s_enterPressed", 0 );
    s_cuentalostFocus();
    emit returnPressed();
    _depura ( "END BLBancoEdit::s_enterPressed", 0 );
}


///
/**
**/
void BLBancoEdit::selectAll()
{
    _depura ( "BLBancoEdit::selectAll", 0 );
    m_cuenta->selectAll();
    _depura ( "END BLBancoEdit::selectAll", 0 );
}


///
/**
**/
void BLBancoEdit::setFocus()
{
    _depura ( "BLBancoEdit::setFocus", 0 );
    m_cuenta->setFocus ( Qt::OtherFocusReason );
    _depura ( "END BLBancoEdit::setFocus", 0 );
}


///
/**
\param texto
\return
**/
void BLBancoEdit::s_cuentatextChanged ( const QString &texto )
{
    _depura ( "BLBancoEdit::s_fechatextChanged", 0 );
/*
    if ( texto == "+" )
        s_searchFecha();
    if ( texto == "*" )
        m_cuenta->setText ( QDate::currentDate().toString ( "dd/MM/yyyy" ) );
    m_cuenta->setText ( normalizafecha ( texto ).toString ( "dd/MM/yyyy" ) );
    if ( texto == "" ) {
        m_cuenta->setText ( "" );
        return;
    } // end if
*/
    emit ( valueChanged ( m_entidad->text() + m_oficina->text() + m_dc->text() + m_cuenta->text() ) );
    _depura ( "END BLBancoEdit::s_fechatextChanged", 0 );
}



///
/**
**/
void BLBancoEdit::s_cuentalostFocus()
{
    _depura ( "BLBancoEdit::s_cuentalostFocus", 0 );
    QString fech = m_entidad->text() + m_oficina->text() + m_dc->text() + m_cuenta->text();
    if ( fech != "" )
        s_cuentatextChanged ( fech );
    _depura ( "END BLBancoEdit::s_cuentalostFocus", 0 );
}

