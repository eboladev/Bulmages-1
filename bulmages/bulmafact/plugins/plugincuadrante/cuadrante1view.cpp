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

#include "cuadrante1view.h"
#include "company.h"
#include "configuracion.h"
#include "plugins.h"

#include "funcaux.h"


/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
Cuadrante1View::Cuadrante1View(Company *comp, QWidget *parent)
        : FichaBf(comp, parent) {
    _depura("Cuadrante1View::Cuadrante1View", 0);
    setAttribute(Qt::WA_DeleteOnClose);
    try {
        setupUi(this);

        /// Lanzamos los plugins.
        if (g_plugins->lanza("Cuadrante1View_Cuadrante1View", this)) return;

        setTitleName(tr("Cuadrante"));
        setDBTableName("cuadrante");
        setDBCampoId("idcuadrante");
        addDBCampo("idcuadrante", DBCampo::DBint, DBCampo::DBPrimaryKey, tr( "ID cuadrante"));
        addDBCampo("idalmacen", DBCampo::DBint, DBCampo::DBNoSave, tr( "Almacen"));
        addDBCampo("fechacuadrante", DBCampo::DBdate, DBCampo::DBNoSave  , tr( "Fecha"));
        addDBCampo("comentcuadrante", DBCampo::DBvarchar, DBCampo::DBNothing, tr( "Comentarios"));
        addDBCampo("aperturacuadrante", DBCampo::DBvarchar, DBCampo::DBNothing, tr( "Apertura Mañana"));
        addDBCampo("cierrecuadrante", DBCampo::DBvarchar, DBCampo::DBNothing, tr( "Cierre Mañana"));
        addDBCampo("apertura1cuadrante", DBCampo::DBvarchar, DBCampo::DBNothing, tr( "Apartura Tarde"));
        addDBCampo("cierre1cuadrante", DBCampo::DBvarchar, DBCampo::DBNothing, tr( "Cierre Tarde"));
        addDBCampo("fiestacuadrante", DBCampo::DBboolean, DBCampo::DBNothing, tr( "Festivo"));

        mui_idalmacen->setEmpresaBase(comp);

        mui_list->setEmpresaBase(comp);
        mui_list->setDBTableName("horario");
        mui_list->setDBCampoId("idhorario");
        mui_list->addSHeader("nomtrabajador",  DBCampo::DBvarchar, DBCampo::DBNoSave,     SHeader::DBNone,                         tr("Nombre Trabajador"));
        mui_list->addSHeader("horainhorario",  DBCampo::DBvarchar, DBCampo::DBNothing,    SHeader::DBNone,                         tr("Hora Inicio"));
        mui_list->addSHeader("horafinhorario", DBCampo::DBvarchar, DBCampo::DBNothing,    SHeader::DBNone,                         tr("Hora Fin"));
        mui_list->addSHeader("idcuadrante",    DBCampo::DBint,     DBCampo::DBNothing,    SHeader::DBNoView | SHeader::DBNoWrite,  tr("ID Cuadrante"));
        mui_list->addSHeader("idhorario",      DBCampo::DBint,     DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite,  tr("ID horario"));
        mui_list->addSHeader("idtrabajador",   DBCampo::DBint,     DBCampo::DBNotNull,    SHeader::DBNoView | SHeader::DBNoWrite,  tr("ID trabajador"));
        mui_list->setinsercion(TRUE);
        mui_list->setDelete(TRUE);
        mui_list->setSortingEnabled(FALSE);

        dialogChanges_cargaInicial();
        meteWindow(windowTitle(), this, FALSE);
    } catch (...) {
        mensajeInfo(tr("Error al crear el almacen"));
    } // end try
    _depura("END Cuadrante1View::Cuadrante1View", 0);
}


/** No precisa acciones adicionales en el destructor.
*/
Cuadrante1View::~Cuadrante1View() {
    _depura("Cuadrante1View::~Cuadrante1View", 0);
    _depura("END Cuadrante1View::~Cuadrante1View", 0);
}

int Cuadrante1View::cargarPost(QString idcuadrante) {
    mui_list->cargar("SELECT * FROM horario LEFT JOIN trabajador ON horario.idtrabajador = trabajador.idtrabajador WHERE idcuadrante=" + idcuadrante);
    return 0;
}

int Cuadrante1View::guardarPost() {
	mui_list->setColumnValue("idcuadrante", DBvalue("idcuadrante"));
	mui_list->guardar();
	emit(save());
	return 0;
}



