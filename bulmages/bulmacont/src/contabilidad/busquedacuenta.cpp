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
#include "../contabilidad/listcuentasview1.h"
#include "../contabilidad/empresa.h"

BusquedaCuenta::BusquedaCuenta(QWidget *parent, const char *name)
        : QWidget(parent, name) {
    _depura("BusquedaCuenta::BusquedaCuenta", 0);
    setupUi(this);
    empresaactual=NULL;
    conexionbase = NULL;
    mdb_idcuenta="";
    mdb_nomcuenta="";
    mdb_codigocuenta="";
    mdb_tipocuenta="";
    _depura("END BusquedaCuenta::BusquedaCuenta", 0);
}


BusquedaCuenta::~BusquedaCuenta() {
    _depura("BusquedaCuenta::~BusquedaCuenta", 0);

}


void BusquedaCuenta::setempresa(empresa *comp)  {
    _depura("BusquedaCuenta::setempresa", 0);
    empresaactual = comp;
    conexionbase = empresaactual->bdempresa();
    numdigitos = empresaactual->numdigitosempresa();
    _depura("END BusquedaCuenta::setempresa", 0);

}


void BusquedaCuenta::setidcuenta(QString val) {
    _depura("BusquedaCuenta::setidcuenta", 0);
    mdb_idcuenta=val;
    QString SQLQuery = "SELECT * FROM cuenta WHERE idcuenta='"+mdb_idcuenta+"'";
    cursor2 *cur = conexionbase->cargacursor(SQLQuery);
    if(!cur->eof()) {
        mdb_codigocuenta = cur->valor("codigo");
        mdb_nomcuenta = cur->valor("descripcion");
	mdb_tipocuenta = cur->valor("tipocuenta");
    } else {
        mdb_idcuenta="";
        mdb_nomcuenta="";
        mdb_codigocuenta="";
	mdb_tipocuenta="";
    }// end if
    delete cur;
    mui_codigocuenta->setText(mdb_codigocuenta);
    mui_nomcuenta->setText(mdb_nomcuenta);
    _depura("END BusquedaCuenta::setidcuenta", 0);

}


void BusquedaCuenta::setcodigocuenta(QString val) {
    _depura("BusquedaCuenta::setcodigocuenta", 0);
    mdb_codigocuenta=val;
    QString SQLQuery = "SELECT * FROM cuenta WHERE codigo='"+mdb_codigocuenta+"'";
    cursor2 *cur = conexionbase->cargacursor(SQLQuery);
    if(!cur->eof()) {
        mdb_idcuenta = cur->valor("idcuenta");
        mdb_nomcuenta = cur->valor("descripcion");
	mdb_tipocuenta = cur->valor("tipocuenta");
    } else {
        mdb_idcuenta="";
        mdb_nomcuenta="";
	mdb_tipocuenta="";
    }// end if
    delete cur;
    mui_codigocuenta->setText(mdb_codigocuenta);
    mui_nomcuenta->setText(mdb_nomcuenta);
    _depura("END BusquedaCuenta::setcodigocuenta", 0);
}


// Bsqueda de Cuentas.
void BusquedaCuenta::s_searchCuenta() {
    _depura("BusquedaCuenta::s_searchCuenta", 0);
	/// Generamos un dialogo
	QDialog *diag = new QDialog(0);
	diag->setModal(true);

	/// Creamos una instancia del selector de cuentas
	listcuentasview1 *listcuentas = new listcuentasview1(empresaactual, diag, tr("Seleccione cuenta", "company"),0, listcuentasview1::SelectMode);

	/// Hacemos la conexion del cerrar de las cuentas con el cerrar dialogo
	connect(listcuentas, SIGNAL(selected(QString)), diag, SLOT(accept()));
	connect(listcuentas, SIGNAL(destroyed(QObject)), diag, SLOT(accept()));
        listcuentas->inicializa();
	
	/// Sacamos el dialogo
	diag->exec();
	if (listcuentas->codcuenta() != "") {
		mdb_idcuenta = listcuentas->idcuenta();
		mdb_codigocuenta= listcuentas->codcuenta();
		mdb_nomcuenta = listcuentas->desccuenta();
		mui_codigocuenta->setText(mdb_codigocuenta);
		mui_nomcuenta->setText(mdb_nomcuenta);
	} // end if
	delete diag;
    _depura("END BusquedaCuenta::s_searchCuenta", 0);
}


void BusquedaCuenta::s_codigocuentatextChanged(const QString &val) {
    _depura("BusquedaCuenta::s_codigocuentatextChanged", 0, val);
    if (val=="+") {
        s_searchCuenta();
        emit(valueChanged(mui_codigocuenta->text()));
    }// end if
    _depura("END BusquedaCuenta::s_codigocuentatextChanged", 0);
}


void BusquedaCuenta::s_lostFocus() {
    _depura("BusquedaCuenta::s_lostFocus", 0);
    mdb_codigocuenta=mui_codigocuenta->text();
    QString cad = mdb_codigocuenta;
    if (cad != "") {
        cad = extiendecodigo(cad,numdigitos);
        cursor2 *cursorcta = conexionbase->cargacuenta(0, cad );
        int num = cursorcta->numregistros();
        if (num == 1) {
            mdb_codigocuenta = cursorcta->valor("codigo");
            mdb_idcuenta = cursorcta->valor("idcuenta");
            mdb_nomcuenta = cursorcta->valor("descripcion");
	    mdb_tipocuenta = cursorcta->valor("tipocuenta");
            mui_codigocuenta->setText(mdb_codigocuenta);
            mui_nomcuenta->setText(mdb_nomcuenta);
        } else {
            mdb_idcuenta = "";
            mdb_codigocuenta = "";
            mdb_nomcuenta = "";
	    mdb_tipocuenta = "";
            mui_nomcuenta->setText("");
        }// end if
        delete cursorcta;
    }// end if
    emit(valueChanged(mdb_idcuenta));
    _depura("END BusquedaCuenta::s_lostFocus", 0);

}

