/***************************************************************************
 *   Copyright (C) 2006 by Tomeu Borras Riera                              *
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

#include "blmaincompany.h"
#include "blcompanydialog.h"
#include "blcountryview.h"


///
/**
\param doc
**/
void BlMainCompany::setListVentanas ( BlWindowListDock *doc )
{
    _depura ( "BlMainCompany::setListVentanas", 0 );
    m_listventanas = doc;
    _depura ( "END BlMainCompany::setListVentanas", 0 );
}


///
/**
\return
**/
BlWorkspace *BlMainCompany::pWorkspace()
{
    _depura ( "BlMainCompany::pWorkspace", 0 );
    _depura ( "END BlMainCompany::pWorkspace", 0 );
    return m_pWorkspace;
}


///
/**
\return
**/
BlWindowListDock *BlMainCompany::listVentanas()
{
    _depura ( "BlMainCompany::listVentanas", 0 );
    _depura ( "END BlMainCompany::listVentanas", 0 );
    return m_listventanas;
}




///
/**
\param qw
**/
void BlMainCompany::setWorkspace ( BlWorkspace *qw )
{
    _depura ( "BlMainCompany::setWorkspace", 0 );
    m_pWorkspace = qw;
    _depura ( "END BlMainCompany::setWorkspace", 0 );
}


///
/**
\param pb
**/
void BlMainCompany::setProgressBar ( QProgressBar *pb )
{
    _depura ( "BlMainCompany::setProgressBar", 0 );
    m_progressbar = pb;
    _depura ( "END BlMainCompany::setProgressBar", 0 );
}


///
/**
\param nom
\param obj
\param compdup
\return
**/
int BlMainCompany::meteWindow ( QString nom, QObject *obj, bool compdup )
{
    _depura ( "BlMainCompany::meteWindow", 0 );
    _depura ( "END BlMainCompany::meteWindow", 0 );
    return m_listventanas->meteWindow ( nom, obj, compdup );
}


///
/**
\param nom
\param obj
\return
**/
int BlMainCompany::seleccionaWindow ( QString nom, QObject *obj )
{
    _depura ( "BlMainCompany::seleccionaWindow", 0 );
    _depura ( "END BlMainCompany::seleccionaWindow", 0 );
    return m_listventanas->seleccionaWindow ( nom, obj );
}


///
/**
\return
**/
int BlMainCompany::deSeleccionaWindow()
{
    _depura ( "BlMainCompany::deSeleccionaWindow", 0 );
    _depura ( "END BlMainCompany::deSeleccionaWindow", 0 );
    return m_listventanas->deSeleccionaWindow();
}


///
/**
\param nom
**/
void BlMainCompany::sacaWindow ( QObject *nom )
{
    _depura ( "BlMainCompany::sacaWindow", 0 );
    m_listventanas->sacaWindow ( nom );
    _depura ( "END BlMainCompany::sacaWindow", 0 );
}


/// No precisa de operaciones en su construccion.
/**
**/
BlMainCompany::BlMainCompany()
{
    _depura ( "BlMainCompany::BlMainCompany", 0 );
    _depura ( "END BlMainCompany::BlMainCompany", 0 );
}


/// El destructor de la clase BlMainCompany borra toda la memoria almacenada.
/**
**/
BlMainCompany::~BlMainCompany()
{
    _depura ( "BlMainCompany::~BlMainCompany", 0 );
    /// cerramos todas las ventanas y las DestructiveClose se borran.
    m_listventanas->vaciar();
    /// \NOTE: El vaciado de las ventanas debe hacerse en la clase hija ya que el destructor de la clase derivada se ejecuta primero y se generaria un segfault.
    _depura ( "END BlMainCompany::~BlMainCompany", 0 );
}


/// Inicializa la base de datos que se pasa, si se pasa una cadena vacia
/// entonces invoca el selector de empresa.
/**
\param bd
\param tipo
**/
void BlMainCompany::init ( QString bd, QString tipo )
{
    _depura ( "BlMainCompany::init", 0 );
    if ( bd == "" ) {
        bd = searchCompany ( tipo );
    } // end if
    /// Hacemos visible el ProgressBar mientras se habre la base de datos y se cargan
    /// los datos de la en las ventanas.
    m_progressbar->setVisible ( TRUE );

    inicializa ( bd );
    _depura ( "END BlMainCompany::init", 0 );
}


/// Se utiliza para mostrar un selector de empresas BlCompanyDialog
/// Al usuario debe seleccionar una empresa y el sistema empieza la inicializacion de
/// clases a partir de dicha inicializacion.
/**
\param tipo
\return
**/
QString BlMainCompany::searchCompany ( QString tipo )
{
    /// El cambio de empresa se realiza desde el selector.
    _depura ( "BlMainCompany::searchCompany", 0 );
    BlCompanyDialog *nuevae = new BlCompanyDialog ( 0, tipo );
    nuevae->exec();
    _depura ( "Vamos a cambiar la empresa", 0 );
    QString bd = nuevae->nomDB();
    _depura ( "Empresa cambiada a " + bd, 0 );
    delete nuevae;
    /// Si no se ha seleccionado ninguna base de datos entonces abortamos.
    if ( bd == "" ) {
        exit ( 1 );
    } // end if
    _depura ( "END BlMainCompany::searchCompany", 0 );
    return bd;
}


/// Cambia el estado de la ventana de Indexador. Visible o no visible.
/**
\param visible
**/
void BlMainCompany::s_indexadorCambiaEstado ( bool visible )
{
    _depura ( "BlMainCompany::s_indexadorCambiaEstado", 0 );
    m_listventanas->cambiaVisible ( visible );
    _depura ( "END BlMainCompany::s_indexadorCambiaEstado", 0 );
}


///
/**
**/
void BlMainCompany::muestraPaises()
{
    _depura ( "BlMainCompany::muestrapaises", 0 );
    BlCountryView *pais = new BlCountryView ( this, 0 );
    m_pWorkspace->addWindow ( pais );
    pais->show();
    _depura ( "END BlMainCompany::muestrapaises", 0 );
}

