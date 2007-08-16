/***************************************************************************
 *   Copyright (C) 2003 by Josep Burcion                                   *
 *   josep@burcion.com                                                     *
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

#include <QComboBox>

#include "cobropagoview.h"
#include "postgresiface2.h"
#include "empresa.h"
#include "aplinteligentesview.h"
#include "busquedacuenta.h"


/// Inicia los encabezados de la tabla y llama a la presentación del listado.
cobropagoview::cobropagoview(Empresa *emp, QWidget *parent)
        : Ficha(parent) {
    _depura("cobropagoview::cobropagoview", 0);
    setAttribute(Qt::WA_DeleteOnClose);
    setupUi(this);
    m_companyact = emp;
    mui_listado->setEmpresaBase(m_companyact);
    /// Inicializamos el listado.
    mui_listado->setDBTableName("prevcobro");
    mui_listado->setDBCampoId("idprevcobro");
    mui_listado->addSHeader("idprevcobro", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoWrite , tr("idprevcobro"));
    mui_listado->addSHeader("fprevistaprevcobro", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoWrite , tr("fprevistaprevcobro"));
    mui_listado->addSHeader("fcobroprevcobro", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNoWrite , tr("fcobroprevcobro"));
    mui_listado->addSHeader("idctacliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite | SHeader::DBNoView, tr("idctacliente"));
    mui_listado->addSHeader("idfpago", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite , tr("idfpago"));
    mui_listado->addSHeader("idcuenta", DBCampo::DBint, DBCampo::DBNothing, SHeader::DBNone , tr("idcuenta"));
    mui_listado->addSHeader("idasiento", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone , tr("idasiento"));
    mui_listado->addSHeader("cantidadprevistaprevcobro", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone , tr("cantidadprevistaprevcobro"));
    mui_listado->addSHeader("cantidadprevcobro", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone , tr("cantidadprevcobro"));
    mui_listado->addSHeader("idregistroiva", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone , tr("idregistroiva"));
    mui_listado->addSHeader("tipoprevcobro", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone , tr("tipoprevcobro"));
    mui_listado->addSHeader("docprevcobro", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone , tr("docprevcobro"));
    mui_listado->setinsercion(FALSE);
    /// Dejamos de inicializar el listado.
    m_cuenta->setEmpresa(emp);
    on_mui_actualizar_clicked();
    m_companyact->meteWindow(windowTitle(), this);
    _depura("END cobropagoview::cobropagoview", 0);
}


cobropagoview::~cobropagoview() {
    _depura("cobropagoview::~cobropagoview", 0);
    m_companyact->sacaWindow(this);
    _depura("END cobropagoview::~cobropagoview", 0);
}


/// SLOT que responde a la pulsacion del botón de actualizar.
void cobropagoview::on_mui_actualizar_clicked() {
    _depura("cobropagoview::s_actualizar", 0);

    /// Hacemos la presentacion con la nueva clase
    QString cadwhere = "";
    mui_listado->cargar("SELECT * FROM prevcobro "
                        " LEFT JOIN cuenta ON cuenta.idcuenta=prevcobro.idcuenta "
                        " LEFT JOIN (SELECT idcuenta AS idctacliente, codigo AS codigoctacliente, descripcion AS nomctacliente FROM cuenta) AS T1 ON t1.idctacliente = prevcobro.idctacliente "
                        " WHERE 1=1 "+ cadwhere);
    s_recalculaSaldo();
    _depura("END cobropagoview::s_actualizar", 0);
}


void cobropagoview::s_guardar() {
    mui_listado->guardar();
}


void cobropagoview::s_recalculaSaldo() {
    _depura("s_recalculaSaldo()", 0);
    Fixed totalcobro("0");
    Fixed totalpago("0");
    for (int i = 0; i < mui_listado->rowCount(); i++) {
        SDBRecord *rec = mui_listado->lineaat(i);
        if (rec) {
            if (rec->DBvalue("tipoprevcobro") == "f") {
                totalcobro = totalcobro + Fixed(rec->DBvalue("cantidadprevcobro"));
            } else {
                totalpago = totalpago + Fixed(rec->DBvalue("cantidadprevcobro"));
            } // end if
        } // end if
    } // end for
    m_totalCobros->setText(totalcobro.toQString());
    m_totalPagos->setText(totalpago.toQString());
    _depura("END s_recalculaSaldo()", 0);
}

