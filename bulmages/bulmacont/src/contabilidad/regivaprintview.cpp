/***************************************************************************
                          regivaprintview.cpp  -  description
                             -------------------
    begin                : dom jul 13 2003
    copyright            : (C) 2003 by Tomeu Borrás Riera
    email                : tborras@conetxia.com
    modificat per        : (C) 2003 Antoni Mirabete i Terés - amirabet@biada.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "regivaprintview.h"
#include "empresa.h"

#include "modelo300.h"
#ifndef WIN32
#include <unistd.h>
#endif
#include <fstream>
#include <qlocale.h>
#include "fixed.h"


extern Mod300ps *modelo;

regivaprintview::regivaprintview(empresa *emp, QWidget *parent, const char *name ) : QDialog(parent,name) {
    setupUi(this);
    fichero = NULL;
    empresaactual=emp;
    conexionbase=emp->bdempresa();
}


regivaprintview::~regivaprintview() {
}

/**************************************************************
 * Se ha pulsado sobre el boton aceptar del formulario
 **************************************************************/
void regivaprintview::accept() {
    if (radiotexto->isChecked())
        presentar("txt");
    if (radiohtml->isChecked())
        presentar("html");
    if (radioPS->isChecked()) {
        //Lanza dialogo de generacion de modelo 300
        //Deberia revisarse el Widget parent de la variable modelo (listivaview--->modelo, en vez de regivaprintview--->modelo)
//        modelo->exec();
    }// end if
}// end accept



void regivaprintview::inicializa1(QString finicial1, QString ffinal1) {
    fechainicial1->setText(finicial1);
    fechafinal1->setText(ffinal1);
}// end if


/**
 * Se ha pulsado sobre el boton aceptar del formulario
 */
void regivaprintview::presentar(char *tipus) {
#ifndef WIN32

    int txt, html, txtapren, htmlapren;
    int error;
    int pid;
    QString data;
    QString datahora;

    QString query;

    // tipus de presentaci
    txt=!strcmp(tipus,"txt");
    html=!strcmp(tipus,"html");
    txtapren=!strcmp(tipus,"txtapren");
    htmlapren=!strcmp(tipus,"htmlapren");

    // Cogemos los valores del formulario.
    QString finicial = fechainicial1->text();
    QString ffinal = fechafinal1->text();

    if (txt | html) {

        char *argstxt[]={"iva.txt","iva.txt",NULL};      //presentació txt normal
        char *argshtml[]={"iva.html","iva.html",NULL};   //presentació html normal

        ofstream fitxersortidatxt(argstxt[0]);     // creem els fitxers de sordida
        ofstream fitxersortidahtml(argshtml[0]);

        if (!fitxersortidatxt)
            txt=0;    // verifiquem que s'hagin creat correctament els fitxers
        if (!fitxersortidahtml)
            html=0;  // es pot millorar el tractament d'errors
        if (txt | html) {                // només continuem si hem pogut crear algun fitxer
            int num1;
            cursor2 *cursorapt;
            conexionbase->begin();
            query.sprintf("SELECT * FROM registroiva, cuenta, borrador, asiento  where cuenta.idcuenta=borrador.idcuenta AND borrador.idborrador=registroiva.idborrador AND asiento.idasiento=borrador.idasiento AND (cuenta.codigo LIKE '43%%' OR cuenta.codigo LIKE '600%%') AND borrador.fecha>='%s' AND borrador.fecha<='%s' ORDER BY asiento.ordenasiento",fechainicial1->text().ascii(), fechafinal1->text().ascii());
            fprintf(stderr,"%s\n",query.ascii());
            cursorapt = conexionbase->cargacursor(query,"mycursor");
            conexionbase->commit();
            // Calculamos cuántos registros van a crearse y dimensionamos la tabla.
            num1 = cursorapt->numregistros();
            int hoja=0;

            if (txt) {
                //presentación txt normal
                fitxersortidatxt.setf(ios::fixed);
                fitxersortidatxt.precision(2);
                fitxersortidatxt << "                                        IVA Repercutido \n" ;
                fitxersortidatxt << "Fecha Inicial: " << finicial.ascii() << 
		"   Fecha Final: " << ffinal.ascii() << endl;
		fitxersortidatxt << "Asiento  Fecha   Cuenta   Descripción                        Base Imponible    Factura \n" ;
                fitxersortidatxt << "__________________________________________________________________________________________________________\n";
            }// end if
            if (html) {
                //presentación html normal
                fitxersortidahtml.setf(ios::fixed);
                fitxersortidahtml.precision(2);
                fitxersortidahtml << "<html>\n";
                fitxersortidahtml << "<head>\n";
                fitxersortidahtml << "  <!DOCTYPE / public \"-//w3c//dtd xhtml 1.0 transitional//en\"\n";
                fitxersortidahtml << "    \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
                fitxersortidahtml << "  <LINK REL=StyleSheet HREF=\"estils.css\" TYPE=\"text/css\" MEDIA=screen>\n";
                fitxersortidahtml << "  <title> IVA Repercutit </title>\n";
                fitxersortidahtml << "</head>\n";
                fitxersortidahtml << "<body>\n";
                fitxersortidahtml << "<table><tr><td colspan=\"10\" class=titoliva> IVA Repercutido <hr></td></tr>\n\n";
                fitxersortidahtml << "<tr><td colspan=\"10\" class periodeiva> Data Inicial: " << finicial.ascii() << " -  Data Final: " << ffinal.ascii() << "<hr></td></tr>\n\n";
                fitxersortidahtml << "<tr><td class=titolcolumnaiva> Asiento </td><td class=titolcolumnaiva> Data </td><td class=titolcolumnaiva> Cuenta </td><td class=titolcolumnaiva> Descripción</td><td class=titolcolumnaiva> Base Imponible </td><td class=titolcolumnaiva> % IVA </td><td class=titolcolumnaiva> Quota IVA </td><td class=titolcolumnaiva> Factura </td><td class=titolcolumnaiva> CIF </td><td class=titolcolumnaiva> Cuenta IVA </td></tr>\n";
            }// end if
            while (!cursorapt->eof()) {
                datahora=cursorapt->valor("fecha");
                data=datahora.mid(0,10);
		int baseimp = cursorapt->valor("baseimp").replace(".","").toInt();
		int total = baseimp * cursorapt->valor("baseimp").replace(".","").toInt()/10000;
		QString numberstr = QString::number(total);
    		numberstr = numberstr.left(numberstr.length()-2)+"."+numberstr.right(2);
                if (txt) {
                    //presentacion txt normal
                    fitxersortidatxt << setw(10) << cursorapt->valor("ordenasiento").toAscii().data() << " " <<
		     data.ascii() << " " << 
		     cursorapt->valor("contrapartida").ascii() << " " << cursorapt->valor("descripcion").ascii() << " " << cursorapt->valor("baseimp").ascii() << " " << 
		     cursorapt->valor("iva").ascii() << " " << 
		     numberstr.ascii() <<" " << 
		     cursorapt->valor("factura").ascii() << " " << 
		     cursorapt->valor("cif").ascii() << " " <<  
		     endl;
                }// end if
                if (html) {
                    //presentacion html normal
                    fitxersortidahtml << "<tr><td class=assentamentiva>" << cursorapt->valor("idasiento").ascii() << "</td><td class=dataiva>" << data.ascii() << "</td><td class=contrapartidaiva>" << cursorapt->valor("contrapartida").ascii() << "</td><td class=descripcioiva>" << cursorapt->valor("descripcion").ascii() << "</td><td class=baseimponibleiva>" << cursorapt->valor("baseimp").ascii() << "</td><td class=tipusiva>" << cursorapt->valor("iva").ascii() << "</td><td class=quotaiva>" << numberstr.ascii() << "</td><td class=facturaiva>" << cursorapt->valor("factura").ascii() << "</td><td class=cifiva>" << cursorapt->valor("cif").ascii() << "</td></tr> \n";
                }// end if
                // Calculamos la siguiente cuenta registro y finalizamos el bucle
                cursorapt->siguienteregistro();
            }// end while
            // Vaciamos el cursor de la base de datos.
            delete cursorapt;
	    
	    QLocale spanish(QLocale::Spanish); // vamos a formatear los números con punto para los millares y coma para los decimales
	    /// AHORA PONEMOS EL RESUMEN DEL IVA REPERCUTIDO
	    QString SQLQuery = "SELECT * FROM cuenta, tipoiva  LEFT JOIN (SELECT idtipoiva, SUM(baseiva) AS tbaseiva FROM iva iva.idregistroiva IN (SELECT idregistroiva FROM registroiva WHERE ffactura >='"+fechainicial1->text()+"' AND ffactura <='"+fechafinal1->text()+"' ) GROUP BY idtipoiva) AS dd ON dd.idtipoiva=tipoiva.idtipoiva WHERE tipoiva.idcuenta = cuenta.idcuenta AND cuenta.codigo LIKE '477%'";
	    conexionbase->begin();
	    cursor2* cur = conexionbase->cargacursor(SQLQuery, "elcursor");
	    conexionbase->commit();
	    int j=0;
	    Fixed tivar("0");
	    Fixed tbaseimpr("0");    
	    while (! cur->eof() ) {
		Fixed baseiva(cur->valor("tbaseiva").replace(".","").ascii());
		Fixed porcent(cur->valor("porcentajetipoiva").replace(".","").ascii());
		Fixed baseimp = baseiva*1000000/porcent;
		QString numberstr = baseimp.toQString();
		
		// Pasamos al formato de representación español las cantidades a mostrar
		QString iva = spanish.toString(cursorapt->valor("tbaseiva").toDouble(),'f',2);
		QString bi = spanish.toString(numberstr.toDouble(),'f',2);
		
		fitxersortidatxt << setiosflags( ios::left ) << setw(16) << cur->valor("nombretipoiva").ascii() << " IVA: ";
		fitxersortidatxt << resetiosflags( ios::left ) << setw(12) << iva.ascii() << " BI: ";
		fitxersortidatxt << setw(10) << bi.ascii() << endl;
		
		tivar = tivar+baseiva;
		tbaseimpr = tbaseimpr+baseimp;           
		cur->siguienteregistro();
		j++;
	    }// end while    
	    delete cur;

            conexionbase->begin();
            query.sprintf("SELECT *,(baseimp+iva) AS total, (iva/baseimp*100)::INTEGER AS cuota FROM registroiva, cuenta, borrador, asiento  WHERE cuenta.idcuenta=borrador.idcuenta AND borrador.idborrador=registroiva.idborrador AND asiento.idasiento=borrador.idasiento AND (cuenta.codigo NOT LIKE '43%%' AND cuenta.codigo NOT LIKE '600%%') AND borrador.fecha>='%s' AND borrador.fecha<='%s'ORDER BY cuota,borrador.fecha",fechainicial1->text().ascii(), fechafinal1->text().ascii());
            fprintf(stderr,"%s\n",query.ascii());
            cursorapt = conexionbase->cargacursor(query,"mycursor");
            conexionbase->commit();
            // Calculamos cuantos registros van a crearse y dimensionamos la tabla.
            num1 = cursorapt->numregistros();
            hoja=0;
            if (txt) {
                //presentación txt normal
                fitxersortidatxt.setf(ios::fixed);
                fitxersortidatxt.precision(2);
                fitxersortidatxt << "                                        IVA Soportado \n" ;
		fitxersortidatxt << "Asiento   Fecha     Cuenta   Descripción                        Subtotal Cuota   IVA       Total   Factura    CIF\n" ;
                fitxersortidatxt << "_______________________________________________________________________________________________________________________\n";
            }
            if (html) {
                //presentación html normal
                fitxersortidahtml.setf(ios::fixed);
                fitxersortidahtml.precision(2);
                fitxersortidahtml << "<html>\n";
                fitxersortidahtml << "<head>\n";
                fitxersortidahtml << "  <!DOCTYPE / public \"-//w3c//dtd xhtml 1.0 transitional//en\"\n";
                fitxersortidahtml << "    \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
                fitxersortidahtml << "  <LINK REL=StyleSheet HREF=\"estils.css\" TYPE=\"text/css\" MEDIA=screen>\n";
                fitxersortidahtml << "  <title> IVA Soportat </title>\n";
                fitxersortidahtml << "</head>\n";
                fitxersortidahtml << "<body>\n";
                fitxersortidahtml << "<tr><td colspan=\"10\" class=titoliva> IVA Soportado <hr></td></tr>\n\n";
                fitxersortidahtml << "<tr><td class=titolcolumnaiva> Asiento </td><td class=titolcolumnaiva> Fecha </td><td class=titolcolumnaiva> Cuenta </td><td class=titolcolumnaiva> Descripcion</td><td class=titolcolumnaiva> Base Imponible </td><td class=titolcolumnaiva> % IVA </td><td class=titolcolumnaiva> Quota IVA </td><td class=titolcolumnaiva> Factura </td><td class=titolcolumnaiva> CIF </td><td class=titolcolumnaiva> Cuenta IVA </td></tr>\n";
            }
            while (!cursorapt->eof()) {
                datahora=cursorapt->valor("fecha");
                data=datahora.mid(0,10);
		// Pasamos al formato de representacion español las cantidades a mostrar
		QString bi = spanish.toString(cursorapt->valor("baseimp").toDouble(),'f',2);
		QString iva = spanish.toString(cursorapt->valor("iva").toDouble(),'f',2);
		QString total = spanish.toString(cursorapt->valor("total").toDouble(),'f',2);
		// Hacemos el calculo de la cuota (asi nos sirve para ver si se calculo el % correcto)
		//int calculo = round(cursorapt->valor("iva").toDouble()/cursorapt->valor("baseimp").toDouble()*100);
		//QString cuota = QString::number(calculo);
		QString cuota = spanish.toString(cursorapt->valor("cuota").toInt());
                // Acumulamos los totales para al final poder escribirlos
                if (txt) {
                    //presentacion txt normal
                    fitxersortidatxt << setiosflags( ios::left ) << setw(8) << cursorapt->valor("ordenasiento").ascii() << setw(12) << data.ascii() << setw(9) << cursorapt->valor("codigo").ascii() << setw(30) << cursorapt->valor("descripcion").left(30).ascii() << " " << resetiosflags( ios::left ) << setw(12) << bi.ascii() << " " << setw(3) << cuota.ascii() << "%" << setw(9) << iva.ascii() << setw(12) << total.ascii() << setiosflags( ios::left ) << "  " << setw(10) << cursorapt->valor("factura").right(8).ascii() <<  setw(10) << cursorapt->valor("cif").ascii() << endl;
                }
                if (html) {
                    //presentacion html normal
                    fitxersortidahtml << "<tr><td class=assentamentiva>" << cursorapt->valor("ordenasiento").ascii() << "</td><td class=dataiva>" << data.ascii() << "</td><td class=contrapartidaiva>" << cursorapt->valor("contrapartida").ascii() << "</td><td class=descripcioiva>" << cursorapt->valor("descripcion").ascii() << "</td><td class=baseimponibleiva>" << cursorapt->valor("baseimp").ascii() << "</td><td class=tipusiva>" << cursorapt->valor("iva").ascii() << "</td><td class=quotaiva>" << total.ascii() << "</td><td class=facturaiva>" << cursorapt->valor("factura").ascii() << "</td><td class=cifiva>" << cursorapt->valor("cif").ascii() << "</td></tr> \n";
                }
                // Calculamos la siguiente cuenta registro y finalizamos el bucle
                cursorapt->siguienteregistro();
            }// end while
            // Vaciamos el cursor de la base de datos.
            delete cursorapt;

	    /// AHORA PONEMOS EL RESUMEN DEL IVA SOPORTADO
	    SQLQuery = "SELECT * FROM cuenta, tipoiva LEFT JOIN (SELECT idtipoiva, SUM(baseiva) AS tbaseiva FROM iva  WHERE iva.idregistroiva IN (SELECT idregistroiva FROM registroiva WHERE ffactura >='"+fechainicial1->text()+"' AND ffactura <='"+fechafinal1->text()+"' ) GROUP BY idtipoiva) AS dd ON dd.idtipoiva=tipoiva.idtipoiva WHERE tipoiva.idcuenta = cuenta.idcuenta AND cuenta.codigo LIKE '472%'";
	    conexionbase->begin();
	    cur = conexionbase->cargacursor(SQLQuery, "elcursor");
	    conexionbase->commit();
	    j=0;
	    Fixed tivas("0");
	    Fixed tbaseimps("0");
	    while (! cur->eof() ) {
		Fixed baseiva(cur->valor("tbaseiva").ascii());
		Fixed porcent(cur->valor("porcentajetipoiva").ascii());
		Fixed ivacalculado = baseiva*porcent/100;
		QString numberstr = ivacalculado.toQString();
		
		// Pasamos al formato de representacion español las cantidades a mostrar
		QString bi = spanish.toString(cur->valor("tbaseiva").toDouble(),'f',2);
		QString iva = spanish.toString(numberstr.toDouble(),'f',2);
		
		if(j==0) fitxersortidatxt << endl;
		fitxersortidatxt << setiosflags( ios::left ) << setw(16) << cur->valor("nombretipoiva").ascii() << " BI: ";
		fitxersortidatxt << resetiosflags( ios::left ) << setw(12) << bi.ascii() << " IVA: ";
		fitxersortidatxt << setw(10) << iva.ascii() << endl;
		
		tivas = tivas+ivacalculado;
		tbaseimps = tbaseimps+baseiva;
		cur->siguienteregistro();
		j++;
	    }// end while
	    delete cur;

            /// AHORA CERRAMOS LOS FICHEROS
            if (txt) {
                fitxersortidatxt.close();
                //presentación txt normal
                if ((pid=fork()) < 0) {
                    perror ("Fork failed");
                    exit(errno);
                }// end if
                if (!pid) {
                    error = execvp(confpr->valor(CONF_EDITOR).ascii(),argstxt);
                }// end if
            }// end if
            if (html) {
                fitxersortidahtml << "\n</table>\n</body>\n</html>\n";
                fitxersortidahtml.close();
                //presentación html normal
                if ((pid=fork()) < 0) {
                    perror ("Fork failed");
                    exit(errno);
                }// end if
                if (!pid) {
                    error = execvp(confpr->valor(CONF_NAVEGADOR).ascii(),argshtml);
                }// end if
            }// end if
        }
    }
#endif
}



