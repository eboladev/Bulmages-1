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
#include "qtable1.h"
#include <qmessagebox.h>
#include <qpopupmenu.h>
#include "busquedacliente.h"
#include "busquedaarticulo.h"
#include "busquedafecha.h"

#include "configuracion.h"
#include "facturaview.h"

#define COL_REFFACTURA 0
#define COL_IDFACTURA 1
#define COL_CODIGOALMACEN 2
#define COL_NUMFACTURA 3
#define COL_NOMCLIENTE 4
#define COL_FFACTURA 5
#define COL_CONTACTFACTURA 6
#define COL_TELFACTURA 7
#define COL_COMENTFACTURA 8
#define COL_IDUSUARI 9
#define COL_IDCLIENTE 10
#define COL_IDALMACEN 11
#define COL_IDSERIE_FACTURA 12

void FacturasList::s_configurar() {
    if(mver_reffactura->isChecked() )
        m_list->showColumn(COL_REFFACTURA);
    else
        m_list->hideColumn(COL_REFFACTURA);

    if(mver_idfactura->isChecked() )
        m_list->showColumn(COL_IDFACTURA);
    else
        m_list->hideColumn(COL_IDFACTURA);

    if(mver_codigoalmacen->isChecked() )
        m_list->showColumn(COL_CODIGOALMACEN);
    else
        m_list->hideColumn(COL_CODIGOALMACEN);

    if(mver_numfactura->isChecked() )
        m_list->showColumn(COL_NUMFACTURA);
    else
        m_list->hideColumn(COL_NUMFACTURA);

    if(mver_nomcliente->isChecked() )
        m_list->showColumn(COL_NOMCLIENTE);
    else
        m_list->hideColumn(COL_NOMCLIENTE);

    if(mver_ffactura->isChecked() )
        m_list->showColumn(COL_FFACTURA);
    else
        m_list->hideColumn(COL_FFACTURA);

    if(mver_contactfactura->isChecked() )
        m_list->showColumn(COL_CONTACTFACTURA);
    else
        m_list->hideColumn(COL_CONTACTFACTURA);

    if(mver_telfactura->isChecked() )
        m_list->showColumn(COL_TELFACTURA);
    else
        m_list->hideColumn(COL_TELFACTURA);

    if(mver_comentfactura->isChecked() )
        m_list->showColumn(COL_COMENTFACTURA);
    else
        m_list->hideColumn(COL_COMENTFACTURA);

    if(mver_idusuari->isChecked() )
        m_list->showColumn(COL_IDUSUARI);
    else
        m_list->hideColumn(COL_IDUSUARI);

    if(mver_idcliente->isChecked() )
        m_list->showColumn(COL_IDCLIENTE);
    else
        m_list->hideColumn(COL_IDCLIENTE);

    if(mver_idalmacen->isChecked() )
        m_list->showColumn(COL_IDALMACEN);
    else
        m_list->hideColumn(COL_IDALMACEN);

    if(mver_idserie_factura->isChecked() )
        m_list->showColumn(COL_IDSERIE_FACTURA);
    else
        m_list->hideColumn(COL_IDSERIE_FACTURA);
}// end s_configurar

FacturasList::FacturasList(QWidget *parent, const char *name, int flag)
: FacturasListBase(parent, name, flag) {
    companyact = NULL;
    m_modo=0;
    m_idfactura="";
    meteWindow(caption(),this);
    hideBusqueda();
    hideConfiguracion();
}// end providerslist

FacturasList::FacturasList(company *comp, QWidget *parent, const char *name)
: FacturasListBase(parent, name) {
    companyact = comp;
    m_cliente->setcompany(companyact);
    m_articulo->setcompany(companyact);
    inicializa();
    m_modo=0;
    m_idfactura="";
    meteWindow(caption(),this);
    hideBusqueda();
    hideConfiguracion();
}


FacturasList::~FacturasList() {
    companyact->sacaWindow(this);
}


void FacturasList::inicializa() {
    m_list->setNumRows( 0 );
    m_list->setNumCols( 0 );
    m_list->setSelectionMode( QTable::SingleRow );
    m_list->setSorting( TRUE );
    m_list->setColumnMovingEnabled( TRUE );
    m_list->setNumCols(13);
    m_list->horizontalHeader()->setLabel( COL_REFFACTURA, tr( "COL_REFFACTURA" ) );
    m_list->horizontalHeader()->setLabel( COL_IDFACTURA, tr( "COL_IDFACTURA" ) );
    m_list->horizontalHeader()->setLabel( COL_NOMCLIENTE, tr( "Cliente" ) );
    m_list->horizontalHeader()->setLabel( COL_CODIGOALMACEN, tr( "Almacén" ) );
    m_list->horizontalHeader()->setLabel( COL_NUMFACTURA, tr( "N Presupuesto" ) );
    m_list->horizontalHeader()->setLabel( COL_FFACTURA, tr( "Fecha" ) );
    m_list->horizontalHeader()->setLabel( COL_IDSERIE_FACTURA, tr( "Fecha" ) );
    m_list->horizontalHeader()->setLabel( COL_CONTACTFACTURA, tr( "Persona Contacto" ) );
    m_list->horizontalHeader()->setLabel( COL_TELFACTURA, tr( "Teléfono" ) );
    m_list->horizontalHeader()->setLabel( COL_COMENTFACTURA, tr( "Comentarios" ) );
    m_list->horizontalHeader()->setLabel( COL_IDUSUARI, tr("COL_IDUSUARI") );
    m_list->horizontalHeader()->setLabel( COL_IDCLIENTE, tr("COL_IDCLIENTE") );
    m_list->horizontalHeader()->setLabel( COL_IDALMACEN, tr("COL_IDALMACEN") );
    m_list->setColumnWidth(COL_REFFACTURA,75);
    m_list->setColumnWidth(COL_IDFACTURA,75);
    m_list->setColumnWidth(COL_NUMFACTURA,75);
    m_list->setColumnWidth(COL_FFACTURA,100);
    m_list->setColumnWidth(COL_CONTACTFACTURA,200);
    m_list->setColumnWidth(COL_TELFACTURA,150);
    m_list->setColumnWidth(COL_COMENTFACTURA,300);
    m_list->setColumnWidth(COL_IDUSUARI,75);
    m_list->setColumnWidth(COL_IDCLIENTE,75);

    if (confpr->valor(CONF_MOSTRAR_ALMACEN)!="YES") {
        m_list->hideColumn(COL_CODIGOALMACEN);
    }// end if
    // Establecemos el color de fondo del extracto. El valor lo tiene la clase configuracion que es global.
    m_list->setPaletteBackgroundColor(confpr->valor(CONF_BG_LISTFACTURASCLIENTE));
    m_list->setReadOnly(TRUE);
    cursor2 * cur= companyact->cargacursor("SELECT * FROM factura LEFT JOIN cliente ON factura.idcliente=cliente.idcliente LEFT JOIN  almacen ON  factura.idalmacen=almacen.idalmacen WHERE 1=1  "+generaFiltro());
    m_list->setNumRows( cur->numregistros() );
    int i=0;
    while (!cur->eof()) {
        m_list->setText(i,COL_REFFACTURA,cur->valor("reffactura"));
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

	/// Hacemos el calculo del total.
	cur = companyact->cargacursor("SELECT SUM(calctotalfactura(idfactura)) AS total FROM factura LEFT JOIN cliente ON factura.idcliente=cliente.idcliente LEFT JOIN  almacen ON  factura.idalmacen=almacen.idalmacen WHERE 1=1  "+generaFiltro());
	m_total->setText(cur->valor("total"));
	delete cur;


   s_configurar();
}// end inicializa


QString FacturasList::generaFiltro() {
    /// Tratamiento de los filtros.
    fprintf(stderr,"Tratamos el filtro \n");
    QString filtro="";
    if (m_filtro->text() != "") {
        filtro = " AND ( descfactura LIKE '%"+m_filtro->text()+"%' ";
        filtro +=" OR nomcliente LIKE '%"+m_filtro->text()+"%') ";
    } else {
        filtro = "";
    }// end if
    if (m_cliente->idcliente() != "") {
        filtro += " AND factura.idcliente="+m_cliente->idcliente();
    }// end if
    if (!m_procesada->isChecked() ) {
        filtro += " AND NOT procesadafactura";
    }// end if
    if (m_articulo->idarticulo() != "") {
        filtro += " AND idfactura IN (SELECT DISTINCT idfactura FROM lfactura WHERE idarticulo='"+m_articulo->idarticulo()+"')";
    }// end if
    if (m_fechain->text() != "") {
	filtro += " AND ffactura >= '"+m_fechain->text()+"' ";
    }// end if
    if (m_fechafin->text() != "") {
	filtro += " AND ffactura <= '"+m_fechafin->text()+"' ";
    }// end if

//    filtro += " ORDER BY numfactura";
    return (filtro);
}// end generaFiltro

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

