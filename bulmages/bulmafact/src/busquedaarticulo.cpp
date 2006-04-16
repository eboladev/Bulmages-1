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


// Bsqueda de Articulos.
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
}// end searchClient


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
}// end setCifClient
