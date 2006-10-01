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
#include "cobropagoview.h"
#include "postgresiface2.h"
#include "empresa.h"
#include "aplinteligentesview.h"
#include "calendario.h"
#include "busquedacuenta.h"

#include <q3table.h>
#include <qcombobox.h>
/*
CREATE TABLE prevcobro (
    idprevcobro serial PRIMARY KEY,
    fprevistaprevcobro date,
    fcobroprevcobro date,
    fpagoprevcobro integer,
    idcuenta integer REFERENCES cuenta(idcuenta),
    idasiento integer REFERENCES asiento(idasiento),
    cantidadprevistaprevcobro double precision,
    cantidadprevcobro  double precision,
    idregistroiva integer NOT NULL REFERENCES registroiva(idregistroiva),
    tipoprevcobro boolean,
    docprevcobro character varying(50)
);

*/
#define COL_SELECCION                  0
#define COL_IDPREVCOBRO                1
#define COL_FPREVISTAPREVCOBRO         2
#define COL_FCOBROPREVCOBRO            3
#define COL_IDCUENTA                   4
#define COL_CODIGO                     5
#define COL_NOMCTA	               6
#define COL_IDASIENTO                  7
#define COL_ORDENASIENTO               8
#define COL_CANTIDADPREVISTAPREVCOBRO  9
#define COL_CANTIDADPREVCOBRO          10
#define COL_IDREGISTROIVA              11
#define COL_CODIGOCTAREGISTROIVA       12
#define COL_ENTREGISTROIVA             13
#define COL_TIPOPREVCOBRO              14
#define COL_DOCPREVCOBRO               15

#include "listlinprevcobroview.h"

/**
  * \brief Inicia los encabezados de la tabla y llama a la presentación del listado.
  */
cobropagoview::cobropagoview(empresa * emp, QWidget *parent) : Ficha(parent, Qt::WDestructiveClose) {
    _depura("cobropagoview::cobropagoview", 0);
    setupUi(this);
    conexionbase = emp->bdempresa();
    m_companyact = emp;

    mui_listado->setcompany(m_companyact);

    numdigitos = emp->numdigitosempresa();
    m_listprevcobro->setcompany(m_companyact);

    m_listprevcobro->chargeBudgetLines();
    m_listprevcobro->pintalistlinprevcobro();


    /// Inicializamos el listado
    ////////////////////////////////////////////////////////
/*
#define COL_SELECCION                  0
#define COL_IDPREVCOBRO                1
#define COL_FPREVISTAPREVCOBRO         2
#define COL_FCOBROPREVCOBRO            3
#define COL_IDCUENTA                   4
#define COL_CODIGO                     5
#define COL_NOMCTA	               6
#define COL_IDASIENTO                  7
#define COL_ORDENASIENTO               8
#define COL_CANTIDADPREVISTAPREVCOBRO  9
#define COL_CANTIDADPREVCOBRO          10
#define COL_IDREGISTROIVA              11
#define COL_CODIGOCTAREGISTROIVA       12
#define COL_ENTREGISTROIVA             13
#define COL_TIPOPREVCOBRO              14
#define COL_DOCPREVCOBRO               15
*/
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
   /// Dejamos de inicializar el listado



    m_cuenta->setempresa(emp);
    s_recalculaSaldo();






    m_companyact->meteWindow(windowTitle(), this);
    _depura("END cobropagoview::cobropagoview", 0);
}


cobropagoview::~cobropagoview() {
   _depura("cobropagoview::~cobropagoview", 0);
   m_companyact->sacaWindow(this);
   _depura("END cobropagoview::~cobropagoview", 0);
}


/**
  * \brief SLOT que responde a la pulsacion del botón de actualizar
  */
void cobropagoview::s_actualizar() {
    fprintf(stderr,"actualizar \n");

    m_listprevcobro->s_setfinprevcobro(m_firstDate->text());
    m_listprevcobro->s_setffiprevcobro(m_lastDate->text());

    if (m_tipoprevcobro->currentText() == "COBROS")
        m_listprevcobro->s_settipoprevcobro("t");

    if (m_tipoprevcobro->currentText() == "PAGOS")
        m_listprevcobro->s_settipoprevcobro("f");

    if (m_tipoprevcobro->currentText() == "TODO")
        m_listprevcobro->s_settipoprevcobro("");
    m_listprevcobro->s_setprocesado(m_procesado->currentText());


    m_listprevcobro->chargeBudgetLines();
    m_listprevcobro->pintalistlinprevcobro();
    s_recalculaSaldo();


/// Hacemos la presentacion con la nueva clase
/*
    fprintf(stderr,"ListLinPrevCobro::chargeBudgetLines\n");
    fprintf(stderr,"Hacemos la carga del cursor\n");
    if (mfilt_idregistroiva != "")
        cadwhere = " AND idregistroiva = "+mfilt_idregistroiva;
    if (mfilt_finprevcobro != "")
        cadwhere += " AND fcobroprevcobro >= '"+mfilt_finprevcobro+"'";
    if ( mfilt_codigocuentaprevcobro != "")
        cadwhere += " AND idcuenta = id_cuenta('"+mfilt_codigocuentaprevcobro+"')";
    if ( mfilt_tipoprevcobro != "")
        cadwhere += " AND tipoprevcobro = '"+mfilt_tipoprevcobro+"'";
    if (mfilt_procesado == "PROCESADO")
        cadwhere += " AND idasiento IS NOT NULL ";
    if (mfilt_procesado == "NO PROCESADO")
        cadwhere += " AND idasiento IS NULL ";
*/
    QString cadwhere = "";

    mui_listado->cargar("SELECT * FROM prevcobro "
                   " LEFT JOIN cuenta ON cuenta.idcuenta=prevcobro.idcuenta "
                   " LEFT JOIN (SELECT idcuenta AS idctacliente, codigo AS codigoctacliente, descripcion AS nomctacliente FROM cuenta) AS T1 ON t1.idctacliente = prevcobro.idctacliente "
                   " WHERE 1=1 "+ cadwhere);


}


void cobropagoview::s_guardar() {
    m_listprevcobro->guardaListLinPrevCobro();
}


void cobropagoview::s_recalculaSaldo() {
    _depura("s_recalculaSaldo()");
    m_totalCobros->setText(m_listprevcobro->totalCobro().toQString());
    m_totalPagos->setText(m_listprevcobro->totalPago().toQString());
}




