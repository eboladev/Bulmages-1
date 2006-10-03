/***************************************************************************
 *   Copyright (C) 2004 by J. M. Torres Rigo                               *
 *   joanmi@bulma.net                                                      *
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
#include <QMessageBox>
#include <QCheckBox>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>

#include "providerslist.h"
#include "funcaux.h"
#include "company.h"
#include "provedit.h"
#include "pgimportfiles.h"


ProveedorList::ProveedorList(company *comp, QWidget *parent, Qt::WFlags flag, edmode editmode)
        : QWidget(parent, flag), pgimportfiles(comp) {
    _depura("ProveedorList::ProveedorList", 0);
    setupUi(this);
    m_companyact = comp;
    mui_list->setcompany(comp);
    hideBusqueda();
    m_modo = editmode;
    m_idprovider = "";
    m_cifprovider = "";
    m_nomprovider = "";
    if (m_modo == EditMode) {
        m_companyact->meteWindow(windowTitle(), this);
    } else {
        setWindowTitle(tr("Selector de proveedores"));
        mui_editar->setHidden(TRUE);
        mui_crear->setHidden(TRUE);
        mui_borrar->setHidden(TRUE);
        mui_exportar->setHidden(TRUE);
        mui_importar->setHidden(TRUE);
        mui_imprimir->setHidden(TRUE);
    } // end if
    presenta();
    _depura("END ProveedorList::ProveedorList", 0);
}


ProveedorList::~ProveedorList() {
    if (m_modo == EditMode)
        m_companyact->sacaWindow(this);
}


void ProveedorList::presenta() {
    _depura("ProveedorList::presenta", 0);
    mui_list->cargar("SELECT * FROM proveedor WHERE nomproveedor LIKE '%" + mui_filtro->text() + "%'");
    _depura("END ProveedorList::presenta", 0);
}


void ProveedorList::on_mui_crear_clicked() {
    _depura("ProveedorList::on_mui_crear_clicked", 0);
    ProveedorView *prov = new ProveedorView(m_companyact, 0);
    m_companyact->m_pWorkspace->addWindow(prov);
    prov->show();
    _depura("END ProveedorList::on_mui_crear_clicked", 0);
}


void ProveedorList::s_findProvider() {
    presenta();
}


void ProveedorList::editar(int row) {
    _depura("ProveedorList::editar", 0);
    m_idprovider = mui_list->DBvalue(QString("idproveedor"), row);
    m_cifprovider = mui_list->DBvalue(QString("cifproveedor"), row);
    m_nomprovider = mui_list->DBvalue(QString("nomproveedor"), row);
    if (m_modo == 0) {
        ProveedorView *prov = new ProveedorView(m_companyact, 0);
        if (prov->cargar(mui_list->DBvalue(QString("idproveedor"), row))) {
            delete prov;
            return;
        }
        m_companyact->m_pWorkspace->addWindow(prov);
        prov->show();
    } else {
        emit(selected(m_idprovider));
    } // end if
    _depura("END ProveedorList::editar", 0);
}


void ProveedorList::on_mui_editar_clicked() {
    int a = mui_list->currentRow();
    if (a >= 0)
        editar(a);
    else
        _depura("Debe seleccionar una linea", 2);
}


/// SLOT que responde a la pulsacion de borrar un determinado proveedor
/// Dicha funcion avisa de la perdida de datos y si se decide continuar
/// Se procede a borrar el proveedor.
void ProveedorList::on_mui_borrar_clicked() {
    _depura("ProveedorList::on_mui_borrar_clicked", 0);
    try {
        QString idprov = mui_list->DBvalue(QString("idproveedor"));
        ProveedorView *prov = m_companyact->newProveedorView();
        prov->cargar(idprov);
        prov->on_mui_borrar_clicked();
        delete prov;
        presenta();
    } catch (...) {
        mensajeInfo(tr("Error al borrar el proveedor"));
    } // end try
    _depura("END ProveedorList::on_mui_borrar_clicked", 0);
}


/// SLOT que se ejecuta al pulsar sobre el boton de imprimir en la ventana de proveedores
void ProveedorList::on_mui_imprimir_clicked() {
    _depura("ProveedorList::on_mui_imprimir_clicked", 0);
    mui_list->imprimirPDF(tr("Listado de Proveedores"));
    _depura("END ProveedorList::on_mui_imprimir_clicked", 0);
}


void ProveedorList::on_mui_exportar_clicked() {
    QFile filexml(QFileDialog::getSaveFileName(this,
                  tr("Seleccione el archivo"),
                  confpr->valor(CONF_DIR_USER),
                  tr("Proveedores (*.xml)")));

    if(filexml.open(QIODevice::WriteOnly)) {
        bulmafact2XML(filexml, IMPORT_PROVEEDORES);
        filexml.close();
    } else {
        _depura("ERROR AL ABRIR EL ARCHIVO\n", 2);
    } // end if
}


void ProveedorList::on_mui_importar_clicked() {
    QFile filexml(QFileDialog::getOpenFileName(this,
                  tr("Elija el archivo"),
                  confpr->valor(CONF_DIR_USER),
                  tr("Proveedores (*.xml)")));

    if (filexml.open(QIODevice::ReadOnly)) {
        XML2BulmaFact(filexml, IMPORT_PROVEEDORES);
        filexml.close();
        presenta();
    } else {
        _depura("ERROR AL ABRIR EL ARCHIVO\n", 2);
    } // end if
}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
ProveedorListSubform::ProveedorListSubform(QWidget *parent) : SubForm2Bf(parent) {
    setDBTableName("proveedor");
    setDBCampoId("idproveedor");
    addSHeader("idproveedor", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("ID proveedor"));
    addSHeader("codproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Codigo"));
    addSHeader("cifproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("C.I.F."));
    addSHeader("nomproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre"));
    addSHeader("nomaltproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre alternativo"));
    addSHeader("codicliproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Codigo"));
    addSHeader("cbancproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Numero de cuenta corriente"));
    addSHeader("comentproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Comentarios"));
    addSHeader("dirproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Direccion"));
    addSHeader("poblproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Poblacion"));
    addSHeader("cpproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Codigo postal"));
    addSHeader("telproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Numero de telefono"));
    addSHeader("faxproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Numero de fax"));
    addSHeader("emailproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Direccion de correo electronico"));
    addSHeader("urlproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Direccion de URL"));
    addSHeader("clavewebproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Clave de acceso a la web del proveedor"));
    setinsercion(FALSE);
    setDelete(FALSE);
}

