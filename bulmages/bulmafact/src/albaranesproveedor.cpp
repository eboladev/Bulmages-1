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

#include <QCheckBox>
#include <QCheckBox>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#include "albaranesproveedor.h"
#include "albaranproveedorview.h"
#include "company.h"
#include "configuracion.h"
#include "funcaux.h"
#include "presupuestoview.h"


/** Constructor de AlbaranesProveedor sin inicializar con la clase company.
    Util para casos en los que aun no se haya inicializado company.
    No sera completamente operativo hasta que se haya inicializado con 
    setcompany.
    
    Inicializa todos los componentes, se pone en modo edicion y mete la ventana en el workSpace.
*/
AlbaranesProveedor::AlbaranesProveedor(QWidget *parent, Qt::WFlags flag)
        : Ficha(parent) {
    _depura("AlbaranesProveedor::AlbaranesProveedor", 0);
    setupUi(this);
    m_companyact = NULL;
    m_modo = 0;
    mdb_idalbaranp = "";
    meteWindow(windowTitle(), this);
    hideBusqueda();
    _depura("END AlbaranesProveedor::AlbaranesProveedor", 0);
}


/** Constructor completo de la clase con el puntero a Company adecuado.
    Inicializa todos los componentes con company.
    Hace una presentacion inicial.
    Mete la ventana en el workSpace.
    Oculta la parte de Busqueda.
*/
AlbaranesProveedor::AlbaranesProveedor(company *comp, QWidget *parent, Qt::WFlags flag)
        : Ficha(parent) {
    _depura("AlbaranesProveedor::AlbaranesProveedor", 0);
    setupUi(this);
    m_companyact = comp;
    m_proveedor->setcompany(comp);
    m_articulo->setcompany(comp);
    mui_list->setcompany(comp);
    presenta();
    m_modo = 0;
    mdb_idalbaranp = "";
    meteWindow(windowTitle(), this);
    hideBusqueda();
    _depura("END AlbaranesProveedor::AlbaranesProveedor", 0);
}


/** Refresca la ventana de listados de albaranes.
*/
AlbaranesProveedor::~AlbaranesProveedor() {
    _depura("AlbaranesProveedor::~AlbaranesProveedor", 0);
    m_companyact->refreshAlbaranesProveedor();
    _depura("END AlbaranesProveedor::~AlbaranesProveedor", 0);
}


/** Saca la ventan del workspace.
    Este metodo es llamado desde la clase Ficha.
*/
int AlbaranesProveedor::sacaWindow() {
    _depura("AlbaranesProveedor::sacaWindow", 0);
    m_companyact->sacaWindow(this);
    _depura("END AlbaranesProveedor::sacaWindow", 0);
    return 0;
}


/** Hace la carga del listado, 
    Calcula el total de albaranes con las opciones de filtrado descritas y
    lo presenta.
*/
void AlbaranesProveedor::presenta() {
    _depura("AlbaranesProveedor::presenta", 0);
    if (m_companyact != NULL ) {
        mui_list->cargar("SELECT *, totalalbaranp AS total, " \
                        "bimpalbaranp AS base, impalbaranp AS impuestos " \
                        "FROM albaranp LEFT " \
                        "JOIN proveedor ON albaranp.idproveedor = " \
                        "proveedor.idproveedor LEFT JOIN almacen ON " \
                        "albaranp.idalmacen = almacen.idalmacen LEFT JOIN " \
                        "forma_pago ON albaranp.idforma_pago = " \
                        "forma_pago.idforma_pago WHERE 1 = 1 " + generaFiltro());

        /// Hacemos el calculo del total.
        cursor2 *cur = m_companyact->cargacursor("SELECT SUM(totalalbaranp) " \
                                        "AS total FROM albaranp LEFT JOIN proveedor ON " \
                                        "albaranp.idproveedor = proveedor.idproveedor LEFT " \
                                        "JOIN almacen ON albaranp.idalmacen = almacen.idalmacen " \
                                        "WHERE 1 = 1 " + generaFiltro());
        m_total->setText(cur->valor("total"));
        delete cur;
    }
    _depura("END AlbaranesProveedor::presenta().", 0);
}


/** Este es un metodo auxiliar para presenta() que se encarga de generar
    la clausula WHERE de la consulta.
*/
QString AlbaranesProveedor::generaFiltro() {
    _depura("AlbaranesProveedor::generaFiltro", 0);
    QString filtro = "";

    if (m_filtro->text() != "") {
        filtro = " AND (descalbaranp LIKE '%" + m_filtro->text() + "%' ";
        filtro +=" OR nomproveedor LIKE '%" + m_filtro->text() + "%') ";
    } else {
        filtro = "";
    }

    if (m_proveedor->idproveedor() != "")
        filtro += " AND albaranp.idproveedor = " + m_proveedor->idproveedor();
    if (!m_procesados->isChecked()) {
        filtro += " AND NOT procesadoalbaranp ";
    } else {
        filtro += " AND procesadoalbaranp ";
    } // end if
    if (m_articulo->idarticulo() != "")
        filtro += " AND idalbaranp IN (SELECT DISTINCT idalbaranp FROM lalbaranp " \
                  "WHERE idarticulo = '" + m_articulo->idarticulo() + "')";
    if (m_fechain->text() != "")
        filtro += " AND fechaalbaranp >= '" + m_fechain->text() + "' ";
    if (m_fechafin->text() != "")
        filtro += " AND fechaalbaranp <= '" + m_fechafin->text() + "' ";
    _depura("END AlbaranesProveedor::generaFiltro", 0);
    return (filtro);
}


/** Realiza la operacion de edicion.
    Si el modo seleccionado es edicion abre la ficha de albaran proveedor y carga en ella el elemento seleccionado.
    Si el modo es seleccion lanza el signal adecuado.
*/    
void AlbaranesProveedor::editar(int row) {
    _depura("AlbaranesProveedor::editar", 0);
    mdb_idalbaranp = mui_list->DBvalue(QString("idalbaranp"), row);
    if (m_modo == 0) {
        AlbaranProveedorView *prov = new AlbaranProveedorView(m_companyact, 0);
        if (prov->cargar(mdb_idalbaranp)) {
            delete prov;
            return;
        } // end if
        m_companyact->m_pWorkspace->addWindow(prov);
        prov->show();
    } else {
        emit(selected(mdb_idalbaranp));
    } // end if
    _depura("END AlbaranesProveedor::editar", 0);
}


/** SLOT de la pulsacion sobre el boton de editar.
    Si existe un elemento seleccionado llama al metodo editar()
*/
void AlbaranesProveedor::on_mui_editar_clicked() {
    _depura("AlbaranesProveedor::on_mui_editar_clicked", 0);
    int a = mui_list->currentRow();
    if (a < 0) {
        mensajeInfo(tr("Debe seleccionar una linea"));
        return;
    } else {
        editar(a);
    } // end if
    _depura("END AlbaranesProveedor::on_mui_editar_clicked", 0);

}


/** La impresion de listados esta completamente delegada a la clase SubForm3
*/
void AlbaranesProveedor::imprimir() {
    _depura("AlbaranesProveedor::imprimir", 0);
	mui_list->imprimirPDF(tr("Albaranes de proveedor"));
    _depura("END AlbaranesProveedor::imprimir", 0);
}


/** Al pulsar sobre el boton borrar se carga la ficha y se invoca al metodo de
    borrar en esta. 
    Tambien se tratan las posibles excepciones que se hayan podido
    producir.
    Tras el borrado repinta la pantalla.
*/
void AlbaranesProveedor::on_mui_borrar_clicked() {
    _depura("AlbaranesProveedor::on_mui_borrar_clicked", 0);
    int a = mui_list->currentRow();
    if (a < 0) {
        mensajeInfo(tr("Debe seleccionar una linea"));
        return;
    } // end if
    try {
        mdb_idalbaranp = mui_list->DBvalue(QString("idalbaranp"));
        if (m_modo == 0) {
            AlbaranProveedorView *apv = m_companyact->newAlbaranProveedorView();
            if (apv->cargar(mdb_idalbaranp)) {
                throw -1;
            } // end if
            apv->on_mui_borrar_clicked();
            apv->close();
        } // end if
        presenta();
    } catch (...) {
        mensajeInfo(tr("Error al borrar albaran de proveedor"));
    } // end try
    _depura("END AlbaranesProveedor::on_mui_borrar_clicked", 0);
}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
/** Configuracion del subformulario de listado.
    Prepara el subformulario para trabajar con la tabla albaranp.
*/
AlbaranesProveedorListSubform::AlbaranesProveedorListSubform(QWidget *parent) : SubForm2Bf(parent) {
    _depura("AlbaranesProveedorListSubform::AlbaranesProveedorListSubform", 0);
    setDBTableName("albaranp");
    setDBCampoId("idalbaranp");
    addSHeader("idalbaranp", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("ID albaran"));
    addSHeader("numalbaranp", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Numero de albaran"));
    addSHeader("descalbaranp", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Descripcion"));
    addSHeader("refalbaranp", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Referencia"));
    addSHeader("fechaalbaranp", DBCampo::DBdate, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha"));
    addSHeader("comentalbaranp", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Comentario"));
    addSHeader("procesadoalbaranp", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Procesado"));
    addSHeader("idproveedor", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("ID proveedor"));
    addSHeader("idforma_pago", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("ID forma de pago"));
    addSHeader("idalmacen", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("ID almacen"));
    addSHeader("nomproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre del proveedor"));
    addSHeader("nomalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre del almacen"));
    addSHeader("descforma_pago", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Descripcion de la forma de pago"));
    addSHeader("base", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Base imponible"));
    addSHeader("impuestos", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Impuestos"));
    addSHeader("total", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Total albaran"));
    setinsercion(FALSE);
    setDelete(FALSE);
    setSortingEnabled(TRUE);
    _depura("END AlbaranesProveedorListSubform::AlbaranesProveedorListSubform", 0);
}

