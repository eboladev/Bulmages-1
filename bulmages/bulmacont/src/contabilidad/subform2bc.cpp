/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include <QMessageBox>
#include <QMenu>
#include <QKeyEvent>
#include <QEvent>

#include "subform3.h"
#include "subform2bc.h"
#include "funcaux.h"


SubForm2Bc::SubForm2Bc(QWidget *parent) : SubForm3(parent) {
    setDelete(TRUE);
};




void SubForm2Bc::pressedAsterisk(int row, int col) {
    SDBRecord *rec = lineaat(row);
    SDBCampo *camp = (SDBCampo *) item(row,col);


    if (camp->nomcampo() != "codigocompletoarticulo")
        return;
}



void SubForm2Bc::pressedSlash(int row, int col) {
    _depura("SubForm2Bc::pressedSlash",2);
    SDBRecord *rec = lineaat(row);
    SDBCampo *camp = (SDBCampo *) item(row,col);
    QString text = editaTexto(camp->text());
    camp->set
    (text);
}

void SubForm2Bc::editFinished(int row, int col) {
    _depura("SubForm2Bc::editFinished",0);
    SDBRecord *rec = lineaat(row);
    SDBCampo *camp = (SDBCampo *) item(row,col);
    camp->refresh();

}

void SubForm2Bc::contextMenuEvent (QContextMenuEvent *) {
    _depura("SubForm2Bc::contextMenuEvent",0);
    QAction *del= NULL;
    int row = currentRow();
    if ( row < 0)
        return;

    int col = currentColumn();
    if ( row < 0)
        return;

    QMenu *popup = new QMenu(this);
    if(m_delete)
        del = popup->addAction(tr("Borrar registro"));
    popup->addSeparator();
    QAction *ajustc = popup->addAction(tr("Ajustar columa"));
    QAction *ajustac = popup->addAction(tr("Ajustar altura"));

    QAction *ajust = popup->addAction(tr("Ajustar columnas"));
    QAction *ajusta = popup->addAction(tr("Ajustar alturas"));

	popup->addSeparator();
    QAction *verconfig = popup->addAction(tr("Ver configurador de subformulario"));

    QAction *opcion = popup->exec(QCursor::pos());
    if (opcion == del)
        borrar(row);
    if (opcion == ajust)
        resizeColumnsToContents();
    if (opcion == ajusta)
        resizeRowsToContents();
    if (opcion == ajustc)
        resizeColumnToContents(col);
    if (opcion == ajustac)
        resizeRowToContents(row);


   if(opcion == verconfig)
	showConfig();

    delete popup;
}

