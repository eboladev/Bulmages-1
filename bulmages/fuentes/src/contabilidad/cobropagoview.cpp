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

#include <qtable.h>

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

cobropagoview::cobropagoview(empresa * emp, QWidget *parent, const char *name) : cobropagodlg(parent, name) {
    conexionbase = emp->bdempresa();
    empresaactual = emp;
    numdigitos = emp->numdigitosempresa();
    m_list->setNumCols(16);
    m_list->horizontalHeader()->setLabel(COL_IDPREVCOBRO,tr("COL_IDPREVCOBRO") );
    m_list->horizontalHeader()->setLabel(COL_FPREVISTAPREVCOBRO,tr("Fecha Prevista") );
    m_list->horizontalHeader()->setLabel(COL_FCOBROPREVCOBRO,tr("Fecha Efectiva") );
    m_list->horizontalHeader()->setLabel(COL_IDCUENTA,tr("IDCUENTA") );
    m_list->horizontalHeader()->setLabel(COL_CODIGO,tr("Cuenta Bancaria") );
    m_list->horizontalHeader()->setLabel(COL_NOMCTA,tr("Nombre Cuenta Bancaria") );
    m_list->horizontalHeader()->setLabel(COL_IDASIENTO,tr("COL_IDASIENTO") );
    m_list->horizontalHeader()->setLabel(COL_ORDENASIENTO, tr("Asiento") );
    m_list->horizontalHeader()->setLabel(COL_CANTIDADPREVISTAPREVCOBRO,tr("Cantidad Prevista") );
    m_list->horizontalHeader()->setLabel(COL_CANTIDADPREVCOBRO,tr("Cantidad Efectiva") );
    m_list->horizontalHeader()->setLabel(COL_IDREGISTROIVA,tr("COL_IDREGISTROIVA") );
    m_list->horizontalHeader()->setLabel(COL_CODIGOCTAREGISTROIVA, tr("Cuenta Cliente/Proveedor") );
    m_list->horizontalHeader()->setLabel(COL_TIPOPREVCOBRO,tr("Tipo") );
    m_list->horizontalHeader()->setLabel(COL_DOCPREVCOBRO,tr("Documento") );
    m_list->horizontalHeader()->setLabel(COL_CODIGOCTAREGISTROIVA,tr("COL_CODIGOCTAREGISTROIVA") );
    m_list->horizontalHeader()->setLabel(COL_ENTREGISTROIVA,tr("COL_ENTREGISTROIVA") );

    m_list->setColumnWidth(COL_SELECCION,25);
    m_list->hideColumn(COL_IDPREVCOBRO);
    m_list->hideColumn(COL_IDCUENTA);
    m_list->hideColumn(COL_IDASIENTO);
    m_list->hideColumn(COL_IDREGISTROIVA);
    inicializa();
}// end cobropagoview


cobropagoview::~cobropagoview() {}


void cobropagoview::inicializa() {
    QString SQLQuery = "SELECT * FROM prevcobro ";
    SQLQuery += " LEFT JOIN cuenta ON cuenta.idcuenta = prevcobro.idcuenta ";
    SQLQuery += " LEFT JOIN asiento ON prevcobro.idasiento=asiento.idasiento ";
    SQLQuery += " LEFT JOIN (SELECT idregistroiva, contrapartida, codigo AS ccontrapartida, descripcion AS nomcontrapartida FROM REGISTROIVA LEFT JOIN cuenta ON registroiva.contrapartida = cuenta.idcuenta) AS tt ON tt.idregistroiva = prevcobro.idregistroiva ";
    SQLQuery += " ORDER BY fprevistaprevcobro ";
    conexionbase->begin();
    cursor2 *cur = conexionbase->cargacursor(SQLQuery, "Masquerys");
    conexionbase->commit();
    m_list->setNumRows(cur->numregistros());
    int i=0;
    while (! cur->eof()) {
        m_list->setText(i,COL_IDPREVCOBRO, cur->valor("idprevcobro"));
        m_list->setText(i,COL_FPREVISTAPREVCOBRO, cur->valor("fprevistaprevcobro"));
        m_list->setText(i,COL_FCOBROPREVCOBRO, cur->valor("fcobroprevcobro"));
        m_list->setText(i,COL_IDCUENTA, cur->valor("idcuenta"));
        m_list->setText(i,COL_CODIGO, cur->valor("codigo"));
        m_list->setText(i,COL_NOMCTA, cur->valor("descripcion"));
        m_list->setText(i,COL_CODIGOCTAREGISTROIVA, cur->valor("ccontrapartida") );
        m_list->setText(i,COL_ENTREGISTROIVA, cur->valor("nomcontrapartida") );
        m_list->setText(i,COL_IDASIENTO, cur->valor("idasiento"));
        m_list->setText(i,COL_ORDENASIENTO, cur->valor("ordenasiento"));
        m_list->setText(i,COL_CANTIDADPREVISTAPREVCOBRO, cur->valor("cantidadprevistaprevcobro"));
        m_list->setText(i,COL_CANTIDADPREVCOBRO, cur->valor("cantidadprevcobro"));
        m_list->setText(i,COL_IDREGISTROIVA, cur->valor("idregistroiva"));
        m_list->setText(i,COL_TIPOPREVCOBRO, cur->valor("tipoprevcobro"));
        m_list->setText(i,COL_DOCPREVCOBRO, cur->valor("docprevcobro"));

        if (cur->valor("idasiento") == "") {
            QCheckTableItem *item = new QCheckTableItem(m_list, "");
            m_list->setItem(i,COL_SELECCION,item);
        }// end if
        cur->siguienteregistro();
        i++;
    }// end while
    delete cur;
}// end inicializa
