//
// C++ Implementation: busquedacuenta
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "busquedacuenta.h"
#include "listcuentasview1.h"
#include "empresa.h"

BusquedaCuenta::BusquedaCuenta(QWidget *parent, const char *name)
        : BusquedaCuentaBase(parent, name) {
    empresaactual=NULL;
    conexionbase = NULL;
    mdb_idcuenta="";
    mdb_nomcuenta="";
    mdb_codigocuenta="";
}


BusquedaCuenta::~BusquedaCuenta() {}


void BusquedaCuenta::setidcuenta(QString val) {
    mdb_idcuenta=val;
    QString SQLQuery = "SELECT * FROM cuenta WHERE idcuenta='"+mdb_idcuenta+"'";
    cursor2 *cur = conexionbase->cargacursor(SQLQuery);
    if(!cur->eof()) {
        mdb_codigocuenta = cur->valor("codigo");
        mdb_nomcuenta = cur->valor("descripcion");
    } else {
        mdb_idcuenta="";
        mdb_nomcuenta="";
        mdb_codigocuenta="";
    }// end if
    delete cur;
    m_codigocuenta->setText(mdb_codigocuenta);
    m_nomcuenta->setText(mdb_nomcuenta);

}


void BusquedaCuenta::setcodigocuenta(QString val) {
    mdb_codigocuenta=val;
    QString SQLQuery = "SELECT * FROM cuenta WHERE codigo='"+mdb_codigocuenta+"'";
    cursor2 *cur = conexionbase->cargacursor(SQLQuery);
    if(!cur->eof()) {
        mdb_idcuenta = cur->valor("idcuenta");
        mdb_nomcuenta = cur->valor("descripcion");
    } else {
        mdb_idcuenta="";
        mdb_nomcuenta="";
    }// end if
    delete cur;
    m_codigocuenta->setText(mdb_codigocuenta);
    m_nomcuenta->setText(mdb_nomcuenta);
}


// Bsqueda de Cuentas.
void BusquedaCuenta::s_searchCuenta() {
    listcuentasview1 *listcuentas = new listcuentasview1(empresaactual);
    listcuentas->setModoLista();
    listcuentas->inicializa();
    listcuentas->exec();
    mdb_idcuenta = listcuentas->idcuenta();
    mdb_codigocuenta= listcuentas->codcuenta();
    mdb_nomcuenta = listcuentas->desccuenta();
    m_codigocuenta->setText(mdb_codigocuenta);
    m_nomcuenta->setText(mdb_nomcuenta);
    delete listcuentas;
}// end searchClient


void BusquedaCuenta::s_codigocuentatextChanged(const QString &val) {
    if (val=="+") {
        s_searchCuenta();
        emit(valueChanged(m_codigocuenta->text()));
    }// end if

}// end setCifClient


void BusquedaCuenta::s_lostFocus() {
    mdb_codigocuenta=m_codigocuenta->text();
    fprintf(stderr,"return_codigoinicial se inicia bien \n");
    QString cad = mdb_codigocuenta;
    if (cad != "") {
        cad = extiendecodigo(cad,numdigitos);
        cursor2 *cursorcta = conexionbase->cargacuenta(0, cad );
        int num = cursorcta->numregistros();
        if (num == 1) {
            fprintf(stderr,"Hay registros que  se muestran \n");
            mdb_codigocuenta = cursorcta->valor("codigo");
            mdb_idcuenta = cursorcta->valor("idcuenta");
            mdb_nomcuenta = cursorcta->valor("descripcion");
            m_codigocuenta->setText(mdb_codigocuenta);
            m_nomcuenta->setText(mdb_nomcuenta);
        } else {
            mdb_idcuenta = "";
            mdb_codigocuenta = "";
            mdb_nomcuenta = "";
            m_nomcuenta->setText("");
        }// end if
        delete cursorcta;
    }// end if
    emit(valueChanged(mdb_idcuenta));
}// end setCifClient

