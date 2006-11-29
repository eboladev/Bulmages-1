/***************************************************************************
 *   Copyright (C) 2006 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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

#include "efactura.h"
#include "funcaux.h"
#include <QFileDialog>
#include <QLineEdit>
#include <QString>

/// Constructor de la clase EFactura (el form)
/// Pasamos como parametro la empresa para poder usar metodos de BD para
/// guardar los datos de configuracion

EFactura::EFactura(company *emp, QWidget *parent) : QWidget(parent) {
	QString query;
	
	setupUi(this);  // Para que el form se inicialice bien
	
	m_companyact = emp;
	
	query = "SELECT valor FROM configuracion WHERE nombre = 'eFactura_server'";
	cursor2 *cur1 = m_companyact->cargacursor(query);
	mui_URLServidorTiempo->setText(cur1->valor("valor"));
	delete cur1;
	
	query = "SELECT valor FROM configuracion WHERE nombre = 'eFactura_certificado'";
	cursor2 *cur2 = m_companyact->cargacursor(query);
	mui_ficheroECertificado->setText(cur2->valor("valor"));
	delete cur2;
	
	query = "SELECT valor FROM configuracion WHERE nombre = 'eFactura_server_valida'";
	cursor2 *cur3 = m_companyact->cargacursor(query);
	mui_URLServidorValidaCert->setText(cur3->valor("valor"));
	delete cur3;
}

/// Destructor

EFactura::~EFactura() {}

/// Funcionalidad del boton guardar

void EFactura::on_mui_guardar_clicked() {
	
	QString query;

	_depura("EFactura::on_mui_guardar_clicked", 0);
	
	if (mui_URLServidorTiempo->isModified()) {
		query = "DELETE FROM configuracion WHERE nombre = 'eFactura_server'";
		m_companyact->ejecuta(query);
		
		query = "INSERT INTO configuracion (nombre, valor) VALUES ('eFactura_server', '";
		query += mui_URLServidorTiempo->text();
		query += "')";
		m_companyact->ejecuta(query);
	}
	
	/// Como el setText() pone siempre a false el valor de retorno de isModified()
	/// hay que hacer siempre el update del campo que indica la ruta del fichero
	/// de certificado digital
	
	query = "DELETE FROM configuracion WHERE nombre = 'eFactura_certificado'";
	m_companyact->ejecuta(query);
	
	query = "INSERT INTO configuracion (nombre, valor) VALUES ('eFactura_certificado', '";
	query += mui_ficheroECertificado->text();
	query += "')";
	m_companyact->ejecuta(query);
}

/// Funcionalidad del boton Examinar en el apartado del certificado

void EFactura::on_mui_examinaECertificado_clicked() {
	_depura("EFactura::on_mui_examinar_clicked", 0);
	
	QString s = QFileDialog::getOpenFileName(
			this,
			"Escoja un fichero por favor",
			"/home",
			"Todos los archivos (*)"
			);

	/// Si se le da a cancelar, s devuelve NULL y se queda el campo de texto vacio
	
	if (s != NULL)
		mui_ficheroECertificado->setText(s);
}

/// Funcionalidad del boton cancelar

void EFactura::on_mui_cancelar_clicked() {
	_depura("EFactura::on_mui_cancelar_clicked", 2);
	
// 	QString query = "SELECT * FROM configuracion";
// 	cursor2 *cur = m_companyact->cargacursor(query);
// 	
// 	while(!cur->eof()) {
// 		_depura(cur->valor("nombre"), 0);
// 		cur->siguienteregistro();
// 	} // end while
// 	
// 	delete cur;
// 	
// 	query = "UPDATE configuracion SET nombre='nombre' WHRE nombre='elnombre'";
// 	m_companyact->begin();
// 	m_companyact->ejecuta(query);
// 	m_companyact->rollback();	
}
