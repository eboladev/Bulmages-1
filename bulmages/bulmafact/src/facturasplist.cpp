//
// C++ Implementation: facturapslist
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "facturasplist.h"
#include "company.h"
#include "qtable1.h"
#include <qmessagebox.h>
#include <qpopupmenu.h>
#include "busquedaproveedor.h"
#include "busquedaarticulo.h"
#include "configuracion.h"
#include "facturapview.h"

#define COL_IDFACTURAP 0
#define COL_CODIGOALMACEN 1
#define COL_NUMFACTURAP 2
#define COL_NOMCLIENTE 3
#define COL_FFACTURAP 4
#define COL_CONTACTFACTURAP 6
#define COL_TELFACTURAP 7
#define COL_COMENTFACTURAP 8
#define COL_IDUSUARI 9
#define COL_IDCLIENTE 10
#define COL_IDALMACEN 11
#define COL_IDSERIE_FACTURAP 12

FacturasProveedorList::FacturasProveedorList(QWidget *parent, const char *name, int flag)
: FacturasProveedorListBase(parent, name, flag) {
    companyact = NULL;
    m_modo=0;
    m_idfacturap="";
    meteWindow(caption(),this);
    hideBusqueda();
    hideConfiguracion();
}// end providerslist

FacturasProveedorList::FacturasProveedorList(company *comp, QWidget *parent, const char *name)
: FacturasProveedorListBase(parent, name) {
    companyact = comp;
    m_proveedor->setcompany(companyact);
    m_articulo->setcompany(companyact);
    inicializa();
    m_modo=0;
    m_idfacturap="";
    meteWindow(caption(),this);
    hideBusqueda();
    hideConfiguracion();
}


FacturasProveedorList::~FacturasProveedorList() {}


void FacturasProveedorList::inicializa() {
    m_list->setNumRows( 0 );
    m_list->setNumCols( 0 );
    m_list->setSelectionMode( QTable::SingleRow );
    m_list->setSorting( TRUE );
    m_list->setColumnMovingEnabled( TRUE );
    m_list->setNumCols(13);
    m_list->horizontalHeader()->setLabel( COL_IDFACTURAP, tr( "COL_IDFACTURAP" ) );
    m_list->horizontalHeader()->setLabel( COL_NOMCLIENTE, tr( "Cliente" ) );
    m_list->horizontalHeader()->setLabel( COL_CODIGOALMACEN, tr( "Almacén" ) );
    m_list->horizontalHeader()->setLabel( COL_NUMFACTURAP, tr( "N Presupuesto" ) );
    m_list->horizontalHeader()->setLabel( COL_FFACTURAP, tr( "Fecha" ) );
    m_list->horizontalHeader()->setLabel( COL_IDSERIE_FACTURAP, tr( "Fecha" ) );
    m_list->horizontalHeader()->setLabel( COL_CONTACTFACTURAP, tr( "Persona Contacto" ) );
    m_list->horizontalHeader()->setLabel( COL_TELFACTURAP, tr( "Teléfono" ) );
    m_list->horizontalHeader()->setLabel( COL_COMENTFACTURAP, tr( "Comentarios" ) );
    m_list->horizontalHeader()->setLabel( COL_IDUSUARI, tr("COL_IDUSUARI") );
    m_list->horizontalHeader()->setLabel( COL_IDCLIENTE, tr("COL_IDCLIENTE") );
    m_list->horizontalHeader()->setLabel( COL_IDALMACEN, tr("COL_IDALMACEN") );
    m_list->setColumnWidth(COL_IDFACTURAP,75);
    m_list->setColumnWidth(COL_NUMFACTURAP,75);
    m_list->setColumnWidth(COL_FFACTURAP,100);
    m_list->setColumnWidth(COL_CONTACTFACTURAP,200);
    m_list->setColumnWidth(COL_TELFACTURAP,150);
    m_list->setColumnWidth(COL_COMENTFACTURAP,300);
    m_list->setColumnWidth(COL_IDUSUARI,75);
    m_list->setColumnWidth(COL_IDCLIENTE,75);
    if (confpr->valor(CONF_MOSTRAR_ALMACEN)!="YES") {
        m_list->hideColumn(COL_CODIGOALMACEN);
    }// end if

    //   listado->setPaletteBackgroundColor(QColor(150,230,230));
    // Establecemos el color de fondo del extracto. El valor lo tiene la clase configuración que es global.
    m_list->setPaletteBackgroundColor("#EEFFFF");
    m_list->setReadOnly(TRUE);
    cursor2 * cur= companyact->cargacursor("SELECT * FROM facturap LEFT JOIN proveedor ON facturap.idproveedor=proveedor.idproveedor WHERE 1=1  "+generaFiltro());
    m_list->setNumRows( cur->numregistros() );
    int i=0;
    while (!cur->eof()) {
        m_list->setText(i,COL_IDFACTURAP,cur->valor("idfacturap"));
        m_list->setText(i,COL_NUMFACTURAP,cur->valor("numfacturap"));
        m_list->setText(i,COL_FFACTURAP,cur->valor("ffacturap"));
        m_list->setText(i,COL_CONTACTFACTURAP,cur->valor("contacfacturap"));
        m_list->setText(i,COL_TELFACTURAP,cur->valor("telfacturap"));
        m_list->setText(i,COL_COMENTFACTURAP,cur->valor("comentfacturap"));
        m_list->setText(i,COL_IDUSUARI,cur->valor("idusuari"));
        m_list->setText(i,COL_IDCLIENTE,cur->valor("idproveedor"));
        m_list->setText(i,COL_NOMCLIENTE,cur->valor("nomproveedor"));
        i++;
        cur->siguienteregistro();
    }// end while
    delete cur;
}// end inicializa


QString FacturasProveedorList::generaFiltro() {
    /// Tratamiento de los filtros.
    fprintf(stderr,"Tratamos el filtro \n");
    QString filtro="";
    if (m_filtro->text() != "") {
        filtro = " AND ( descfacturap LIKE '%"+m_filtro->text()+"%' ";
        filtro +=" OR nomproveedor LIKE '%"+m_filtro->text()+"%') ";
    } else {
        filtro = "";
    }// end if
    if (m_proveedor->idproveedor() != "") {
        filtro += " AND facturap.idproveedor="+m_proveedor->idproveedor();
    }// end if
    if (!m_procesada->isChecked() ) {
        filtro += " AND NOT procesadafacturap";
    }// end if
    if (m_articulo->idarticulo() != "") {
        filtro += " AND idfacturap IN (SELECT DISTINCT idfacturap FROM lfacturap WHERE idarticulo='"+m_articulo->idarticulo()+"')";
    }// end if
    filtro += " ORDER BY numfacturap";
    return (filtro);
}// end generaFiltro

void FacturasProveedorList::doubleclicked(int a, int , int , const QPoint &) {
    m_idfacturap = m_list->text(a,COL_IDFACTURAP);
    if (m_modo ==0 && m_idfacturap != "") {
        FacturaProveedorView *bud = new FacturaProveedorView(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de FacturasProveedor", "company"));
        bud->cargaFacturaProveedor(m_idfacturap);
        bud->show();
    } else {
        close();
    }// end if
}


void FacturasProveedorList::s_nuevaFacturaProveedor() {
    FacturaProveedorView *bud = new FacturaProveedorView(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de FacturasProveedor", "company"));
    bud->show();
    bud->pintaFacturaProveedor();
}// end s_nuevaFacturaProveedor

void FacturasProveedorList::s_editarFacturaProveedor() {
    m_idfacturap = m_list->text(m_list->currentRow(),COL_IDFACTURAP);
    if (m_idfacturap != "") {
        FacturaProveedorView *bud = new FacturaProveedorView(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de FacturasProveedor", "company"));
        bud->cargaFacturaProveedor(m_idfacturap);
        bud->show();
    }// end if
}


void FacturasProveedorList::s_borrarFacturaProveedor() {
    m_idfacturap = m_list->text(m_list->currentRow(),COL_IDFACTURAP);
    if (m_idfacturap != "") {
        FacturaProveedorView *bud = new FacturaProveedorView(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de FacturasProveedor", "company"));
        bud->cargaFacturaProveedor(m_idfacturap);
        bud->borraFacturaProveedor();
        delete bud;
    }// end if
    inicializa();
}
