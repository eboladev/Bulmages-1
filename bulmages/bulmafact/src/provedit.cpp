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

#include "provedit.h"
#include "company.h"
#include "funcaux.h"
#include "plugins.h"


ProveedorView::ProveedorView(company *comp, QWidget *parent, const char *name)
        : QWidget(parent, name, Qt::WDestructiveClose), DBRecord(comp), dialogChanges(this) {
    _depura("ProveedorView::ProveedorView", 0);
    try {
        setDBTableName("proveedor");
        setDBCampoId("idproveedor");
        addDBCampo("idproveedor", DBCampo::DBint, DBCampo::DBPrimaryKey, tr("ID proveedor"));
        addDBCampo("nomproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr("Nombre del proveedor"));
        addDBCampo("nomaltproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr("Nombre alternativo del proveedor"));
        addDBCampo("cifproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr("C.I.F. del proveedor"));
        addDBCampo("codicliproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr("Codigo cliente"));
        addDBCampo("cbancproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr("Banco proveedor"));
        addDBCampo("dirproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr("Direccion"));
        addDBCampo("poblproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr("Poblacion"));
        addDBCampo("provproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr("Provincia"));
        addDBCampo("cpproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr("Codigo postal"));
        addDBCampo("telproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr("Numero de telefono"));
        addDBCampo("faxproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr("Numero de fax"));
        addDBCampo("emailproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr("Direccion electronica"));
        addDBCampo("urlproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr("URL"));
        addDBCampo("comentproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr("Comentarios"));
        addDBCampo("codproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr("Codigo"));

        setupUi(this);
        m_companyact = comp;

        m_provproveedor->setcompany(m_companyact);
        m_provproveedor->setProvincia("");

        /// Desabilitamos los tabs que aun no se usan
        masdf->setTabEnabled(5, FALSE);
        masdf->setTabEnabled(6, FALSE);
        masdf->setTabEnabled(7, FALSE);

        /// Cargamos el listado de pedidos del proveedor y dejamos presentable.
        _depura("inicializamos el listado de pedidos\n", 0);
        m_listpedidosprov->setcompany(m_companyact);
        m_listpedidosprov->hideBusqueda();

        m_albaranesprov->setcompany(m_companyact);
        m_albaranesprov->hideBusqueda();

        m_listfacturasprov->setcompany(m_companyact);
        m_listfacturasprov->hideBusqueda();

        m_listpagosprov->setcompany(m_companyact);
        m_listpagosprov->hideBusqueda();

        m_companyact->meteWindow(caption(), this, FALSE);
        dialogChanges_cargaInicial();
    } catch(...) {
        mensajeInfo(tr("Error al crear el proveedor"));
    } // end try

    _depura("END ProveedorView::ProveedorView", 0);
}


ProveedorView::~ProveedorView() {
    m_companyact->sacaWindow(this);
}


/// Esta funcion carga un proveedor de la base de datos y lo presenta.
/// Si el parametro pasado no es un identificador valido entonces se pone
/// la ventana de edicion en modo de insercion.
int ProveedorView::cargar(QString idprov) {
    _depura("ProveedorView::cargar", 0);
    try {
        QString query = "SELECT * FROM proveedor WHERE idproveedor = " + idprov;
        cursor2 * cur = m_companyact->cargacursor(query);
        if (!cur->eof()) {
            DBload(cur);
        } // end if
        delete cur;
        m_nomproveedor->setText(DBvalue("nomproveedor"));
        m_nomaltproveedor->setText(DBvalue("nomaltproveedor"));
        m_cifproveedor->setText(DBvalue("cifproveedor" ));
        m_codicliproveedor->setText(DBvalue("codicliproveedor" ));
        m_cbancproveedor->setText(DBvalue("cbancproveedor" ));
        m_dirproveedor->setText(DBvalue("dirproveedor"));
        m_poblproveedor->setText( DBvalue("poblproveedor" ));
        m_cpproveedor->setText(DBvalue("cpproveedor"));
        m_telproveedor->setText(DBvalue("telproveedor"));
        m_faxproveedor->setText(DBvalue("faxproveedor"));
        m_emailproveedor->setText(DBvalue("emailproveedor"));
        m_urlproveedor->setText(DBvalue("urlproveedor"));
        m_comentproveedor->setText(DBvalue("comentproveedor" ));
        m_provproveedor->setProvincia(DBvalue("provproveedor"));
        mui_codproveedor->setText(DBvalue("codproveedor"));
        dialogChanges_cargaInicial();

        /// Cargamos las ventanas auxiliares.
        m_listpedidosprov->setidproveedor(DBvalue("idproveedor"));
        m_listpedidosprov->presenta();
        m_albaranesprov->setidproveedor(DBvalue("idproveedor"));
        m_albaranesprov->presenta();
        m_listfacturasprov->setidproveedor(DBvalue("idproveedor"));
        m_listfacturasprov->presenta();
        m_listpagosprov->setidproveedor(DBvalue("idproveedor"));
        m_listpagosprov->presentar();

        /// Cambiamos el titulo de la ventana para que salga reflejado donde toca.
        setWindowTitle(tr("Proveedor") + " " + DBvalue("nomproveedor"));
        m_companyact->meteWindow(caption(), this);
    } catch(...) {
        return -1;
    } // end try

    _depura("END ProveedorView::cargar", 0);
    return 0;
}


/// Esta funcion se ejecuta cuando se ha pulsado sobre el boton de nuevo.
void ProveedorView::on_mui_crear_clicked() {
    if (dialogChanges_hayCambios()) {
        int val = QMessageBox::warning(this,
                                       tr("Guardar proveedor"),
                                       tr("Desea guardar los cambios?"),
                                       tr("&Si"), tr("&No"), tr("&Cancelar"), 0, 2);
        if (val == 0)
            if (guardar())
                return;
        if (val == 2)
            return;
    } // end if

    DBRecord::DBclear();
    m_nomproveedor->setText("");
    m_nomaltproveedor->setText("");
    m_cifproveedor->setText("");
    m_codicliproveedor->setText("");
    m_cbancproveedor->setText("");
    m_dirproveedor->setText("");
    m_poblproveedor->setText("");
    m_cpproveedor->setText("");
    m_telproveedor->setText("");
    m_faxproveedor->setText("");
    m_emailproveedor->setText("");
    m_urlproveedor->setText("");
    m_comentproveedor->setText("");
    mui_codproveedor->setText("");
    dialogChanges_cargaInicial();
}


/// Esta funcion es la respuesta a la pulsacion del boton de guardar
/// Comprueba si es una insercion o una modificacion y hace los pasos
/// pertinentes.
int ProveedorView::guardar() {
    _depura("ProveedorView::guardar", 0);
    /// Disparamos los plugins con presupuesto_imprimirPresupuesto
    int res = g_plugins->lanza("ProveedorView_guardar", this);
    if (res != 0)
        return 0;
    setDBvalue("nomproveedor", m_nomproveedor->text());
    setDBvalue("nomaltproveedor", m_nomaltproveedor->text());
    setDBvalue("cifproveedor", m_cifproveedor->text());
    setDBvalue("codicliproveedor", m_codicliproveedor->text());
    setDBvalue("cbancproveedor", m_cbancproveedor->text());
    setDBvalue("dirproveedor", m_dirproveedor->text());
    setDBvalue("poblproveedor", m_poblproveedor->text());
    setDBvalue("cpproveedor", m_cpproveedor->text());
    setDBvalue("telproveedor", m_telproveedor->text());
    setDBvalue("faxproveedor", m_faxproveedor->text());
    setDBvalue("emailproveedor", m_emailproveedor->text());
    setDBvalue("urlproveedor", m_urlproveedor->text());
    setDBvalue("comentproveedor", m_comentproveedor->text());
    setDBvalue("provproveedor", m_provproveedor->currentText());
    setDBvalue("codproveedor", mui_codproveedor->text());

    QString id;
    m_companyact->begin();
    try {
        DBRecord::guardar();
        m_companyact->commit();
        dialogChanges_cargaInicial();
        _depura("END ProveedorView::guardar", 0);
        return 0;
    } catch(...) {
        _depura("error al guardar el proveedor", 1);
        m_companyact->rollback();
        return -1;
    }
}


/// Esta funcion se ejecuta cuando se ha pulsado sobre el boton de borrar.
void ProveedorView::on_mui_borrar_clicked() {
    if (DBvalue("idproveedor") != "")
        if (QMessageBox::Yes == QMessageBox::question(this,
                tr("Borrar proveedor"),
                tr("Esta a punto de borrar un proveedor. Estos datos pueden dar problemas."),
                QMessageBox::Yes, QMessageBox::No))
            if (!DBRecord::borrar()) {
                dialogChanges_cargaInicial();
                close();
            } // end if
}


void ProveedorView::closeEvent(QCloseEvent *e) {
    _depura("ProveedorView::closeEvent", 0);
    if (dialogChanges_hayCambios()) {
        int val = QMessageBox::warning(this,
                                       tr("Guardar proveedor"),
                                       tr("Desea guardar los cambios?"),
                                       tr("&Si"), tr("&No"), tr("&Cancelar"), 0, 2);
        if (val == 0)
            if (guardar())
                e->ignore();
        if (val == 2)
            e->ignore();
    } // end if
}


void ProveedorView::on_mui_aceptar_clicked() {
    if (!guardar())
        close();
}

