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
    m_list->hideColumn(COL_FCOBROPREVCOBRO);
    m_list->hideColumn(COL_CANTIDADPREVCOBRO);
    inicializa();
}// end cobropagoview


cobropagoview::~cobropagoview() {}


/** \brief Inicializa el listado para que muestre los cobros y pagos iniciales.
  * Selecciona los prevcobros de la base de datos
  * Dimensiona las tablas.
  * Itera para cada elemento y pone los valores correspondientes.
  * También crea el checkbox para cada elemento y lo pone en la tabla.
  */
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

/**
  * \brief SLOT que respoonde a la creación de un asiento de cobro o pago a partir de la gestion de cobros y pagos.
  * Descripción:
  * 1.- Calculamos los campos Total, Tipo de Asiento (compra/venta), Cuenta bancaria y cuenta de cliente
  * 2.- Determinamos si es un cobro o un pago.
  * 3.- Cargamos la plantilla de cobro o pago y le metemos los valores necesarios
  * 4.- Generamos el asiento a partir del asiento inteligente.
  */
void cobropagoview::s_creaPago() {
      QString idainteligente;
      double total=0;
      QString codcuenta;
      QString codbanco;
      QString tipo;
      /// Calculamos los campos necesarios.
      /* El calculo de los campos requeridos es una iteración por la tabla. */
      for (int i=0; i< m_list->numRows(); i++) {
      		fprintf(stderr,"Iteración para los elementos de la lista %d\n", i);
		if (m_list->text(i,COL_IDPREVCOBRO) != "") {
			fprintf(stderr,"Existe el elemento %d\n", i);
			QTableItem *check = m_list->item(i,COL_SELECCION);
			fprintf(stderr,"Vamos a testear \n");
			if (check->rtti()==2) {
				QCheckTableItem *check1 = (QCheckTableItem *) check;
				if (check1->isChecked()) {
					fprintf(stderr,"Este entra \n");
					total += m_list->text(i, COL_CANTIDADPREVISTAPREVCOBRO).toFloat();
					codbanco = m_list->text(i, COL_CODIGO);
					codcuenta =  m_list->text(i, COL_CODIGOCTAREGISTROIVA);
					tipo = m_list->text(i, COL_TIPOPREVCOBRO);
					if ( tipo == "t") {
					   tipo = "COBRO"; 
					} else {
					   tipo = "PAGO";
					}// end if
				}// end if
			}// end if
		}// end if
      }// end for
      
      fprintf(stderr,"------------\n");
      fprintf(stderr,"total: %2.2f -- banco %s -- cliente %s -- tipo %s\n", total, codbanco.ascii(), codcuenta.ascii(), tipo.ascii());
      fprintf(stderr,"------------\n");
      
      /// Buscamos cual es el asiento inteligente que realiza la amortización.
      QString query = "SELECT * FROM ainteligente, configuracion WHERE descripcion=valor AND configuracion.nombre='Cobro'";
      conexionbase->begin();
      cursor2 *cur = conexionbase->cargacursor(query,"asiento_de_cobro");
      conexionbase->commit();
      if (!cur->eof()) {
         idainteligente = cur->valor("idainteligente");
      }// end if
      delete cur;	
	
      /// Se va a generar el asiento
      int numasiento = 0; 
      aplinteligentesview *nueva=new aplinteligentesview(0,"");
      nueva->inicializa(conexionbase, numasiento, empresaactual->intapuntsempresa());
      nueva->muestraplantilla(idainteligente.toInt());
/*      nueva->setvalores("$cuenta$",cuentaamort);
      nueva->setvalores("$cuentabien$",cuenta);
      nueva->setvalores("$fechaasiento$",table1->text(row,COL_FECHA));
      nueva->setvalores("$cuota$",table1->text(row,COL_CUOTA));  
*/
}// end s_creaPago


/**
  * \brief SLOT que responde a la pulsación del botón de actualizar
  */
void cobropagoview::s_actualizar() {
	fprintf(stderr,"actualizar \n");
}// end s_actualizar



