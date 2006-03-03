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
cobropagoview::cobropagoview(empresa * emp, QWidget *parent, const char *name) : cobropagodlg(parent, name) {
    conexionbase = emp->bdempresa();
    empresaactual = emp;

    numdigitos = emp->numdigitosempresa();
    m_listprevcobro->setcompany(empresaactual);

    m_listprevcobro->chargeBudgetLines();
    m_listprevcobro->pintalistlinprevcobro();

    m_cuenta->setempresa(emp);
    s_recalculaSaldo();
}// end cobropagoview


cobropagoview::~cobropagoview() {}


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
}// end s_actualizar


void cobropagoview::s_guardar() {
    m_listprevcobro->guardaListLinPrevCobro();
}// end s_guardar


void cobropagoview::s_recalculaSaldo() {
    _depura("s_recalculaSaldo()");
    m_totalCobros->setText(m_listprevcobro->totalCobro().toQString());
    m_totalPagos->setText(m_listprevcobro->totalPago().toQString());
}// end s_recalculaSaldo




