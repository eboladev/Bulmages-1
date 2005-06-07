//
// C++ Implementation: pedidosclientelist
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "pedidosclientelist.h"
#include "pedidoclienteview.h"
#include "company.h"
#include <qtable.h>
#include <qmessagebox.h>
#include <qpopupmenu.h>
#include "configuracion.h"

#define COL_IDPEDIDOCLIENTE 0
#define COL_CODIGOALMACEN 1
#define COL_NUMPEDIDOCLIENTE 2
#define COL_NOMCLIENTE 3
#define COL_FECHAPEDIDOCLIENTE 4
#define COL_CONTACTFACTURA 6
#define COL_TELFACTURA 7
#define COL_COMENTFACTURA 8
#define COL_IDUSUARI 9
#define COL_IDCLIENTE 10
#define COL_IDALMACEN 11
#define COL_IDSERIE_FACTURA 12


PedidosClienteList::PedidosClienteList(company *comp, QWidget *parent, const char *name)
 : PedidosClienteListBase(parent, name)
{


    companyact = comp;
    inicializa();
    m_modo=0;
    m_idpedidocliente="";
    companyact->meteWindow(caption(),this);
}


PedidosClienteList::~PedidosClienteList()
{
}



void PedidosClienteList::inicializa() {
    m_list->setNumRows( 0 );
    m_list->setNumCols( 0 );
    m_list->setSelectionMode( QTable::SingleRow );
    m_list->setSorting( TRUE );
    m_list->setColumnMovingEnabled( TRUE );
    m_list->setNumCols(13);
    m_list->horizontalHeader()->setLabel( COL_IDPEDIDOCLIENTE, tr( "COL_IDPEDIDOCLIENTE" ) );
    m_list->horizontalHeader()->setLabel( COL_NOMCLIENTE, tr( "Cliente" ) );
    m_list->horizontalHeader()->setLabel( COL_CODIGOALMACEN, tr( "Almac�" ) );
    m_list->horizontalHeader()->setLabel( COL_NUMPEDIDOCLIENTE, tr( "N Presupuesto" ) );
    m_list->horizontalHeader()->setLabel( COL_FECHAPEDIDOCLIENTE, tr( "Fecha" ) );
    m_list->horizontalHeader()->setLabel( COL_IDSERIE_FACTURA, tr( "Fecha" ) );
    m_list->horizontalHeader()->setLabel( COL_CONTACTFACTURA, tr( "Persona Contacto" ) );
    m_list->horizontalHeader()->setLabel( COL_TELFACTURA, tr( "Tel�ono" ) );
    m_list->horizontalHeader()->setLabel( COL_COMENTFACTURA, tr( "Comentarios" ) );
    m_list->horizontalHeader()->setLabel( COL_IDUSUARI, tr("COL_IDUSUARI") );
    m_list->horizontalHeader()->setLabel( COL_IDCLIENTE, tr("COL_IDCLIENTE") );
    m_list->horizontalHeader()->setLabel( COL_IDALMACEN, tr("COL_IDALMACEN") );
    m_list->setColumnWidth(COL_IDPEDIDOCLIENTE,75);
    m_list->setColumnWidth(COL_NUMPEDIDOCLIENTE,75);
    m_list->setColumnWidth(COL_FECHAPEDIDOCLIENTE,100);
    m_list->setColumnWidth(COL_CONTACTFACTURA,200);
    m_list->setColumnWidth(COL_TELFACTURA,150);
    m_list->setColumnWidth(COL_COMENTFACTURA,300);
    m_list->setColumnWidth(COL_IDUSUARI,75);
    m_list->setColumnWidth(COL_IDCLIENTE,75);
    /*
    	m_list->setColumnWidth(COL_IDALMACEN,75);
    	m_list->setColumnWidth(COL_NOMCLIENTE,200);
    	m_list->setColumnWidth(COL_CODIGOALMACEN,75);
    	m_list->hideColumn(COL_IDPEDIDOCLIENTE);
    	m_list->hideColumn(COL_IDCLIENTE);
    	m_list->hideColumn(COL_IDALMACEN);
    */

    if (confpr->valor(CONF_MOSTRAR_ALMACEN)!="YES") {
        m_list->hideColumn(COL_CODIGOALMACEN);
    }// end if

    //   listado->setPaletteBackgroundColor(QColor(150,230,230));
    // Establecemos el color de fondo del extracto. El valor lo tiene la clase configuracion que es global.
    m_list->setPaletteBackgroundColor("#FFEEFF");
    m_list->setReadOnly(TRUE);
    cursor2 * cur= companyact->cargacursor("SELECT * FROM pedidocliente, cliente, almacen where pedidocliente.idcliente=cliente.idcliente AND pedidocliente.idalmacen=almacen.idalmacen");
    m_list->setNumRows( cur->numregistros() );
    int i=0;
    while (!cur->eof()) {
        m_list->setText(i,COL_IDPEDIDOCLIENTE,cur->valor("idpedidocliente"));
        m_list->setText(i,COL_NUMPEDIDOCLIENTE,cur->valor("numpedidocliente"));
        m_list->setText(i,COL_FECHAPEDIDOCLIENTE,cur->valor("fechapedidocliente"));
        m_list->setText(i,COL_IDCLIENTE,cur->valor("idcliente"));
        m_list->setText(i,COL_IDALMACEN,cur->valor("idalmacen"));
        m_list->setText(i,COL_NOMCLIENTE,cur->valor("nomcliente"));
        m_list->setText(i,COL_CODIGOALMACEN,cur->valor("codigoalmacen"));
        i++;
        cur->siguienteregistro();
    }// end while
    delete cur;
}// end inicializa


void PedidosClienteList::doubleclicked(int a, int , int , const QPoint &) {
   fprintf(stderr,"PedidosClienteList::doubleclicked()\n");
   m_idpedidocliente = m_list->text(a,COL_IDPEDIDOCLIENTE);
   if (m_modo == 0 && m_idpedidocliente != "") {
      PedidoClienteView *bud = new PedidoClienteView(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de Pedidos de Clientes", "company"));
      bud->cargaPedidoCliente(m_idpedidocliente);
      fprintf(stderr,"Vamos a mostrar el formulario\n");
      bud->show();
      fprintf(stderr,"Hemos mostrado el formulario\n");
   } else {
      close();
   }// end if
}

