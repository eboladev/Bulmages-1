//
// C++ Implementation: Inventario
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "inventario.h"
#include "company.h"
#include <qfile.h>
//Added by qt3to4:
#include <QTextStream>
#include "funcaux.h"

Inventario::Inventario(company *comp) {
    _depura("Inventario::Inventario",0);
    companyact=comp;
    vaciaInventario();
}

Inventario::~Inventario() {}


void Inventario::borraInventario() {
    if (mdb_idinventario != "") {
	_depura("vamos a borrar las lineas del inventario",0);
        listalineas->borrarListControlStock();
	_depura("Vamos a borrar el resto",0);
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM inventario WHERE idinventario="+mdb_idinventario);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        companyact->commit();
        vaciaInventario();
        pintaInventario();
    }// end if
}// end borraInventario


void Inventario::vaciaInventario() {
    mdb_idinventario = "";
    mdb_nominventario = "";
    mdb_fechainventario = "";

}// end vaciaInventario

void Inventario::pintaInventario() {
    _depura("pintaInventario\n",0);
    pintaidinventario(mdb_idinventario);
    pintanominventario(mdb_nominventario);
    pintafechainventario(mdb_fechainventario);
    // Pinta el subformulario de detalle del Inventario.
    listalineas->pintaListControlStock();
}// end pintaInventario


// Esta funci� carga un Inventario.
void Inventario::cargaInventario(QString idbudget) {
    mdb_idinventario = idbudget;
    QString query = "SELECT * FROM inventario  WHERE idinventario="+mdb_idinventario;
    cursor2 * cur= companyact->cargacursor(query);
    if (!cur->eof()) {
        mdb_nominventario  = cur->valor("nominventario");
        mdb_fechainventario = cur->valor("fechainventario");
    }// end if
    delete cur;
    listalineas->cargaListControlStock(idbudget);
    pintaInventario();
}// end chargeBudget


void Inventario::guardaInventario() {
    _depura("Inventario::guardaInventario()",0);
    companyact->begin();
    if (mdb_idinventario == "") {
        /// Se trata de una insercion
        QString SQLQuery = "INSERT INTO inventario (nominventario, fechainventario) VALUES ('"+
	companyact->sanearCadena(mdb_nominventario)+"',"+
	"'"+companyact->sanearCadena(mdb_fechainventario)+"')";

        int error = companyact->ejecuta(SQLQuery);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        cursor2 *cur = companyact->cargacursor("SELECT MAX(idinventario) AS m FROM inventario");
        if (!cur->eof())
            setidinventario (cur->valor("m"));
        delete cur;
        companyact->commit();
    } else {
        /// Se trata de una modificacion
        QString SQLQuery = "UPDATE inventario SET ";
        SQLQuery += " nominventario='"+companyact->sanearCadena(mdb_nominventario)+"'";
        SQLQuery += " ,fechainventario='"+companyact->sanearCadena(mdb_fechainventario)+"'";
        SQLQuery += " WHERE idinventario="+companyact->sanearCadena(mdb_idinventario);
        companyact->begin();
        int error = companyact->ejecuta(SQLQuery);
	if (error) {
		companyact->rollback();
		return;
	}// end if
        companyact->commit();
    }// end if
    listalineas->guardaListControlStock();
    cargaInventario(mdb_idinventario);
}// end guardaInventario


void Inventario::imprimirInventario() {
/*
    /// Copiamos el archivo
    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"facturap.rml";
    archivo = "cp "+archivo+" /tmp/facturap.rml";
    system (archivo.ascii());
    
    /// Copiamos el logo
    archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"logo.jpg";
    archivo = "cp "+archivo+" /tmp/logo.jpg";
    system (archivo.ascii());
    
    QFile file;
    file.setName( "/tmp/facturap.rml" );
    file.open( QIODevice::ReadOnly );
    QTextStream stream(&file);
    QString buff = stream.read();
    file.close();
    QString fitxersortidatxt;
    // L�ea de totales del presupuesto

    QString SQLQuery = "SELECT * FROM proveedor WHERE idproveedor="+mdb_idproveedor;
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        buff.replace("[dirproveedor]",cur->valor("dirproveedor"));
        buff.replace("[poblproveedor]",cur->valor("poblproveedor"));
        buff.replace("[telproveedor]",cur->valor("telproveedor"));
        buff.replace("[nomproveedor]",cur->valor("nomproveedor"));
        buff.replace("[cifproveedor]",cur->valor("cifproveedor"));
    }// end if
    buff.replace("[numfacturap]",mdb_numfacturap);
    buff.replace("[ffacturap]",mdb_ffacturap);
    buff.replace("[comentfacturap]",mdb_comentfacturap);
    buff.replace("[descfacturap]",mdb_descfacturap);
    buff.replace("[reffacturap]",mdb_reffacturap);
    buff.replace("[codigoserie_facturap]",mdb_codigoserie_facturap);
    fitxersortidatxt = "<blockTable style=\"tabla\" colWidths=\"10cm, 2cm, 2cm, 3cm\" repeatRows=\"1\">";
    fitxersortidatxt += "<tr>";
    fitxersortidatxt += "	<td>Concepto</td>";
    fitxersortidatxt += "	<td>Cantidad</td>";
    fitxersortidatxt += "	<td>Precio Und.</td>";
    fitxersortidatxt += "	<td>Total</td>";
    fitxersortidatxt += "</tr>";

    QString l;
    LinInventario *linea;
    uint i = 0;
    for ( linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next() ) {
        fitxersortidatxt += "<tr>";
        fitxersortidatxt += "	<td>"+linea->desclfacturap()+"</td>";
        fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",linea->cantlfacturap().toFloat())+"</td>";
        fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",linea->pvplfacturap().toFloat())+"</td>";
        fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",linea->cantlfacturap().toFloat() * linea->pvplfacturap().toFloat())+"</td>";
        fitxersortidatxt += "</tr>";
        i++;
    }// end for
    fitxersortidatxt += "<tr>";
    fitxersortidatxt += "	<td></td>";
    fitxersortidatxt += "	<td></td>";
    fitxersortidatxt += "	<td>Base</td>";
    fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",listalineas->calculabase())+"</td>";
    fitxersortidatxt += "</tr>";
    fitxersortidatxt += "<tr>";
    fitxersortidatxt += "	<td></td>";
    fitxersortidatxt += "	<td></td>";
    fitxersortidatxt += "	<td>Iva</td>";
    fitxersortidatxt += "	<td>"+l.sprintf("%2.2f", listalineas->calculaiva())+"</td>";
    fitxersortidatxt += "</tr>";
    fitxersortidatxt += "<tr>";
    fitxersortidatxt += "	<td></td>";
    fitxersortidatxt += "	<td></td>";
    fitxersortidatxt += "	<td>Total</td>";
    fitxersortidatxt += "	<td>"+l.sprintf("%2.2f",listalineas->calculabase()+listalineas->calculaiva())+"</td>";
    fitxersortidatxt += "</tr>";
    fitxersortidatxt += "</blockTable>";

    buff.replace("[story]",fitxersortidatxt);
    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        stream << buff;
        file.close();
    }// end if
    system("trml2pdf.py /tmp/facturap.rml > /tmp/facturap.pdf");
    system("kpdf /tmp/facturap.pdf");
*/
} //end imprimirInventario



