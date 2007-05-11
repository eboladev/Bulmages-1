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
#include <QCheckBox>
#include <QFile>
#include <QCheckBox>
#include <QTextStream>
#include <QMenu>

#include "configuracion.h"
#include "cobroslist.h"
#include "company.h"
#include "cobroview.h"
#include "funcaux.h"


/** Inicializa todos los componentes.
    Mete la ventana en el workSpace.
    Este constructor no es completo, debe inicializarse con setcompany para que la clase pueda operar.    
*/
CobrosList::CobrosList(QWidget *parent, Qt::WFlags flag)
        : FichaBf(NULL, parent, flag) {
    _depura("CobrosList::CobrosList", 0);
    setupUi(this);
    setModoEdicion();
    mdb_idcobro = "";
    meteWindow(windowTitle(), this);
    hideBusqueda();
    _depura("END CobrosList::CobrosList", 0);

}


/** Inicializa todos los componentes.
    Hace una presentacion inicial.
    Mete la ventana en el workSpace.
*/
CobrosList::CobrosList(company *comp, QWidget *parent, Qt::WFlags flag)
        : FichaBf(comp, parent, flag) {
    _depura("CobrosList::CobrosList",0);
    setupUi(this);
    m_cliente->setEmpresaBase(comp);
    mui_list->setEmpresaBase(comp);
    mui_idbanco->setEmpresaBase(comp);
    mui_idbanco->setidbanco("0");
    presenta();
    setModoEdicion();
    mdb_idcobro = "";
    meteWindow(windowTitle(), this);
    hideBusqueda();
    _depura("END CobrosList::CobrosList", 0);
}


/** No requiere acciones especiales en el destructor.
*/
CobrosList::~CobrosList() {
    _depura("CobrosList::~CobrosList", 0);
    empresaBase()->sacaWindow(this);
    _depura("END CobrosList::~CobrosList", 0);
}


/** Hace la carag del listado.
    PAra ello genera los SELECTS con ayuda de generaFiltro y los pasa al SubFormulario para que los presente.
    Tambien hace un select de calculo de totales y lo presenta en el textEdit correspondiente.
*/
void CobrosList::presenta() {
    _depura("CobrosList::presentar", 0);
    if (empresaBase() != NULL) {
        mui_list->cargar("SELECT * FROM cobro NATURAL LEFT JOIN cliente NATURAL LEFT JOIN trabajador NATURAL LEFT JOIN banco WHERE 1 = 1 " + generaFiltro());
        /// Hacemos el calculo del total.
	Fixed total = mui_list->sumarCampo("cantcobro");
	m_total->setText(total.toQString());
    } // end if
    _depura("END CobrosList::presentar", 0);
}


/** Metodo auxiliar que genera la clausula WHERE del listado con las opciones de filtrado especificadas.
*/
QString CobrosList::generaFiltro() {
    _depura("CobrosList::generaFiltro", 0);
    QString filtro = "";
    if (m_filtro->text() != "") {
        filtro = " AND ( desccobro LIKE '%" + m_filtro->text() + "%' ";
    } else {
        filtro = "";
    } // end if
    if (m_cliente->idcliente() != "") {
        filtro += " AND cobro.idcliente = " + m_cliente->idcliente();
    } // end if

    QString subfiltro = " AND ";
    if (mui_efectivos->isChecked()) {
        filtro += " AND NOT previsioncobro";
        subfiltro = " OR ";
    } // end if

    if (mui_previsiones->isChecked()) {
        filtro += subfiltro + " previsioncobro";
    } // end if

    if (m_fechain->text() != "")
        filtro += " AND fechacobro >= '" + m_fechain->text() + "' ";

    if (m_fechafin->text() != "")
        filtro += " AND fechacobro <= '" + m_fechafin->text() + "' ";

    if (mui_idbanco->idbanco() != "") 
	filtro += " AND idbanco = "+mui_idbanco->idbanco();

    _depura("END CobrosList::generaFiltro", 0);
    return (filtro);
}


/** SLOT que responde a la pulsacion del boton editar en el listado.
    comprueba que exista un elemento seleccionado y llama a la funcion de editar().
*/
/// \TODO: Deberia crearse el metodo editar() en lugar de llamar a on_mui_list_cellDoubleClicked().
void CobrosList::on_mui_editar_clicked() {
    _depura("CobrosList::on_mui_editar_clicked", 0);
    int a = mui_list->currentRow();
    if (a >= 0) {
        on_mui_list_cellDoubleClicked(a, 0);
    } else {
        mensajeInfo(tr("Debe seleccionar una linea"));
    } // end if
    _depura("END CobrosList::on_mui_editar_clicked", 0);
}


/** SLOT que responde a la pulsacion del boton de crear en el listado.
    Instancia la clase CobroView, y la presenta.
*/
void CobrosList::on_mui_crear_clicked() {
    _depura("CobrosList::on_mui_crear_clicked", 0);
    CobroView *bud = empresaBase()->newCobroView();
    empresaBase()->m_pWorkspace->addWindow(bud);
    bud->show();
    bud->setidcliente(m_cliente->idcliente());
    bud->pintar();
    _depura("CobrosList::on_mui_crear_clicked", 0);
}


/** La impresion de listados esta completamente delegada a la clase SubForm3
*/
void CobrosList::imprimir() {
    _depura("CobrosList::imprimir", 0);
    mui_list->imprimirPDF(tr("Cobros a clientes"));
    _depura("END CobrosList::imprimir", 0);
}


/** SLOT que responde a la pulsacion del boton borrar.
    Comprueba que exista un elemento seleccionado.
    Instancia la clase CobroView, la inicializa y le lanza el metodo borrar.
    Esta es la forma correcta de implementar un borrado a partir de un listado
    ya que de esta forma si existen plugins que alteren el borrado tambien seran invocados.
*/
void CobrosList::on_mui_borrar_clicked() {
    _depura("CobrosList::on_mui_borrar_clicked",0);
    int a = mui_list->currentRow();
    if (a < 0) {
        mensajeInfo(tr("Debe seleccionar una linea"));
        return;
    } // end if
    try {
        mdb_idcobro = mui_list->DBvalue("idcobro");
        if (modoEdicion()) {
            CobroView *cv = empresaBase()->newCobroView();
            if (cv->cargar(mdb_idcobro))
                throw -1;
            cv->on_mui_borrar_clicked();
            cv->close();
        } // end if
        presenta();
    } catch (...) {
        mensajeInfo(tr("Error al borrar el cobro a cliente"));
    } // end try
    _depura("END:CobrosList::on_mui_borrar_clicked",0);
}


/** SLOT que responde al doble click en el subformulario.
    Dependiendo del modo (Edicion o Seleccion) hace unas operaciones u otras.
*/
/// \TODO: Deberia crearse el metodo editar y este llamar a ese.
void CobrosList::on_mui_list_cellDoubleClicked(int, int) {
    _depura("CobrosList::on_mui_list_cellDoubleClicked", 0);
    try {
        mdb_idcobro = mui_list->DBvalue("idcobro");
        if (modoEdicion()) {
            CobroView *bud = empresaBase()->newCobroView();
            if (bud->cargar(mdb_idcobro)) {
                delete bud;
                return;
            } // end if
            empresaBase()->m_pWorkspace->addWindow(bud);
            bud->show();
        } else {
            close();
        } // end if
    } catch(...) {
        mensajeInfo(tr("Debe seleccionar una fila primero"));
    } // end try
    _depura("END CobrosList::on_mui_list_cellDoubleClicked", 0);

}

/** SLOT que responde a la peticion de menu contextual en el subformulario.
*/
/// \TODO: Revisar si este metodo es util.
void CobrosList::on_mui_list_customContextMenuRequested(const QPoint &) {
    _depura("PagosList::on_mui_list_customContextMenuRequested", 0);
    int a = mui_list->currentRow();
    if (a < 0)
        return;
    QMenu *popup = new QMenu(this);
    QAction *edit = popup->addAction(tr("Editar cobro"));
    QAction *del = popup->addAction(tr("Borrar cobro"));
    QAction *opcion = popup->exec(QCursor::pos());
    if (opcion == del)
        on_mui_borrar_clicked();
    if (opcion == edit)
        on_mui_editar_clicked();
    delete popup;
    _depura("PagosList::on_mui_list_customContextMenuRequested", 0);
}

/** Inicializa la clase con el puntero a la company que se esta utilizando
**/
void CobrosList::setEmpresaBase(company *comp) {
    PEmpresaBase::setEmpresaBase(comp);
    m_cliente->setEmpresaBase(comp);
    mui_list->setEmpresaBase(comp);
    mui_idbanco->setEmpresaBase(comp);
    mui_idbanco->setidbanco("");
}

/** Devuelve el identificador del cobro seleccionado
**/
QString CobrosList::idcobro() {
    _depura("CobrosList::idcobro", 0);
    _depura("END CobrosList::idcobro", 0);
    return mdb_idcobro;
}

/** Oculta el layer de botones
**/
void CobrosList::hideBotonera() {
    _depura("CobrosList::hideBotonera", 0);
    m_botonera->hide();
    _depura("END CobrosList::hideBotonera", 0);
}

/** Muestra el layer de botones
**/
void CobrosList::showBotonera() {
    _depura("CobrosList::showBotonera", 0);
    m_botonera->show();
    _depura("END CobrosList::showBotonera", 0);
}

/** Oculta el layer de opciones de filtrado
**/
void CobrosList::hideBusqueda() {
    _depura("CobrosList::hideBusqueda", 0);
    m_busqueda->hide();
    _depura("END CobrosList::hideBusqueda", 0);
}

/** Muestra el layer de opciones de filtrado
**/
void CobrosList::showBusqueda() {
    _depura("CobrosList::showBusqueda", 0);
    m_busqueda->show();
    _depura("END CobrosList::showBusqueda", 0);
}

/** Inicializa la la case con un cliente determiando. Lo establece en las opciones de filtrado
    no actualiza el listado
**/
void CobrosList::setidcliente(QString val) {
    m_cliente->setidcliente(val);
}

/** SLOT automatico que se ejecuta al pulsar sobre el boton de imprimir
**/
void CobrosList::on_mui_imprimir_clicked() {
    _depura("CobrosList::on_mui_imprimir_clicked", 0);
    imprimir();
    _depura("END CobrosList::on_mui_imprimir_clicked", 0);
}

/** SLOT automatico que se ejecuta al pulsar sobre el boton de actualizar
**/
void CobrosList::on_mui_actualizar_clicked() {
    _depura("CobrosList::on_mui_actualizar_clicked", 0);
    presenta();
    _depura("END CobrosList::on_mui_actualizar_clicked", 0);
}

/** SLOT automatico que se ejecuta al pulsar sobre el boton de configurar listado
**/
void CobrosList::on_mui_configurar_toggled(bool checked) {
    _depura("CobrosList::on_mui_configurar_toggled", 0);
    if (checked) {
        mui_list->showConfig();
    } else {
        mui_list->hideConfig();
    } // end if
    _depura("END CobrosList::on_mui_configurar_toggled", 0);
}





/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
/** Prepara el subformulario para trabajar con la tabla cobro.
    Establece que no se puedan insertar elementos ya que es un listado.
*/
CobrosListSubForm::CobrosListSubForm(QWidget *parent) : SubForm2Bf(parent) {
    _depura("CobrosListSubForm::CobrosListSubForm", 0);
    setDBTableName("cobro");
    setDBCampoId("idcobro");
    addSHeader("idcobro", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("ID cobro"));
    addSHeader("idcliente", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("ID cliente"));
    addSHeader("nomcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre"));
    addSHeader("cifcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("C.I.F."));
    addSHeader("telcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Numero de telefono"));
    addSHeader("mailcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Direccion de correo electronico"));
    addSHeader("fechacobro", DBCampo::DBdate, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha de cobro"));
    addSHeader("cantcobro", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Cantidad"));
    addSHeader("nombanco", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Banco"));
    addSHeader("refcobro", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Referencia del cobro"));
    addSHeader("previsioncobro", DBCampo::DBboolean, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Prevision de cobro"));
    addSHeader("comentcobro", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Comentarios"));
    addSHeader("idtrabajador", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("ID trabajador"));
    addSHeader("nomtrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre del trabajador"));
    addSHeader("apellidostrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Apellidos del trabajador"));
    setinsercion(FALSE);
    setDelete(FALSE);
    setSortingEnabled(TRUE);
    _depura("END CobrosListSubForm::CobrosListSubForm", 0);
}


CobrosListSubForm::~CobrosListSubForm() {
    _depura("CobrosListSubForm::~CobrosListSubForm", 0);
    _depura("END CobrosListSubForm::~CobrosListSubForm", 0);
}

