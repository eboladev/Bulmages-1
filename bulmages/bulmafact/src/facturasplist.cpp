/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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
#include <QFile>
#include <QTextStream>
#include <QString>

#include "facturasplist.h"
#include "company.h"
#include "busquedaproveedor.h"
#include "busquedaarticulo.h"
#include "configuracion.h"
#include "facturapview.h"
#include "plugins.h"

/** Inicializa todos los componentes del listado a NULL para que no exista consusion sobre
    si un elemento ha sido inicializado o no.
    Al construir la clase con este constructor debe ser inicializada con setcompany().
    Mete la ventana en el workSpace.
*/
FacturasProveedorList::FacturasProveedorList(QWidget *parent, Qt::WFlags flag)
        : Ficha(parent, flag) {
    _depura("FacturasProveedorList::FacturasProveedorList", 0);
    setupUi(this);
    /// Disparamos los plugins.
    int res = g_plugins->lanza("FacturasProveedorList_EFacturaBotonImportar", this);
    if (res != 0) {
        return;
    } // end if
    m_companyact = NULL;
    m_modo = 0;
    mdb_idfacturap = "";
    meteWindow(windowTitle(), this);
    hideBusqueda();
    _depura("FacturasProveedorList::FacturasProveedorList", 0);
}


/** Inicializa todos los componentes, hace la carga inicial con \ref presenta()
    mete la ventana en el workSpace().
*/
FacturasProveedorList::FacturasProveedorList(company *comp, QWidget *parent)
        : Ficha(parent) {
    _depura("FacturasProveedorList::FacturasProveedorList", 0);
    setupUi(this);

    /// Disparamos los plugins.
    int res = g_plugins->lanza("FacturasProveedorList_FacturasProveedorList", this);
    if (res != 0) {
    	return;
    } // end if
    m_companyact = comp;
    m_proveedor->setcompany(m_companyact);
    m_articulo->setcompany(m_companyact);
    mui_list->setcompany(comp);
    presenta();
    m_modo = 0;
    mdb_idfacturap = "";
    meteWindow(windowTitle(), this);
    hideBusqueda();
    _depura("END FacturasProveedorList::FacturasProveedorList", 0);
}


/** No precisa de acciones especiales en el destructor.
*/
FacturasProveedorList::~FacturasProveedorList() {
    _depura("FacturasProveedorList::~FacturasProveedorList", 0);
    m_companyact->sacaWindow(this);
    _depura("END FacturasProveedorList::~FacturasProveedorList", 0);
}


/** Hace la carga inicial del listado.
    Tambien hace el calculo de totales y lo presenta.
*/
void FacturasProveedorList::presenta() {
    _depura("FacturasProveedorList::presenta", 0);
    mui_list->cargar("SELECT *, totalfacturap AS total, bimpfacturap AS base, impfacturap AS impuestos  FROM facturap LEFT JOIN proveedor ON facturap.idproveedor=proveedor.idproveedor WHERE 1=1  " + generaFiltro());

    /// Hacemos el calculo del total.
    cursor2 *cur = m_companyact->cargacursor("SELECT SUM(totalfacturap) AS total FROM facturap LEFT JOIN proveedor ON facturap.idproveedor=proveedor.idproveedor WHERE 1=1  " + generaFiltro());
    m_total->setText(cur->valor("total"));
    delete cur;
    _depura("END FacturasProveedorList::presenta", 0);
}


/** Metodo auxiliar que crea la clausula WHERE del query de carga  \ref presenta() 
    La clausula WHERE utiliza todas las opciones de filtrado para crearse.
*/
QString FacturasProveedorList::generaFiltro() {
    _depura("FacturasProveedorList::generaFiltro", 0);
    /// Tratamiento de los filtros.
    QString filtro = "";
    if (m_filtro->text() != "") {
        filtro = " AND ( descfacturap LIKE '%" + m_filtro->text() + "%' ";
        filtro +=" OR nomproveedor LIKE '%" + m_filtro->text() + "%') ";
    } else {
        filtro = "";
    } // end if
    if (m_proveedor->idproveedor() != "") {
        filtro += " AND facturap.idproveedor = " + m_proveedor->idproveedor();
    } // end if
    if (!m_procesada->isChecked() ) {
        filtro += " AND NOT procesadafacturap ";
    } else {
        filtro += " AND procesadafacturap ";
    }// end if
    if (m_articulo->idarticulo() != "") {
        filtro += " AND idfacturap IN (SELECT DISTINCT idfacturap FROM lfacturap WHERE idarticulo = '" + m_articulo->idarticulo() + "')";
    } // end if

    if (m_fechain->text() != "")
        filtro += " AND ffacturap >= '" + m_fechain->text() + "' ";

    if (m_fechafin->text() != "")
        filtro += " AND ffacturap <= '" + m_fechafin->text() + "' ";

    _depura("END FacturasProveedorList::generaFiltro", 0);
    return (filtro);
}

/// Este metodo responmde a la accion predeterminada sobre el listado
/// Normalmente consiste en abrir la ficha del elemento seleccionado,
/// en este caso, una factura de proveedor
void FacturasProveedorList::editar(int row) {
    _depura("FacturasProveedorList::editar", 0);
    try {
	   mdb_idfacturap = mui_list->DBvalue(QString("idfacturap"), row);
	   if (m_modo == 0) {
		  FacturaProveedorView *prov = m_companyact->newFacturaProveedorView();
		  if (prov->cargar(mdb_idfacturap)) {
			 delete prov;
			 return;
		  } // end if
		  m_companyact->m_pWorkspace->addWindow(prov);
		  prov->show();
	   } else {
		  emit(selected(mdb_idfacturap));
	   } // end if
    } catch(...) {
    	mensajeInfo(tr("Error al cargar la factura proveedor"));
    } // end try
    _depura("END FacturasProveedorList::editar", 0);
}


/** SLOT que responde a la pulsacion del boton mui_editar en el formulario.
    comprueba que exista una fila seleccionada y llama al metodo \ref editar()
*/
void FacturasProveedorList::on_mui_editar_clicked() {
    _depura("FacturasProveedorList::on_mui_editar_clicked", 0);
    int a = mui_list->currentRow();
    if (a < 0) {
        mensajeInfo(tr("Debe seleccionar una linea"));
        return;
    } else {
        editar(a);
    } // end if
    _depura("END FacturasProveedorList::on_mui_editar_clicked", 0);
}


/** SLOT que responde a la pulsacion del boton mui_borrar en el formulario.
    Crea una instancia de \ref FacturaProveedorView carga el elemento seleccionado
    y lanza el metodo \ref FacturaProveedorView::on_mui_borrar_clicked()
    Una vez borrado recarga el listado para que se actualicen los cambios.
*/
void FacturasProveedorList::on_mui_borrar_clicked() {
    _depura("FacturasProveedorList::on_mui_borrar_clicked", 0);
    int a = mui_list->currentRow();
    if (a < 0) {
        mensajeInfo(tr("Debe seleccionar una linea"));
        return;
    } // end if
    try {
	   mdb_idfacturap = mui_list->DBvalue("idfacturap");
	   FacturaProveedorView *bud = m_companyact->newFacturaProveedorView();
	   bud->cargar(mdb_idfacturap);
	   bud->on_mui_borrar_clicked();
	   delete bud;
	   presenta();
    } catch (...) {
    	mensajeInfo(tr("Error al borrar la factura de proveedor"));
    } // end try
    _depura("END FacturasProveedorList::on_mui_borrar_clicked", 0);
}


/** SLOT que responde a la pulsacion del boton mui_imprimir.
    La impresion de listados esta completamente delegada en SubForm3
*/
void FacturasProveedorList::on_mui_imprimir_clicked() {
    _depura("FacturasProveedorList::on_mui_imprimir_clicked", 0);
    mui_list->imprimirPDF(tr("Facturas de proveedores"));
    _depura("END FacturasProveedorList::on_mui_imprimir_clicked", 0);
}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
/** Prepara el subformulario para que trabaje con la tabla facturap.
*/
FacturasProveedorListSubform::FacturasProveedorListSubform(QWidget *parent) : SubForm2Bf(parent) {
    _depura("FacturasProveedorListSubform::FacturasProveedorListSubform", 0);
    setDBTableName("facturap");
    setDBCampoId("idfacturap");
    addSHeader("reffacturap", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Referencia factura"));
    addSHeader("idfacturap", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("Id factura"));
    addSHeader("numfacturap", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Numero factura"));
    addSHeader("nomproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre proveedor"));
    addSHeader("ffacturap", DBCampo::DBdate, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha factura"));
    addSHeader("contactfacturap", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Persona de contacto"));
    addSHeader("telfacturap", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Telefono factura"));
    addSHeader("comentfacturap", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Comentario factura"));
    addSHeader("idtrabajador", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Id trabajador"));
    addSHeader("idproveedor", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Id proveedor"));
    addSHeader("total", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Total"));
    addSHeader("base", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Base imponible"));
    addSHeader("impuestos", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Impuestos"));
    setinsercion(FALSE);
    setDelete(FALSE);
    setSortingEnabled(TRUE);
    _depura("END FacturasProveedorListSubform::FacturasProveedorListSubform", 0);
}

