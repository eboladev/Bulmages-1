/***************************************************************************
 *   Copyright (C) 2006 by Tomeu Borras Riera                              *
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

#include "ficha.h"


Ficha::Ficha(QWidget *parent, Qt::WFlags f) : QWidget(parent, f), dialogChanges(this) {}


void Ficha::on_mui_aceptar_clicked() {
	_depura("Ficha::on_mui_aceptar_clicked", 0);
	try {
		if (guardar()) throw -1;
		close();
	} catch (...) {
	} // end try
}


void Ficha::on_mui_borrar_clicked() {
    _depura("Ficha::on_mui_borrar_clicked", 0);
    int val = QMessageBox::warning(this,
                                   tr("Borrar ")+windowTitle(),
                                   tr("Desea eliminar la ")+windowTitle(),
                                   tr("&Si"), tr("&No"), tr("&Cancelar"), 0, 2);
    if (val == 0) {
        if (!borrar()) {
            dialogChanges_cargaInicial();
            _depura(windowTitle()+" borrado satisfactoriamente.", 2);
            close();
        } // end if
    } // end if
    _depura("END Ficha::on_mui_borrar_clicked", 0);
}


void Ficha::closeEvent(QCloseEvent *e) {
    _depura("Ficha::closeEvent", 0);
    try {
	if (dialogChanges_hayCambios())  {
		int val = QMessageBox::warning(this,
					tr("Guardar ")+windowTitle(),
					tr("Desea guardar los cambios?"),
					tr("&Si"), tr("&No"), tr("&Cancelar"), 0, 2);
		if (val == 0)
		guardar();
		if (val == 2)
		e->ignore();
	} // end if
    } catch(...) {
	e->ignore();
    } // end try
}

