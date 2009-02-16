/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#include <fstream>

#include <QMessageBox>
#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <QToolButton>
#include <QTextStream>
#include <QLayout>
#include <QMessageBox>
#include <QCloseEvent>

#include "pagoview.h"
#include "company.h"
#include "blconfiguration.h"
#include "busquedafecha.h"
#include "blfunctions.h"


///
/**
\param comp
\param parent
**/
PagoView::PagoView ( Company *comp, QWidget *parent )
        :  FichaBf ( comp, parent )
{
    _depura ( "PagoView::PagoView", 0 );
    try {
        setAttribute ( Qt::WA_DeleteOnClose );
        setupUi ( this );
        centrarEnPantalla ( this );
        /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
        mui_idproveedor->setMainCompany ( comp );
        mui_refpago->setMainCompany ( comp );
        mui_idbanco->setMainCompany ( comp );

        setTitleName ( _( "Pago" ) );
        setDBTableName ( "pago" );
        setDBCampoId ( "idpago" );
        addDBCampo ( "idpago", BlDbField::DBint, BlDbField::DBPrimaryKey, _( "Id pago" ) );
        addDBCampo ( "idproveedor", BlDbField::DBint, BlDbField::DBNotNull, _( "Id proveedor" ) );
        addDBCampo ( "previsionpago", BlDbField::DBboolean, BlDbField::DBNothing, _( "Previcion de pago" ) );
        addDBCampo ( "fechapago", BlDbField::DBdate, BlDbField::DBNothing, _( "Fecha de pago" ) );
        addDBCampo ( "refpago", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Referencia de pago" ) );
        addDBCampo ( "cantpago", BlDbField::DBnumeric, BlDbField::DBNotNull, _( "Cantidad" ) );
        addDBCampo ( "comentpago", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Comentario del pago" ) );
        addDBCampo ( "idbanco", BlDbField::DBint, BlDbField::DBNothing, _( "Banco" ) );

	/// Establecemos los parametros de busqueda del Cliente
	mui_idproveedor->setLabel ( _( "Proveedor:" ) );
	mui_idproveedor->setTableName( "proveedor" );
	mui_idproveedor->m_valores["cifproveedor"] = "";
	mui_idproveedor->m_valores["nomproveedor"] = "";

        meteWindow ( windowTitle(), this, FALSE );
        pintar();
        dialogChanges_cargaInicial();
    } catch ( ... ) {
        mensajeInfo ( _( "Error al crear el pago" ), this );
    } // end try
    _depura ( "Fin de la inicializacion de PagoView\n", 0 );
    _depura ( "END PagoView::PagoView", 0 );
}


///
/**
**/
PagoView::~PagoView()
{
    _depura ( "PagoView::~PagoView", 0 );
    _depura ( "END PagoView::~PagoView", 0 );
}

