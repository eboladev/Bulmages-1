//
// C++ Implementation: facturaslist
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "facturaslist.h"
#include "company.h"
#include <qtable.h>
#include <qmessagebox.h>
#include <qpopupmenu.h>
#include "configuracion.h"
#include "facturaview.h"

#define COL_IDFACTURA 0
#define COL_CODIGOALMACEN 1
#define COL_NUMFACTURA 2
#define COL_NOMCLIENTE 3
#define COL_FFACTURA 4
#define COL_CONTACTFACTURA 6
#define COL_TELFACTURA 7
#define COL_COMENTFACTURA 8
#define COL_IDUSUARI 9
#define COL_IDCLIENTE 10
#define COL_IDALMACEN 11
#define COL_IDSERIE_FACTURA 12

/*
- FACTURACIO>Albarans:
-- Albarans pendents: S'entendran com albarans pendents tots aquells dels quals no existeixi ticket, factura ni nofactura.
-- Numero
-- Data
-- Factura a clients.
CREATE TABLE factura (
	idfactura serial PRIMARY KEY,
	idserie_factura char(2) NOT NULL,
   numfactura integer NOT NULL,
   ffactura date,
	idalmacen integer NOT NULL REFERENCES almacen(idalmacen),
   contactfactura character varying(90),
   telfactura character varying(20),
   comentfactura character varying(3000),
   idusuari integer,
   idcliente integer REFERENCES cliente(idcliente),
	UNIQUE (idalmacen, idserie_factura, numfactura)	
);
 
 
-- Linea de presupuesto
-- Numero
-- Descripcio: Descripció de l'article en el moment de ser presupostat.
-- Quantitat
-- PVP: Preu de l'article en el moment de ser pressupostat
-- Descompte: Percentatge de descompte en línia.
-- Linia de pressupost a clients.
CREATE TABLE lfactura (
   idlfactura serial PRIMARY KEY,
   desclfactura character varying(150),
   cantlfactura float,
   pvplfactura float,
   ivalfactura numeric(5,2),
   descuentolfactura float,
   idfactura integer NOT NULL REFERENCES factura(idfactura),
   idarticulo integer REFERENCES articulo(idarticulo)
);
*/



FacturasList::FacturasList(company *comp, QWidget *parent, const char *name)
: FacturasListBase(parent, name) {

    companyact = comp;
    inicializa();
    m_modo=0;
    m_idfactura="";
    companyact->meteWindow(caption(),this);
}


FacturasList::~FacturasList() {}


void FacturasList::inicializa() {
    m_list->setNumRows( 0 );
    m_list->setNumCols( 0 );
    m_list->setSelectionMode( QTable::SingleRow );
    m_list->setSorting( TRUE );
    m_list->setColumnMovingEnabled( TRUE );
    m_list->setNumCols(13);
    m_list->horizontalHeader()->setLabel( COL_IDFACTURA, tr( "COL_IDFACTURA" ) );
    m_list->horizontalHeader()->setLabel( COL_NOMCLIENTE, tr( "Cliente" ) );
    m_list->horizontalHeader()->setLabel( COL_CODIGOALMACEN, tr( "Almacï¿½" ) );
    m_list->horizontalHeader()->setLabel( COL_NUMFACTURA, tr( "N Presupuesto" ) );
    m_list->horizontalHeader()->setLabel( COL_FFACTURA, tr( "Fecha" ) );
    m_list->horizontalHeader()->setLabel( COL_IDSERIE_FACTURA, tr( "Fecha" ) );
    m_list->horizontalHeader()->setLabel( COL_CONTACTFACTURA, tr( "Persona Contacto" ) );
    m_list->horizontalHeader()->setLabel( COL_TELFACTURA, tr( "Telï¿½ono" ) );
    m_list->horizontalHeader()->setLabel( COL_COMENTFACTURA, tr( "Comentarios" ) );
    m_list->horizontalHeader()->setLabel( COL_IDUSUARI, tr("COL_IDUSUARI") );
    m_list->horizontalHeader()->setLabel( COL_IDCLIENTE, tr("COL_IDCLIENTE") );
    m_list->horizontalHeader()->setLabel( COL_IDALMACEN, tr("COL_IDALMACEN") );
    m_list->setColumnWidth(COL_IDFACTURA,75);
    m_list->setColumnWidth(COL_NUMFACTURA,75);
    m_list->setColumnWidth(COL_FFACTURA,100);
    m_list->setColumnWidth(COL_CONTACTFACTURA,200);
    m_list->setColumnWidth(COL_TELFACTURA,150);
    m_list->setColumnWidth(COL_COMENTFACTURA,300);
    m_list->setColumnWidth(COL_IDUSUARI,75);
    m_list->setColumnWidth(COL_IDCLIENTE,75);
    /*
    	m_list->setColumnWidth(COL_IDALMACEN,75);
    	m_list->setColumnWidth(COL_NOMCLIENTE,200);
    	m_list->setColumnWidth(COL_CODIGOALMACEN,75);
    	m_list->hideColumn(COL_IDFACTURA);
    	m_list->hideColumn(COL_IDCLIENTE);
    	m_list->hideColumn(COL_IDALMACEN);
    */

    if (confpr->valor(CONF_MOSTRAR_ALMACEN)!="YES") {
        m_list->hideColumn(COL_CODIGOALMACEN);
    }// end if

    //   listado->setPaletteBackgroundColor(QColor(150,230,230));
    // Establecemos el color de fondo del extracto. El valor lo tiene la clase configuracion que es global.
    m_list->setPaletteBackgroundColor("#EEFFFF");
    m_list->setReadOnly(TRUE);
    cursor2 * cur= companyact->cargacursor("SELECT * FROM factura, cliente, almacen where factura.idcliente=cliente.idcliente AND factura.idalmacen=almacen.idalmacen");
    m_list->setNumRows( cur->numregistros() );
    int i=0;
    while (!cur->eof()) {
        m_list->setText(i,COL_IDFACTURA,cur->valor("idfactura"));
        m_list->setText(i,COL_NUMFACTURA,cur->valor("numfactura"));
        m_list->setText(i,COL_FFACTURA,cur->valor("ffactura"));
        m_list->setText(i,COL_CONTACTFACTURA,cur->valor("contacfactura"));
        m_list->setText(i,COL_TELFACTURA,cur->valor("telfactura"));
        m_list->setText(i,COL_COMENTFACTURA,cur->valor("comentfactura"));
        m_list->setText(i,COL_IDUSUARI,cur->valor("idusuari"));
        m_list->setText(i,COL_IDCLIENTE,cur->valor("idcliente"));
        m_list->setText(i,COL_IDALMACEN,cur->valor("idalmacen"));
        m_list->setText(i,COL_NOMCLIENTE,cur->valor("nomcliente"));
        m_list->setText(i,COL_CODIGOALMACEN,cur->valor("codigoalmacen"));
        i++;
        cur->siguienteregistro();
    }// end while
    delete cur;
}// end inicializa


void FacturasList::doubleclicked(int a, int , int , const QPoint &) {
   m_idfactura = m_list->text(a,COL_IDFACTURA);
   if (m_modo ==0 && m_idfactura != "") {
      FacturaView *bud = new FacturaView(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de Facturas", "company"));
      bud->cargaFactura(m_idfactura);
      bud->show();
   } else {
      close();
   }// end if
}

