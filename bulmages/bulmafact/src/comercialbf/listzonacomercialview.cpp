/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C. (Porting to QT4)              *
 *   fcojavmc@todo-redes.com                                               *
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
#include <Q3Table>
#include <QWidget>
#include <QComboBox>
#include <Q3PopupMenu>
#include <QToolButton>
#include <QTextStream>
#include <QLayout>
#include <QMessageBox>
#include <fstream>


#include "listzonacomercialview.h"
#include "company.h"
#include "configuracion.h"
#include "busquedacliente.h"
#include "busquedafecha.h"
using namespace std;
#include "funcaux.h"

// DEBUGMODE => 0 = Disabled; 1 = Enabled;
#define DEBUGMODE 1


ListZonaComercialView::ListZonaComercialView( company *comp , QWidget *parent, const char *name) : QDialog(parent, name, Qt::WDestructiveClose) {
	_depura("INIT_ListZonaComercialView::ListZonaComercialView", DEBUGMODE);
	setupUi(this);
	companyact = comp;
	mui_listado->setcompany(comp);
	mui_listado->cargar();
	_depura("END_ListZonaComercialView::ListZonaComercialView", DEBUGMODE);
}


ListZonaComercialView::~ListZonaComercialView() {
}




/// ===================================== SUBFORMULARIO ===============================================
ListZonaComercialSubForm::ListZonaComercialSubForm(QWidget *parent, const char *) : SubForm2Bf(parent) {
    setDBTableName("zonacomercial");
    setDBCampoId("idzonacomercial");
    addSHeader("idzonacomercial", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, "idzonacomercial");
    addSHeader("nomzonacomercial", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, "nomzonacomercial");
	setinsercion(TRUE);
}
