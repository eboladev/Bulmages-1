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
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QMessageBox>

#include "bnuevaempresa.h"
#include "abreempresaview.h"


BNuevaEmpresa::BNuevaEmpresa(QWidget *parent, Qt::WFlags f)
        : QDialog(parent, f) {
    setupUi(this);

    QObject::connect(PushButton1, SIGNAL(clicked()), this, SLOT(accept()));
    QObject::connect(PushButton1_2, SIGNAL(clicked()), this, SLOT(close()));
}


BNuevaEmpresa::~BNuevaEmpresa() {}


void BNuevaEmpresa::accept() {
    /// Falta comprobar que tengas permisos para crear nuevas empresas.
    QString nombredb;
    QString nombreEmp;
    QString ejemp = ejercicioempresa->text();

    nombredb = bdempresa->text().trimmed()+ejercicioempresa->text().trimmed();
    nombreEmp = nombreempresa->text().trimmed();

    /// Comprobamos que se han introducido todos los datos para avisar en caso contrario.
    if ((nombredb == "") || (nombreEmp == "") || (ejemp == "") ) {
        QMessageBox::warning(this, tr("Datos incompletos"),
                             tr("No puede dejar campos en blanco para crear una nueva empresa.\n"), tr("&Aceptar"), tr("&Cancelar"), 0, 0 );
        return;
    } // end if

    QString cadena = confpr->valor(CONF_PROGDATA);
    cadena += "dbmodels/creabulmacont --texto " + nombredb + " 1 " + nombreEmp + " " + ejercicioempresa->text().trimmed() + " " + "";
    system(cadena.toAscii().constData());
    /// Hacemos una recarga de empresas pq sabemos a ciencia cierta que ha cambiado el listado.
    abreempresaview *abre = new abreempresaview(NULL, "hola", "hola");
    abre->on_mui_actualizar_clicked();
    delete abre;
    close();
}

