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
#include <QMessageBox>
#include <Q3PopupMenu>
#include <QFile>
#include <QTextStream>

#include "inventariosview.h"
#include "company.h"
#include "qtable1.h"
#include "funcaux.h"
#include "inventarioview.h"


#define COL_IDINVENTARIO 0
#define COL_NOMINVENTARIO 1
#define COL_FECHAINVENTARIO 2




InventariosView::InventariosView(QWidget *parent, const char *name, Qt::WFlags flag)
        : QWidget(parent, name, flag) {
    setupUi(this);
    companyact = NULL;
    meteWindow(caption(),this);
}


InventariosView::InventariosView(company *comp, QWidget *parent, const char *name)
        : QWidget(parent, name, Qt::WDestructiveClose) {
    setupUi(this);
    companyact = comp;
    mui_listado->setcompany(comp);
    mui_listado->cargar();
    meteWindow(caption(),this);
}


InventariosView::~InventariosView() {
    companyact->sacaWindow(this);
}



void InventariosView::on_mui_editar_clicked() {
    int a = mui_listado->currentRow();
    QString idinventario = mui_listado->item(a,COL_IDINVENTARIO)->text();
    if ( idinventario != "") {
        InventarioView *bud = new InventarioView(companyact,0,theApp->translate("Edicion de inventario", "company"));
        if (bud->cargar(idinventario)) return;
        companyact->m_pWorkspace->addWindow(bud);
        bud->show();
    } else {
	_depura("Debe seleccionar una linea",2);
    }// end if

}


void InventariosView::on_mui_borrar_clicked() {
    int a = mui_listado->currentRow();
    if (a >= 0) {
        QString idinventario = mui_listado->item(a,COL_IDINVENTARIO)->text();
        if (idinventario != "") {
            InventarioView *inv = new InventarioView(companyact,0,theApp->translate("Edicion de inventario", "company"));
            companyact->m_pWorkspace->addWindow(inv);
            inv->cargar(idinventario);
            /// Hacemos el borrado sin mostrar pantalla ni nada
            inv->on_mui_borrar_clicked();
	    mui_listado->cargar();
        }// end if
    }// end if
}



/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
InventariosSubForm::InventariosSubForm(QWidget *parent, const char *) : SubForm2Bf(parent) {
    setDBTableName("inventario");
    setDBCampoId("idinventario");
    addSHeader("idinventario", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoView, "idalbaran");
    addSHeader("nominventario", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "nominventario");
    addSHeader("fechainventario", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "fechainventario");
    setinsercion(FALSE);
};


