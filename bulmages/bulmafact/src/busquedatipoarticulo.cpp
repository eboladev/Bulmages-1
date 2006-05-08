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

#include "busquedatipoarticulo.h"
#include "tiposarticuloview.h"
#include "company.h"


BusquedaTipoArticulo::BusquedaTipoArticulo(QWidget *parent, const char *name)
: QWidget(parent, name) {
    setupUi(this);
    m_companyact=NULL;
    mdb_idtipo_articulo="";
    mdb_desctipo_articulo="";
    mdb_codtipo_articulo="";
}


BusquedaTipoArticulo::~BusquedaTipoArticulo() {}


void BusquedaTipoArticulo::setidtipo_articulo(QString val) {
    mdb_idtipo_articulo=val;
    QString SQLQuery = "SELECT * FROM tipo_articulo WHERE idtipo_articulo='"+mdb_idtipo_articulo+"'";
    cursor2 *cur = m_companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        mdb_codtipo_articulo = cur->valor("codtipo_articulo");
        mdb_desctipo_articulo = cur->valor("desctipo_articulo");
    } else {
        mdb_idtipo_articulo="";
        mdb_desctipo_articulo="";
        mdb_codtipo_articulo="";
    }// end if
    delete cur;
    m_codtipo_articulo->setText(mdb_codtipo_articulo);
    m_desctipo_articulo->setText(mdb_desctipo_articulo);
}


void BusquedaTipoArticulo::setcodtipo_articulo(QString val) {
    mdb_codtipo_articulo=val;
    QString SQLQuery = "SELECT * FROM tipo_articulo WHERE codtipo_articulo='"+mdb_codtipo_articulo+"'";
    cursor2 *cur = m_companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        mdb_idtipo_articulo = cur->valor("idtipo_articulo");
        mdb_desctipo_articulo = cur->valor("desctipo_articulo");
    } else {
        mdb_idtipo_articulo="";
        mdb_desctipo_articulo="";
    }// end if
    delete cur;
    m_codtipo_articulo->setText(mdb_codtipo_articulo);
    m_desctipo_articulo->setText(mdb_desctipo_articulo);
}


// Busqueda de TipoArticulos.
void BusquedaTipoArticulo::on_mui_buscar_clicked() {
	TipoArticuloList *tip = m_companyact->newTipoArticuloList();
	tip->setModoConsulta();
	if (tip->exec() == 1) {
		m_codtipo_articulo->setText(tip->codtipo_articulo());
		mdb_codtipo_articulo = tip->codtipo_articulo();
		m_desctipo_articulo->setText(tip->desctipo_articulo());
		mdb_desctipo_articulo = tip->desctipo_articulo();
		mdb_idtipo_articulo = tip->idtipo_articulo();		
	}// end if
	delete tip;
}


void BusquedaTipoArticulo::on_m_codtipo_articulo_textChanged(const QString &val) {
    mdb_codtipo_articulo=val;
    QString SQLQuery = "SELECT * FROM tipo_articulo WHERE codtipo_articulo='"+mdb_codtipo_articulo+"'";
    cursor2 *cur = m_companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        mdb_idtipo_articulo = cur->valor("idtipo_articulo");
        mdb_desctipo_articulo = cur->valor("desctipo_articulo");
    } else {
        mdb_idtipo_articulo="";
        mdb_desctipo_articulo="";
    }// end if
    delete cur;
    m_codtipo_articulo->setText(mdb_codtipo_articulo);
    m_desctipo_articulo->setText(mdb_desctipo_articulo);
    emit(valueChanged(mdb_idtipo_articulo));
}

