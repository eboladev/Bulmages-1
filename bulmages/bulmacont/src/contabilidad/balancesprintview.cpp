/***************************************************************************
                          balancesprintview.cpp  -  description
                             -------------------
    begin                : jue oct 9 2003
    copyright            : (C) 2003 by Tomeu Borrás Riera
    email                : tborras@conetxia.com
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "balancesprintview.h"
#include <qlineedit.h>
#include <q3datetimeedit.h>
#include <q3progressbar.h>
#include "funcaux.h"
#include "calendario.h"
#include "empresa.h"
#include "fixed.h"

#ifndef WIN32
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif

balancesprintview::balancesprintview(empresa *emp,QWidget *parent, const char *name ) : balancesprintdlg(parent,name) {
    empresaactual = emp;
    conexionbase = emp->bdempresa();
    fichero = NULL;
}// end balancesprintview

balancesprintview::~balancesprintview() {}// end ~balancesprintview



/*********************************************************************************
 * Se ha pulsado sobre el boton de aceptar del formulario con lo que podemos     *
 * pasar a hacer la impresión.                                                   *
 *********************************************************************************/
void balancesprintview::accept() {
    QString arch = confpr->valor(CONF_DIR_USER)+"balance.txt";

    string cad;
    QString fechainicial = fechain->text();
    QString fechafinal = fechafin->text();
	/// Si no se especifica fecha inicial se pone una suficientemente antigua.
    if (fechainicial == "") fechainicial = "01/01/1900";
	/// Si no se especifica fecha final se pone una suficientemente futura.
    if (fechafinal == "") fechafinal = "31/12/2100";


    FILE *mifile;
    mifile = fopen(arch.ascii(),"wt");
    if (mifile == NULL) {
	_depura("No se ha podido abrir el fichero de destino\n",2);
	return;
    }// end if

    if (mifile != NULL) {
	/// Cargamos los componentes e itereamos para cada uno de ellos.
        QString query1 = "SELECT * FROM compbalance WHERE idbalance = "+idbalance+" ORDER BY orden";
        cursor2 *cursor = conexionbase->cargacursor(query1);
        int numregistros = cursor->numregistros();
        progreso->reset();
        progreso->setTotalSteps(numregistros);
        progreso->setProgress(1);
        while (!cursor->eof()) {
            QString query = "SELECT saldompatrimonial("+cursor->valor("idmpatrimonial")+",'"+fechainicial+"','"+fechafinal+"') AS saldot";
            conexionbase->begin();
            cursor2 *mycursor = conexionbase->cargacursor(query);
            conexionbase->commit();
            int i=0;
            Fixed valor("0");
            while (!mycursor->eof()) {
                int orden = cursor->valor("tabulacion").toInt();
                QString texto = "";
                for (int j=0; j<orden; j++)
                    texto += "   ";
                texto += cursor->valor("concepto");
                valor = Fixed(mycursor->valor("saldot"));
		if (valor != Fixed("0") || orden <=1) 
			if (valor != Fixed("0")) {
                		fprintf(mifile, "%-60.60s %10.10s\n", texto.ascii(), valor.toQString().ascii());
			} else {
                		fprintf(mifile, "%-60.60s\n", texto.ascii());
			}// end if
                i++;
                mycursor->siguienteregistro();
            }// end while
            delete mycursor;
            // Actualizamos la barra de progreso
            progreso->setProgress(progreso->progress()+1);
            cursor->siguienteregistro();
        }// end while
        delete cursor;
        fclose(mifile);
    }// end if

    QString cadena = confpr->valor(CONF_EDITOR)+" "+confpr->valor(CONF_DIR_USER)+"balance.txt &";
    system(cadena.ascii());
}// end accept


void balancesprintview::setidbalance(QString id) {
    idbalance = id;
    QString query ="SELECT * FROM balance WHERE idbalance="+idbalance;
    cursor2 *cur = conexionbase->cargacursor(query, "micurs");
    if (!cur->eof()) {
        m_nomBalance->setText(cur->valor("nombrebalance"));
    }// end if
    delete cur;
}// end setidbalance



