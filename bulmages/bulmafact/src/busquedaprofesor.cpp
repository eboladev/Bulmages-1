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

#include "busquedaprofesor.h"
#include "company.h"
#include "funcaux.h"


/** Inicializa la clase poniendo a NULL todos los valores para que no haya confusion
    en el hecho de que la clase aun no ha sido completamente inicializada.
*/
/**
\param parent
**/
BusquedaProfesor::BusquedaProfesor ( QWidget *parent )
        : Busqueda ( parent )
{
    _depura ( "BusquedaProfesor::BusquedaProfesor", 0 );

    /// Establecemos los Buddies:
    mui_labelBusqueda->setText ( _( "Profesor:" ) );
    mui_labelBusqueda->setBuddy ( mui_buscar );

	m_tabla = "profesor";
	m_valores["nombreprofesor"] = "";

    _depura ( "END BusquedaProfesor::BusquedaProfesor", 0 );
}


/** No requiere de acciones especiales en el destructor de clase.
*/
/**
**/
BusquedaProfesor::~BusquedaProfesor()
{
    _depura ( "BusquedaProfesor::~BusquedaProfesor", 0 );
    _depura ( "END BusquedaProfesor::~BusquedaProfesor", 0 );
}



/** SLOT que responde a la accion de buscar un profesor.
    Abre el listado de profesors en modo Seleccionar y espera a que cerremos
    dicha ventana o que seleccionemos un elemento.
    Una vez determinado el elemento lo considera como el elemento seleccionado
    y lo muestra.
*/
/**
**/
void BusquedaProfesor::on_mui_buscar_clicked()
{
    _depura ( "BusquedaProfesor::on_mui_buscar_clicked", 0 );
/*
    QDialog *diag = new QDialog ( 0 );
    diag->setModal ( true );
    diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
    centrarEnPantalla ( diag );

    ClientsList *clients = new ClientsList ( ( Company * ) empresaBase(), diag, 0, ClientsList::SelectMode );
    connect ( clients, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );

    /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
    /// para que sea redimensionable y aparezca el titulo de la ventana.
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget ( clients );
    layout->setMargin ( 0 );
    layout->setSpacing ( 0 );
    diag->setLayout ( layout );
    diag->setWindowTitle ( clients->windowTitle() );

    diag->exec();
    if ( clients->idclient() != "" ) {
        setidprofesor ( clients->idclient() );
    } // end if
    delete diag;
*/
    _depura ( "END BusquedaProfesor::on_mui_buscar_clicked", 0 );
}

