/***************************************************************************
                          abreempresaview.cpp  -  description
                             -------------------
    begin                : Wed Nov 27 2002
    copyright            : (C) 2002 by Tomeu Borrás Riera
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
#include "abreempresaview.h"
#include "postgresiface2.h"



abreempresaview::abreempresaview(QWidget *parent, int tipo, const char *name, bool modal) : abreempresadlg(0,name,modal) { 
   listDB();
}// end abreempresaview


abreempresaview::~abreempresaview(){
}// end ~abreempresaview



void abreempresaview::insertCompany(QString nombre, QString ano, QString archivo, QString tipo) {
            QListViewItem *it =new QListViewItem(empresas);
            it->setText(0,nombre);
            it->setText(1,ano);
            it->setText(2,archivo);
            it->setText(3,tipo);
}// end insertCompany


void abreempresaview::accept() {
   QListViewItem *it;
   it = empresas->currentItem();
   empresabd= it->text(2);
   nombreempresa= it->text(0);
   tipo = it->text(3);
   close();
}// end accept


void abreempresaview::closeEvent(QCloseEvent * e) {
    e->accept();
}


/** \brief esta función es una primera prueba para empezar a eliminar la metabase.
Intentará listar todas las bases de datos.
*/
void abreempresaview::listDB() {
	// Desabilitamos las alertas para que no aparezcan warnings con bases de datos que no son del sistema.
	confpr->setValor(CONF_ALERTAS_DB,"No");
	postgresiface2 *db, *db1;
	db = new postgresiface2();
	db->inicializa(QString("template1"), confpr->valor(CONF_LOGIN_USER), confpr->valor(CONF_PASSWORD_USER));
	db->begin();
	QString nombre;
	QString nomdb="";
	QString ano;
	QString tipo;
	cursor2 *curs= db->cargacursor("SELECT datname FROM pg_database","otroquery");
	db->commit();
	fprintf(stderr,"LISTADO DE BASES DE DATOS DISPONIBLES\n");
	fprintf(stderr,"--------------------------------------\n");
	fprintf(stderr,"Usuario %s, Password %s\n", confpr->valor(CONF_LOGIN_USER).c_str(), confpr->valor(CONF_PASSWORD_USER).c_str());
	while (! curs->eof()) {	
		db1 = new postgresiface2();
		db1->inicializa(curs->valor("datname"), confpr->valor(CONF_LOGIN_USER), confpr->valor(CONF_PASSWORD_USER));
		db1->begin();
		cursor2 *curs1 = db1->cargacursor("SELECT * FROM configuracion WHERE nombre='Tipo'","masquery");
		if (!curs1->eof() ) {
			tipo=curs1->valor("valor");
			nomdb=curs->valor("datname");
		} // end if
		delete curs1;
		curs1 = db1->cargacursor("SELECT * FROM configuracion WHERE nombre='NombreEmpresa'","masquery1");

		if (!curs1->eof() ) {
			nombre=curs1->valor("valor");
		} // end if
		delete curs1;
		curs1 = db1->cargacursor("SELECT * FROM configuracion WHERE nombre='Ejercicio'","masquery2");

		if (!curs1->eof() ) {
			ano=curs1->valor("valor");
		} // end if
		delete curs1;		
		if (nomdb != "") {
			insertCompany(nombre,ano,nomdb,tipo);
			nomdb="";
		}// end if
		db1->commit();
		delete db1;
		curs->siguienteregistro();
	}// end while
	fprintf(stderr,"FIN DE LISTADO DE BASES DE DATOS DISPONIBLES\n");
	fprintf(stderr,"--------------------------------------------\n");
	delete curs;
	delete db;	
	confpr->setValor(CONF_ALERTAS_DB,"Yes");

}// end listDB

