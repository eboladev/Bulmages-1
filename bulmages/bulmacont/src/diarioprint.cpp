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

#include "diarioprint.h"
#include "listcuentasview1.h"
#include "empresa.h"


diarioprint::diarioprint(Empresa  *emp) {
    _depura("diarioprint::diarioprint", 0);
    fichero = NULL;
    empresaactual = emp;
    conexionbase = empresaactual->bdempresa();
    _depura("END diarioprint::diarioprint", 0);
}


diarioprint::~diarioprint() {}


int diarioprint::inicializa(postgresiface2 *conn) {
    _depura("diarioprint::inicializa", 0);
    conexionbase = conn;
    _depura("diarioprint::inicializa", 0);
    return 0;
}


void diarioprint::inicializa1(char *finicial1, char *ffinal1) {
    _depura("diarioprint::inicializa1", 0);
    finicial = finicial1;
    ffinal = ffinal1;
    _depura("diarioprint::inicializa1", 0);
}


void diarioprint::inicializa2(char *fich) {
    _depura("diarioprint::inicializa2", 0);
    fichero = fich;
    _depura("diarioprint::inicializa2", 0);
}


/// Se ha pulsado sobre el bot&oacute;n aceptar del formulario.
void diarioprint::accept() {
    _depura("diarioprint::accept", 0);
    float debe, haber;
    int idcuenta;
    int idasiento;
    string fecha;
    string fechaasiento;
    string descripcion;
    string concepto;
    string codigocuenta;
    string cad;
    cursor2 *cursoraux, *cursoraux1, *cursoraux2;

    FILE *mifile;
    mifile = fopen("diario.txt", "wt");
    if (mifile != NULL) {
        fprintf(mifile, "Fecha Inicial: %s, Fecha Final: %s\n", finicial, ffinal);
        fprintf(mifile, "%5.5s %10.10s %10s %30.30s %9.2s %9.2s\n", "ASIENTO", "FECHA", "SUBCUENTA", "DESCRIPCION", "DEBE", "HABER");
        fprintf(mifile, "----------------------------------------------------------------------------------------------------------\n");
        conexionbase->begin();
        cursoraux = conexionbase->cargaasientosfecha(finicial, ffinal);
        for (; !cursoraux->eof(); cursoraux->siguienteregistro()) {
            fprintf(stderr, "bucle\n");
            fechaasiento = cursoraux->valor(2).toAscii().constData();
            idasiento = atoi(cursoraux->valor(0).toAscii());
            cursoraux1 = conexionbase->cargaapuntes(idasiento);
            for (; !cursoraux1->eof(); cursoraux1->siguienteregistro()) {
                fecha = cursoraux1->valor(4).toAscii().constData();
                descripcion = cursoraux1->valor(5).toAscii().constData();
                concepto = cursoraux->valor(1).toAscii().constData();
                debe = atof(cursoraux1->valor(8).toAscii());
                haber = atof(cursoraux1->valor(9).toAscii());
                idcuenta = atoi(cursoraux1->valor(6).toAscii());

                cursoraux2 = conexionbase->cargacuenta(idcuenta, NULL);
                if (!cursoraux2->eof()) {
                    codigocuenta = cursoraux2->valor(1).toAscii().constData();
                } // end if
                cursoraux2->cerrar();
                delete cursoraux2;
                fprintf(mifile, "%5d %-10.10s %10s %-30.30s %9.2f %9.2f\n", idasiento, fecha.c_str(), codigocuenta.c_str(), descripcion.c_str(), debe, haber);
            } // end for
            cursoraux1->cerrar();
            delete cursoraux1;
            fprintf(mifile, "\n");
        } // end for
        delete cursoraux;
        conexionbase->commit();
        fclose(mifile);
    } // end if

    /// Hacemos la llamada de sistema para imprimir.
    QString cadaux = confpr->valor(CONF_EDITOR) + " diario.txt";
    system(cadaux.toAscii().constData());
    _depura("END diarioprint::accept", 0);
}

