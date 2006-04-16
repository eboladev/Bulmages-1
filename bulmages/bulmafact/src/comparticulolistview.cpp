//
// C++ Implementation: ListCompArticuloView
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
#include <QMenu>
#include <QKeyEvent>
#include <QEvent>

#include "articulolist.h"
#include "comparticulolistview.h"
#include "funcaux.h"


ListCompArticuloView::ListCompArticuloView(QWidget *parent, const char *) : SubForm2Bf(parent) {
    setDBTableName("comparticulo");
    setDBCampoId("idcomponente");
    addSHeader("codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, "codigocompletoarticulo");
    addSHeader("nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "nomarticulo");
    addSHeader("cantcomparticulo", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, "cantcomparticulo");
    addSHeader("idcomponente", DBCampo::DBint, DBCampo::DBPrimaryKey | DBCampo::DBNotNull, SHeader::DBNoView, "idcomparticulo");
    addSHeader("idarticulo", DBCampo::DBint, DBCampo::DBPrimaryKey | DBCampo::DBNotNull, SHeader::DBNoView, "idarticulo");

	setinsercion(TRUE);
}

void ListCompArticuloView::pressedAsterisk(int row, int col) {
    SDBRecord *rec = lineaat(row);
    SDBCampo *camp = (SDBCampo *) item(row,col);
    if (camp->nomcampo() != "codigocompletoarticulo")
        return;
    _depura("ListCompArticuloView::searchArticle",0);
    ArticuloList *artlist = new ArticuloList((company *)companyact(), NULL, theApp->translate("Seleccione Artic�ulo","company"),0,ArticuloList::SelectMode);
    // Esto es convertir un QWidget en un sistema modal de dialogo.
    this->setEnabled(false);
    artlist->show();
    while(!artlist->isHidden())
        theApp->processEvents();
    this->setEnabled(true);
    QString idArticle = artlist->idArticle();
    delete artlist;
    cursor2 *cur = companyact()->cargacursor("SELECT * FROM articulo WHERE idarticulo="+idArticle);
    if (!cur->eof() ) {
        rec->setDBvalue("idcomponente",idArticle);
        rec->setDBvalue("codigocompletoarticulo", cur->valor("codigocompletoarticulo"));
        rec->setDBvalue("nomarticulo", cur->valor("nomarticulo"));
    }
};

void ListCompArticuloView::editFinished(int row, int col) {
    _depura("ListCompArticuloView::editFinished",0);
    SDBRecord *rec = lineaat(row);
    SDBCampo *camp = (SDBCampo *) item(row,col);
    camp->refresh();
    if (camp->nomcampo() == "codigocompletoarticulo") {
        cursor2 *cur = companyact()->cargacursor("SELECT * FROM articulo WHERE codigocompletoarticulo='"+camp->text()+"'");
        if (!cur->eof() ) {
            rec->setDBvalue("idcomponente",cur->valor("idarticulo"));
            rec->setDBvalue("codigocompletoarticulo", cur->valor("codigocompletoarticulo"));
            rec->setDBvalue("nomarticulo", cur->valor("nomarticulo"));
        }
    }
};

