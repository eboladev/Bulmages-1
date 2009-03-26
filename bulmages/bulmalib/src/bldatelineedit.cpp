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

#include "bldatelineedit.h"


/// Inicializa el objeto y hace todas las conexiones necesarias.
/**
\param parent
**/
BlDateLineEdit::BlDateLineEdit ( QWidget *parent ) : QLineEdit ( parent )
{
    _depura ( "BlDateLineEdit::BlDateLineEdit", 0 );
    installEventFilter ( this );
    QObject::connect ( this, SIGNAL ( editingFinished() ), this, SLOT ( on_mui_editingFinished() ) );
    _depura ( "END BlDateLineEdit::BlDateLineEdit", 0 );
}


///No requiere de acciones especiales en el destructor.
/**
**/
BlDateLineEdit::~BlDateLineEdit()
{
    _depura ( "BlDateLineEdit::~BlDateLineEdit", 0 );
    _depura ( "END BlDateLineEdit::~BlDateLineEdit", 0 );
}


///
/**
\param val
**/
void BlDateLineEdit::setText ( QString val )
{
    _depura ( "BlDateLineEdit::setText", 0 );
    QLineEdit::setText ( val );
    _depura ( "END BlDateLineEdit::setText", 0 );
}


///
/**
\param val
**/
void BlDateLineEdit::setFieldValue ( QString val )
{
    _depura ( "BlDateLineEdit::setFieldValue", 0 );
    QLineEdit::setText ( val );
    _depura ( "END BlDateLineEdit::setFieldValue", 0 );
}


///
/**
**/
void BlDateLineEdit::selectAll()
{
    _depura ( "BlDateLineEdit::selectAll", 0 );
    _depura ( "END BlDateLineEdit::selectAll", 0 );
}


///
/**
\return
**/
QString BlDateLineEdit::text()
{
    _depura ( "BlDateLineEdit::text", 0 );
    _depura ( "END BlDateLineEdit::text", 0 );
    return QLineEdit::text();
}


///
/**
\return
**/
QString BlDateLineEdit::fieldValue()
{
    _depura ( "BlDateLineEdit::fieldValue", 0 );
    _depura ( "END BlDateLineEdit::fieldValue", 0 );
    return QLineEdit::text();
}


///
/**
**/
void BlDateLineEdit::on_mui_editingFinished()
{
    _depura ( "BlDateLineEdit::on_mui_editingFinished", 0 );
    setText ( normalizafecha ( text() ).toString ( "dd/MM/yyyy" ) );
    _depura ( "END BlDateLineEdit::on_mui_editingFinished", 0 );
}


///
/**
\param obj
\param event
\return
**/
bool BlDateLineEdit::eventFilter ( QObject *obj, QEvent *event )
{
    _depura ( "BlDateLineEdit::eventFilter", 0 );
    /*
        if (event->type() == QEvent::KeyRelease) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            int key = keyEvent->key();
            switch (key) {
            case Qt::Key_Return:
            case Qt::Key_Enter:
                setText(normalizafecha(text()).toString());
                break;
          } // end switch
        } // end if
    */
    /*
        if (event->type() == QEvent::FocusOut) {
            setText("1111");
            return TRUE;
        } // end if
    */
    _depura ( "END BlDateLineEdit::eventFilter", 0 );
    return QLineEdit::eventFilter ( obj, event );
}

