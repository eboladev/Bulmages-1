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
    companyact->begin();
    cursor2 * cur= companyact->cargacursor(query, "querypresupuesto");
    companyact->commit();
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
        chargeBudgetDiscounts(idbudget);
        calculateImports();
    }// end if
    delete cur;

    m_initialValues = calculateValues();
    listalineas->chargeBudgetLines(idbudget);
    pintaPresupuesto();
}// end chargeBudget



// Carga líneas de presupuesto
void listlinpresupuesto::chargeBudgetLines(QString idbudget) {
	companyact->begin();
	cursor2 * cur= companyact->cargacursor("SELECT * FROM lpresupuesto, articulo WHERE idpresupuesto="+idbudget+" AND articulo.idarticulo=lpresupuesto.idarticulo","unquery");
	companyact->commit();
	int i=0;
	while (!cur->eof()) {
	
	
		/// Creamos un elemento del tipo linpresupuesto y lo agregamos a la lista.
		linpresupuesto *lin = new linpresupuesto(companyact,
		cur->valor("idlpresupuesto"),
		cur->valor("desclpresupuesto"),
		cur->valor("cantlpresupuesto"),
		cur->valor("pvplpresupuesto"),
		cur->valor("descuentolpresupuesto"),
		cur->valor("idpresupuesto"),
		cur->valor("idarticulo")
		);
		m_lista.append(lin);
/*		
		m_list->setText(i,COL_IDLPRESUPUESTO,cur->valor("idlpresupuesto"));
		m_list->setText(i,COL_DESCLPRESUPUESTO,cur->valor("desclpresupuesto"));
		m_list->setText(i,COL_CANTLPRESUPUESTO,QString().sprintf("%0.3f",cur->valor("cantlpresupuesto").toFloat()));
		m_list->setText(i,COL_PVPLPRESUPUESTO,QString().sprintf("%0.2f",cur->valor("pvplpresupuesto").toFloat()));
		m_list->setText(i,COL_DESCUENTOLPRESUPUESTO,QString().sprintf("%0.2f",cur->valor("descuentolpresupuesto").toFloat()));
		m_list->setText(i,COL_IDPRESUPUESTO,cur->valor("idpresupuesto"));
		m_list->setText(i,COL_IDARTICULO,cur->valor("idarticulo"));
		m_list->setText(i,COL_CODARTICULO,cur->valor("codarticulo"));
		m_list->setText(i,COL_NOMARTICULO,cur->valor("nomarticulo"));
		m_list->setText(i,COL_TIPO_IVA,cur->valor("idtipo_iva"));
		companyact->begin();
		cursor2 * cur2= companyact->cargacursor("SELECT porcentasa_iva FROM tasa_iva WHERE idtipo_iva="+cur->valor("idtipo_iva")+" AND fechatasa_iva<='"+m_fpresupuesto->text()+"' ORDER BY fechatasa_iva DESC","unquery2");
		companyact->commit();
		if (!cur2->eof()) {
			m_list->setText(i,COL_TASATIPO_IVA,cur2->valor("porcentasa_iva"));
		}
		delete cur2;
*/
		i++;
		cur->siguienteregistro();
	}// end while
//	if (i>0) m_list->setNumRows(i);
	delete cur;
}// end chargeBudgetLines


