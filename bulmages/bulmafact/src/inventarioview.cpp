//
// C++ Implementation: inventarioview.cpp
// Description:
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
// Copyright: See COPYING file that comes with this distribution
//
//
#include "inventarioview.h"
#include "company.h"
#include "listcontrolstockview.h"
#include "inventario.h"
#include "funcaux.h"

#include <QMessageBox>
#include <Q3Table>
#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <Q3PopupMenu>
#include <QToolButton>
#include <QLayout>
#include <fstream>
using namespace std;

InventarioView::InventarioView(company *comp, QWidget *parent, const char *name)
: InventarioBase(parent, name, Qt::WDestructiveClose) , Inventario (comp) {
    /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
    _depura("inicializamos el subformulario",0);
    subform2->setcompany(comp);
    setListControlStock(subform2);
    comp->meteWindow(caption(),this);
    _depura("Fin de la inicializacion de Inventario\n");
}


InventarioView::~InventarioView() {
    companyact->sacaWindow(this);
}


/**
  * Esta función se ejecuta cuando se ha pulsado sobre el botón de borrar *
  */
void InventarioView::s_delete() {
    if (DBvalue("idinventario") != "") {
        if ( QMessageBox::question(this,tr("Borrar Inventario"),tr("Esta a punto de borrar un Inventario, Desea continuar?."),tr("Si"),tr("No"),0,1,0) == 0) {
            borraInventario();
        }// end if
    }// end if
}// end boton_borrar
