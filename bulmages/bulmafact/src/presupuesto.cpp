//
// C++ Implementation: presupuesto
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "presupuesto.h"
#include "company.h"


presupuesto::presupuesto(company *comp) {
    companyact=comp;
    m_idpresupuesto = "0";
    m_idclient = "";
    mdb_idcliente= "";
    mdb_idalmacen= "";
    mdb_numpresupuesto= "";
    mdb_fpresupuesto= "";
    mdb_vencpresupuesto= "";
    mdb_contactpresupuesto= "";
    mdb_telpresupuesto= "";
    mdb_comentpresupuesto= "";
    mdb_nomcliente= "";
    mdb_cifcliente= "";
    mdb_codigoalmacen= "";
    mdb_nomalmacen= "";
}

presupuesto::~presupuesto() {}

void presupuesto::pintaPresupuesto() {
	fprintf(stderr,"INICIO de pintaPresupuesto\n");
    pintaIdClient(mdb_idcliente);
    pintaIdAlmacen(mdb_idalmacen);
    pintaNumPresupuesto(mdb_numpresupuesto);
    pintaFPresupuesto(mdb_fpresupuesto);
    pintaVencPresupuesto(mdb_vencpresupuesto);
    pintaContractPresupuesto(mdb_contactpresupuesto);
    pintaTelPresupuesto(mdb_telpresupuesto);
    pintaComentPresupuesto(mdb_comentpresupuesto);
    pintaNomClient(mdb_nomcliente);
    pintaCifClient(mdb_cifcliente);
    pintaCodigoAlmacen(mdb_codigoalmacen);
    pintaNomAlmacen(mdb_nomalmacen);
    // Pinta el subformulario de detalle del presupuesto.
    listalineas->pintalistlinpresupuesto();
    fprintf(stderr,"FIN de pintaPresupuesto\n");
}// end pintaPresupuesto


// Esta función carga un presupuesto.
void presupuesto::chargeBudget(QString idbudget) {
    m_idpresupuesto = idbudget;
    mdb_idpresupuesto = idbudget;
    inicialize();
    QString query = "SELECT * FROM presupuesto LEFT JOIN cliente ON cliente.idcliente = presupuesto.idcliente LEFT JOIN almacen ON  presupuesto.idalmacen = almacen.idalmacen WHERE idpresupuesto="+idbudget;
    cursor2 * cur= companyact->cargacursor(query);
    if (!cur->eof()) {
        mdb_idcliente= cur->valor("idcliente");
        mdb_idalmacen= cur->valor("idalmacen");
        mdb_numpresupuesto= cur->valor("numpresupuesto");
        mdb_fpresupuesto= cur->valor("fpresupuesto");
        mdb_vencpresupuesto= cur->valor("vencpresupuesto");
        mdb_contactpresupuesto= cur->valor("contactpresupuesto");
        mdb_telpresupuesto= cur->valor("telpresupuesto");
        mdb_comentpresupuesto= cur->valor("comentpresupuesto");
        mdb_nomcliente= cur->valor("nomcliente");
        mdb_cifcliente= cur->valor("cifcliente");
        mdb_codigoalmacen= cur->valor("codigoalmacen");
        mdb_nomalmacen= cur->valor("nomalmacen");
	mdb_idusuari = cur->valor("idusuari");
        chargeBudgetDiscounts(idbudget);
        calculateImports();
    }// end if
    delete cur;

    m_initialValues = calculateValues();
    listalineas->chargeBudgetLines(idbudget);
    pintaPresupuesto();
}// end chargeBudget

void presupuesto::guardapresupuesto() {
	if (mdb_idpresupuesto == "") {
		/// Se trata de una inserción
		QString SQLQuery = "INSERT INTO presupuesto (numpresupuesto, fpresupuesto, contactpresupuesto, telpresupuesto, vencpresupuesto, comentpresupuesto, idusuari, idcliente, idalmacen) VALUES ("+mdb_numpresupuesto+","+mdb_fpresupuesto+","+mdb_contactpresupuesto+","+mdb_telpresupuesto+","+mdb_vencpresupuesto+","+mdb_comentpresupuesto+","+mdb_idusuari+","+mdb_idcliente+","+mdb_idalmacen+")";
		companyact->begin();
		companyact->ejecuta(SQLQuery);
		cursor2 *cur = companyact->cargacursor("SELECT MAX(idpresupuesto) AS m FROM presupuesto");
		if (!cur->eof())
			mdb_idpresupuesto = cur->valor("idpresupuesto");
		delete cur;
		companyact->commit();
	} else {
		/// Se trata de una modificación
		QString SQLQuery = "UPDATE presupuesto SET ";
		SQLQuery += " numpresupuesto="+mdb_numpresupuesto;
		SQLQuery += " ,fpresupuesto='"+mdb_fpresupuesto+"'";
		SQLQuery += " ,contactpresupuesto='"+mdb_contactpresupuesto+"'";
		SQLQuery += " ,telpresupuesto='"+mdb_telpresupuesto+"'";
		SQLQuery += " ,vencpresupuesto='"+mdb_vencpresupuesto+"'";
		SQLQuery += " ,comentpresupuesto='"+mdb_comentpresupuesto+"'";
		SQLQuery += " ,idusuari="+mdb_idusuari;
		SQLQuery += " ,idcliente="+mdb_idcliente;
		SQLQuery += " ,idalmacen="+mdb_idalmacen;
		SQLQuery += " WHERE idpresupuesto="+mdb_idpresupuesto;
		companyact->begin();
		companyact->ejecuta(SQLQuery);
		companyact->commit();
	}// end if
	listalineas->guardalistlinpresupuesto();
}// end guardapresupuesto



