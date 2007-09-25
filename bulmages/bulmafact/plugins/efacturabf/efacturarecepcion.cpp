/***************************************************************************
 *   Copyright (C) 2006 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
 *   Contribution by Tomeu Borras tborras@conetxia.com			   *
 *									   *
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

#include "QString"

#include "efacturarecepcion.h"
#include "funcaux.h"
#include <QFileDialog>
#include <QLineEdit>

#define EMAIL "efactura@conetxia.com"
#define DIREMAIL "/home/arturo/efacturamail"


///
/**
\param emp
\param parent
**/
EFacturaRecepcion::EFacturaRecepcion(Company *emp, QWidget *parent) : FichaBf(emp, parent) {
        _depura("EFacturaRecepcion::EFacturaRecepcion", 0);
	QString query;
	
	setupUi(this);  // Para que el form se inicialice bien
        _depura("END EFacturaRecepcion::EFacturaRecepcion", 0);
}


///
/**
**/
EFacturaRecepcion::~EFacturaRecepcion() {
        _depura("EFacturaRecepcion::~EFacturaRecepcion", 0);
        _depura("EFacturaRecepcion::~EFacturaRecepcion", 0);
}


///
/**
\return
**/
void EFacturaRecepcion::on_mui_recibir_clicked() {
	_depura("EFacturaRecepcion::on_mui_salir_clicked", 0);
	
	/// Obtenemos los valores de configuracion necesarios para obtener eFacturas por e-mail
	
	QString query = "SELECT valor FROM configuracion WHERE nombre = 'eFactura_dirEmail'";
	cursor2 *dirEmail = empresaBase()->cargacursor(query);

	query = "SELECT valor FROM configuracion WHERE nombre = 'eFactura_serverRec'";
	cursor2 *serverRec = empresaBase()->cargacursor(query);
	
	query = "SELECT valor FROM configuracion WHERE nombre = 'eFactura_Email'";
	cursor2 *Email = empresaBase()->cargacursor(query);

	query = "SELECT valor FROM configuracion WHERE nombre = 'eFactura_Password'";
	cursor2 *Password = empresaBase()->cargacursor(query);

	/// Comprobamos que el directorio especificado existeAboutView
	
	QDir *direfactura = new QDir(dirEmail->valor("valor"));
	
	if (!direfactura->exists()) {
		_depura("El directorio especificado en la configuracion para la eFactura no existe. Por favor, creelo e intentelo de nuevo", 2);
		
		return;
	}
		
	/// Llamamos al script para recibir e-mails con los parametros obtenidos
	
	QString llamada_qstring = "cd " + dirEmail->valor("valor") + " ; python /home/arturo/bulmages/trunk/bulmages/installbulmages/efactura/recibemail.py " + serverRec->valor("valor") + " " + Email->valor("valor") + " " + Password->valor("valor");
	
	char llamada[512];
	
	strncpy(llamada, llamada_qstring.toLatin1(), sizeof(llamada)-1);
		
	system(llamada);
	
	/// Obtenemos los directorios alojados en nuestra carpeta de eFacturas recibidas
	
	// Filtramos solo por directorios que no sean enlaces
	direfactura->setFilter(QDir::Dirs);
	// Ordenamos por fecha de modificacion
	direfactura->setSorting(QDir::Name);
	
	QFileInfoList *listadirs = new QFileInfoList(direfactura->entryInfoList());
	
	mui_facturasRecibidas->setColumnCount(2);
	
	int cont = 0;
	
	mui_facturasRecibidas->setRowCount(0);
	
	QFileInfo *fileInfo = NULL;
	QTableWidgetItem *item = NULL;
	QFileInfoList *archivos = NULL;

	for (int i = 0; i < listadirs->size(); i++) {
		
		fileInfo = new QFileInfo(listadirs->at(i));
		
		/// Evitamos los directorios "." y ".."
		
		if (!fileInfo->fileName().startsWith(".")) {
			
			/// Entramos en el directorio
			
			if (!direfactura->cd(fileInfo->fileName())) {
				_depura("Error intentando entrar en " + fileInfo->fileName(), 2);
				continue;
			}
			
			/// Buscamos solo archivos
			
			direfactura->setFilter(QDir::Files);
			
			/// Obtenemos la lista de archivos (en teoria solo debe de haber uno, el XML con la efactura)
			
			archivos = new QFileInfoList(direfactura->entryInfoList());
			
			if (archivos->size() > 1) {
				_depura("El directorio " + direfactura->absolutePath() + " contiene mas de un archivo. Por favor, arreglelo dejando solo el fichero XML que contenga la e-factura.", 2);
				direfactura->cdUp();
				continue;
			}
			
			/// Obtenemos info del archivo
			
			fileInfo = new QFileInfo(archivos->at(0));
			
			/// Creamos una nueva fila y la usamos
			
			mui_facturasRecibidas->insertRow(cont);
			
			/// Nombre del fichero
			
			item = new QTableWidgetItem(fileInfo->fileName());
			mui_facturasRecibidas->setItem(cont, 0, item);
			
			/// Nombre del directorio que lo contiene
			
			item = new QTableWidgetItem(direfactura->absolutePath());
			mui_facturasRecibidas->setItem(cont, 1, item);
			
			cont++;
			
			/// Volvemos al directorio de efacturas
			
			direfactura->cdUp();
			
		} // end if
	}
	
	/// Obtenemos el nombre del archivo XML incluido en cada uno de ellos y
	/// los mostramos en pantalla
		
	/// Liberamos memoria con los datos que ya no necesitamos
	
// 	delete item;
	delete fileInfo;
	delete direfactura;
	delete dirEmail;
	delete serverRec;
	delete Email;
	delete Password;

	_depura("END EFacturaRecepcion::on_mui_salir_clicked", 0);
}


///
/**
**/
void EFacturaRecepcion::on_mui_abrir_clicked() {
	_depura("EFacturaRecepcion::on_mui_salir_clicked", 0);

	_depura("END EFacturaRecepcion::on_mui_salir_clicked", 0);
}


///
/**
**/
void EFacturaRecepcion::on_mui_salir_clicked() {
	_depura("EFacturaRecepcion::on_mui_salir_clicked", 0);
	
	close();
	
	_depura("END EFacturaRecepcion::on_mui_salir_clicked", 0);
}
