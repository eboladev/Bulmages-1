//
// C++ Implementation: linpresupuesto
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "linpresupuesto.h"

    linpresupuesto::linpresupuesto(company *comp) {
        companyact = comp;
	vacialinpresupuesto();
    }
    
    linpresupuesto::linpresupuesto(company *comp, QString idlinpresupuesto) {
        companyact = comp;
    	QString SQLQuery = "SELECT * FROM lpresupuesto, articulo WHERE lpresupuesto.idarticulo=articulo.idarticulo AND idlpresupuesto="+idlinpresupuesto;
	cursor2 *cur = companyact->cargacursor(SQLQuery);
	if (!cur->eof()) {
		mdb_idlpresupuesto = cur->valor("idlpresupuesto");
		mdb_desclpresupuesto = cur->valor("desclpresupuesto");
		mdb_cantlpresupuesto = cur->valor("cantlpresupuesto");
		mdb_pvplpresupuesto = cur->valor("pvplpresupuesto");
		mdb_descuentolpresupuesto = cur->valor("descuentolpresupuesto");
		mdb_idpresupuesto = cur->valor("idpresupuesto");
		mdb_idarticulo = cur->valor("idarticulo");
		mdb_codigocompletoarticulo = cur->valor("codigocompletoarticulo");
		mdb_nomarticulo = cur->valor("nomarticulo");
	} else {
		vacialinpresupuesto();
	}// end if
    
    }// end linpresupuesto
    
    linpresupuesto::linpresupuesto(company *comp, QString a, QString b, QString c, QString d, QString e, QString f, QString g, QString h, QString i) {
        companyact = comp;
        mdb_idlpresupuesto = a;
        mdb_desclpresupuesto = b;
        mdb_cantlpresupuesto = c;
        mdb_pvplpresupuesto =d;
        mdb_descuentolpresupuesto = e;
        mdb_idpresupuesto = f;
        mdb_idarticulo = g;
	mdb_codigocompletoarticulo = h;
	mdb_nomarticulo = i;
    }// end linpresupuesto
    
    
    linpresupuesto::~linpresupuesto() {}
    

void linpresupuesto::vacialinpresupuesto() {
        mdb_idlpresupuesto = "";
        mdb_desclpresupuesto = "";
        mdb_cantlpresupuesto = "";
        mdb_pvplpresupuesto ="";
        mdb_descuentolpresupuesto = "";
        mdb_idpresupuesto = "";
        mdb_idarticulo = "";
	mdb_codigocompletoarticulo = "";
	mdb_nomarticulo = "";
}
