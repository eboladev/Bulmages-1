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

#include "QString"

#include "efacturarecepcion.h"
#include "funcaux.h"
#include <QFileDialog>
#include <QLineEdit>

#define EMAIL "efactura@conetxia.com"
#define DIREMAIL "/home/arturo/efacturamail"

EFacturaRecepcion::EFacturaRecepcion(company *emp, QWidget *parent) : QWidget(parent) {
	QString query;
	
	setupUi(this);  // Para que el form se inicialice bien
	
	//m_companyact = emp;
}

EFacturaRecepcion::~EFacturaRecepcion() {}

void EFacturaRecepcion::on_mui_recibir_clicked() {
	_depura("EFacturaRecepcion::on_mui_salir_clicked", 0);
	
	QString diremail = "/home/arturo/efacturamail";

	_depura("recibiendo...", 2);
	system("cd /home/arturo/efacturamail ; python /usr/lib/bulmages/recibemail.py mail.conetxia.com efactura@conetxia.com art5r4");
	_depura("recepcion completada", 2);

	_depura("END EFacturaRecepcion::on_mui_salir_clicked", 0);
}

void EFacturaRecepcion::on_mui_abrir_clicked() {
	_depura("EFacturaRecepcion::on_mui_salir_clicked", 0);

	_depura("END EFacturaRecepcion::on_mui_salir_clicked", 0);
}

void EFacturaRecepcion::on_mui_salir_clicked() {
	_depura("EFacturaRecepcion::on_mui_salir_clicked", 0);
	close();
	_depura("END EFacturaRecepcion::on_mui_salir_clicked", 0);
}
