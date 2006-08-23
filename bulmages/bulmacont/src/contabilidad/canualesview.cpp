/***************************************************************************
                          CAnualesView.cpp  -  description
                             -------------------
    begin                : s� oct 18 2003
    copyright            : (C) 2003 by Tomeu Borr� Riera
    email                : tborras@conetxia.com
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "canualesview.h"
#include "compbalanceview.h"
#include "canualesprintview.h"
#include <q3table.h>
#include <q3filedialog.h>
#include <qmessagebox.h>

#include "importbalance.h"
#include "empresa.h"


#define COL_CODIGO 0
#define COL_NOMBRE 1

CAnualesView::CAnualesView(empresa *emp, QWidget *parent, const char *name ) : QWidget(parent,name, Qt::WDestructiveClose) {
    _depura("CAnualesView::CAnualesView", 0);
    setupUi(this);
    m_companyact = emp;
    m_modo = 0;
    inicializatabla();
    m_companyact->meteWindow(caption(), this);
    _depura("END CAnualesView::CAnualesView", 0);
}


CAnualesView::~CAnualesView() {
    _depura("CAnualesView::~CAnualesView\n", 0);
    m_companyact->sacaWindow(this);
    _depura("END CAnualesView::~CAnualesView\n", 0);
}


void CAnualesView::inicializatabla()  {
    _depura("CAnualesView::inicializatabla", 0);
    listado->setNumRows(0);
    listado->setNumCols(2);
    listado->horizontalHeader()->setLabel( COL_CODIGO, tr( "CODIGO" ) );
    listado->horizontalHeader()->setLabel( COL_NOMBRE, tr( "Nombre Balance" ) );
    listado->hideColumn(2);
    listado->hideColumn(0);
    listado->setColumnWidth(1,400);
    QDir dir("/home/tborras/bulmages/trunk/bulmages/installbulmages/balances");
    dir.setFilter(QDir::Files );
    dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList();
    listado->setNumRows(list.size());

    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        listado->setText(i,COL_CODIGO, fileInfo.filePath());
        listado->setText(i,COL_NOMBRE, fileInfo.fileName());
    } // end for
    listado->setReadOnly(TRUE);
    _depura("END CAnualesView::inicializatabla", 0);
}


void CAnualesView::dbtabla(int , int , int ,const QPoint &) {
    imprimir();
}

void CAnualesView::imprimir() {
    QString idbalance = listado->text(listado->currentRow(),COL_CODIGO);
    CAnualesPrintView *b = new CAnualesPrintView(m_companyact,0);
    b->setidbalance(idbalance);
    b->exec();
    delete b;
}


