//
// C++ Implementation: busquedatipo_articulo
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "busquedatipoarticulo.h"
#include "tiposarticuloview.h"
#include "company.h"

BusquedaTipoArticulo::BusquedaTipoArticulo(QWidget *parent, const char *name)
: BusquedaTipoArticuloBase(parent, name) {
    companyact=NULL;
    mdb_idtipo_articulo="";
    mdb_desctipo_articulo="";
    mdb_codtipo_articulo="";
}


BusquedaTipoArticulo::~BusquedaTipoArticulo() {}

void BusquedaTipoArticulo::setidtipo_articulo(QString val) {
    mdb_idtipo_articulo=val;
    QString SQLQuery = "SELECT * FROM tipo_articulo WHERE idtipo_articulo='"+mdb_idtipo_articulo+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
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
}// end setidtipo_articulo


void BusquedaTipoArticulo::setcodtipo_articulo(QString val) {
    mdb_codtipo_articulo=val;
    QString SQLQuery = "SELECT * FROM tipo_articulo WHERE codtipo_articulo='"+mdb_codtipo_articulo+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
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


// Bsqueda de TipoArticulos.
void BusquedaTipoArticulo::s_searchTipoArticulo() {
	tiposarticuloview *tip = new tiposarticuloview(companyact, 0,0);
	tip->setModoConsulta();
	if (tip->exec() == 1) {
		m_codtipo_articulo->setText(tip->codtipo_articulo());
		mdb_codtipo_articulo = tip->codtipo_articulo();
		m_desctipo_articulo->setText(tip->desctipo_articulo());
		mdb_desctipo_articulo = tip->desctipo_articulo();
		mdb_idtipo_articulo = tip->idtipo_articulo();		
	}// end if
	delete tip;
}// end searchClient


void BusquedaTipoArticulo::s_codtipo_articulotextChanged(const QString &val) {
    mdb_codtipo_articulo=val;
    QString SQLQuery = "SELECT * FROM tipo_articulo WHERE codtipo_articulo='"+mdb_codtipo_articulo+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
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
}// end setCifClient
