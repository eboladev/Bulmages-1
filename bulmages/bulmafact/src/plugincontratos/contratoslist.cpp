/***************************************************************************
 *   Copyright (C) 2004 by J. M. Estopa Rey                                *
 *   pepma@telefonica.net                                                  *
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

#include <QLineEdit>
#include <QTextStream>
#include <QFileDialog>
#include <QCheckBox>
#include <QMessageBox>

#include "contratoslist.h"
#include "company.h"
#include "funcaux.h"
#include "pgimportfiles.h"
#include "informereferencia.h"
#include "plugins.h"
#include "contratoview.h"


/** Inicializa todos los componentes del listado.
    Hace una presentacion inicial.
    Mete la ventana en el workSpace si estamos en modo edicion.
*/
ContratosList::ContratosList(company *comp, QWidget *parent, Qt::WFlags flag, edmode editmode)
        : Ficha(parent, flag), pgimportfiles(comp) {
    _depura("ContratosList::ContratosList", 0);
    setAttribute(Qt::WA_DeleteOnClose);
    setupUi(this);
    m_companyact = comp;
    /// Disparamos los plugins.
    int res = g_plugins->lanza("ContratosList_ContratosList", this);
    if (res != 0)
        return;

    mui_list->setcompany(comp);
    mdb_idcontrato = "";
    mdb_nomcontrato = "";
    m_modo = editmode;
    hideBusqueda();
    /// Si estamos en el modo edici&oacute;n metemos la ventana en el workSpace.
    if (m_modo == EditMode) {
        m_companyact->meteWindow(windowTitle(), this);
    } else {
        setWindowTitle(tr("Selector de contratos"));
        mui_editar->setHidden(TRUE);
        mui_crear->setHidden(TRUE);
        mui_borrar->setHidden(TRUE);
        mui_imprimir->setHidden(TRUE);
    } // end if
    presenta();
    _depura("END ContratosList::ContratosList", 0);
}


/** No requiere acciones especiales en el destructor de la clase.
*/
ContratosList::~ContratosList() {
    _depura("ContratosList::~ContratosList", 0);
    m_companyact->sacaWindow(this);
    _depura("END ContratosList::~ContratosList", 0);
}


/** Todo el listado de contratos lo presenta el Subformulario mui_list del tipo ListLinCliente.
*/
/// \TODO: Mejorar el sistema de filtrado incluyendo una funcion de generar Filtro.
void ContratosList::presenta() {
    _depura("ContratosList::presenta", 0);
    mui_list->cargar("SELECT * FROM contrato NATURAL LEFT JOIN cliente  WHERE nomcontrato LIKE '%" + m_findClient->text() + "%' ORDER BY nomcontrato");
    _depura("END ContratosList::presenta", 0);
}


/** LA accion de editar tiene una doble vertiente dependiendo del modo en que se ha abierto la ventana.
    Modo Edicion:
	- Crea una instancia de ClienteView, lo inicializa y lo presenta.
    Modo Seleccion:
	- Establece cual es el elemento seleccionado y se cierra la ventana para devolver el control.
*/
void ContratosList::editar(int row) {

    _depura("ContratosList::editar", 0);
    mdb_idcontrato = mui_list->DBvalue("idcontrato", row);
    mdb_refcontrato = mui_list->DBvalue("refcontrato", row);
    mdb_nomcontrato = mui_list->DBvalue("nomcontrato", row);
    if (m_modo == 0) {
        ContratoView *prov = new ContratoView(m_companyact);
        if (prov->cargar(mdb_idcontrato)) {
            delete prov;
            return;
        } // end if
        m_companyact->m_pWorkspace->addWindow(prov);
        prov->show();
    } else {
        emit(selected(mdb_idcontrato));
    } // end if
    _depura("END ContratosList::editar", 0);

}


/** SLOT que responde a la pulsacion del boton editar.
    Comprueba que existe un elemento seleccionado y llama al metodo editar().
*/
void ContratosList::on_mui_editar_clicked() {
    _depura("ContratosList::on_mui_editar_clicked", 0);
    if (mui_list->currentRow() < 0) {
        _depura("Debe seleccionar un elemento", 2);
        return;
    } // end if
    editar(mui_list->currentRow());
    _depura("END ContratosList::on_mui_editar_clicked", 0);
}


/** SLOT que responde a la pulsacion del boton imprimir.
    La impresion de listados esta completamente delegada a SubForm3
*/
void ContratosList::on_mui_imprimir_clicked() {
    _depura("ContratosList::on_mui_imprimir_clicked", 0);
    mui_list->imprimirPDF(tr("Listado de Contratos"));
    _depura("ContratosList::on_mui_imprimir_clicked", 0);
}




/** SLOT que responde a la pulsacion del boton borrar.
    Instancia la clase ClienteView, lo inicializa con el contrato seleccionado y le lanza el evento de borrar.
    Esta es la forma adecuada de borrar desde el listado ya que asi preservamos el tema plugins.
*/
void ContratosList::on_mui_borrar_clicked() {
    _depura("ContratosList::on_mui_borrar_clicked", 0);
    try {
/*
        QString idcontrato = mui_list->DBvalue("idcontrato");
        ClienteView *cli = m_companyact->newClienteView();
        if (cli->cargar(idcontrato)) {
            delete cli;
            throw -1;
        } // end if
        cli->on_mui_borrar_clicked();
        delete cli;
        presenta();
*/
    } catch (...) {
        mensajeInfo(tr("Error al borrar un contrato"));
    } // end try
    _depura("END:ContratosList::on_mui_borrar_clicked", 0);
}


/** SLOT que responde a la pulsacion del boton exportar.
    Saca un dialog selector de archivo para que indiquemos a que archivo exportar.
    Llama a bulmafact2XML para hacer la exportacion.
*/
void ContratosList::on_mui_exportar_clicked() {
    _depura("ContratosList::on_mui_exportar_clicked", 0);
    QFile filexml(QFileDialog::getSaveFileName(
                      this,
                      tr("Elija el archivo"),
                      confpr->valor(CONF_DIR_USER),
                      tr("Clientes (*.xml)")));

    if (filexml.open(QIODevice::WriteOnly)) {
        bulmafact2XML(filexml, IMPORT_CLIENTES);
        filexml.close();
    } else {
        _depura("ERROR AL ABRIR EL ARCHIVO\n", 2);
    } // end if
    _depura("END ContratosList::on_mui_exportar_clicked", 0);
}


/** SLOT que responde a la pulsacion del boton importar.
    Saca un selector de archivos para indicar desde que archivo importar.
    Llama a XML2BulmaFact para hacer la importacion.
    Refresca el listado.
*/
void ContratosList::on_mui_importar_clicked() {
    _depura("ContratosList::on_mui_importar_clicked", 0);
    QFile filexml(QFileDialog::getOpenFileName(
                      this,
                      tr("Elija el archivo"),
                      confpr->valor(CONF_DIR_USER),
                      tr("Clientes (*.xml)")));

    if (filexml.open(QIODevice::ReadOnly)) {
        XML2BulmaFact(filexml, IMPORT_CLIENTES);
        filexml.close();
        presenta();
    } else {
        _depura("ERROR AL ABRIR EL ARCHIVO\n", 2);
    } // end if
    _depura("ContratosList::on_mui_importar_clicked", 0);
}

/** Establece el modo de funcionamiento como selector para esta ventana
**/
void ContratosList::selectMode() {
    m_modo = SelectMode;
}

/** Establece el modo de funcionamiento como selector para edicion para esta ventana
**/
void ContratosList::editMode() {
    m_modo = EditMode;
}

/** Devuelve el identificador del contrato seleccionado
**/
QString ContratosList::idcontrato() {
    return mdb_idcontrato;
}

/** Devuelve el nombre del contrato seleccionado
**/
QString ContratosList::nomcontrato() {
    return mdb_nomcontrato;
}

/** Devuelve el cif del contrato seleccionado
**/
QString ContratosList::refcontrato() {
    return mdb_refcontrato;
}
/** Oculta la botonera
*/
void ContratosList::hideBotonera() {
    m_botonera->hide();
}

/** Muestra la botonera
**/
void ContratosList::showBotonera() {
    m_botonera->show();
}

/** Oculta el layer de busqueda
**/
void ContratosList::hideBusqueda() {
    m_busqueda->hide();
}

/** Muestra el layer de busqueda
**/
void ContratosList::showBusqueda() {
    m_busqueda->show();
}

/** SLOT automatico que se ejecuta al cambiar el texto del QLineEdit de filtrado general
**/
void ContratosList::on_m_filtro_textChanged(const QString &text) {
    if (text.size() >= 3) {
        on_mui_actualizar_clicked();
    } // end if
}

/** SLOT automatico que se ejecuta al hacer doble click sobre un elemento determinado de la lista
**/
void ContratosList::on_mui_list_itemDoubleClicked(QTableWidgetItem *) {
    on_mui_editar_clicked();
}

/** SLOT automatico que se ejecuta al pulsar sobre el boton de crear en la botonera
**/
void ContratosList::on_mui_crear_clicked() {
    m_companyact->s_newClienteView();
}

/** SLOT automatico que se ejecuta al pulsar sobre el boton de actualizar en la botonera
**/
void ContratosList::on_mui_actualizar_clicked() {
    presenta();
}

/** SLOT automatico que se ejecuta al pulsar sobre el boton configurar en la botonera
**/
void ContratosList::on_mui_configurar_toggled(bool checked) {
    if (checked) {
        mui_list->showConfig();
    } else {
        mui_list->hideConfig();
    } // end if
}

    company *ContratosList::getcompany() {return m_companyact;}

/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
/** PRepara el subformulario para trabajar con la tabla contrato.
    Establece que no se puedan insertar ni borrar lineas.
*/
ContratosListSubform::ContratosListSubform(QWidget *parent, const char *) : SubForm2Bf(parent) {
    _depura("ContratosListSubform::ContratosListSubform", 0);
    setDBTableName("contrato");
    setDBCampoId("idcontrato");
    addSHeader("idcontrato", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("ID contrato"));
    addSHeader("codcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Codigo"));
    addSHeader("cifcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("C.I.F."));
    addSHeader("refcontrato", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Referencia"));
    addSHeader("nomcontrato", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre de contrato"));
    addSHeader("nomcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre alternativo del contrato"));
    addSHeader("fincontrato", DBCampo::DBdate, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha de alta del contrato"));
    addSHeader("ffincontrato", DBCampo::DBdate, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha de baja del contrato"));
    addSHeader("periodicidadcontrato", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha de baja del contrato"));
    setinsercion(FALSE);
    setDelete(FALSE);
    setSortingEnabled(TRUE);
    _depura("END ContratosListSubform::ContratosListSubform", 0);
}

ContratosListSubform::~ContratosListSubform() {
    _depura("ContratosListSubform::~ContratosListSubform", 0);
    _depura("END ContratosListSubform::~ContratosListSubform", 0);
}
