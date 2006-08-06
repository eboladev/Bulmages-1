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

#include "clientslist.h"
#include "clienteview.h"
#include "company.h"
#include "qtable1.h"
#include "funcaux.h"
#include "pgimportfiles.h"

#define EDIT_MODE 0


ClientsList::ClientsList(company *comp, QWidget *parent, const char *name, Qt::WFlags flag, edmode editmode)
        : QWidget(parent, name, flag), pgimportfiles(comp) {
    _depura("ClientsList::ClientsList", 0);
    setupUi(this);
    m_companyact = comp;
    mui_list->setcompany(comp);
    mdb_idcliente = "";
    mdb_cifcliente = "";
    mdb_nomcliente = "";
    m_modo = editmode;
    hideBusqueda();
    presenta();
    /// Si estamos en el modo edicion metemos la ventana en el lugar apropiado.
    if (m_modo == EditMode) {
        m_companyact->meteWindow(caption(), this);
    } else {
	setCaption(tr("Selector de Clientes"));
	mui_editar->setHidden(TRUE);
	mui_crear->setHidden(TRUE);
	mui_borrar->setHidden(TRUE);
	mui_exportar->setHidden(TRUE);
	mui_importar->setHidden(TRUE);
	mui_imprimir->setHidden(TRUE);
    } // end if
    _depura("END ClientsList::ClientsList", 0);
}


ClientsList::~ClientsList() {
    if (m_modo==EditMode)
        m_companyact->sacaWindow(this);
}


/// Iniciamos los clientes.
/// Hacemos la consulta a la base de datos y presentamos el listado.
void ClientsList::presenta() {
    _depura("ClientsList::presenta", 0);
    cursor2 * cur= m_companyact->cargacursor("SELECT * FROM cliente  WHERE nomcliente LIKE '%" + m_findClient->text() + "%' ORDER BY nomcliente");
    mui_list->cargar(cur);
    delete cur;
    _depura("END ClientsList::presenta", 0);
}


void ClientsList::editar(int row) {
    _depura("ClientsList::editar", 0);
    mdb_idcliente = mui_list->DBvalue("idcliente", row);
    mdb_cifcliente = mui_list->DBvalue("cifcliente", row);
    mdb_nomcliente = mui_list->DBvalue("nomcliente", row);
    if (m_modo == 0) {
        ClienteView *prov = m_companyact->newClienteView();
        if (prov->cargar(mdb_idcliente)) {
            delete prov;
            return;
        } // end if
        m_companyact->m_pWorkspace->addWindow(prov);
        prov->show();
    } else {
        emit(selected(mdb_idcliente));
    } // end if
    _depura("END ClientsList::editar", 0);
}


void ClientsList::on_mui_editar_clicked() {
    if (mui_list->currentRow() < 0) {
        _depura("Debe seleccionar un elemento", 2);
        return;
    } // end if
    editar(mui_list->currentRow());
}


void ClientsList::on_mui_imprimir_clicked() {
    QString archivo = confpr->valor(CONF_DIR_OPENREPORTS) + "clientes.rml";
    QString archivod = confpr->valor(CONF_DIR_USER) + "clientes.rml";
    QString archivologo = confpr->valor(CONF_DIR_OPENREPORTS) + "logo.jpg";

    /// Copiamos el archivo.
#ifdef WINDOWS

    archivo = "copy " + archivo + " " + archivod;
#else

    archivo = "cp " + archivo + " " + archivod;
#endif

    system (archivo.ascii());
    /// Copiamos el logo.
#ifdef WINDOWS

    archivologo = "copy " + archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
#else

    archivologo = "cp " + archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
#endif

    system (archivologo.ascii());

    QFile file;
    file.setName(archivod);
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString buff = stream.read();
    file.close();
    QString fitxersortidatxt;

    /// Linea de totales del presupuesto.
    fitxersortidatxt = "<blockTable style=\"tabla\" repeatRows=\"1\">";
    fitxersortidatxt += mui_list->imprimir();
    fitxersortidatxt += "</blockTable>";

    buff.replace("[story]", fitxersortidatxt);

    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << buff;
        file.close();
    } // end if

    /// Crea el pdf y lo muestra.
    invocaPDF("clientes");
}


void ClientsList::on_mui_borrar_clicked() {
    _depura("ClientsList::on_mui_borrar_clicked", 0);
    try {
        QString idcliente = mui_list->DBvalue("idcliente");
        ClienteView *cli = m_companyact->newClienteView();
        if (cli->cargar(idcliente) ) {
		delete cli;
		throw -1;
	} // end if
        cli->on_mui_borrar_clicked();
        delete cli;
        presenta();
    } catch(...) {
        mensajeInfo(tr("Error al borrar un cliente"));
    } // end try
    _depura("END:ClientsList::on_mui_borrar_clicked", 0);
}


void ClientsList::on_mui_exportar_clicked() {
    QFile filexml(QFileDialog::getSaveFileName(
                      this,
                      tr("Elija el archivo"),
                      confpr->valor(CONF_DIR_USER),
                      tr("Clientes (*.xml)")));

    if(filexml.open(QIODevice::WriteOnly)) {
        bulmafact2XML(filexml, IMPORT_CLIENTES);
        filexml.close();
    } else {
        _depura("ERROR AL ABRIR EL ARCHIVO\n", 2);
    } // end if
}


void ClientsList::on_mui_importar_clicked() {
    QFile filexml(QFileDialog::getOpenFileName(
                      this,
                      tr("Elija el archivo"),
                      confpr->valor(CONF_DIR_USER),
                      tr("Clientes (*.xml)")));

    if (filexml.open(QIODevice::ReadOnly)) {
        XML2BulmaFact(filexml, IMPORT_CLIENTES);
        filexml.close();
        presenta();
    }  else  {
        _depura("ERROR AL ABRIR EL ARCHIVO\n", 2);
    } // end if
}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
ClienteListSubform::ClienteListSubform(QWidget *parent, const char *) : SubForm2Bf(parent) {
    setDBTableName("cliente");
    setDBCampoId("idcliente");
    addSHeader("idcliente", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("ID cliente"));
    addSHeader("codcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Codigo"));
    addSHeader("cifcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("C.I.F."));
    addSHeader("nomcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre de cliente"));
    addSHeader("nomaltcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre alternativo del cliente"));
    addSHeader("bancocliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Numero cuenta corriente"));
    addSHeader("dircliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Direccion"));
    addSHeader("poblcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Poblacion"));
    addSHeader("cpcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Codigo postal"));
    addSHeader("telcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Numero de telefono"));
    addSHeader("faxcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Numero de fax"));
    addSHeader("mailcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Direccion de correo electronica"));
    addSHeader("urlcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Direccion URL"));
    addSHeader("corpcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Corporacion"));
    addSHeader("faltacliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha de alta del cliente"));
    addSHeader("fbajacliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha de baja del cliente"));
    addSHeader("comentcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Comentarios"));
    setinsercion(FALSE);
    setDelete(FALSE);
}

