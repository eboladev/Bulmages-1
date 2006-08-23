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


#define COL_ARCHIVO 0
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

    mui_listado->clear();
    mui_listado->setColumnCount(2);

    QStringList headerlabels;
    headerlabels << tr("Archivo") << tr("Archivo");

    mui_listado->setHorizontalHeaderLabels(headerlabels);

    mui_listado->setColumnWidth(COL_ARCHIVO, 290);
    mui_listado->hideColumn(COL_NOMBRE);

    QDir dir("/home/tborras/bulmages/trunk/bulmages/installbulmages/balances");
    dir.setFilter(QDir::Files );
    dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList();
    mui_listado->setRowCount(list.size());

    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        QTableWidgetItem *newItem1 = new QTableWidgetItem(fileInfo.filePath(), 0);
        QTableWidgetItem *newItem2 = new QTableWidgetItem(fileInfo.fileName(), 0);
        mui_listado->setItem(i, COL_ARCHIVO, newItem1);
        mui_listado->setItem(i, COL_NOMBRE, newItem2);
    } // end for
    _depura("END CAnualesView::inicializatabla", 0);
}


void CAnualesView::on_mui_listado_itemDoubleClicked(QTableWidgetItem *) {
    _depura("CAnualesView::on_listado_itemDoubleclicked", 0);
    imprimir();
    _depura("END CAnualesView::on_listado_itemDoubleclicked", 0);
}

void CAnualesView::imprimir() {
    QString idbalance = mui_listado->item(mui_listado->currentRow(),COL_ARCHIVO)->text();
    CAnualesPrintView *b = new CAnualesPrintView(m_companyact,0);
    b->setidbalance(idbalance);
    b->exec();
    delete b;
}


