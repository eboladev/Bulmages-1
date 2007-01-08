/***************************************************************************
 *   Copyright (C) 2005 by Alvaro de Miguel                                *
 *   alvaro.demiguel@gmail.com                                             *
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
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#include "albaranclientelist.h"
#include "albaranclienteview.h"
#include "company.h"
#include "funcaux.h"


/** Constructor de la clase sin inicializacion de company. Usando este
    constructor no se olvide de usar setcompany para que la clase no de
    excepciones.
    El constructor crea la pantalla, la pone en editmodo por defecto y la mete 
    en el listado de ventanas.
    Usando esta clase tampoco se inicializan bien los widgets que contiene.
*/
AlbaranClienteList::AlbaranClienteList(QWidget *parent, Qt::WFlags flag, edmode editmodo)
        : Ficha(parent) {
    _depura("AlbaranClienteList::AlbaranClienteList", 0);
    setupUi(this);
    m_companyact = NULL;
    m_modo = editmodo;
    mdb_idalbaran = "";
    if (m_modo == EditMode)
        meteWindow(windowTitle(), this);
    hideBusqueda();
    _depura("END AlbaranClienteList::AlbaranClienteList", 0);
}

/** Constructor de la clase.
    Inicializa toda la ventana y los widgets que esta contiene.
    Hace una presentacion inicial de los albaranes de cliente.
    Pone la pantalla en modo de edicion por defecto  a no ser que se especifique lo contrario.
    Mete la ventana en el workspace.
*/
AlbaranClienteList::AlbaranClienteList(company *comp, QWidget *parent, Qt::WFlags flag, edmode editmodo)
        : Ficha(parent) {
    _depura("AlbaranClienteList::AlbaranClienteList", 0);
    setupUi(this);
    m_companyact = comp;
    m_cliente->setcompany(comp);
    m_articulo->setcompany(comp);
    mui_list->setcompany(comp);
    presenta();
    m_modo = editmodo;
    mdb_idalbaran = "";
    if (m_modo == EditMode)
        m_companyact->meteWindow(windowTitle(), this);
    hideBusqueda();
    _depura("END AlbaranClienteList::AlbaranClienteList", 0);
}

/** Destructor de la clase */
AlbaranClienteList::~AlbaranClienteList() {
    _depura("AlbaranClienteList::~AlbaranClienteList", 0);
    _depura("END AlbaranClienteList::~AlbaranClienteList", 0);
}

/** Carga el listado de la base de datos y lo presenta.
    Tambien carga el total y lo presenta.
*/
void AlbaranClienteList::presenta() {
    _depura("AlbaranClienteList::presenta\n");

    mui_list->cargar("SELECT *, totalalbaran AS total, bimpalbaran AS base, impalbaran AS impuestos FROM albaran LEFT JOIN  cliente ON albaran.idcliente=cliente.idcliente LEFT JOIN almacen ON albaran.idalmacen=almacen.idalmacen LEFT JOIN forma_pago ON albaran.idforma_pago = forma_pago.idforma_pago WHERE 1=1  "+generarFiltro());

    /// Hacemos el calculo del total.
    cursor2 *cur = m_companyact->cargacursor("SELECT SUM(totalalbaran) AS total FROM albaran LEFT JOIN cliente ON albaran.idcliente=cliente.idcliente LEFT JOIN almacen ON almacen.idalmacen=albaran.idalmacen where 1=1 "+generarFiltro());
    m_total->setText(cur->valor("total"));
    delete cur;

    _depura("End AlbaranClienteList::presenta");
}


/**  Este metodo se activa cuando bien pulsando sobre el boton de editar
     o bien haciendo doble click en el modo de edicion se desea invocar la accion
     Editar el elemento si estamos en modo editmode o cerrar la ventana y emitir
     un signal selected() si estamos en el modo selector.
      
     Primero determina el idalbaran seleccionado, luego crea la instancia de
     la ventana de edicion AlbaranClienteView y lo mete en el workspace.
     Por ultimo hace que dicha ventana carge de la base de datos el idalbaran
     seleccionado.
*/
void AlbaranClienteList::editar(int row) {
    _depura("AlbaranClienteList::editar", 0);
    mdb_idalbaran = mui_list->DBvalue(QString("idalbaran"), row);
    if (m_modo == 0) {
        AlbaranClienteView *prov = m_companyact->newAlbaranClienteView();
        if (prov->cargar(mdb_idalbaran)) {
            delete prov;
            return;
        } // end if
        m_companyact->m_pWorkspace->addWindow(prov);
        prov->show();
    } else {
        emit(selected(mdb_idalbaran));
    } // end if
    _depura("END AlbaranClienteList::editar", 0);
}


/** Metodo que responde a la pulsacion del boton editar en el formulario.
    Comprueba que exista un elemento seleccionado y si es el caso llama
    al metodo editar, si no termina devolviendo un mensaje.
*/
void AlbaranClienteList::on_mui_editar_clicked() {
    _depura("AlbaranClienteList::on_mui_editar_clicked", 0);
    int a = mui_list->currentRow();
    if (a >= 0)
        editar(a);
    else
        _depura("Debe seleccionar una linea", 2);
    _depura("END AlbaranClienteList::on_mui_editar_clicked", 0);

}


/** Responde a la pulsacion del boton borrar en el formulario.
    Carga el elemento seleccionado e invoca a su metodo de borrar.
*/
/// \todo: Comprobar que se libera bien la memoria.
/// \todo: Intentar que no se tenga que recargar todo el listado y que simplemente se borre la fila seleccionada.
void AlbaranClienteList::on_mui_borrar_clicked() {
    _depura("AlbaranClienteList::on_mui_borrar_clicked", 0);
    try {
        mdb_idalbaran = mui_list->DBvalue(QString("idalbaran"));
        if (m_modo == 0) {
            AlbaranClienteView *prov = m_companyact->newAlbaranClienteView();
            if (prov->cargar(mdb_idalbaran))
                throw -1;
            prov->borrar();
        } // end if
        presenta();
    } catch(...) {
        mensajeInfo(tr("Error al borrar el albaran cliente"));
    } // end try
    _depura("END AlbaranClienteList::on_mui_borrar_clicked", 0);
}


/** La impresion de listados es estandarizada por la clase DBRecord
*/
/// \TODO: Se podria hacer una clase derivada de Ficha que fuese Listado y que directamente implementase el metodo on_mui_imprimir.
void AlbaranClienteList::imprimir() {
    _depura("AlbaranClienteList::imprimir", 0);
    mui_list->imprimirPDF(tr("Albaranes Cliente"));
    _depura("END AlbaranClienteList::imprimir", 0);
}


/** Este metodo ayuda a construir el query de filtrado construyendo la clausula
    WHERE a partir de las opciones de filtrado.
*/
QString AlbaranClienteList::generarFiltro() {
    /// Tratamiento de los filtros.
    _depura("AlbaranClienteList::generarFiltro", 0);
    QString filtro = "";

    if (m_filtro->text() != "") {
        filtro = " AND ( descalbaran LIKE '%" + m_filtro->text() + "%' ";
        filtro +=" OR refalbaran LIKE '"+m_filtro->text()+"%' ";
        filtro +=" OR nomcliente LIKE '%" + m_filtro->text() + "%') ";
    } else {
        filtro = "";
    } // end if

    if (m_cliente->idcliente() != "")
        filtro += " AND albaran.idcliente='" + m_cliente->idcliente() + "'";

    if (m_articulo->idarticulo() != "")
        filtro += " AND idalbaran IN (SELECT DISTINCT idalbaran FROM lalbaran WHERE idarticulo='" + m_articulo->idarticulo() + "')";

    if (!m_procesados->isChecked() )
        filtro += " AND NOT procesadoalbaran";

    if (m_fechain->text() != "")
        filtro += " AND fechaalbaran >= '" + m_fechain->text() + "' ";

    if (m_fechafin->text() != "")
        filtro += " AND fechaalbaran <= '" + m_fechafin->text() + "' ";

    _depura("END AlbaranClienteList::generarFiltro", 0);
    return (filtro);
}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
/** Constructor del subformulario de albaranes a cliente.
    Configura el subformulario para que use la tabla albaran.
*/
AlbaranClienteListSubform::AlbaranClienteListSubform(QWidget *parent) : SubForm2Bf(parent) {
    _depura("AlbaranClienteListSubform::AlbaranClienteListSubform", 0);
    setDBTableName("albaran");
    setDBCampoId("idalbaran");
    addSHeader("refalbaran", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("Referencia de albaran"));
    addSHeader("codigoalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Codigo de almacen"));
    addSHeader("numalbaran", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Numero de albaran"));
    addSHeader("descalbaran", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Descripcion"));
    addSHeader("fechaalbaran", DBCampo::DBdate, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha de albaran"));
    addSHeader("nomcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre de cliente"));
    addSHeader("idforma_pago", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("ID forma de pago"));
    addSHeader("descforma_pago", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Descripcion de forma de pago"));
    addSHeader("idtrabajador", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("ID trabajador"));
    addSHeader("idcliente", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("ID cliente"));
    addSHeader("idalbaran", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("ID albaran"));
    addSHeader("comentalbaran", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Comentarios del albaran"));
    addSHeader("idalmacen", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("ID almacen"));
    addSHeader("base", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Base imponible"));
    addSHeader("impuestos", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Impuestos"));
    addSHeader("total", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Total"));
    setinsercion(FALSE);
    setDelete(FALSE);
    setSortingEnabled(TRUE);
    _depura("END AlbaranClienteListSubform::AlbaranClienteListSubform", 0);
}

