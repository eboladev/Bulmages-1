/***************************************************************************
 *   Copyright (C) 2003 by Josep Burcion                                   *
 *   josep@burcion.com                                                     *
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
 ***************************************************************************/
#include "bnuevaempresa.h"
#include "abreempresaview.h"

#include <qmessagebox.h>

BNuevaEmpresa::BNuevaEmpresa(QWidget * parent, const char * name, Qt::WFlags f)
  : QDialog(parent,name,f) {
  setupUi(this);

  QObject::connect(PushButton1, SIGNAL(clicked()), this, SLOT(accept()));
  QObject::connect(PushButton1_2, SIGNAL(clicked()), this, SLOT(close()));
}// end BNuevaEmpresa


BNuevaEmpresa::~BNuevaEmpresa() {}

void BNuevaEmpresa::accept() {
    // Falta comprobar que tengas permisos para crear nuevas empresas.
    QString nombredb;
    QString nombreEmp;
    QString ejemp = ejercicioempresa->text();

    nombredb = bdempresa->text().stripWhiteSpace()+ejercicioempresa->text().stripWhiteSpace();
    nombreEmp = nombreempresa->text().stripWhiteSpace();


    /// Comprobamos que se han introducido todos los datos para avisar en caso contrario.
    if ( (nombredb == "")
            || (nombreEmp == "")
            || (ejemp == "") ) {
        QMessageBox::warning( this, tr("Datos Incompletos"),
                              tr("No puede dejar campos en blanco para crear una nueva empresa.\n"),tr("Aceptar"),tr("Cancelar"),0, 0 );
        return;
    }// end if

    QString cadena = confpr->valor(CONF_PROGDATA);
    cadena += "dbmodels/creabulmages --texto "+nombredb+" 1 "+nombreEmp+" "+ ejercicioempresa->text().stripWhiteSpace()+" "+"";
    system(cadena.ascii());
    postgresiface2 *DBconn = new postgresiface2();
    DBconn->inicializa(nombredb);
    QString query = "UPDATE configuracion SET valor='"+nombreEmp+"' WHERE nombre='NombreEmpresa'";
    DBconn->begin();
    DBconn->ejecuta(query);

	query = "UPDATE configuracion SET valor='"+ejercicioempresa->text()+"' WHERE nombre='Ejercicio'";
	DBconn->ejecuta(query);
    /// Creamos el ejercicio
    for (int x=0; x<=12; x++) {
        query.sprintf("INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES('%s', '%d', 'f')",ejemp.ascii(),x);
        DBconn->ejecuta(query);
    }// end for
    DBconn->commit();
    delete DBconn;

	/// Hacemos una recarga de empresas pq sabemos a ciencia cierta que ha cambiado el listado.
	abreempresaview *abre= new abreempresaview(NULL,"hola","hola");
	abre->on_mui_actualizar_clicked();
	delete abre;
    close();
}// end accept
