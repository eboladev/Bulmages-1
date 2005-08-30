/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borrás Riera                              *
 *   tborras@conetxia.com                                                  *
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

// PRESUPUESTOS
/*
-- Entendemos que un presupuesto es una relación de materiales y trabajos cuantificada que
-- hacemos a petición de un cliente determinado
-- Numero
-- Data: Data d'emisió del presupost.
-- PersContacte: Nom de persona de contacte (si cal).
-- TelfContacte: Teléfon.
-- Venciment: Data máxima de validesa del presupost.
-- Comentaris
--  Pressupost a clients.
CREATE TABLE presupuesto (
   idpresupuesto serial PRIMARY KEY,
   numpresupuesto integer,
   fpresupuesto date,
   contactpresupuesto character varying(90),
   telpresupuesto character varying(20),
   vencpresupuesto date,
   comentpresupuesto character varying(3000),
   idusuari integer,busquedaAlmacen1
   
   idcliente integer REFERENCES cliente(idcliente)
);
*/

/*
-- Linea de presupuesto
-- Numero
-- Descripcio: Descripciónde l'article en el moment de ser presupostat.
-- Quantitat
-- PVP: Preu de l'article en el moment de ser pressupostat
-- Descompte: Percentatge de descompte en línia.
-- Linia de pressupost a clients.
CREATE TABLE lpresupuesto (
   idlpresupuesto integer PRIMARY KEY,
   desclpresupuesto character varying(150),
   cantlpresupuesto float,
   pvplpresupuesto float,
   descuentolpresupuesto float,
   
   idpresupuesto integer NOT NULL REFERENCES presupuesto(idpresupuesto),
   idarituclo integer REFERENCES articulo(idarticulo)
);
*/

#include "cobroview.h"

#include "company.h"
#include "configuracion.h"
#include "busquedacliente.h"
#include "busquedafecha.h"
#include <qmessagebox.h>

#include <qtable.h>
#include <qwidget.h>
#include <qobjectlist.h>
#include <qcombobox.h>
#include <qpopupmenu.h>
#include <qtoolbutton.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qlayout.h>
#include <qmessagebox.h>

#include <fstream>
using namespace std;

#include "funcaux.h"

#define COL_IDLPRESUPUESTO 0
#define COL_IDARTICULO 1
#define COL_CODARTICULO 2
#define COL_NOMARTICULO 3
#define COL_DESCLPRESUPUESTO 4
#define COL_CANTLPRESUPUESTO 5
#define COL_PVPLPRESUPUESTO 6
#define COL_DESCUENTOLPRESUPUESTO 7
#define COL_IDPRESUPUESTO 8
#define COL_REMOVE 9
#define COL_TASATIPO_IVA 10
#define COL_TIPO_IVA 11

#define COL_DESCUENTO_IDDPRESUPUESTO 0
#define COL_DESCUENTO_CONCEPTDPRESUPUESTO 1
#define COL_DESCUENTO_PROPORCIONDPRESUPUESTO 2
#define COL_DESCUENTO_REMOVE 3

#define coma "'"

CobroView::CobroView( company *comp , QWidget *parent, const char *name) : CobroBase(parent, name, Qt::WDestructiveClose) , Cobro(comp) {
    /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
    m_cliente->setcompany(comp);
    inicialize();
//    comp->meteWindow(caption(),this);
    fprintf(stderr,"Fin de la inicialización de CobroView\n");
}// end CobroView


CobroView::~CobroView() {
    companyact->sacaWindow(this);
}// end ~CobroView


void CobroView::inicialize() {
}// end inicialize





void CobroView::s_removeCobroView() {
    fprintf(stderr,"Iniciamos el boton_borrar\n");
    if (QMessageBox::warning( this, "BulmaFact - Presupuestos", "Desea borrar este presupuesto", "Si", "No") == 0) {
        borraCobro();
    }// end if
}// end boton_borrar










