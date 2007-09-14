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

#include "almacenview.h"
#include "company.h"
#include "configuracion.h"
#include "plugins.h"

#include "funcaux.h"


/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
AlmacenView::AlmacenView(Company *comp, QWidget *parent)
        : FichaBf(comp, parent) {
    _depura("AlmacenView::AlmacenView", 0);
    setAttribute(Qt::WA_DeleteOnClose);
    try {
        setupUi(this);

	/// Lanzamos los plugins.
	if (g_plugins->lanza("AlmacenView_AlmacenView", this)) return;

        setTitleName(tr("Almacen"));
        setDBTableName("almacen");
        setDBCampoId("idalmacen");
        addDBCampo("idalmacen", DBCampo::DBint, DBCampo::DBPrimaryKey, tr( "ID almacen"));
        addDBCampo("codigoalmacen", DBCampo::DBvarchar, DBCampo::DBNotNull, tr( "Codigo almacen"));
        addDBCampo("nomalmacen", DBCampo::DBvarchar, DBCampo::DBNotNull  , tr( "Nombre almacen"));
        addDBCampo("diralmacen", DBCampo::DBvarchar, DBCampo::DBNothing, tr( "Direccion almacen"));
        addDBCampo("poblalmacen", DBCampo::DBvarchar, DBCampo::DBNothing, tr( "Poblacion almacen"));
        addDBCampo("cpalmacen", DBCampo::DBvarchar, DBCampo::DBNothing, tr( "Codigo postal"));
        addDBCampo("telalmacen", DBCampo::DBvarchar, DBCampo::DBNothing, tr( "Telefono almacen"));
        addDBCampo("faxalmacen", DBCampo::DBvarchar, DBCampo::DBNothing, tr( "Fax almacen"));
        addDBCampo("emailalmacen", DBCampo::DBvarchar, DBCampo::DBNothing, tr( "email almacen"));
        addDBCampo("fechaalmacen", DBCampo::DBdate, DBCampo::DBNoSave, tr( "Fecha almacen"));

        dialogChanges_cargaInicial();
        meteWindow(windowTitle(), this, FALSE);
    } catch (...) {
        mensajeInfo(tr("Error al crear el almacen"));
    } // end try
    _depura("END AlmacenView::AlmacenView", 0);
}


/** No precisa acciones adicionales en el destructor.
*/
AlmacenView::~AlmacenView() {
    _depura("AlmacenView::~AlmacenView", 0);
    _depura("END AlmacenView::~AlmacenView", 0);
}






