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




abreempresaview::abreempresaview(const char *name, bool modal) : abreempresadlg(0,name,modal) {
//   padre = parent;

/*
   postgresiface2 *apuestatealgo;
   apuestatealgo = new postgresiface2();
   apuestatealgo->inicializa( confpr->valor(CONF_METABASE).c_str() );
   QListViewItem *it;
   cursor2 *a;
   apuestatealgo->begin();
   a=apuestatealgo->cargaempresas();
   apuestatealgo->commit();
   while (!a->eof()) {
         it =new QListViewItem(empresas);
         it->setText(0,a->valor(1));
         it->setText(1,a->valor(2));
         it->setText(2,a->valor(3));
         a->siguienteregistro();
   }// end while
   delete a;
   delete apuestatealgo;
*/

   listDB();
   intentos=0;
   entrada=0;
}// end abreempresaview


abreempresaview::~abreempresaview(){
}// end ~abreempresaview


/** \brief Se ha pulsado sobre cerrar en la ventana de selector
  * lo que hacemos es cancelar la ejecución del programa saliendo del mismo
 */
void abreempresaview::quitar() {
	exit(1);
}// end close


void abreempresaview::close() {
	if (entrada == 0) exit(1);
	QDialog::close();
}// end if


/** \brief Se ha pulsado el boton de aceptar sobre el selector
  * Comprueba que se puede abrir la empresa y si es así
  * carga los datos necesarios en el programa y hace los pasos requeridos.
  * En caso contrario aborta la ejecución.
  */
void abreempresaview::accept() {
   QListViewItem *it;
   int num;
   it = empresas->currentItem();
   nombre=login->text();
   contrasena=password->text();
   empresabd= it->text(2);
   ejercicioMetaDB = it->text(1);
   nombreempresa= it->text(0);
 
    
   // El uso de esta función es dudoso.
   postgresiface2 *DBConn = new postgresiface2();
   num = DBConn->cargaempresa(empresabd, nombre, contrasena);
   delete DBConn;
   if (num >0) {
/*   
       postgresiface2 *DBConn2= new postgresiface2();
       DBConn2->inicializa(confpr->valor(CONF_METABASE).c_str());
       DBConn2->begin();
       QString query;
       query.sprintf("SELECT permisos FROM usuario_empresa, empresa, usuario WHERE usuario_empresa.idempresa=empresa.idempresa and usuario_empresa.idusuario=usuario.idusuario and empresa.nombredb='%s' and usuario.login='%s'",empresabd.ascii(),nombre.ascii());
       cursor2 * recordSet = DBConn2->cargacursor(query,"recordSet");
       DBConn2->commit();
       
       ///       empresaactual->nombreusuario = nombre;
       confpr->setValor(PRIVILEGIOS_USUARIO, recordSet->valor("permisos"));
*/       
       ///Empezamos un nuevo modo de guardar algunas preferencias de los usuarios en la base de datos
       confpr->cargarEntorno(empresabd);
       ctllog->add(LOG_SEG | LOG_TRA, 1,"AbrViw004", "Entrando usuario: --"+nombre+"-- hacia la empresa: --"+nombreempresa+"-- con los permisos -"+confpr->valor(CONF_PRIVILEGIOS_USUARIO).c_str()+"-" );
//       delete recordSet;
//       delete DBConn2;
       // Indicamos que hemos pasado por este punto para que no se
       // aborte la ejecución del programa.
       entrada=1;
       close();
   }//end if

   // Si se supera el numero de intentos abortamos la ejecución
   if ((intentos+=1)>3) exit(1);
}// end accept


void abreempresaview::insertCompany(QString nombre, QString ano, QString archivo, QString tipo) {
            QListViewItem *it =new QListViewItem(empresas);
            it->setText(0,nombre);
            it->setText(1,ano);
            it->setText(2,archivo);
            it->setText(3,tipo);
}// end insertCompany

/** \brief esta función es una primera prueba para empezar a eliminar la metabase.
Intentará listar todas las bases de datos.
*/
void abreempresaview::listDB() {
	// Desabilitamos las alertas para que no aparezcan warnings con bases de datos que no son del sistema.
	confpr->setValor(CONF_ALERTAS_DB,"No");
	postgresiface2 *db, *db1;
	db = new postgresiface2();
	db->inicializa(QString("template1"));
	db->begin();
	QString nombre;
	QString nomdb="";
	QString ano;
	QString tipo;
	cursor2 *curs= db->cargacursor("SELECT datname FROM pg_database","otroquery");
	db->commit();
	fprintf(stderr,"LISTADO DE BASES DE DATOS DISPONIBLES\n");
	fprintf(stderr,"--------------------------------------\n");

	while (! curs->eof()) {	
		db1 = new postgresiface2();
		db1->inicializa(curs->valor("datname").ascii());
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
		if (nomdb != "" && tipo== "BulmaCont") {
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

