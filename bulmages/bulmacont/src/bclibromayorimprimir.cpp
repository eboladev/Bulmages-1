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

#include "bclibromayorimprimir.h"
#include "blfixed.h"


///
/**
\param emp
**/
BcLibroMayorImprimir::BcLibroMayorImprimir ( BcCompany *emp ) : BlMainCompanyPointer ( emp )
{
    _depura ( "BcLibroMayorImprimir::BcLibroMayorImprimir", 0 );
    fichero = NULL;
    _depura ( "END BcLibroMayorImprimir::BcLibroMayorImprimir", 0 );
}


///
/**
**/
BcLibroMayorImprimir::~BcLibroMayorImprimir()
{
    _depura ( "BcLibroMayorImprimir::~BcLibroMayorImprimir", 0 );
    _depura ( "END BcLibroMayorImprimir::~BcLibroMayorImprimir", 0 );
}


///
/**
\param codinicial1
\param codfinal1
\param finicial1
\param ffinal1
**/
void BcLibroMayorImprimir::inicializa1 ( QString codinicial1, QString codfinal1, QString finicial1, QString ffinal1 )
{
    _depura ( "BcLibroMayorImprimir::inicializa1", 0 );
    codinicial = codinicial1;
    codfinal = codfinal1;
    finicial = finicial1;
    ffinal = ffinal1;
    _depura ( "END BcLibroMayorImprimir::inicializa1", 0 );
}


///
/**
\param fich
**/
void BcLibroMayorImprimir::inicializa2 ( char *fich )
{
    _depura ( "BcLibroMayorImprimir::inicializa2", 0 );
    fichero = fich;
    _depura ( "BcLibroMayorImprimir::inicializa2", 0 );
}



/// Se ha pulsado sobre el bot&oacute;n aceptar del formulario.
/**
**/
void BcLibroMayorImprimir::accept()
{
    _depura ( "BcLibroMayorImprimir::accept", 0 );
    BlFixed debe ( "0" ), haber ( "0" ), saldo ( "0" );
    BlFixed debeinicial ( "0" ), haberinicial ( "0" ), saldoinicial ( "0" );
    BlFixed debefinal ( "0" ), haberfinal ( "0" ), saldofinal ( "0" );

    int idcuenta;
    int idasiento;
    char *textasiento;
    int activo;
    QString cad;
    BlDbRecordSet *cursoraux, *cursoraux1, *cursoraux2;

    FILE *mifile;
    mifile = fopen ( "mayor.txt", "wt" );
    if ( mifile != NULL ) {
        mainCompany() ->begin();
        cursoraux = mainCompany() -> loadQuery ( "SELECT * FROM cuenta WHERE codigo >= '" + codinicial + "' AND codigo <= '" + codfinal + "' ORDER BY codigo" );
        while ( !cursoraux->eof() ) {
            idcuenta = atoi ( cursoraux->valor ( "idcuenta" ).toAscii() );

            QString query = "SELECT * FROM apunte where idcuenta = " + cursoraux->valor ( "idcuenta" ) + " AND fecha >= '" + finicial + "' AND fecha <= '" + ffinal + "' ORDER BY fecha";
            cursoraux1 = mainCompany() ->loadQuery ( query );
            if ( !cursoraux1->eof() ) {
                activo = strcmp ( ( char * ) cursoraux->valor ( 13 ).toAscii().constData() , "f" );
                fprintf ( mifile, "\n\n%12s %50s", cursoraux->valor ( 1 ).toAscii().constData(), cursoraux->valor ( 2 ).toAscii().constData() );
                if ( activo ) {
                    fprintf ( mifile, " Cuenta de Activo\n" );
                } else {
                    fprintf ( mifile, " Cuenta de Pasivo\n" );
                } // end if
                cursoraux2 = mainCompany() ->loadQuery ( "SELECT sum(debe) as tdebe, sum(haber)as thaber FROM apunte WHERE idcuenta = " + cursoraux->valor ( "idcuenta" ) + " AND fecha <'" + finicial + "'" );
                if ( !cursoraux2->eof() ) {
                    debeinicial = BlFixed ( cursoraux2->valor ( 0 ).toAscii().constData() );
                    haberinicial = BlFixed ( cursoraux2->valor ( 1 ).toAscii().constData() );
                    if ( activo ) {
                        saldoinicial = debeinicial - haberinicial;
                    } else {
                        saldoinicial = haberinicial - debeinicial;
                    } // end if
                    fprintf ( mifile, "%5.5s %10.10s %10.10s %-40.40s %10.10s %10.10s %10.10s\n", "Asiento", "Fecha", "Asiento", "Descripcion", "Debe", "Haber", "Saldo" );
                    fprintf ( mifile, "                                                 SUMAS ANTERIORES... %10.2s %10.2s %10.2s\n", debeinicial.toQString().toAscii().constData(), haberinicial.toQString().toAscii().constData(), saldoinicial.toQString().toAscii().constData() );
                    fprintf ( mifile, "---------------------------------------------------------------------------------------------------------\n" );
                } // end if
                saldo = saldoinicial;
                debefinal = debeinicial;
                haberfinal = haberinicial;
                for ( ; !cursoraux1->eof(); cursoraux1->nextRecord() ) {
                    idasiento = atoi ( cursoraux1->valor ( 2 ).toAscii() );
                    textasiento = ( char * ) cursoraux1->valor ( 5 ).toAscii().constData();
                    debe = BlFixed ( cursoraux1->valor ( 8 ).toAscii().constData() );
                    haber = BlFixed ( cursoraux1->valor ( 9 ).toAscii().constData() );
                    if ( activo ) {
                        saldo = saldo + debe - haber;
                    } else {
                        saldo = saldo + haber - debe;
                    } // end if
                    debefinal = debefinal + debe;
                    haberfinal = haberfinal + haber;
                    cad = cursoraux1->valor ( 4 ).toAscii().constData();
                    fprintf ( mifile, "%5.5d %10.10s %10.10s %-40.40s %10.2s %10.2s %10.2s\n", idasiento, cad.left ( 10 ).toAscii().constData(), textasiento, cursoraux1->valor ( 5 ).toAscii().constData(), debe.toQString().toAscii().constData(), haber.toQString().toAscii().constData(), saldo.toQString().toAscii().constData() );
                } // end for
                if ( activo ) {
                    saldofinal = debefinal - haberfinal;
                } else {
                    saldofinal = haberfinal - debefinal;
                } // end if
                fprintf ( mifile, "                                       -----------------------------------------------------------------\n" );
                fprintf ( mifile, "                                                  TOTAL SUBCUENTA... %10.2s %10.2s %10.2s\n", debefinal.toQString().toAscii().constData(), haberfinal.toQString().toAscii().constData(), saldofinal.toQString().toAscii().constData() );
                cursoraux2->cerrar();
                delete cursoraux2;
            } // end if
            cursoraux1->cerrar();
            delete cursoraux1;
            cursoraux->nextRecord();
        } // end while
        fclose ( mifile );
        mainCompany() ->commit();
        delete cursoraux;
    } // end if
    QString cadaux = g_confpr->valor ( CONF_EDITOR ) + " mayor.txt";
    system ( cadaux.toAscii().constData() );
    _depura ( "END BcLibroMayorImprimir::accept", 0 );
}
