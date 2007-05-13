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

#include "articulolist.h"
#include "comparticulolistview.h"
#include "funcaux.h"


/** Prepara el subformulario para trabajar con la tabla comparticulo de la base de datos.
*/
ListCompArticuloView::ListCompArticuloView(QWidget *parent, const char *)
        : SubForm2Bf(parent) {
    _depura("ListCompArticuloView::ListCompArticuloView", 0);
    setDBTableName("comparticulo");
    setDBCampoId("idcomponente");
    addSHeader("codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, tr("Codigo completo del articulo"));
    addSHeader("nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre del articulo"));
    addSHeader("cantcomparticulo", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("Cantidad de componente de articulo"));
    addSHeader("idcomponente", DBCampo::DBint, DBCampo::DBPrimaryKey | DBCampo::DBNotNull, SHeader::DBNoView, tr("ID componente"));
    addSHeader("idarticulo", DBCampo::DBint, DBCampo::DBPrimaryKey | DBCampo::DBNotNull, SHeader::DBNoView, tr("ID articulo"));
    setinsercion(TRUE);
    _depura("END ListCompArticuloView::ListCompArticuloView", 0);
}

/** SLOT que responde a la pulsacion de Ctrl + * en el subformulario.
    Dependiendo de la columna que este seleccionada saca el buscador de articulos.
*/
void ListCompArticuloView::pressedAsterisk(int row, int col) {
    _depura("ListCompArticuloView::pressedAsterisk", 0);
    SDBRecord *rec = lineaat(row);
    SDBCampo *camp = (SDBCampo *) item(row,col);
    if (camp->nomcampo() != "codigocompletoarticulo")
        return;
    _depura("ListCompArticuloView::searchArticle", 0);
    ArticuloList *artlist = new ArticuloList((company *)empresaBase(), NULL, 0, ArticuloList::SelectMode);
    /// Esto es convertir un QWidget en un sistema modal de dialogo.
    this->setEnabled(false);
    artlist->show();
    while(!artlist->isHidden())
        theApp->processEvents();
    this->setEnabled(true);
    QString idArticle = artlist->idarticulo();
    delete artlist;
    cursor2 *cur = empresaBase()->cargacursor("SELECT * FROM articulo WHERE idarticulo=" + idArticle);
    if (!cur->eof() ) {
        rec->setDBvalue("idcomponente", idArticle);
        rec->setDBvalue("codigocompletoarticulo", cur->valor("codigocompletoarticulo"));
        rec->setDBvalue("nomarticulo", cur->valor("nomarticulo"));
    } // end if
    _depura("END ListCompArticuloView::pressedAsterisk", 0);
}


/** SLOT que responde a la finalizacion de edicion de una celda en el subformulario.
    Comprueba que sea la celda de codigo de articulo y si es asi pone el resto de componentes.
*/
void ListCompArticuloView::editFinished(int row, int col) {
    _depura("ListCompArticuloView::editFinished", 0);
    SDBRecord *rec = lineaat(row);
    SDBCampo *camp = (SDBCampo *) item(row, col);
    camp->refresh();
    if (camp->nomcampo() == "codigocompletoarticulo") {
        cursor2 *cur = empresaBase()->cargacursor("SELECT * FROM articulo WHERE codigocompletoarticulo='" + camp->text() + "'");
        if (!cur->eof() ) {
            rec->setDBvalue("idcomponente", cur->valor("idarticulo"));
            rec->setDBvalue("codigocompletoarticulo", cur->valor("codigocompletoarticulo"));
            rec->setDBvalue("nomarticulo", cur->valor("nomarticulo"));
        } // end if
    } // end if
    _depura("END ListCompArticuloView::editFinished", 0);
}

/** Destructor de la clase
**/
ListCompArticuloView::~ListCompArticuloView() {
	_depura("ListCompArticuloView::~ListCompArticuloView", 0);
	_depura("END ListCompArticuloView::~ListCompArticuloView", 0);
}

/** Realiza la carga del listado creando el query necesario para dicha carga
**/
void ListCompArticuloView::cargar(QString idarticulo) {
        _depura("ListCompActiculo::cargar", 0);
        mdb_idarticulo = idarticulo;
        cursor2 * cur= empresaBase()->cargacursor("SELECT * FROM comparticulo, articulo WHERE comparticulo.idarticulo=" + mdb_idarticulo + " AND articulo.idarticulo = comparticulo.idcomponente");
        SubForm3::cargar(cur);
        delete cur;
        _depura("END ListCompActiculo::cargar", 0);
}


