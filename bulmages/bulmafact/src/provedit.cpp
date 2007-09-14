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
#include <QCloseEvent>

#include "company.h"
#include "funcaux.h"
#include "plugins.h"
#include "provedit.h"


ProveedorView::ProveedorView(Company *comp, QWidget *parent)
        : FichaBf(comp, parent) {
    _depura("ProveedorView::ProveedorView", 0);
    setAttribute(Qt::WA_DeleteOnClose);
    try {
        setTitleName(tr("Proveedor"));
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
        addDBCampo("idprovincia", DBCampo::DBint, DBCampo::DBNothing, tr("Provincia"));
        addDBCampo("cpproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr("Codigo postal"));
        addDBCampo("telproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr("Numero de telefono"));
        addDBCampo("faxproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr("Numero de fax"));
        addDBCampo("emailproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr("Direccion electronica"));
        addDBCampo("urlproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr("URL"));
        addDBCampo("comentproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr("Comentarios"));
        addDBCampo("codproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, tr("Codigo"));
        addDBCampo("regimenfiscalproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Proveedor", "Regimen Fiscal"));
        addDBCampo("idforma_pago", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("Proveedor", "Forma_Pago"));
        addDBCampo("recargoeqproveedor", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate("Proveedor", "Recargo de Equivalencia"));
        addDBCampo("irpfproveedor", DBCampo::DBnumeric, DBCampo::DBNothing, QApplication::translate("Proveedor", "IRPF"));


        setupUi(this);

        m_provproveedor->setEmpresaBase(empresaBase());
        m_provproveedor->setIdProvincia("");

        /// Desabilitamos los tabs que aun no se usan
        masdf->setTabEnabled(5, FALSE);
        masdf->setTabEnabled(6, FALSE);
        masdf->setTabEnabled(7, FALSE);

        /// Cargamos el listado de pedidos del proveedor y dejamos presentable.
        m_listpedidosprov->setEmpresaBase(empresaBase());
        m_listpedidosprov->hideBusqueda();

        m_albaranesprov->setEmpresaBase(empresaBase());
        m_albaranesprov->hideBusqueda();

        m_listfacturasprov->setEmpresaBase(empresaBase());
        m_listfacturasprov->hideBusqueda();

        m_listpagosprov->setEmpresaBase(empresaBase());
        m_listpagosprov->hideBusqueda();

        mui_forma_pago->setEmpresaBase(empresaBase());
        mui_forma_pago->setidforma_pago("0");


        meteWindow(windowTitle(), this, FALSE);
        dialogChanges_cargaInicial();
    } catch (...) {
        mensajeInfo(tr("Error al crear el proveedor"));
    } // end try

    _depura("END ProveedorView::ProveedorView", 0);
}


ProveedorView::~ProveedorView() {
    _depura("ProveedorView::~ProveedorView", 0);
    _depura("END ProveedorView::~ProveedorView", 0);
}



/// Esta funcion carga un proveedor de la base de datos y lo presenta.
/// Si el parametro pasado no es un identificador valido entonces se pone
/// la ventana de edicion en modo de insercion.
int ProveedorView::cargar(QString idprov) {
    _depura("ProveedorView::cargar", 0, idprov);
    try {
        QString query = "SELECT * FROM proveedor WHERE idproveedor = " + idprov;
        cursor2 * cur = empresaBase()->cargacursor(query);
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
        m_comentproveedor->setPlainText(DBvalue("comentproveedor" ));
        m_provproveedor->setIdProvincia(DBvalue("idprovincia"));
        mui_codproveedor->setText(DBvalue("codproveedor"));
        mui_forma_pago->setidforma_pago(DBvalue("idforma_pago"));
        mui_regimenfiscalproveedor->setRegimenFiscal(DBvalue("regimenfiscalproveedor"));
        mui_irpfproveedor->setText(DBvalue("irpfproveedor"));

        /// Pintamos el recargo de equivalencia
        if (DBvalue("recargoeqproveedor") == "t") {
            mui_recargoeqproveedor->setChecked(TRUE);
        } else {
            mui_recargoeqproveedor->setChecked(FALSE);
        } // end if


        /// Cargamos las ventanas auxiliares.
        m_listpedidosprov->setidproveedor(DBvalue("idproveedor"));
        m_listpedidosprov->presentar();
        m_albaranesprov->setidproveedor(DBvalue("idproveedor"));
        m_albaranesprov->presentar();
        m_listfacturasprov->setidproveedor(DBvalue("idproveedor"));
        m_listfacturasprov->presentar();
        m_listpagosprov->setidproveedor(DBvalue("idproveedor"));
        m_listpagosprov->presentar();


        /// Reseteamos el control de cambios.
        dialogChanges_cargaInicial();

        /// Cambiamos el titulo de la ventana para que salga reflejado donde toca.
        setWindowTitle(tr("Proveedor") + " " + DBvalue("nomproveedor"));
        meteWindow(windowTitle(), this);
    } catch (...) {
        return -1;
    } // end try

    _depura("END ProveedorView::cargar", 0);
    return 0;
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
    setDBvalue("comentproveedor", m_comentproveedor->toPlainText());
    setDBvalue("idprovincia", m_provproveedor->idProvincia());
    setDBvalue("codproveedor", mui_codproveedor->text());
    setDBvalue("idforma_pago", mui_forma_pago->idforma_pago());
    setDBvalue("recargoeqproveedor",  mui_recargoeqproveedor->isChecked() ? "TRUE" : "FALSE");
    setDBvalue("regimenfiscalproveedor", mui_regimenfiscalproveedor->currentText());
    setDBvalue("irpfproveedor", mui_irpfproveedor->text());
    QString id;
    empresaBase()->begin();
    try {
        DBRecord::guardar();
        empresaBase()->commit();
        dialogChanges_cargaInicial();
        _depura("END ProveedorView::guardar", 0);
        return 0;
    } catch (...) {
        _depura("error al guardar el proveedor", 1);
        empresaBase()->rollback();
        return -1;
    }
}


/** Metodo que permite borrar un cliente.
    Hace un delete sobre la base de datos del cliente que esta viendose.
*/
/// \TODO: Deberia meterse dentro de un bloque de depuracion try{} catch{}.
int ProveedorView::borrar() {
    _depura("ProveedorView::borrar", 0);
    DBRecord::borrar();
    empresaBase()->refreshProveedores();
    return 0;
    _depura("END ProveedorView::borrar", 0);
}


