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

// Este es el archivo en el que se almacenan las empresas que existen.
// Es un archivo separado por comas.
#define LISTEMPRESAS "listempresas.lst"

#include <fstream>
using namespace std;


#define ABRE_NOMBRE  0
#define ABRE_ANO     1
#define ABRE_ARCHIVO 2
#define ABRE_TIPO    3

/** \brief se encarga de intentar abrir una nueva empresa
  * @param parent La ventana que hace la llamada
  * @param tipo   String que indica si es contabilidad o facturacion (bulmacont, bulmafact)
  * @param name nombre de la ventana
  * @param modal Indica si la ventana debe comportar de forma modal o no. (por defecto si)
  */
abreempresaview::abreempresaview(QWidget *parent, QString tipo, const char *name, bool modal) : abreempresadlg(parent,name,modal) {
   m_tipo = tipo;
   m_tipoempresa = "";
   cargaArchivo();
}// end abreempresaview


abreempresaview::~abreempresaview(){
}// end ~abreempresaview


/** \brief Inserta una compañia en el QList empresas del dialogo
  * @param nombre Nombre de la empresa
  * @param ano Ejercicio de la empresa (aunque pueden ser varios)
  * @param archivos Nombre de la base de datos
  * @param tipo Tipo de base de datos (BulmaCont o BulmaFact)
  */
void abreempresaview::insertCompany(QString nombre, QString ano, QString archivo, QString tipo) {
            QListViewItem *it =new QListViewItem(empresas);
            it->setText(ABRE_NOMBRE,nombre);
            it->setText(ABRE_ANO,ano);
            it->setText(ABRE_ARCHIVO,archivo);
            it->setText(ABRE_TIPO,tipo);
}// end insertCompany


/** \brief Se ha pulsado sobre el botón de aceptar con lo que iniciamos la variables y cerramos esta ventana ya que ha cumplico con su cometido
  */
void abreempresaview::accept() {
   QListViewItem *it;
   it = empresas->currentItem();
   m_empresabd= it->text(ABRE_ARCHIVO);
   m_nombreempresa= it->text(ABRE_NOMBRE);
   m_tipoempresa = it->text(ABRE_TIPO);
   close();
}// end accept


void abreempresaview::closeEvent(QCloseEvent * e) {
    e->accept();
}


/** \brief carga del archivo de empresas las empresas disponibles.
  */
void abreempresaview::cargaArchivo() {
   QString dir1 = getenv("HOME");
   dir1 = dir1 + "/.bulmages/"+LISTEMPRESAS;
   ifstream filestr(dir1.ascii());
   string nombre, ano, nombd, tipo;
        while (filestr.good()) {
		getline(filestr,nombre,',');
		getline(filestr,ano,',');
		getline(filestr,nombd,',');
		if (getline(filestr,tipo,'\n') ) {
			QString eltipo=tipo.c_str();
			fprintf(stderr,"%s\n",tipo.c_str());
			if (eltipo == m_tipo || m_tipo=="") {
				insertCompany(nombre.c_str(),ano.c_str(),nombd.c_str(),tipo.c_str());
			}// end if
		}// end if
        }// end while
   filestr.close();
}// end cargaArchivo

/** \brief Guarda en el archivo de empresas las empresas disponibles
  * También actualiza el listado de empresas visibles.
  */
void abreempresaview::guardaArchivo() {
	string dir1 = getenv("HOME");
	dir1 = dir1 + "/.bulmages/"+LISTEMPRESAS;
	ofstream filestr((char *) dir1.c_str());
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
			if (tipo == m_tipo || m_tipo=="") {
                        	insertCompany(nombre,ano,nomdb,tipo);
			}// end if
			// Independientemente de si deben mostrarse o no hay que guardarlas en el archivo.
			filestr << nombre << ",";
			filestr << ano << ",";
			filestr << nomdb << ",";
			filestr << tipo  << endl;
                       	nomdb="";
                }// end if
                db1->commit();
                delete db1;
                curs->siguienteregistro();
        }// end while
        delete curs;
        delete db;
        confpr->setValor(CONF_ALERTAS_DB,"Yes");
	filestr.close();
}// end guardaArchivo


void abreempresaview::s_reloadButton() {
	empresas->clear();
	guardaArchivo();
}// end s_reloadButton

