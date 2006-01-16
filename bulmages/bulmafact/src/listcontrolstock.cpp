//
// C++ Implementation: ListControlStock
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "listcontrolstock.h"
#include "company.h"
#include "controlstock.h"
#include "funcaux.h"

ListControlStock::ListControlStock(company *comp) {
    companyact = comp;
    m_lista.setAutoDelete(TRUE);
    mdb_idinventario="";
}// end ListControlStock

ListControlStock::ListControlStock() {
    	   fprintf(stderr,"Constructor de ListControlStock\n");
           companyact=NULL;
           m_lista.setAutoDelete(TRUE);
           mdb_idinventario="";
}// end ListControlStock

ListControlStock::~ListControlStock() {}


void ListControlStock::nuevalinea( QString idalmacen, QString idarticulo, QString stockantcontrolstock, QString stocknewcontrolstock, QString punteocontrolstock, QString codigocompletoarticulo, QString nomarticulo, QString nomalmacen, QString codigoalmacen) {
        ControlStock *lin = new ControlStock(companyact,
                              mdb_idinventario,
                              idalmacen,
                              idarticulo,
                              stockantcontrolstock,
                              stocknewcontrolstock,
                              punteocontrolstock,
                              codigocompletoarticulo,
                              nomarticulo,
                              nomalmacen,
			      codigoalmacen
			      );
	m_lista.append(lin);
}// end nuevalinea

ControlStock *ListControlStock::linpos(int pos) {
	return (m_lista.at(pos));
}// end linpos


// Carga l�eas de FacturaProveedor
void ListControlStock::cargaListControlStock(QString idbudget) {
    _depura("ListControlStock::cargaListControlStock("+idbudget+")\n", 0);
    vaciar();
    mdb_idinventario = idbudget;
    _depura("Hacemos la carga del cursor\n",0);
    cursor2 * cur= companyact->cargacursor("SELECT * FROM controlstock, articulo, almacen WHERE articulo.idarticulo= controlstock.idarticulo AND almacen.idalmacen=controlstock.idalmacen AND controlstock.idinventario="+mdb_idinventario);
    int i=0;
    while (!cur->eof())   {
        /// Creamos un elemento del tipo ControlStock y lo agregamos a la lista.
        ControlStock *lin = new ControlStock(companyact,
                              cur->valor("idinventario"),
                              cur->valor("idalmacen"),
                              cur->valor("idarticulo"),
                              cur->valor("stockantcontrolstock"),
                              cur->valor("stocknewcontrolstock"),
                              cur->valor("punteocontrolstock"),
                              cur->valor("codigocompletoarticulo"),
                              cur->valor("nomarticulo"),
                              cur->valor("nomalmacen"),
			      cur->valor("codigoalmacen"));
        m_lista.append(lin);
        i++;
        cur->siguienteregistro();
    }// end while
    delete cur;
    _depura("Fin de ListControlStock::cargaListControlStock\n",0);
}// end chargeBudgetLines


void ListControlStock::guardaListControlStock() {
    _depura("ListControlStock::guardaListControlStock()",0);
    ControlStock *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        linea->guardaControlStock();
        i++;
    }// end for
}// en guardaListControlStock



 
void ListControlStock::vaciar() {
    mdb_idinventario = "";
    m_lista.clear();
}// end guardaListControlStock


void ListControlStock::borrarListControlStock() {
    _depura("ListControlStock::borrar",0);
    if (mdb_idinventario != "")  {
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM controlstock WHERE idinventario="+mdb_idinventario);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        companyact->commit();
    }// end if
}// end borrar


void ListControlStock::borrarControlStock(int pos) {
    _depura("ListControlStock::borrar(int)",0);
    ControlStock *linea;
    linea = m_lista.at(pos);
    linea->borrar();
    m_lista.remove(pos);
    pintaListControlStock();
}// end borraControlStock


void ListControlStock::pregenerar() {
	_depura("pregenerar",0);
	QString SQLQuery = "SELECT * FROM articulo, almacen ORDER BY codigoalmacen, codigocompletoarticulo";
	cursor2 *cur = companyact->cargacursor(SQLQuery);
	while (!cur->eof()) {
		_depura("agregamos una nueva linea",0);
		ControlStock *lin = new ControlStock(companyact);
		lin->setidalmacen(cur->valor("idalmacen"));
		lin->setcodigoalmacen(cur->valor("codigoalmacen"));
		lin->setidarticulo(cur->valor("idarticulo"));
		lin->setcodigocompletoarticulo(cur->valor("codigocompletoarticulo"));
		lin->setstocknewcontrolstock("0");
		lin->setpunteocontrolstock("FALSE");
		lin->setidinventario(mdb_idinventario);
		m_lista.append(lin);
		cur->siguienteregistro();
	}// end while
	delete cur;
	pintaListControlStock();
}
