//
// C++ Implementation: %{MODULE}
//
// Description:
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "familiasview.h"
#include "company.h"
#include <qlistview.h>
#include <qmap.h>

#define COL_NOMFAMILIA 0
#define COL_CODFAMILIA 1
#define COL_DESCFAMILIA 2
#define COL_IDFAMILIA  3
#define COL_CODCOMPLETOFAMILIA 4


familiasview::familiasview(company *comp, QWidget *parent, const char *name) : familiasdlg(parent, name) {
    companyact = comp;


    m_listFamilias->addColumn("NOMBRE",-1);
    m_listFamilias->addColumn("CODIGO",-1);
    m_listFamilias->addColumn("DESCRIPCION",-1);
    m_listFamilias->addColumn("IDFAMILIA",0);
    m_listFamilias->addColumn("CODIGOCOMPLETO",-1);

    pintar();
}// end familiasview


familiasview::~familiasview() {}// end ~familiasview


void familiasview::pintar() {
    QListViewItem * it;
//    QListViewItem *Lista[10000];
    QMap <int, QListViewItem*> Lista1;
    int padre;
    int idfamilia=0;
    cursor2 *cursoraux1, *cursoraux2;
    m_listFamilias->clear();
    cursoraux1 = companyact->cargacursor("SELECT * FROM familia WHERE padrefamilia ISNULL ORDER BY idfamilia");
    while (!cursoraux1->eof()) {
        padre = atoi( cursoraux1->valor("padrefamilia").ascii());
        idfamilia = atoi( cursoraux1->valor("idfamilia").ascii());
        it =new QListViewItem(m_listFamilias);
        Lista1[idfamilia]=it;
        it->setText(COL_NOMFAMILIA, cursoraux1->valor("nombrefamilia"));
        it->setText(COL_CODFAMILIA,cursoraux1->valor("codigofamilia"));
        it->setText(COL_DESCFAMILIA, cursoraux1->valor("descfamilia"));
        it->setText(COL_IDFAMILIA, cursoraux1->valor("idfamilia"));
        it->setText(COL_CODCOMPLETOFAMILIA, cursoraux1->valor("codigocompletofamilia"));
        it->setOpen(true);
        cursoraux1->siguienteregistro ();
    }// end while
    delete cursoraux1;


    cursoraux2= companyact->cargacursor("SELECT * FROM familia WHERE padrefamilia IS NOT NULL ORDER BY idfamilia");
    while (!cursoraux2->eof()) {
        padre = atoi( cursoraux1->valor("padrefamilia").ascii());
        idfamilia = atoi( cursoraux1->valor("idfamilia").ascii());
        it = new QListViewItem(Lista1[padre]);
        Lista1[idfamilia]=it;
        it->setText(COL_NOMFAMILIA, cursoraux1->valor("nombrefamilia"));
        it->setText(COL_CODFAMILIA,cursoraux1->valor("codigofamilia"));
        it->setText(COL_DESCFAMILIA, cursoraux1->valor("descfamilia"));
        it->setText(COL_IDFAMILIA, cursoraux1->valor("idfamilia"));
        it->setText(COL_CODCOMPLETOFAMILIA, cursoraux1->valor("codigocompletofamilia"));
        it->setOpen(true);
        cursoraux2->siguienteregistro();
    }// end while
    delete cursoraux2;

    s_releaseModificado();
}// end pintar


