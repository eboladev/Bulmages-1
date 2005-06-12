//
// C++ Implementation: busquedacliente
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "busquedaarticulo.h"
#include "articleslist.h"
#include "company.h"

BusquedaArticulo::BusquedaArticulo(QWidget *parent, const char *name)
: BusquedaArticuloBase(parent, name) {
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


// Bsqueda de Clientes.
void BusquedaArticulo::s_searchArticulo() {
    fprintf(stderr,"Busqueda de un client\n");
    articleslist *articulos = new articleslist(companyact, NULL, tr("Seleccione articulo","company"));
    // , WType_Dialog| WShowModal
    articulos->modoseleccion();
    // Esto es convertir un QWidget en un sistema modal de dialogo.
    this->setEnabled(false);
    articulos->show();
    while(!articulos->isHidden())
        theApp->processEvents();
    this->setEnabled(true);
    if (articulos->codigocompletoarticulo() !="" && articulos->codigocompletoarticulo() !=NULL) {
        m_codigocompletoarticulo->setText(articulos->codigocompletoarticulo());
        mdb_codigocompletoarticulo = articulos->codigocompletoarticulo();
        m_nomarticulo->setText(articulos->nomarticulo());
        mdb_nomarticulo = articulos->nomarticulo();
        mdb_idarticulo = articulos->idarticulo();
    }// end if
    delete articulos;
//    emit(valueChanged(mdb_idarticulo));
}// end searchClient


void BusquedaArticulo::s_codigocompletoarticulotextChanged(const QString &val) {
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
}// end setCifClient
