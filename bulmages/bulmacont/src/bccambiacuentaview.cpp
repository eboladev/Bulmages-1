/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#include  "bccambiacuentaview.h"
//#include  "bcplancontablelistview.h"
#include  "bccompany.h"

#define NUMDIGITOS   (BcCompany *)mainCompany())->numDigitosEmpresa()


///
/**
\param emp
\param parent
\param flag
**/
BcCambiaCuentaView::BcCambiaCuentaView ( BcCompany *emp, QWidget *parent, Qt::WFlags flag )
        : QDialog ( parent, flag ), BlMainCompanyPointer ( emp )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    /// Preparamos la cuentaorigen para que trabaje sobre cuentas
    mui_codigoorigen->setMainCompany ( emp );
    mui_codigoorigen->setLabel ( _ ( "Cuenta Origen:" ) );
    mui_codigoorigen->setTableName ( "cuenta" );
    mui_codigoorigen->setFieldId("idcuenta");
    mui_codigoorigen->m_valores["descripcion"] = "";
    mui_codigoorigen->m_valores["codigo"] = "";

    /// Preparamos la cuentadestino para que trabaje sobre cuentas
    mui_codigodestino->setMainCompany ( emp );
    mui_codigodestino->setLabel ( _ ( "Cuenta Destino:" ) );
    mui_codigodestino->setTableName ( "cuenta" );
    mui_codigodestino->setFieldId("idcuenta");
    mui_codigodestino->m_valores["descripcion"] = "";
    mui_codigodestino->m_valores["codigo"] = "";



    
}


///
/**
**/
BcCambiaCuentaView::~BcCambiaCuentaView()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void BcCambiaCuentaView::accept()
{
    BL_FUNC_DEBUG
    QString origen = mui_codigoorigen->id();
    QString destino = mui_codigodestino->id();
    QString ainicial = asientoinicial->text();
    QString afinal = asientofinal->text();
    QString finicial = fechainicial->text();
    QString ffinal = fechafinal->text();

    /// Inicia transaccion.
    mainCompany() ->begin();


    /// Busca los asientos que hay que abrir antes de hacer la actualizacion.
    QString query1 = "SELECT idasiento FROM apunte WHERE idcuenta = " + origen;
    if ( ainicial != "" ) {
        query1 = query1 + " AND idasiento IN (SELECT idasiento FROM asiento WHERE ordenasiento >= " + ainicial + ")";
    } // end if
    if ( afinal != "" ) {
        query1 = query1 + " AND idasiento IN (SELECT idasiento FROM asiento WHERE ordenasiento <= " + afinal + ")";
    } // end if
    if ( finicial != "" ) {
        query1 = query1 + " AND fecha >= '" + finicial + "'";
    } // end if
    if ( ffinal != "" ) {
        query1 = query1 + " AND fecha <= '" + ffinal + "'";
    } // end if


    BlDbRecordSet *cur = mainCompany()->loadQuery(query1);

    if (cur != NULL) {
        cur->firstRecord();
	while ( !cur->eof() ) {
	    // Abre los asientos
	    mainCompany()->runQuery("SELECT abreasiento(" + cur->value("idasiento") + ")");
	    cur->nextRecord();
	} // end while
    } // end if


    /// Ahora todo esta en modo borrador. Se puede iniciar la actualizacion.


    /// Modificamos los borradores.
    QString query2 = "UPDATE borrador SET idcuenta = " + destino + " WHERE idcuenta = " + origen + "";
    if ( ainicial != "" ) {
        query2 = query2 + " AND idasiento IN (SELECT idasiento FROM asiento WHERE ordenasiento >= " + ainicial + ")";
    } // end if
    if ( afinal != "" ) {
        query2 = query2 + " AND idasiento IN (SELECT idasiento FROM asiento WHERE ordenasiento <= " + afinal + ")";
    } // end if
    if ( finicial != "" ) {
        query2 = query2 + " AND fecha >= '" + finicial + "'";
    } // end if
    if ( ffinal != "" ) {
        query2 = query2 + " AND fecha <= '" + ffinal + "'";
    } // end if


    mainCompany() ->runQuery ( query2 );


    /// Modificamos las contrapartidas de los borradores.
    QString query3 = "UPDATE borrador SET contrapartida = " + destino + " WHERE contrapartida = " + origen ;
    if ( ainicial != "" ) {
        query3 = query3 + " AND idasiento IN (SELECT idasiento FROM asiento WHERE ordenasiento >= " + ainicial + ")";
    } // end if
    if ( afinal != "" ) {
        query3 = query3 + " AND idasiento IN (SELECT idasiento FROM asiento WHERE ordenasiento <= " + afinal + ")";
    } // end if
    if ( finicial != "" ) {
        query3 = query3 + " AND fecha >= '" + finicial + "'";
    } // end if
    if ( ffinal != "" ) {
        query3 = query3 + " AND fecha <= '" + ffinal + "'";
    } // end if


    mainCompany() ->runQuery ( query3 );



    // Completada la actualizacion se cierran los asientos.
    

    if (cur != NULL) {
        cur->firstRecord();
	while ( !cur->eof() ) {
	    // Abre los asientos
	    mainCompany()->runQuery("SELECT cierraasiento(" + cur->value("idasiento") + ")");
	    cur->nextRecord();
	} // end while
    } // end if



    /// Modificamos los registros de IVA.
    QString query4 = "UPDATE registroiva SET contrapartida = " + destino + " WHERE contrapartida = " + origen;
    if ( ainicial != "" ) {
        query4 = query4 + " AND idborrador IN (SELECT idborrador FROM borrador,asiento WHERE ordenasiento >= " + ainicial + " AND borrador.idasiento = asiento.idasiento)";
    } // end if
    if ( afinal != "" ) {
        query4 = query4 + " AND idborrador IN (SELECT idborrador FROM borrador,asiento WHERE ordenasiento <= " + afinal + " AND borrador.idasiento = asiento.idasiento)";
    } // end if
    if ( finicial != "" ) {
        query4 = query4 + " AND ffactura >= '" + finicial + "'";
    } // end if
    if ( ffinal != "" ) {
        query4 = query4 + " AND ffactura <= '" + ffinal + "'";
    } // end if


    mainCompany() ->runQuery ( query4 );


    mainCompany() ->commit();
    done ( 1 );
    
}

