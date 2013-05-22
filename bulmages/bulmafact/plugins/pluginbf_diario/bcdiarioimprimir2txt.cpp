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

#include <unistd.h>

#include "bcdiarioimprimir2txt.h"
#include "bfcompany.h"


///
/**
\param emp
**/
BcDiarioImprimir2TXT::BcDiarioImprimir2TXT ( BfCompany  *emp ) : BlMainCompanyPointer ( emp )
{
    BL_FUNC_DEBUG
    fichero = NULL;
    
}


///
/**
**/
BcDiarioImprimir2TXT::~BcDiarioImprimir2TXT()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param finicial1
\param ffinal1
**/
void BcDiarioImprimir2TXT::inicializa1 ( char *finicial1, char *ffinal1 )
{
    BL_FUNC_DEBUG
    finicial = finicial1;
    ffinal = ffinal1;
    
}


///
/**
\param fich
**/
void BcDiarioImprimir2TXT::inicializa2 ( char *fich )
{
    BL_FUNC_DEBUG
    fichero = fich;
    
}


/// Se ha pulsado sobre el bot&oacute;n aceptar del formulario.
/**
**/
void BcDiarioImprimir2TXT::accept()
{
    BL_FUNC_DEBUG
    float debe, haber;
    int idcuenta;
    int idasiento;
    QString fecha;
    QString fechaasiento;
    QString descripcion;
    QString concepto;
    QString codigocuenta;
    QString cad;
    BlDbRecordSet *cursoraux, *cursoraux1, *cursoraux2;

    FILE *mifile;
    mifile = fopen ( "diario.txt", "wt" );
    if ( mifile != NULL ) {
        fprintf ( mifile, "Fecha Inicial: %s, Fecha Final: %s\n", finicial, ffinal );
        fprintf ( mifile, "%5.5s %10.10s %10s %30.30s %9.2s %9.2s\n", "ASIENTO", "FECHA", "SUBCUENTA", "DESCRIPCION", "DEBE", "HABER" );
        fprintf ( mifile, "----------------------------------------------------------------------------------------------------------\n" );
        mainCompany() ->begin();
        cursoraux = mainCompany() -> loadQuery ( "SELECT * FROM asiento WHERE fecha >= '" + QString ( finicial ) + "' AND fecha <= '" + QString ( ffinal ) + "' ORDER BY fecha" );
        for ( ; !cursoraux->eof(); cursoraux->nextRecord() ) {
            fechaasiento = cursoraux->value( "fechaasiento" ).toAscii().constData();
            idasiento = atoi ( cursoraux->value( "idasiento" ).toAscii() );
            cursoraux1 = mainCompany() ->loadQuery ( "SELECT * FROM apunte where idasiento = " + cursoraux->value( "idasiento" ) + " ORDER BY idapunte" );
            for ( ; !cursoraux1->eof(); cursoraux1->nextRecord() ) {
                fecha = cursoraux1->value( 4 ).toAscii().constData();
                descripcion = cursoraux1->value( 5 ).toAscii().constData();
                concepto = cursoraux->value( 1 ).toAscii().constData();
                debe = atof ( cursoraux1->value( 8 ).toAscii() );
                haber = atof ( cursoraux1->value( 9 ).toAscii() );
                idcuenta = atoi ( cursoraux1->value( "idcuenta" ).toAscii() );

                cursoraux2 = mainCompany() ->loadQuery ( "SELECT * FROM cuenta WHERE idcuenta = " + cursoraux1->value( "idcuenta" ) );

                if ( !cursoraux2->eof() ) {
                    codigocuenta = cursoraux2->value( "codigo" ).toAscii().constData();
                } // end if
                cursoraux2->cerrar();
                delete cursoraux2;
                fprintf ( mifile, "%5d %-10.10s %10s %-30.30s %9.2f %9.2f\n", idasiento, fecha.toAscii().data(), codigocuenta.toAscii().data(), descripcion.toAscii().data(), debe, haber );
            } // end for
            cursoraux1->cerrar();
            delete cursoraux1;
            fprintf ( mifile, "\n" );
        } // end for
        delete cursoraux;
        mainCompany() ->commit();
        fclose ( mifile );
    } // end if

    /// Hacemos la llamada de sistema para imprimir.
    QString cadaux = g_confpr->value( CONF_EDITOR ) + " diario.txt";
    system ( cadaux.toAscii().constData() );
    
}

