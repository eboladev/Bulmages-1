#include "informereferencia.h"
#include "company.h"
#include "postgresiface2.h"
#include <qstring.h>
#include <qfile.h>
//Added by qt3to4:
#include <QTextStream>
#include "funcaux.h"

InformeReferencia::InformeReferencia (company *comp) {
    companyact = comp;
}

InformeReferencia::~InformeReferencia() {}

void InformeReferencia::generarinforme() {
    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"informereferencia.rml";
    QString archivod = confpr->valor(CONF_DIR_USER)+"informereferencia.rml";
    QString archivologo=confpr->valor(CONF_DIR_OPENREPORTS)+"logo.jpg";

    /// Copiamos el archivo
#ifdef WINDOWS

    archivo = "copy "+archivo+" "+archivod;
#else

    archivo = "cp "+archivo+" "+archivod;
#endif

    system (archivo.ascii());

    /// Copiamos el logo

#ifdef WINDOWS

    archivologo = "copy "+archivologo+" "+confpr->valor(CONF_DIR_USER)+"logo.jpg";
#else

    archivologo = "cp "+archivologo+" "+confpr->valor(CONF_DIR_USER)+"logo.jpg";
#endif


    QFile file;
    file.setName( archivod );
    file.open( QIODevice::ReadOnly );
    QTextStream stream(&file);
    QString buff = stream.read();
    file.close();
    QString fitxersortidatxt="";
    // L�ea de totales del presupuesto

    fitxersortidatxt += "<blockTable style=\"tablatotales\" colWidths=\"11cm, 2cm, 2cm, 2cm, 2cm\" repeatRows=\"1\">\n";
    fitxersortidatxt += "<tr>\n";
    fitxersortidatxt += "	<td>Articulo</td>\n";
    fitxersortidatxt += "	<td>Pres.</td>\n";
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

    fitxersortidatxt = "<blockTable style=\"tablatotales\" colWidths=\"3cm, 3cm, 3cm, 3cm, 3cm\" repeatRows=\"1\">\n";
    fitxersortidatxt += "<tr>\n";
    fitxersortidatxt += "	<td>Pres.</td>\n";
    fitxersortidatxt += "	<td>Pedido</td>\n";
    fitxersortidatxt += "	<td>Entregado</td>\n";
    fitxersortidatxt += "	<td>Facturado</td>\n";
    fitxersortidatxt += "	<td>Cobrado</td>\n";
    fitxersortidatxt += "</tr>\n";

    /// Calculo de las cantidades totales en moneda.
    fitxersortidatxt += "<tr>\n";

    /// Total presupuestado
    SQLQuery = "SELECT SUM(calctotalpres(idpresupuesto)) AS tpres FROM presupuesto WHERE refpresupuesto='"+m_referencia+"' ";
    cur = companyact->cargacursor(SQLQuery);
    fitxersortidatxt += "	<td>"+cur->valor("tpres")+"</td>\n";
    delete cur;

    /// Total Pedido
    SQLQuery = "SELECT SUM(calctotalpedcli(idpedidocliente)) AS tpedcli FROM pedidocliente WHERE refpedidocliente='"+m_referencia+"' ";
    cur = companyact->cargacursor(SQLQuery);
    fitxersortidatxt += "	<td>"+cur->valor("tpedcli")+"</td>\n";
    delete cur;

    /// Total trabajado
    SQLQuery = "SELECT SUM(calctotalalbaran(idalbaran)) AS talb FROM albaran WHERE refalbaran='"+m_referencia+"' ";
    cur = companyact->cargacursor(SQLQuery);
    fitxersortidatxt += "	<td>"+cur->valor("talb")+"</td>\n";
    delete cur;

    /// Total facturado
    SQLQuery = "SELECT SUM(calctotalfactura(idfactura)) AS tfact FROM factura WHERE reffactura='"+m_referencia+"' ";
    cur = companyact->cargacursor(SQLQuery);
    fitxersortidatxt += "	<td>"+cur->valor("tfact")+"</td>\n";
    delete cur;

    /// Total cobrado
    SQLQuery = "SELECT SUM(cantcobro) AS tcobro FROM cobro WHERE refcobro = '"+m_referencia+"'";
    cur = companyact->cargacursor(SQLQuery);
    fitxersortidatxt += "	<td>"+cur->valor("tcobro")+"</td>\n";
    delete cur;

    fitxersortidatxt += "</tr>\n";
    fitxersortidatxt += "</blockTable>\n";

    buff.replace("[totales]", fitxersortidatxt);


    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        stream << buff;
        file.close();
    }// End if
    invocaPDF("informereferencia");
}

