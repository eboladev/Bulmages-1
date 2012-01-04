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

#include "bcasientoduplicarview.h"
#include "bccompany.h"
#include "blfunctions.h"

#define NUMDIGITOS   mainCompany()->numDigitosEmpresa()


///
/**
\param emp
\param parent
\param flag
**/
DuplicarAsientoView::DuplicarAsientoView ( BcCompany *emp, QWidget *parent, Qt::WFlags flag )
        : QDialog ( parent, flag ), BlMainCompanyPointer ( emp )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    fdinicial->setText ( QDate::currentDate().toString ( "dd/MM/yyyy" ) );
    
}


///
/**
**/
DuplicarAsientoView::~DuplicarAsientoView()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param ainicial
\param afinal
**/
void DuplicarAsientoView::inicializa ( QString ainicial, QString afinal )
{
    BL_FUNC_DEBUG
    aoinicial->setText ( ainicial );
    aofinal->setText ( afinal );
    QString query = "SELECT * FROM asiento WHERE ordenasiento = " + ainicial;
    mainCompany() ->begin();
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( query, "hola" );
    mainCompany() ->commit();
    if ( !cur->eof() ) {
        foinicial->setText ( cur->value( "fecha" ).left ( 10 ) );
    } // end if
    delete cur;
    aoinicial->selectAll();
    aoinicial->setFocus();
    
}

///
/**
**/
void DuplicarAsientoView::lostFocus()
{
    BL_FUNC_DEBUG
    QString ainicial = aoinicial->text();
    QString query = "SELECT * FROM asiento WHERE ordenasiento = " + ainicial;
    mainCompany() ->begin();
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( query, "hola" );
    mainCompany() ->commit();
    if ( !cur->eof() ) {
        foinicial->setText ( cur->value( "fecha" ).left ( 10 ) );
    } // end if
    delete cur;
    
}


///
/**
**/
void DuplicarAsientoView::on_mui_aceptar_clicked()
{
    BL_FUNC_DEBUG
    QString asientoi = aoinicial->text();
    QString asientof = aofinal->text();
    QString query1, query2;
    QString textidasiento;
    //int ordeninicial = 0;
    //int idasientoinicial = 0;

    QString idasiento;
    QString ordenasiento;

    QString textordeninicial;
    QDate fedinicial = blNormalizeDate ( fdinicial->text() );

    /*
        /// Buscamos el orden asiento para la duplicaci&oacute;n.
        QString query = "SELECT max(ordenasiento) AS orden FROM asiento ";
        mainCompany()->begin();
        BlDbRecordSet *cur = mainCompany()->loadQuery(query);
        if (!cur->eof()) {
            ordeninicial = atoi(cur->value("orden").toAscii()) + 1;
        } // end if
        delete cur;

        query1 = "SELECT max(idasiento) AS maxim FROM asiento";
        BlDbRecordSet *cursaux = mainCompany()->loadQuery(query1);
        if (!cursaux->eof()) {
            idasiento = atoi(cursaux->value("maxim").toAscii());
            idasientoinicial = atoi(cursaux->value("maxim").toAscii()) + 1;
        } // end if
        delete cursaux;
    */

    query1 = "SELECT * FROM asiento WHERE ordenasiento >= " + asientoi + " AND ordenasiento <= " + asientof + " AND EXTRACT (YEAR FROM fecha) = EXTRACT (YEAR FROM '" + fedinicial.toString ( "dd/MM/yyyy" ) + "'::date)";
    BlDbRecordSet *curasiento = mainCompany() ->loadQuery ( query1 );
    while ( !curasiento->eof() ) {

        query1 = "INSERT INTO asiento (descripcion, fecha, comentariosasiento) VALUES('" + curasiento->value( "descripcion" ) + "','" + fedinicial.toString ( "dd/MM/yyyy" ) + "','" + curasiento->value( "comentariosasiento" ) + "')";
        mainCompany() ->runQuery ( query1 );


        query1 = "SELECT * FROM asiento  ORDER BY idasiento DESC LIMIT 1";
        BlDbRecordSet *cursaux = mainCompany() ->loadQuery ( query1 );
        if ( !cursaux->eof() ) {
            idasiento = cursaux->value( "idasiento" );
            ordenasiento = cursaux->value( "ordenasiento" );
        } // end if
        delete cursaux;



        query2 = "SELECT * FROM borrador WHERE idasiento = " + curasiento->value( "idasiento" );
        BlDbRecordSet *curborrador = mainCompany() ->loadQuery ( query2 );

        while ( !curborrador->eof() ) {
            QString textiddiario = curborrador->value( "iddiario" );
            if ( textiddiario == "" ) {
                textiddiario = "NULL";
            } // end if
            QString textfecha = curborrador->value( "fecha" );
            textfecha = fedinicial.toString ( "dd/MM/yyyy" );
            QString textconceptocontable = curborrador->value( "conceptocontable" );
            QString textidcuenta = curborrador->value( "idcuenta" );
            if ( textidcuenta == "" ) {
                textidcuenta = "NULL";
            } // end if
            QString textdescripcion = curborrador->value( "descripcion" );
            QString textdebe = curborrador->value( "debe" );
            QString texthaber = curborrador->value( "haber" );
            QString textcontrapartida = curborrador->value( "contrapartida" );
            if ( textcontrapartida == "" ) {
                textcontrapartida = "NULL";
            } // end if
            QString textorden = curborrador->value( "orden" );
            if ( textorden == "" ) {
                textorden = "0";
            } // end if
            query2 = "INSERT INTO borrador (orden, idasiento, iddiario, fecha, conceptocontable, idcuenta, descripcion, debe, haber, contrapartida) VALUES (" + textorden + "," + idasiento + "," + textiddiario + ",'" + textfecha + "','" + textconceptocontable + "'," + textidcuenta + ",'" + textdescripcion + "'," + textdebe + "," + texthaber + "," + textcontrapartida + ")";
            mainCompany() ->runQuery ( query2 );
            curborrador->nextRecord();
        } // end while
        delete curborrador;
        query2 = "SELECT cierraasiento(" + idasiento + ")";
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( query2 );
        delete cur;
        curasiento->nextRecord();
    } // end while
    delete curasiento;
    mainCompany() ->commit();
    done ( 1 );
    
}

