/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#include "busquedaarticulo.h"
#include "articulolist.h"
#include "company.h"
#include "funcaux.h"


BusquedaArticulo::BusquedaArticulo(QWidget *parent, const char *name)
: QWidget(parent, name) {
	setupUi(this);
    companyact=NULL;
    mdb_idarticulo="";
    mdb_nomarticulo="";
    mdb_codigocompletoarticulo="";
}


BusquedaArticulo::~BusquedaArticulo() {}


void BusquedaArticulo::setidarticulo(QString val) {
    mdb_idarticulo=val;
    QString SQLQuery = "SELECT * FROM articulo WHERE idarticulo='"+mdb_idarticulo+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        mdb_codigocompletoarticulo = cur->valor("codigocompletoarticulo");
        mdb_nomarticulo = cur->valor("nomarticulo");
    } else {
        mdb_idarticulo="";
        mdb_nomarticulo="";
        mdb_codigocompletoarticulo="";
    }// end if
    delete cur;
    m_codigocompletoarticulo->setText(mdb_codigocompletoarticulo);
    m_nomarticulo->setText(mdb_nomarticulo);

}


void BusquedaArticulo::setcodigocompletoarticulo(QString val) {
    mdb_codigocompletoarticulo=val;
    QString SQLQuery = "SELECT * FROM articulo WHERE codigocompletoarticulo='"+mdb_codigocompletoarticulo+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        mdb_idarticulo = cur->valor("idarticulo");
        mdb_nomarticulo = cur->valor("nomarticulo");
    } else {
        mdb_idarticulo="";
        mdb_nomarticulo="";
    }// end if
    delete cur;
    m_codigocompletoarticulo->setText(mdb_codigocompletoarticulo);
    m_nomarticulo->setText(mdb_nomarticulo);
}


// Busqueda de articulos.
void BusquedaArticulo::on_mui_buscar_clicked() {
    _depura("BusquedaArticulo::on_mui_buscar_clicked",0);
    QDialog *diag=new QDialog(0);
    diag->setModal(true);
    ArticuloList *articulos = new ArticuloList(companyact, diag, tr("Seleccione articulo","company"),0,ArticuloList::SelectMode);
    connect(articulos, SIGNAL(selected(QString)), diag, SLOT(accept()));
    diag->exec();
    if (articulos->codigocompletoarticulo() !="") {
        m_codigocompletoarticulo->setText(articulos->codigocompletoarticulo());
        mdb_codigocompletoarticulo = articulos->codigocompletoarticulo();
        m_nomarticulo->setText(articulos->nomarticulo());
        mdb_nomarticulo = articulos->nomarticulo();
        mdb_idarticulo = articulos->idarticulo();
    }// end if
    delete diag;
}


void BusquedaArticulo::on_m_codigocompletoarticulo_textChanged(const QString &val) {
    mdb_codigocompletoarticulo=val;
    QString SQLQuery = "SELECT * FROM articulo WHERE codigocompletoarticulo='"+mdb_codigocompletoarticulo+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        mdb_idarticulo = cur->valor("idarticulo");
        mdb_nomarticulo = cur->valor("nomarticulo");
    } else {
        mdb_idarticulo="";
        mdb_nomarticulo="";
    }// end if
    delete cur;
    m_codigocompletoarticulo->setText(mdb_codigocompletoarticulo);
    m_nomarticulo->setText(mdb_nomarticulo);
    emit(valueChanged(mdb_idarticulo));
}

