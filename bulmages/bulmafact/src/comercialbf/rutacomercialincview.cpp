/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borr� Riera                              *
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

#include "rutacomercialincview.h"

#include "company.h"
#include "configuracion.h"
#include "busquedacliente.h"
#include "busquedafecha.h"
#include <QMessageBox>

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
#define coma "'"

RutaComercialIncView::RutaComercialIncView( company *comp , QWidget *parent, const char *name) : QWidget(parent, name, Qt::WDestructiveClose) {
	setupUi(this);
    _depura("RutaComercialIncView::RutaComercialIncView",0);
	m_companyact = comp;
    mui_rutacomercial->setcompany(comp);
    mui_incidenciacomercial->setcompany(comp);
    m_companyact->meteWindow(caption(),this);
    _depura("END RutaComercialIncView::RutaComercialIncView",0);
}


RutaComercialIncView::~RutaComercialIncView() {
    m_companyact->sacaWindow(this);
    _depura("END RutaComercialIncView::~RutaComercialIncView",0);
}




void RutaComercialIncView::on_mui_borrar_clicked() {
    _depura("RutaComercialIncView::on_mui_borrar_clicked\n",0);
    if (QMessageBox::warning( this, "BulmaFact - Rutas Comerciales", "Desea borrar esta ruta", "Si", "No") == 0) {
        mui_rutacomercial->borrar();
	mui_incidenciacomercial->borrar();
    }// end if
}



void RutaComercialIncView::closeEvent( QCloseEvent *e) {
	_depura("closeEvent",0);
}






