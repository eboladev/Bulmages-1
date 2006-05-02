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

/*
-- Los proveedores son los que nos suminstran articulos y/o servicios.
-- COMPROVACIONS D'INTEGRITAT>Gen�iques:
-- 1 Article t�1 sol prove�or principal.
-- 1 Article t�1 sol prove�or referent.
-- CAMPOS
-- ======
-- Codi: Clau artificial.
-- Nom: Nom comercial o fiscal.
-- Nom_alternatiu: Nom comercial o fiscal.
-- CIF: Codi d'Identificaci�Fiscal.
-- CodiCli: Codi de client amb que ens facturen. �il per a identificar-nos.
-- C_Banc
-- Comentaris
-- Adre�: Adre�.
-- Poblaci� Poblaci�
-- CProv: Codi de provincia (dos primers d�its del codi postal).
-- sCP: Tres darrers d�its del codi postal.
-- Telf: Tel�on.
-- Fax: Fax.
-- Email: eMail.
-- Url: Url.
-- CompteWeb: Dades de login si disposen de tenda o tarifes en l�ia
CREATE TABLE proveedor (
 idproveedor serial PRIMARY KEY,
 nomproveedor character varying(200),
 nomaltproveedor character varying(200),
 cifproveedor character varying(12),
 codicliproveedor character varying(30),
 cbancproveedor character varying(20),
 comentproveedor character varying(2000),
 dirproveedor character varying(50),
 poblproveedor character varying(50),
 cpproveedor character varying(9) NOT NULL,
 telproveedor character varying(12),
 faxproveedor character varying(12),
 emailproveedor character varying(100),
 urlproveedor character varying(100),
 clavewebproveedor character varying(100)
);
*/

#include "provedit.h"
#include "company.h"
#include <QLineEdit>
#include <QMessageBox>
#include <Q3Table>

#include <funcaux.h>


ProveedorView::ProveedorView(company *comp, QWidget *parent, const char *name)
        : QWidget(parent, name, Qt::WDestructiveClose) {
    _depura("ProveedorView::ProveedorView",0);

    setupUi(this);
    m_companyact = comp;
    mdb_idproveedor = "0";

    m_provproveedor->setcompany(m_companyact);
    m_provproveedor->setProvincia("");


    /// Desabilitamos los tabs que aun no se usan
    masdf->setTabEnabled(5,FALSE);
    masdf->setTabEnabled(6,FALSE);
    masdf->setTabEnabled(7,FALSE);


    /// Cargamos el listado de pedidos del proveedor y dejamos presentable.
    _depura("inicializamos el listado de pedidos\n",0);
    m_listpedidosprov->setcompany(m_companyact);
    m_listpedidosprov->hideBusqueda();

    m_albaranesprov->setcompany(m_companyact);
    m_albaranesprov->hideBusqueda();

    m_listfacturasprov->setcompany(m_companyact);
    m_listfacturasprov->hideBusqueda();

    m_listpagosprov->setcompany(m_companyact);
    m_listpagosprov->hideConfiguracion();
    m_listpagosprov->hideBusqueda();

    if (m_companyact->meteWindow(caption(),this))
        return;
    s_releaseModificado();
    _depura("END ProveedorView::ProveedorView",0);
}

ProveedorView::~ProveedorView() {
    m_companyact->sacaWindow(this);
}


/************************************************************************
* Esta funci� carga un proveedor de la base de datos y lo presenta.    *
* Si el parametro pasado no es un identificador v�ido entonces se pone *
* la ventana de edici� en modo de inserci�                            *
*************************************************************************/
int ProveedorView::chargeprovider(QString idprov) {
    _depura("ProveedorView::chargeprovider",0);
    int error = 0;
    mdb_idproveedor = idprov;
    fprintf(stderr,"chargeprovider activado \n");
    if (mdb_idproveedor != "0") {
        QString SQLQuery = "SELECT * FROM proveedor WHERE idproveedor="+idprov;
        cursor2 *cur= m_companyact->cargacursor(SQLQuery);
        if (!cur->eof()) {
            mdb_idproveedor = idprov;
            m_nomproveedor->setText(cur->valor("nomproveedor"));
            m_nomaltproveedor->setText(cur->valor("nomaltproveedor"));
            m_cifproveedor->setText(cur->valor("cifproveedor"));
            m_codicliproveedor->setText(cur->valor("codicliproveedor"));
            m_cbancproveedor->setText(cur->valor("cbancproveedor"));
            m_dirproveedor->setText(cur->valor("dirproveedor"));
            m_poblproveedor->setText(cur->valor("poblproveedor"));
            m_cpproveedor->setText(cur->valor("cpproveedor"));
            m_telproveedor->setText(cur->valor("telproveedor"));
            m_faxproveedor->setText(cur->valor("faxproveedor"));
            m_emailproveedor->setText(cur->valor("emailproveedor"));
            m_urlproveedor->setText(cur->valor("urlproveedor"));
            m_provproveedor->setProvincia(cur->valor("provproveedor"));
            m_comentproveedor->setText(cur->valor("comentproveedor"));


            /// Inicializamos el listado
            _depura("Vamos a cargar el proveedor",0);
            m_listpedidosprov->setidproveedor(cur->valor("idproveedor"));
            m_listpedidosprov->presenta();
            m_albaranesprov->setidproveedor(cur->valor("idproveedor"));
            m_albaranesprov->presenta();
            m_listfacturasprov->setidproveedor(cur->valor("idproveedor"));
            m_listfacturasprov->presenta();
            m_listpagosprov->setidproveedor(cur->valor("idproveedor"));
            m_listpagosprov->presentar();

            /// Cambiamos el titulo de la ventana para que salga reflejado donde toca.
            setCaption("Proveedor "+cur->valor("nomproveedor"));
            error = m_companyact->meteWindow(caption(),this);

        } else {
            mdb_idproveedor="0";
        }// end if
        delete cur;
    }// end if

    if (error)
        return 1;

    s_releaseModificado();
    _depura("END ProveedorView::chargeprovider",0);
    return 0;
}


/************************************************************************
* Esta funci� se ejecuta cuando se ha pulsado sobre el bot� de nuevo  *
*************************************************************************/
void ProveedorView::on_mui_crear_clicked() {
    if (m_modificado  ) {
        if ( QMessageBox::warning( this, "Guardar Proveedor",
                                   "Desea guardar los cambios.",
                                   QMessageBox::Ok ,
                                   QMessageBox::Cancel ) == QMessageBox::Ok)
            on_mui_guardar_clicked();
    }// end if

    mdb_idproveedor = "0";
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
    s_releaseModificado();
}

/*************************************************************************
* Esta funcion es la respuesta a la pulsacion del boton de guardar       *
* Comprueba si es una insercion o una modificacion y hace los pasos      *
* pertinentes                                                            *
**************************************************************************/
void ProveedorView::on_mui_guardar_clicked() {
    if (mdb_idproveedor != "0") {
        QString SQLQuery = "UPDATE proveedor SET urlproveedor='"+m_companyact->sanearCadena(m_urlproveedor->text())+"'";
        SQLQuery += " , nomproveedor='"+m_companyact->sanearCadena(m_nomproveedor->text())+"'";
        SQLQuery += " , nomaltproveedor='"+m_companyact->sanearCadena(m_nomaltproveedor->text())+"'";
        SQLQuery += " , cifproveedor='"+m_companyact->sanearCadena(m_cifproveedor->text())+"'";
        SQLQuery += " , codicliproveedor='"+m_companyact->sanearCadena(m_codicliproveedor->text())+"'";
        SQLQuery += " , cbancproveedor='"+m_companyact->sanearCadena(m_cbancproveedor->text())+"'";
        SQLQuery += " , dirproveedor='"+m_companyact->sanearCadena(m_dirproveedor->text())+"'";
        SQLQuery += " , poblproveedor='"+m_companyact->sanearCadena(m_poblproveedor->text())+"'";
        SQLQuery += " , cpproveedor='"+m_companyact->sanearCadena(m_cpproveedor->text())+"'";
        SQLQuery += " , telproveedor='"+m_companyact->sanearCadena(m_telproveedor->text())+"'";
        SQLQuery += " , faxproveedor='"+m_companyact->sanearCadena(m_faxproveedor->text())+"'";
        SQLQuery += " , emailproveedor='"+m_companyact->sanearCadena(m_emailproveedor->text())+"'";
        SQLQuery += " , provproveedor='"+m_companyact->sanearCadena(m_provproveedor->currentText())+"'";
        SQLQuery += " , comentproveedor='"+m_companyact->sanearCadena(m_comentproveedor->text())+"'";
        SQLQuery += " WHERE idproveedor ="+mdb_idproveedor;
        m_companyact->begin();
        int error = m_companyact->ejecuta(SQLQuery);
        if (error) {
            m_companyact->rollback();
            return;
        }// end if
        m_companyact->commit();
    } else {
        QString SQLQuery = " INSERT INTO proveedor (nomproveedor, nomaltproveedor, cifproveedor, codicliproveedor, cbancproveedor, dirproveedor, poblproveedor, cpproveedor, telproveedor, faxproveedor, urlproveedor, emailproveedor, provproveedor, comentproveedor)";
        SQLQuery += " VALUES (";
        SQLQuery += "'"+m_companyact->sanearCadena(m_nomproveedor->text())+"'";
        SQLQuery += ",'"+m_companyact->sanearCadena(m_nomaltproveedor->text())+"'";
        SQLQuery += ",'"+m_companyact->sanearCadena(m_cifproveedor->text())+"'";
        SQLQuery += ",'"+m_companyact->sanearCadena(m_codicliproveedor->text())+"'";
        SQLQuery += ",'"+m_companyact->sanearCadena(m_cbancproveedor->text())+"'";
        SQLQuery += ",'"+m_companyact->sanearCadena(m_dirproveedor->text())+"'";
        SQLQuery += ",'"+m_companyact->sanearCadena(m_poblproveedor->text())+"'";
        SQLQuery += ",'"+m_companyact->sanearCadena(m_cpproveedor->text())+"'";
        SQLQuery += ",'"+m_companyact->sanearCadena(m_telproveedor->text())+"'";
        SQLQuery += ",'"+m_companyact->sanearCadena(m_faxproveedor->text())+"'";
        SQLQuery += ",'"+m_companyact->sanearCadena(m_urlproveedor->text())+"'";
        SQLQuery += ",'"+m_companyact->sanearCadena(m_emailproveedor->text())+"'";
        SQLQuery += ",'"+m_companyact->sanearCadena(m_provproveedor->currentText())+"'";
        SQLQuery += ",'"+m_companyact->sanearCadena(m_comentproveedor->text())+"'";
        SQLQuery += ")";
        m_companyact->begin();
        int error = m_companyact->ejecuta(SQLQuery);
        if (error) {
            m_companyact->rollback();
            return;
        }// end if
        cursor2 * cur = m_companyact->cargacursor("SELECT max(idproveedor) AS id FROM proveedor");
        mdb_idproveedor = cur->valor("id");
        delete cur;
        m_companyact->commit();
    }// end if
    s_releaseModificado();
}// end accept

/************************************************************************
* Esta funci� se ejecuta cuando se ha pulsado sobre el bot� de borrar *
*************************************************************************/
void ProveedorView::on_mui_borrar_clicked() {
    if (mdb_idproveedor != "0") {
        if ( QMessageBox::Yes == QMessageBox::question(this,"Borrar Proveedor","Esta a punto de borrar un proveedor, Estos datos pueden dar problemas.",QMessageBox::Yes, QMessageBox::No)) {
            QString SQLQuery="DELETE FROM proveedor WHERE idproveedor="+mdb_idproveedor;
            m_companyact->begin();
            int error = m_companyact->ejecuta(SQLQuery);
            if (error) {
                m_companyact->rollback();
                return;
            }// end if
            m_companyact->commit();
            close();
        }// end if
    }// end if
}// end boton_borrar



void ProveedorView::close() {
    if (m_modificado  ) {
        if ( QMessageBox::warning( this, "Guardar Proveedor",
                                   "Desea guardar los cambios.",
                                   QMessageBox::Ok ,
                                   QMessageBox::Cancel ) == QMessageBox::Ok)
            on_mui_guardar_clicked();
    }// end if
    QWidget::close();
    delete this;
}// end close


void ProveedorView::on_mui_aceptar_clicked() {
	on_mui_guardar_clicked();
	close();
}

