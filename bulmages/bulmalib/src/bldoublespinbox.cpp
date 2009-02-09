/***************************************************************************
 *   Copyright (C) 2007 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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

#include "bldoublespinbox.h"


///
/**
\param parent
**/
BlDoubleSpinBox::BlDoubleSpinBox ( QWidget *parent ) : QDoubleSpinBox ( parent )
{
    _depura ( "BlDoubleSpinBox::BlDoubleSpinBox", 0 );
    installEventFilter ( this );
    setAlignment ( Qt::AlignRight );
    setButtonSymbols ( QAbstractSpinBox::PlusMinus );
    _depura ( "END BlDoubleSpinBox::BlDoubleSpinBox", 0 );
}


///
/**
**/
BlDoubleSpinBox::~BlDoubleSpinBox()
{
    _depura ( "BlDoubleSpinBox::~BlDoubleSpinBox", 0 );
    _depura ( "END BlDoubleSpinBox::~BlDoubleSpinBox", 0 );
}


///
/**
\return
**/
QString const BlDoubleSpinBox::text()
{
    _depura ( "BlDoubleSpinBox::text", 0 );
    QString a = QDoubleSpinBox::text();
    //a = a.replace(",", ".");
    _depura ( "END BlDoubleSpinBox::text", 0 );
    return a;
}


///
/**
\param valor
**/
void BlDoubleSpinBox::setValue ( double valor )
{
    _depura ( "BlDoubleSpinBox::setValue", 0 );
    QDoubleSpinBox::setValue ( valor );
    _depura ( "END BlDoubleSpinBox::setValue", 0 );
}


///
/**
\param obj
\param event
\return
**/
bool BlDoubleSpinBox::eventFilter ( QObject *obj, QEvent *event )
{
    _depura ( "BlDoubleSpinBox::eventFilter", 0 );
    /// Si es una pulsacion o release de tecla se hace la funcionalidad especificada.
    if ( event->type() == QEvent::KeyPress ) {
        QKeyEvent * keyEvent = static_cast<QKeyEvent *> ( event );
        int key = keyEvent->key();
        Qt::KeyboardModifiers mod = keyEvent->modifiers();
        switch ( key ) {
        case Qt::Key_Period:
            /// Se pone esto para que funcione el teclado numerico la introduccion del 'punto'
            /// como separador decimal.
            QLineEdit * linea = QAbstractSpinBox::lineEdit();
            /// Comprueba que en el texto no haya ya una 'coma'.
            QString strLinea = linea->text();
            if ( !strLinea.contains ( ",", Qt::CaseInsensitive ) ) {
                linea->setText ( linea->text() + "," );
            } // end if
            return TRUE;
        } // end switch
    } // end if
    _depura ( "END BlDoubleSpinBox::eventFilter", 0 );
    return QDoubleSpinBox::eventFilter ( obj, event );
}

