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
    vaciaPresupuesto();
}

presupuesto::~presupuesto() {}


void presupuesto::borraPresupuesto() {
    if (mdb_idpresupuesto != "") {
        listalineas->borrar();
        companyact->begin();
        companyact->ejecuta("DELETE FROM prfp WHERE idpresupuesto="+mdb_idpresupuesto);
        companyact->ejecuta("DELETE FROM presupuesto WHERE idpresupuesto="+mdb_idpresupuesto);
        companyact->commit();
        vaciaPresupuesto();
        pintaPresupuesto();
    }// end if
}// end borraPresupuesto


void presupuesto::vaciaPresupuesto() {
    m_idpresupuesto = "0";
    m_idclient = "";
    mdb_idpresupuesto = "";
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
    mdb_procesadopresupuesto = "FALSE";
    mdb_descpresupuesto = "";
//    listalineas->vaciar();
}// end vaciaPresupuesto

void presupuesto::pintaPresupuesto() {
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
    pintaprocesadopresupuesto(mdb_procesadopresupuesto);
    pintadescpresupuesto(mdb_descpresupuesto);
    // Pinta el subformulario de detalle del presupuesto.
    listalineas->pintalistlinpresupuesto();
    
    pintatotales(listalineas->calculabase(), listalineas->calculaiva());
}// end pintaPresupuesto


// Esta funci� carga un presupuesto.
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
	mdb_descpresupuesto = cur->valor("descpresupuesto");
	if (cur->valor("procesadopresupuesto") == "t")
		mdb_procesadopresupuesto = "TRUE";
	else
		mdb_procesadopresupuesto = "FALSE";
        //        chargeBudgetDiscounts(idbudget);
        //        calculateImports();
    }// end if
    delete cur;

    //    m_initialValues = calculateValues();
    fprintf(stderr,"Vamos a cargar las lineas\n");
    listalineas->chargeBudgetLines(idbudget);
    pintaPresupuesto();
}// end chargeBudget


void presupuesto::guardapresupuesto() {
    companyact->begin();
    if (mdb_numpresupuesto == "") {
        QString SQLQueryn = "SELECT MAX(numpresupuesto)+1 as num FROM presupuesto";
        cursor2 *cur= companyact->cargacursor(SQLQueryn);
        if (!cur->eof())
            mdb_numpresupuesto = cur->valor("num");
        delete cur;
    }// end if
    if (mdb_idusuari="")
        mdb_idusuari="NULL";
    if (mdb_idpresupuesto == "") {
        /// Se trata de una inserci�
        QString SQLQuery = "INSERT INTO presupuesto (numpresupuesto, fpresupuesto, contactpresupuesto, telpresupuesto, vencpresupuesto, comentpresupuesto, idusuari, idcliente, idalmacen, procesadopresupuesto, descpresupuesto) VALUES ("+mdb_numpresupuesto+",'"+mdb_fpresupuesto+"','"+mdb_contactpresupuesto+"','"+mdb_telpresupuesto+"','"+mdb_vencpresupuesto+"','"+mdb_comentpresupuesto+"',"+mdb_idusuari+","+mdb_idcliente+","+mdb_idalmacen+","+mdb_procesadopresupuesto+","+mdb_descpresupuesto+")";

        companyact->ejecuta(SQLQuery);
        cursor2 *cur = companyact->cargacursor("SELECT MAX(idpresupuesto) AS m FROM presupuesto");
        if (!cur->eof())
            mdb_idpresupuesto = cur->valor("idpresupuesto");
        delete cur;
        companyact->commit();
    } else {
        /// Se trata de una modificaci�
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
        SQLQuery += " ,procesadopresupuesto="+mdb_procesadopresupuesto;	
	SQLQuery += " ,descpresupuesto='"+mdb_descpresupuesto+"'";
        SQLQuery += " WHERE idpresupuesto="+mdb_idpresupuesto;
        companyact->begin();
        companyact->ejecuta(SQLQuery);
        companyact->commit();
    }// end if
    listalineas->guardalistlinpresupuesto();
}// end guardapresupuesto


void presupuesto::setCifClient(QString val) {
    mdb_cifcliente=val;
    QString SQLQuery = "SELECT * FROM cliente WHERE cifcliente='"+mdb_cifcliente+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        mdb_idcliente = cur->valor("idcliente");
        mdb_nomcliente = cur->valor("nomcliente");
    } else {
        mdb_idcliente="";
        mdb_nomcliente="";
    }// end if
    delete cur;
    pintaNomClient(mdb_nomcliente);
}// end setCifClient

void presupuesto::setCodigoAlmacen(QString val) {
    mdb_codigoalmacen=val;
    QString SQLQuery = "SELECT * FROM almacen WHERE codigoalmacen='"+mdb_codigoalmacen+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        mdb_idalmacen = cur->valor("idalmacen");
        mdb_nomalmacen = cur->valor("nomalmacen");
    } else {
        mdb_idalmacen="";
        mdb_nomalmacen="";
    }// end if
    delete cur;
    pintaNomAlmacen(mdb_nomalmacen);
}// end setCodigoAlmacen

