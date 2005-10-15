#include "informereferencia.h"
#include "company.h"
#include "postgresiface2.h"
#include <qstring.h>
#include <qfile.h>

InformeReferencia::InformeReferencia (company *comp) {
	companyact = comp;
}

InformeReferencia::~InformeReferencia() {}

void InformeReferencia::generarinforme() {

    /// Copiamos el archivo
    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"informereferencia.rml";
    archivo = "cp "+archivo+" /tmp/informereferencia.rml";
    system (archivo.ascii());

    /// Copiamos el logo
    archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"logo.jpg";
    archivo = "cp "+archivo+" /tmp/logo.jpg";
    system (archivo.ascii());


    QFile file;
    file.setName( "/tmp/informereferencia.rml" );
    file.open( IO_ReadOnly );
    QTextStream stream(&file);
    QString buff = stream.read();
    file.close();
    QString fitxersortidatxt="";
    // L�ea de totales del presupuesto

    fitxersortidatxt += "<blockTable style=\"tablatotales\" colWidths=\"7cm, 3cm, 3cm, 3cm, 3cm\" repeatRows=\"1\">\n";
    fitxersortidatxt += "<tr>\n";
    fitxersortidatxt += "	<td>Articulo</td>\n";
    fitxersortidatxt += "	<td>Presupuestado</td>\n";
    fitxersortidatxt += "	<td>Pedido</td>\n";
    fitxersortidatxt += "	<td>Entregado</td>\n";
    fitxersortidatxt += "	<td>Facturado</td>\n";
    fitxersortidatxt += "</tr>\n";


	fprintf(stderr,"GENERACION DEL INFORME \n");
	QString SQLQuery = " SELECT * FROM articulo ";

	SQLQuery        += " LEFT JOIN (SELECT idarticulo, SUM(cantlpresupuesto) AS cantlpresupuestot  FROM lpresupuesto WHERE idpresupuesto IN (SELECT idpresupuesto FROM presupuesto WHERE refpresupuesto = '"+m_referencia+"') GROUP BY idarticulo) AS t1 ON t1.idarticulo = articulo.idarticulo ";
	
	SQLQuery        += " LEFT JOIN (SELECT idarticulo, SUM(cantlpedidocliente) AS cantlpedidoclientet  FROM lpedidocliente WHERE idpedidocliente IN (SELECT idpedidocliente FROM pedidocliente WHERE refpedidocliente = '"+m_referencia+"') GROUP BY idarticulo) AS t2 ON t2.idarticulo = articulo.idarticulo ";	
	
	SQLQuery        += " LEFT JOIN (SELECT idarticulo, SUM(cantlalbaran) AS cantlalbarant  FROM lalbaran WHERE idalbaran IN (SELECT idalbaran FROM albaran WHERE refalbaran = '"+m_referencia+"') GROUP BY idarticulo) AS t3 ON t3.idarticulo = articulo.idarticulo ";		

	SQLQuery        += " LEFT JOIN (SELECT idarticulo, SUM(cantlfactura) AS cantlfacturat  FROM lfactura WHERE idfactura IN (SELECT idfactura FROM factura WHERE reffactura = '"+m_referencia+"') GROUP BY idarticulo) AS t4 ON t4.idarticulo = articulo.idarticulo ";		
	
	SQLQuery        += " WHERE  (cantlpresupuestot <>0 OR cantlpedidoclientet <> 0 OR cantlalbarant <> 0 OR cantlfacturat <> 0) ";
	
	fprintf(stdout,"%s\n",SQLQuery.ascii());
	cursor2 *cur = companyact->cargacursor(SQLQuery);
	while (!cur->eof() ) {
		fitxersortidatxt +="<tr>\n";
		fitxersortidatxt += "<td>"+cur->valor("nomarticulo")+"</td>\n";
		fitxersortidatxt += "<td>"+cur->valor("cantlpresupuestot")+"</td>\n";
		fitxersortidatxt += "<td>"+cur->valor("cantlpedidoclientet")+"</td>\n";
		fitxersortidatxt += "<td>"+cur->valor("cantlalbarant")+"</td>\n";
		fitxersortidatxt += "<td>"+cur->valor("cantlfacturat")+"</td>\n";
		fitxersortidatxt +="</tr>\n";
		cur->siguienteregistro();
	}// end while 
	delete cur;

    fitxersortidatxt += "</blockTable>\n";

    buff.replace("[referencia]", m_referencia);
    buff.replace("[story]",fitxersortidatxt);
    if ( file.open( IO_WriteOnly ) ) {
        QTextStream stream( &file );
        stream << buff;
        file.close();
    }// End if
    system("trml2pdf.py /tmp/informereferencia.rml > /tmp/informereferencia.pdf");
    system("kpdf /tmp/informereferencia.pdf &");
}

