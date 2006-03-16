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

#include "subform2bf.h"
#include "funcaux.h"
#include "articulolist.h"


SubForm2Bf::SubForm2Bf(QWidget *parent, const char *) : SubForm2(parent) {
};

void SubForm2Bf::pressedAsterisk(int row, int col) {
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
        rec->setDBvalue("idarticulo",idArticle);
        rec->setDBvalue("codigocompletoarticulo", cur->valor("codigocompletoarticulo"));
        rec->setDBvalue("nomarticulo", cur->valor("nomarticulo"));
    }
};

void SubForm2Bf::editFinished(int row, int col) {
    _depura("SubForm2Bf::editFinished",0);
    SDBRecord *rec = lineaat(row);
    SDBCampo *camp = (SDBCampo *) item(row,col);
    camp->refresh();
    if (camp->nomcampo() == "codigocompletoarticulo") {
        cursor2 *cur = companyact()->cargacursor("SELECT * FROM articulo WHERE codigocompletoarticulo='"+camp->text()+"'");
        if (!cur->eof() ) {
            rec->setDBvalue("idarticulo",cur->valor("idarticulo"));
            rec->setDBvalue("codigocompletoarticulo", cur->valor("codigocompletoarticulo"));
            rec->setDBvalue("nomarticulo", cur->valor("nomarticulo"));
        }
    }
};


void SubForm2Bf::contextMenuEvent (QContextMenuEvent *) {
    _depura("SubForm2Bf::contextMenuEvent",0);
    int a = currentRow();
    if ( a < 0)
        return;
    QMenu *popup = new QMenu(this);
    QAction *del = popup->addAction(tr("Borrar"));
    QAction *opcion = popup->exec(QCursor::pos());
    if (opcion == del)
        borrar(a);
    delete popup;
}

