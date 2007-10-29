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

        /// Inicializamos componentes.
        mui_idc_coste->setEmpresaBase(comp);
        mui_idc_coste->setidc_coste("");
        mui_ingresos->setEmpresaBase(comp);
        mui_gastos->setEmpresaBase(comp);

        /// Inicializamos el listado.
        mui_ingresos->setDBTableName("lingpresupuestoc");
        mui_ingresos->setDBCampoId("idlingpresupuestoc");
        mui_ingresos->addSHeader("idcuenta", DBCampo::DBint, DBCampo::DBNothing, SHeader::DBNoView, tr("Id. Cuenta"));
        mui_ingresos->addSHeader("codigo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, tr("Codigo"));
        mui_ingresos->addSHeader("descripcioncuenta", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, tr("Codigo"));
        mui_ingresos->addSHeader("cantlingpresupuestoc", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("Cantidad"));
        mui_ingresos->addSHeader("precioundlingpresupuestoc", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("Precio Und"));
        mui_ingresos->addSHeader("ejerciciolingpresupuestoc", DBCampo::DBint, DBCampo::DBNothing, SHeader::DBNone, tr("Ejercicio"));
        mui_ingresos->addSHeader("comentlingpresupuestoc", DBCampo::DBvarchar, DBCampo::DBNothing, SHeader::DBNone, tr("Comentarios"));
        mui_ingresos->addSHeader("desclingpresupuestoc", DBCampo::DBvarchar, DBCampo::DBNothing, SHeader::DBNone, tr("Descripcion"));
        mui_ingresos->addSHeader("idlingpresupuestoc", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView , tr("Id linea de Ingreso Presupuesto"));
        mui_ingresos->addSHeader("idpresupuestoc", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Id. Proyecto"));
        mui_ingresos->addSHeader("ordenlingpresupuestoc", DBCampo::DBint, DBCampo::DBNothing, SHeader::DBNoView, tr("Orden"));
        mui_ingresos->setinsercion(TRUE);
        mui_ingresos->setOrdenEnabled(TRUE);
        mui_ingresos->setOrdenPorQuery(FALSE);


        /// Inicializamos el listado.
        mui_gastos->setDBTableName("lgaspresupuestoc");
        mui_gastos->setDBCampoId("idlgaspresupuestoc");
        mui_gastos->addSHeader("idcuenta", DBCampo::DBint, DBCampo::DBNothing, SHeader::DBNoView, tr("Id. Cuenta"));
        mui_gastos->addSHeader("codigo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, tr("Codigo"));
        mui_gastos->addSHeader("descripcioncuenta", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, tr("Codigo"));
        mui_gastos->addSHeader("cantlgaspresupuestoc", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("Cantidad"));
        mui_gastos->addSHeader("precioundlgaspresupuestoc", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("Precio Und"));
        mui_gastos->addSHeader("ejerciciolgaspresupuestoc", DBCampo::DBint, DBCampo::DBNothing, SHeader::DBNone, tr("Ejercicio"));
        mui_gastos->addSHeader("comentlgaspresupuestoc", DBCampo::DBvarchar, DBCampo::DBNothing, SHeader::DBNone, tr("Comentarios"));
        mui_gastos->addSHeader("desclgaspresupuestoc", DBCampo::DBvarchar, DBCampo::DBNothing, SHeader::DBNone, tr("Descripcion"));
        mui_gastos->addSHeader("idlgaspresupuestoc", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView , tr("Id linea de Ingreso Presupuesto"));
        mui_gastos->addSHeader("idpresupuestoc", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Id. Proyecto"));
        mui_gastos->addSHeader("ordenlgaspresupuestoc", DBCampo::DBint, DBCampo::DBNothing, SHeader::DBNoView, tr("Orden"));
        mui_gastos->setinsercion(TRUE);
        mui_gastos->setOrdenEnabled(TRUE);
        mui_gastos->setOrdenPorQuery(FALSE);


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



///
/**
\param idcuadrante
\return
**/
int ProyectoView::cargarPost(QString idcuadrante) {
    mui_ingresos->cargar("SELECT idlingpresupuestoc, idpresupuestoc, ordenlingpresupuestoc, lingpresupuestoc.idcuenta, cantlingpresupuestoc, precioundlingpresupuestoc, ejerciciolingpresupuestoc, comentlingpresupuestoc, desclingpresupuestoc, t1.codigo, t1.descripcion AS descripcioncuenta FROM lingpresupuestoc LEFT JOIN (SELECT *  FROM cuenta) AS t1 ON lingpresupuestoc.idcuenta = t1.idcuenta WHERE idpresupuestoc=" + DBvalue("idpresupuestoc"));
    mui_gastos->cargar("SELECT idlgaspresupuestoc, idpresupuestoc, ordenlgaspresupuestoc, lgaspresupuestoc.idcuenta, cantlgaspresupuestoc, precioundlgaspresupuestoc, ejerciciolgaspresupuestoc, comentlgaspresupuestoc, desclgaspresupuestoc, t1.codigo, t1.descripcion AS descripcioncuenta  FROM lgaspresupuestoc LEFT JOIN (SELECT *  FROM cuenta) AS t1 ON lgaspresupuestoc.idcuenta = t1.idcuenta WHERE idpresupuestoc=" + DBvalue("idpresupuestoc"));
    return 0;
}


///
/**
\return
**/
int ProyectoView::guardarPost() {
	mui_ingresos->setColumnValue("idpresupuestoc", DBvalue("idpresupuestoc"));
	mui_ingresos->guardar();
	mui_gastos->setColumnValue("idpresupuestoc", DBvalue("idpresupuestoc"));
	mui_gastos->guardar();
	emit(save());
	return 0;
}



