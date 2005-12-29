//
// C++ Implementation: busquedafamilia
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "busquedafamilia.h"
#include "familiasview.h"
#include "company.h"

BusquedaFamilia::BusquedaFamilia(QWidget *parent, const char *name)
: BusquedaFamiliaBase(parent, name) {
    companyact=NULL;
    mdb_idfamilia="";
    mdb_nombrefamilia="";
    mdb_codigocompletofamilia="";
}


BusquedaFamilia::~BusquedaFamilia() {}

void BusquedaFamilia::setidfamilia(QString val) {
    mdb_idfamilia=val;
    QString SQLQuery = "SELECT * FROM familia WHERE idfamilia='"+mdb_idfamilia+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        mdb_codigocompletofamilia = cur->valor("codigocompletofamilia");
        mdb_nombrefamilia = cur->valor("nombrefamilia");
    } else {
        mdb_idfamilia="";
        mdb_nombrefamilia="";
        mdb_codigocompletofamilia="";
    }// end if
    delete cur;
    m_codigocompletofamilia->setText(mdb_codigocompletofamilia);
    m_nombrefamilia->setText(mdb_nombrefamilia);
}// end setidfamilia


void BusquedaFamilia::setcodigocompletofamilia(QString val) {
    mdb_codigocompletofamilia=val;
    QString SQLQuery = "SELECT * FROM familia WHERE codigocompletofamilia='"+mdb_codigocompletofamilia+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        mdb_idfamilia = cur->valor("idfamilia");
        mdb_nombrefamilia = cur->valor("nombrefamilia");
    } else {
        mdb_idfamilia="";
        mdb_nombrefamilia="";
    }// end if
    delete cur;
    m_codigocompletofamilia->setText(mdb_codigocompletofamilia);
    m_nombrefamilia->setText(mdb_nombrefamilia);
}


// Bsqueda de Familias.
void BusquedaFamilia::s_searchFamilia() {
	familiasview *fam = new familiasview(companyact, 0,0);
	fam->setModoConsulta();
	if (fam->exec() == 1) {
		m_codigocompletofamilia->setText(fam->codigoCompletoFamilia());
		mdb_codigocompletofamilia = fam->codigoCompletoFamilia();
		m_nombrefamilia->setText(fam->nombreFamilia());
		mdb_nombrefamilia = fam->nombreFamilia();
		mdb_idfamilia = fam->idFamilia();		
	}// end if
	delete fam;
    emit(valueChanged(mdb_idfamilia));	
}// end searchClient


void BusquedaFamilia::s_codigofamiliatextChanged(const QString &val) {
    mdb_codigocompletofamilia=val;
    QString SQLQuery = "SELECT * FROM familia WHERE codigocompletofamilia='"+mdb_codigocompletofamilia+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        mdb_idfamilia = cur->valor("idfamilia");
        mdb_nombrefamilia = cur->valor("nombrefamilia");
    } else {
        mdb_idfamilia="";
        mdb_nombrefamilia="";
    }// end if
    delete cur;
    m_codigocompletofamilia->setText(mdb_codigocompletofamilia);
    m_nombrefamilia->setText(mdb_nombrefamilia);
    emit(valueChanged(mdb_idfamilia));
}// end setCifClient
