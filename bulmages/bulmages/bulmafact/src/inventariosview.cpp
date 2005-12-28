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
#include "inventariosview.h"
#include "company.h"
#include "qtable1.h"
#include <qmessagebox.h>
#include <q3popupmenu.h>
#include <qfile.h>
//Added by qt3to4:
#include <QTextStream>
#include "funcaux.h"
#include "inventarioview.h"


#define COL_IDINVENTARIO 0
#define COL_NOMINVENTARIO 1
#define COL_FECHAINVENTARIO 2




InventariosView::InventariosView(QWidget *parent, const char *name, Qt::WFlags flag)
        : InventariosBase(parent, name, flag) {
    companyact = NULL;
    meteWindow(caption(),this);
}// end providerslist


InventariosView::InventariosView(company *comp, QWidget *parent, const char *name)
        : InventariosBase(parent, name, Qt::WDestructiveClose) {
    companyact = comp;
    inicializa();
    meteWindow(caption(),this);
}


InventariosView::~InventariosView() {
    companyact->sacaWindow(this);
}


void InventariosView::inicializa() {
    _depura("InventariosView::inicializa()",1);
    m_list->setNumRows( 0 );
    m_list->setNumCols( 0 );
    m_list->setSelectionMode( Q3Table::SingleRow );
    m_list->setSorting( TRUE );
    m_list->setColumnMovingEnabled( TRUE );
    m_list->setNumCols(3);
    m_list->horizontalHeader()->setLabel( COL_IDINVENTARIO, tr( "COL_IDINVENTARIO" ) );
    m_list->horizontalHeader()->setLabel( COL_NOMINVENTARIO, tr( "COL_NOMINVENTARIO" ) );
    m_list->horizontalHeader()->setLabel( COL_FECHAINVENTARIO, tr( "COL_FECHAINVENTARIO" ) );
    m_list->setColumnWidth(COL_FECHAINVENTARIO,75);
    // Establecemos el color de fondo del extracto. El valor lo tiene la clase configuracion que es global.
    m_list->setPaletteBackgroundColor(confpr->valor(CONF_BG_LISTFACTURASCLIENTE));
    m_list->setReadOnly(TRUE);
    cursor2 * cur= companyact->cargacursor("SELECT * FROM inventario");
    m_list->setNumRows( cur->numregistros() );
    int i=0;
    while (!cur->eof()) {
        m_list->setText(i,COL_IDINVENTARIO,cur->valor("idinventario"));
        m_list->setText(i,COL_NOMINVENTARIO,cur->valor("nominventario"));
        m_list->setText(i,COL_FECHAINVENTARIO,cur->valor("fechainventario"));
        i++;
        cur->siguienteregistro();
    }// end while
    delete cur;
    _depura("END InventariosView::inicializa()",1);
}// end inicializa




void InventariosView::doubleclicked(int a, int , int , const QPoint &) {
    QString idinventario = m_list->text(a,COL_IDINVENTARIO);
    if ( idinventario != "") {
        InventarioView *bud = new InventarioView(companyact,0,theApp->translate("Edicion Inventario", "company"));
        companyact->m_pWorkspace->addWindow(bud);
        bud->cargaInventario(idinventario);
        bud->show();
    } else {
        close();
    }// end if
}


void InventariosView::s_edit() {
    int a = m_list->currentRow();
    if (a >= 0) {
        QString idinventario = m_list->text(a,COL_IDINVENTARIO);
        if (idinventario != "") {
            InventarioView *inv = new InventarioView(companyact,0,theApp->translate("Edicion Inventario", "company"));
            companyact->m_pWorkspace->addWindow(inv);
            inv->cargaInventario(idinventario);
            inv->show();
        }// end if
    }// end if
}// end s_edit


void InventariosView::s_delete() {
    int a = m_list->currentRow();
    if (a >= 0) {
        QString idinventario = m_list->text(a,COL_IDINVENTARIO);
        if (idinventario != "") {
            InventarioView *inv = new InventarioView(companyact,0,theApp->translate("Edicion Inventario", "company"));
            companyact->m_pWorkspace->addWindow(inv);
            inv->cargaInventario(idinventario);
            /// Hacemos el borrado sin mostrar pantalla ni nada
            inv->s_delete();
            inicializa();
        }// end if
    }// end if
}// end s_edit
