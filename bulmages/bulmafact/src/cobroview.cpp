/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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


#include "cobroview.h"

#include "company.h"
#include "configuracion.h"
#include "busquedacliente.h"
#include "busquedafecha.h"
#include <QMessageBox>

#include <Q3Table>
#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <Q3PopupMenu>
#include <QToolButton>
#include <QFile>
#include <QTextStream>
#include <QLayout>
#include <QMessageBox>

#include <QCloseEvent>

#include <fstream>
using namespace std;

#include "funcaux.h"

#define COL_IDLPRESUPUESTO 0
#define COL_IDARTICULO 1
#define COL_CODARTICULO 2
#define COL_NOMARTICULO 3
#define COL_DESCLPRESUPUESTO 4
#define COL_CANTLPRESUPUESTO 5
#define COL_PVPLPRESUPUESTO 6
#define COL_DESCUENTOLPRESUPUESTO 7
#define COL_IDPRESUPUESTO 8
#define COL_REMOVE 9
#define COL_TASATIPO_IVA 10
#define COL_TIPO_IVA 11

#define COL_DESCUENTO_IDDPRESUPUESTO 0
#define COL_DESCUENTO_CONCEPTDPRESUPUESTO 1
#define COL_DESCUENTO_PROPORCIONDPRESUPUESTO 2
#define COL_DESCUENTO_REMOVE 3

#define coma "'"


CobroView::CobroView( company *comp , QWidget *parent, const char *name) : QDialog(parent, name, Qt::WDestructiveClose) , Cobro(comp) ,dialogChanges(this) {
	_depura("CobroView::CobroView",0);
	setupUi(this);
    /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
    mui_cliente->setcompany(comp);
    dialogChanges_cargaInicial();
    _depura("END CobroView::CobroView",0);
}


CobroView::~CobroView() {
    companyact->sacaWindow(this);
}


void CobroView::on_mui_borrar_clicked() {
    _depura("CobroView::on_mui_borrar_clicked\n",0);
    if (QMessageBox::warning( this, tr("BulmaFact - Presupuestos"), tr("Desea borrar este presupuesto"), tr("&Si"), tr("&No")) == 0) {
        borrar();
    }// end if
}


void CobroView::closeEvent( QCloseEvent *e) {
	_depura("closeEvent",0);
    if (dialogChanges_hayCambios())  {
        int val = QMessageBox::warning( this, tr("Guardar cobro"),
                                   tr("Desea guardar los cambios?"),tr("&Si"),tr("&No"),tr("&Cancelar"),0,2);
	if (val == 0) 
            on_mui_guardar_clicked();
	if (val == 2)
	    e->ignore();
    }// end if	
}

