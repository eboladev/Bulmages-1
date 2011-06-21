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

#include "blbankedit.h"


int pesosdc[] = {6, 3, 7, 9, 10, 5, 8, 4, 2, 1};


///  Inicializa el objeto y hace todas las conexiones necesarias.
/**
\param parent
**/
BlBankEdit::BlBankEdit ( QWidget *parent ) : BlWidget ( parent )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    QObject::connect ( m_cuenta, SIGNAL ( returnPressed() ), this, SLOT ( s_returnPressed() ) );
    QObject::connect ( m_cuenta, SIGNAL ( editingFinished() ), this, SLOT ( s_cuentalostFocus() ) );
    QObject::connect ( m_cuenta, SIGNAL ( editingFinished() ), this, SIGNAL ( editingFinished() ) );
    
    /// Establecemos el FocusProxy para que el foco funcione bien
    setFocusProxy(m_entidad);
    
}


/// No requiere de acciones especiales en el destructor.
/**
**/
BlBankEdit::~BlBankEdit()
{
    BL_FUNC_DEBUG
    
}


/// Comprueba que los DC son correctos y si no lo son da un mensaje de aviso
void BlBankEdit::checkControlDigit()
{
    BL_FUNC_DEBUG
    QString cad1 = m_entidad->text() + m_oficina->text();
    QString cad2 = m_cuenta->text();
    int dc1 = 0;
    int dc2 = 0;

    /// Si no hay cuenta bancaria puesta entonces no hacemos comprobaciones.
    if ( cad1.size() + cad2.size() == 0 )
        return;


    int resparcial = 0;
    for ( int i = 0; i < 8; i++ ) {
        resparcial += cad1[8-i-1].digitValue() * pesosdc[i];
    } // end for
    dc1 = 11 - ( resparcial % 11 );
    if ( dc1 == 11 ) dc1 = 0;
    if ( dc1 == 10 ) dc1 = 1;

    int resparcial1 = 0;
    for ( int i = 0; i < 10; i++ ) {
        resparcial1 += cad2[10-i-1].digitValue() * pesosdc[i];
    } // end for
    dc2 = 11 - ( resparcial1 % 11 );
    if ( dc2 == 11 ) dc2 = 0;
    if ( dc2 == 10 ) dc2 = 1;

    QString dc = QString::number ( dc1 ) + QString::number ( dc2 );

    /// Si los digitos de control no se corresponden damos un error.
    if ( dc != m_dc->text() ) {
      blMsgInfo (_ ( "Cuenta bancaria incorrecta" ));
        throw - 1;
    } // end if
    
}


///
/**
\param val
**/
void BlBankEdit::setText ( QString val )
{
    BL_FUNC_DEBUG
    s_cuentalostFocus();
    m_entidad->setText ( val.left ( 4 ) );
    m_oficina->setText ( val.left ( 8 ).right ( 4 ) );
    m_dc->setText ( val.left ( 10 ).right ( 2 ) );
    m_cuenta->setText ( val.left ( 20 ).right ( 10 ) );
    
}


///
/**
\param val
**/
void BlBankEdit::setFieldValue ( QString val )
{
    BL_FUNC_DEBUG
    setText ( val );
    
}




///
/**
\return
**/
QString BlBankEdit::text()
{
    BL_FUNC_DEBUG
    
    s_cuentalostFocus();
    QString val = m_entidad->text() + m_oficina->text() + m_dc->text() + m_cuenta->text();
    checkControlDigit();
    return val;
}


///
/**
\return
**/
QString BlBankEdit::fieldValue()
{
    BL_FUNC_DEBUG
    
    return text();
}


///
/**
**/
void BlBankEdit::s_returnPressed()
{
    BL_FUNC_DEBUG
    s_cuentalostFocus();
    emit returnPressed();
    
}


///
/**
**/
void BlBankEdit::selectAll()
{
    BL_FUNC_DEBUG
    m_cuenta->selectAll();
    
}


///
/**
**/
void BlBankEdit::setFocus()
{
    BL_FUNC_DEBUG
    m_cuenta->setFocus ( Qt::OtherFocusReason );
    
}


///
/**
\param texto
\return
**/
void BlBankEdit::s_cuentatextChanged ( const QString &texto )
{
    BL_FUNC_DEBUG
    /*
        if ( texto == "+" )
            s_searchFecha();
        if ( texto == "*" )
            m_cuenta->setText ( QDate::currentDate().toString ( "dd/MM/yyyy" ) );
        m_cuenta->setText ( blNormalizeDate ( texto ).toString ( "dd/MM/yyyy" ) );
        if ( texto == "" ) {
            m_cuenta->setText ( "" );
            return;
        } // end if
    */
    emit ( valueChanged ( m_entidad->text() + m_oficina->text() + m_dc->text() + m_cuenta->text() ) );
    
}



///
/**
**/
void BlBankEdit::s_cuentalostFocus()
{
    BL_FUNC_DEBUG
    QString fech = m_entidad->text() + m_oficina->text() + m_dc->text() + m_cuenta->text();
    if ( fech != "" )
        s_cuentatextChanged ( fech );
    
}

