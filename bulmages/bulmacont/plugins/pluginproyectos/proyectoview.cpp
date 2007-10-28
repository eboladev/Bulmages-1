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

#include <QMessageBox>
#include <QCloseEvent>
#include <QFile>
#include <QTextStream>

#include <fstream>

#include "proyectoview.h"
#include "empresa.h"
#include "configuracion.h"
#include "plugins.h"

#include "funcaux.h"


/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
/**
\param comp
\param parent
**/
ProyectoView::ProyectoView(Empresa *comp, QWidget *parent)
        : FichaBc(comp, parent) {
    _depura("ProyectoView::ProyectoView", 0);
    setAttribute(Qt::WA_DeleteOnClose);
    try {
        setupUi(this);

	/// Lanzamos los plugins.
	if (g_plugins->lanza("ProyectoView_ProyectoView", this)) return;

        setTitleName(tr("Proyecto"));
        setDBTableName("presupuestoc");
        setDBCampoId("idpresupuestoc");
        addDBCampo("idpresupuestoc", DBCampo::DBint, DBCampo::DBPrimaryKey, tr( "ID almacen"));
        addDBCampo("idc_coste", DBCampo::DBint, DBCampo::DBNotNull, tr("Id CCoste"));
        addDBCampo("nompresupuestoc", DBCampo::DBvarchar, DBCampo::DBNotNull, tr("Nom Presupuesto"));
        addDBCampo("fechapresupuestoc", DBCampo::DBdate, DBCampo::DBNothing, tr("fecha"));
	addDBCampo("comentpresupuestoc", DBCampo::DBvarchar, DBCampo::DBNothing, tr("Comentarios"));

	mui_idc_coste->setEmpresaBase(comp);
        mui_idc_coste->setidc_coste("");

        dialogChanges_cargaInicial();
        meteWindow(windowTitle(), this, FALSE);
    } catch (...) {
        mensajeInfo(tr("Error al crear el almacen"));
    } // end try
    _depura("END ProyectoView::ProyectoView", 0);
}


/// No precisa acciones adicionales en el destructor.

/**
**/
ProyectoView::~ProyectoView() {
    _depura("ProyectoView::~ProyectoView", 0);
    _depura("END ProyectoView::~ProyectoView", 0);
}






