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
#include <QFile>
//Added by qt3to4:
#include <QTextStream>
#include "funcaux.h"


Inventario::Inventario(company *comp) : DBRecord(comp) {
    _depura("Inventario::Inventario",0);
    companyact=comp;

    setDBTableName("inventario");
    setDBCampoId("idinventario");
    addDBCampo("idinventario", DBCampo::DBint, DBCampo::DBPrimaryKey, "Identificador Presupuesto");
    addDBCampo("fechainventario", DBCampo::DBdate, DBCampo::DBNothing, "Identificador Presupuesto");
    addDBCampo("nominventario", DBCampo::DBvarchar, DBCampo::DBNothing, "Identificador Presupuesto");
}

Inventario::~Inventario() {}


int Inventario::borrar() {
    if (DBvalue("idinventario") != "") {
        _depura("vamos a borrar las lineas del inventario",0);
        companyact->begin();

        listalineas->borrar();
        _depura("Vamos a borrar el resto",0);
        int error = companyact->ejecuta("DELETE FROM inventario WHERE idinventario="+DBvalue("idinventario"));
        if (error) {
            companyact->rollback();
            return -1;
        }// end if
        companyact->commit();
    }// end if
    return 0;
}


void Inventario::vaciaInventario() {
    DBclear();
}

void Inventario::pintaInventario() {
    _depura("pintaInventario\n",0);
    pintaidinventario(DBvalue("idinventario"));
    pintanominventario(DBvalue("nominventario"));
    pintafechainventario(DBvalue("fechainventario"));
    // Pinta el subformulario de detalle del Inventario.
//    listalineas->pintaListControlStock();
}


// Esta funci� carga un Inventario.
int Inventario::cargar(QString idbudget) {
    QString query = "SELECT * FROM inventario  WHERE idinventario="+idbudget;
    cursor2 * cur= companyact->cargacursor(query);
    if (!cur->eof()) {
        DBload(cur);
    }// end if
    delete cur;
    listalineas->cargar(idbudget);
    pintaInventario();
    return 0;
}


int  Inventario::guardar() {
    _depura("Inventario::guardaInventario()",0);
    QString id;
    companyact->begin();
    int error = DBsave(id);
    if (error ) {
        companyact->rollback();
        return -1;
    }// end if
    setidinventario(id);
    error = listalineas->guardar();
    if (error) {
	companyact->rollback();
	return -1;
    }// end if
    companyact->commit();
    return 0;
}



void Inventario::pregenerar() {
    _depura("Inventario::pregenerar",0);
//    listalineas->pregenerar();
}

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



