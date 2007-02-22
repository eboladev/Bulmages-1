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

#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#include "busquedaarticulo.h"
#include "busquedacliente.h"
#include "busquedafecha.h"
#include "company.h"
#include "configuracion.h"
#include "facturaslist.h"
#include "facturaview.h"
#include "funcaux.h"
#include "plugins.h"


/** Prepara todos los componentes i los inicializa a NULL para que no haya dudas sobre si
    ya han sido inicializados o no.
    Inicializando la clase con este constructor precisa que sea establecido el company con setcompany.
*/
FacturasList::FacturasList(QWidget *parent, Qt::WFlags flag, edmode editmodo)
        : Ficha(parent, flag) {
    _depura("FacturasList::FacturasList", 0);
    setupUi(this);
    /// Disparamos los plugins.
    int res = g_plugins->lanza("FacturasList_FacturasList", this);
    if (res != 0)
        return;
    m_companyact = NULL;
    m_modo = editmodo;
    mdb_idfactura = "";
    if (m_modo == EditMode) {
        meteWindow(windowTitle(), this);
    } // end if
    hideBusqueda();
    _depura("END FacturasList::FacturasList", 0);
}

/** Inicializa todos los componentes y prepara la ventana para funcionar.
*/
FacturasList::FacturasList(company *comp, QWidget *parent, Qt::WFlags flag, edmode editmodo)
        : Ficha(parent, flag) {
    _depura("FacturasList::FacturasList", 0);
    setupUi(this);
    /// Disparamos los plugins.
    int res = g_plugins->lanza("FacturasList_FacturasList", this);
    if (res != 0)
        return;
    m_companyact = comp;
    m_cliente->setcompany(m_companyact);
    m_articulo->setcompany(m_companyact);
    mui_list->setcompany(m_companyact);
    presenta();
    m_modo = editmodo;
    mdb_idfactura = "";
    if (m_modo == EditMode) {
        meteWindow(windowTitle(), this);
    } // end if
    hideBusqueda();
    _depura("END FacturasList::FacturasList", 0);
}


/** No precisa de acciones especiales en el destructor de la clase.
*/
FacturasList::~FacturasList() {
    _depura("FacturasList::~FacturasList", 0);
    _depura("END FacturasList::~FacturasList", 0);
}


/** Hace la carga del subformulario y el calculo de los totales.
*/
void FacturasList::presenta() {
    _depura("FacturasList::presenta", 0);

    mui_list->cargar("SELECT *, totalfactura AS total, bimpfactura AS base, impfactura AS impuestos FROM factura LEFT JOIN cliente ON factura.idcliente = cliente.idcliente LEFT JOIN almacen ON factura.idalmacen = almacen.idalmacen WHERE 1 = 1 " + generaFiltro());

    /// Hacemos el calculo del total.
    cursor2 *cur = m_companyact->cargacursor("SELECT SUM(totalfactura) AS total FROM factura LEFT JOIN cliente ON factura.idcliente = cliente.idcliente LEFT JOIN almacen ON factura.idalmacen = almacen.idalmacen WHERE 1 = 1 " + generaFiltro());
    m_total->setText(cur->valor("total"));
    delete cur;

    _depura("END FacturasList::presenta", 1);
}


/** Funcion auxiliar que genera la clausula WHERE de la consulta del listado a partir de las opciones de filtrado que el usuario haya especificado.
*/
QString FacturasList::generaFiltro() {
    _depura("FacturasList::generaFiltro", 0);
    /// Tratamiento de los filtros.
    QString filtro = "";
    if (m_filtro->text() != "") {
        filtro = " AND ( descfactura LIKE '%" + m_filtro->text() + "%' ";
        filtro +=" OR reffactura LIKE '" + m_filtro->text() + "%' ";
        filtro +=" OR nomcliente LIKE '%" + m_filtro->text() + "%') ";
    } else {
        filtro = "";
    } // end if
    if (m_cliente->idcliente() != "") {
        filtro += " AND factura.idcliente = " + m_cliente->idcliente();
    } // end if
    if (!m_procesada->isChecked() ) {
        filtro += " AND NOT procesadafactura ";
    } else {
        filtro += " AND procesadafactura ";
    }// end if
    if (m_articulo->idarticulo() != "") {
        filtro += " AND idfactura IN (SELECT DISTINCT idfactura FROM lfactura WHERE idarticulo = '" + m_articulo->idarticulo() + "') ";
    } // end if
    if (m_fechain->text() != "") {
        filtro += " AND ffactura >= '" + m_fechain->text() + "' ";
    } // end if
    if (m_fechafin->text() != "") {
        filtro += " AND ffactura <= '" + m_fechafin->text() + "' ";
    } // end if
    _depura("END FacturasList::generaFiltro", 0);
    return (filtro);
}


/** Metodo que responde a la accion por defecto sobre el listado (doble click o boton de editar)
    Si estamos en modo edicion abre la ficha de Factura y carga el elemento seleccionado.
    Si estamos en modo seleccion cierra la ventana y emite el SIGNAL de que se ha seleccionado una factura.
*/
void FacturasList::editar(int row) {
    _depura("FacturasList::editar", 0);
    mdb_idfactura = mui_list->DBvalue(QString("idfactura"), row);
    if (m_modo == 0) {
        FacturaView *prov = m_companyact->newFacturaView();
        if (prov->cargar(mdb_idfactura)) {
            delete prov;
            return;
        } // end if
        m_companyact->m_pWorkspace->addWindow(prov);
        prov->show();
    } else {
        emit(selected(mdb_idfactura));
    } // end if
    _depura("END FacturasList::editar", 0);
}

/** SLOT que responde a la pulsacion del boton editar en el listado.
    Comprueba que exista un elemento seleccionado y
    llama al metodo \ref editar().
*/
void FacturasList::on_mui_editar_clicked() {
    _depura("FacturasList::on_mui_editar_clicked", 0);
    int a = mui_list->currentRow();
    if (a >= 0) {
        editar(a);
    } else {
        mensajeInfo(tr("Debe seleccionar una linea"));
    } // end if
    _depura("END FacturasList::on_mui_editar_clicked", 0);
}


/** SLOT que responde a la pulsacion del boton de imprimir.
    La impresion de listados esta completamente delegada en SubForm2Bf
*/
void FacturasList::on_mui_imprimir_clicked() {
    _depura("FacturasList::on_mui_imprimir_clicked", 0);
    mui_list->imprimirPDF(tr("Facturas a clientes"));
    _depura("FacturasList::on_mui_imprimir_clicked", 0);
}


/** SLOT que responde a la pulsacion del boton borrar.
    Crea una instancia de FacuturaView, carga el elemento seleccionado e invoca al 
    metodo de borrar.
    La ventaja de hacerlo de esta forma es que si hay plugins en la factura, estos tambien se ejecutaran.
*/
void FacturasList::on_mui_borrar_clicked() {
    _depura("FacturasList::on_mui_borrar_clicked", 0);
    int a = mui_list->currentRow();
    if (a < 0) {
        mensajeInfo(tr("Debe seleccionar una linea"));
        return;
    } // end if
    try {
        mdb_idfactura = mui_list->DBvalue(QString("idfactura"));
        if (m_modo == 0) {
            FacturaView *fv = m_companyact->newFacturaView();
            if (fv->cargar(mdb_idfactura))
                throw -1;
            fv->on_mui_borrar_clicked();
            fv->close();
        } // end if
        presenta();
    } catch (...) {
        mensajeInfo(tr("Error al borrar la factura a cliente"));
    } // end try
    _depura("END FacturasList::on_mui_borrar_clicked", 0);
}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
/** Prepara el subformulario para trabajar con la tabla factura.
*/
FacturasListSubform::FacturasListSubform(QWidget *parent, const char *) : SubForm2Bf(parent) {
    _depura("FacturasListSubform::FacturasListSubform", 0);
    /// Disparamos los plugins.
    int res = g_plugins->lanza("FacturasListSubform_FacturasListSubform", this);
    if (res != 0)
        return;
    setDBTableName("factura");
    setDBCampoId("idfactura");
    addSHeader("idfactura", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Id factura"));
    addSHeader("reffactura", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("Ref factura"));
    addSHeader("codigoserie_factura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Serie"));
    addSHeader("numfactura", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Numero"));
    addSHeader("ffactura", DBCampo::DBdate, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha"));
    addSHeader("cifcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("CIF cliente"));
    addSHeader("nomcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre cliente"));
    addSHeader("codigoalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Codigo almacen"));
    addSHeader("contactfactura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Contact factura"));
    addSHeader("telfactura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Telefono factura"));
    addSHeader("comentfactura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Comentario factura"));
    addSHeader("idtrabajador", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Id trabajador"));
    addSHeader("idcliente", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Id cliente"));
    addSHeader("idalmacen", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Id almacen"));
    addSHeader("total", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Total"));
    addSHeader("base", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Base imponible"));
    addSHeader("impuestos", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Impuestos"));
    setinsercion(FALSE);
    setDelete(FALSE);
    setSortingEnabled(TRUE);
    _depura("END FacturasListSubform::FacturasListSubform", 0);
}

