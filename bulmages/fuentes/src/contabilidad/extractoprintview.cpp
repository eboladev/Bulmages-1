/***************************************************************************
                          extractoprintview.cpp  -  description
                             -------------------
    begin                : jue jun 26 2003
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
 ***************************************************************************/
#include <unistd.h>
#include <qwidget.h>
#include <qcheckbox.h>

#include "extractoprintview.h"
#include "listcuentasview1.h"
#include "selectccosteview.h"
#include "selectcanalview.h"
#include "empresa.h"

/*
#ifdef REPORTS
#include "rtkinputbges.h"
#include "rtkqtreportviewer.h"
using namespace RTK;
#endif
*/

/** \brief Constructor de la clase que inicializa los parametros necesarios para esta
  * @param emp Empresa con la que va a trabajar esta clase
  * @param parent Ventana que llama a esta ventana
  * @param name Nombre de la ventana
  * Aqui también se inicializa la variable global conexionbase.
  */
ExtractoPrintView::ExtractoPrintView(empresa *emp, QWidget *parent=0, const char *name=0 ) : ExtractoPrintDlg(parent,name) {
    fichero=NULL;
    empresaactual=emp;
    conexionbase = emp->bdempresa();
    m_filt = NULL;
}// end ExtractoPrintView

ExtractoPrintView::~ExtractoPrintView() {}

/** \brief Inicializa la clase con algunos parámetros por defecto
  * @param fechainicial Fecha para empezar el extracto
  * @param fechafinal  Fecha para terminar el extracto
  * @param codi Codigo de cuenta inicial del extracto
  * @param codf Codigo de cuenta final del extracto
  */
void ExtractoPrintView::inicializa1(QString fechainicial, QString fechafinal, QString codi, QString codf) {
    fechainicial1->setText(fechainicial);
    fechafinal1->setText(fechafinal);
    codigoinicial->setText(codi);
    codigofinal->setText(codf);
}// end inicializa1

/**************************************************************
 * Se ha pulsado sobre el boton aceptar del formulario
 **************************************************************/
void ExtractoPrintView::accept() {
    // Versió per si només permetem escollir una opci
    if (radiotexto->isChecked())
        presentar("txt");
    else if (radiohtml->isChecked())
        presentar("html");
    else if (radiokugar->isChecked())
        presentakugar();
    else
        pruebasRTK();
}

/** \brief se va a hacer una impresión de Libro Mayor con kugar
  */
void ExtractoPrintView::presentakugar() {
    int txt=1;
    float debe, haber;
    int idasiento=0;
    string fecha;
    string fechaasiento;
    string descripcion;
    string conceptocontable;
    string codigo;
    string ordenasiento;
    string cad;
    string desc1;
    string codcontrapartida;
    string codigoant="0";
    string desccontrapartida;
    cursor2 *cursoraux;



    /// La impresión siempre se hace en el archivo extracte.kud situado en el directorio de trabajo
    char *argstxt[]={"extracte.kud","extracte.kud",NULL};      //presentació txt normal
    ofstream fitxersortidatxt(argstxt[0]);     // creem els fitxers de sordida
    if (!fitxersortidatxt)
        txt=0;    // verifiquem que s'hagin creat correctament els fitxers

    if (txt) {
        /// Sacamos la cabecera del documento de kugar.
        fitxersortidatxt.setf(ios::fixed)
            ;
        fitxersortidatxt.precision(2);
        fitxersortidatxt << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" ;
        fitxersortidatxt << "<!DOCTYPE KugarData [\n" ;
        fitxersortidatxt << "\t<!ELEMENT KugarData (Row* )>\n" ;
        fitxersortidatxt << "\t\t<!ATTLIST KugarData\n";
        fitxersortidatxt << "\t\tTemplate CDATA #REQUIRED>\n";
        fitxersortidatxt << "\t<!ELEMENT Row EMPTY>\n";
        fitxersortidatxt << "\t<!ATTLIST Row \n";
        fitxersortidatxt << "\t\tlevel CDATA #REQUIRED\n";
        fitxersortidatxt << "\t\tdescripcion CDATA #REQUIRED\n";
        fitxersortidatxt << "\t\tfecha CDATA #REQUIRED\n";
        fitxersortidatxt << "\t\tcodigo CDATA #REQUIRED\n";
        fitxersortidatxt << "\t\tconceptocontable CDATA #REQUIRED\n";
        fitxersortidatxt << "\t\tdesc1 CDATA #REQUIRED\n";
        fitxersortidatxt << "\t\tordenasiento CDATA #REQUIRED\n";
        fitxersortidatxt << "\t\tdebe CDATA #REQUIRED\n";
        fitxersortidatxt << "\t\thaber CDATA #REQUIRED>\n";
        fitxersortidatxt << "]>\n\n";
        fitxersortidatxt << "<KugarData Template=\"" << confpr->valor(CONF_DIR_KUGAR).ascii() <<"extracte.kut\">\n";
    }// end if

    /// Montamos la consulta que genera el listado
    QString query = montaQuery();
    /// Cargamos el query
    conexionbase->begin();
    cursoraux = conexionbase->cargacursor(query,"elquery");
    conexionbase->commit();
    for(;!cursoraux->eof();cursoraux->siguienteregistro()) {
        ordenasiento = atoi(cursoraux->valor("ordenasiento").ascii());
        descripcion = cursoraux->valor("descripcion").ascii();
        fecha = cursoraux->valor("fecha").left(10).ascii();
        codigo = cursoraux->valor("codigo").ascii();
        conceptocontable = cursoraux->valor("conceptocontable").ascii();
        desc1 = cursoraux->valor("desc1").ascii();
        ordenasiento = cursoraux->valor("ordenasiento").ascii();
        debe = atof(cursoraux->valor("debe").ascii());
        haber = atof(cursoraux->valor("haber").ascii());
        codcontrapartida = cursoraux->valor("codcontrapartida").ascii();
        desccontrapartida = cursoraux->valor("desccontrapartida").ascii();
        if (codigoant != codigo) {
            /// Ponemos el row de nivel 0 ya que es por donde se agrupa.
            /// Asi que kugar pide que se ponga el row dos veces por los agrupamientos.
            fitxersortidatxt << "\t<Row level=\"0\" asiento=\""<< idasiento <<"\"";
            fitxersortidatxt << " descripcion =\""<< conceptocontable.c_str() <<"\"";
            fitxersortidatxt << " fecha=\""<< fecha <<"\"";
            fitxersortidatxt << " codigo=\""<< codigo.c_str() <<"\"";
            fitxersortidatxt << " conceptocontable=\""<< conceptocontable.c_str() <<"\"";
            fitxersortidatxt << " desc1=\""<< desc1.c_str() <<"\"";
            fitxersortidatxt << " ordenasiento=\""<< ordenasiento.c_str() <<"\"";
            fitxersortidatxt << " debe=\""<< debe <<"\"";
            fitxersortidatxt << " haber=\""<< haber <<"\"\n" ;
            fitxersortidatxt << " codcontrapartida=\""<< codcontrapartida <<"\"\n" ;
            fitxersortidatxt << " desccontrapartida=\""<< desccontrapartida <<"\"/>\n" ;
        }// end if
        if (txt) {
            //presentació txt normal
            fitxersortidatxt << "\t<Row level=\"1\" asiento=\""<< idasiento <<"\"";
            fitxersortidatxt << " descripcion =\""<< conceptocontable.c_str() <<"\"";
            fitxersortidatxt << " fecha=\""<< fecha <<"\"";
            fitxersortidatxt << " codigo=\""<< codigo.c_str() <<"\"";
            fitxersortidatxt << " conceptocontable=\""<< conceptocontable.c_str() <<"\"";
            fitxersortidatxt << " desc1=\""<< desc1.c_str() <<"\"";
            fitxersortidatxt << " ordenasiento=\""<< ordenasiento.c_str() <<"\"";
            fitxersortidatxt << " debe=\""<< debe <<"\"";
            fitxersortidatxt << " haber=\""<< haber <<"\"\n" ;
            fitxersortidatxt << " codcontrapartida=\""<< codcontrapartida <<"\"\n" ;
            fitxersortidatxt << " desccontrapartida=\""<< desccontrapartida <<"\"/>\n" ;
        }// end if
        codigoant = codigo;
    }// end for
    delete cursoraux;
    fitxersortidatxt <<"</KugarData>\n";
    fitxersortidatxt.close();

    system("kugar extracte.kud");
}// end presentakugar


/** \brief Esta función monta la consulta que se va a realizar contra la base de datos
  * La consulta es de bastante detalle y por eso es conveniente dedicar una función a realizarla
  * Además dicha consulta puede ser invocada desde distintos sitios
  */
QString ExtractoPrintView::montaQuery() {
    fprintf(stderr,"Presentar\n");
    /// Cogemos los valores del formulario para poder hacer un filtraje adecuado
    QString finicial = fechainicial1->text();
    QString ffinal = fechafinal1->text();
    QString cinicial = codigoinicial->text();
    QString cfinal = codigofinal->text();
    QString contra = m_filt->codigocontrapartida->text();

    // Preparamos el string para que aparezca una u otra cosa segun el punteo.
    QString tipopunteo;
    tipopunteo="";
    if (m_filt->punteotodos->isChecked()) {
        tipopunteo = "";
    } else if (m_filt->punteopunteado->isChecked()) {
        tipopunteo = " AND apunte.punteo=TRUE ";
    } else {
        tipopunteo = " AND apunte.punteo=FALSE ";
    }// end if
    if (contra != "") {
        tipopunteo += " AND apunte.contrapartida = id_cuenta('"+contra+"') ";
    }// end if

    selectcanalview *scanal=empresaactual->getselcanales();
    selectccosteview *scoste=empresaactual->getselccostes();
    QString ccostes = scoste->cadcoste();
    if (ccostes != "")
        ccostes.sprintf(" AND idc_coste IN (%s) ", ccostes.ascii());
    QString ccanales = scanal->cadcanal();
    if (ccanales != "")
        ccanales.sprintf(" AND idcanal IN (%s) ", ccanales.ascii());

    QString tabla;
    if (m_filt->m_asAbiertos->isChecked()) {
        tabla= "borrador";
    } else {
        tabla = "apunte";
    }// end if

    QString query = "SELECT desccontrapartida, codcontrapartida, "+tabla+".contrapartida AS contrapartida, ordenasiento, cuenta.descripcion AS descripcion, "+tabla+".debe AS debe , "+tabla+".haber AS haber, conceptocontable, idc_coste, codigo, cuenta.descripcion AS desc1, "+tabla+".fecha AS fecha FROM "+tabla+"";
    query += " LEFT JOIN asiento ON "+tabla+".idasiento=asiento.idasiento ";
    query += " LEFT JOIN cuenta ON "+tabla+".idcuenta=cuenta.idcuenta ";
    query += " LEFT JOIN (SELECT codigo AS codcontrapartida, idcuenta as contrapartida, descripcion AS desccontrapartida  FROM cuenta) AS t1 ON "+tabla+".contrapartida = t1.contrapartida ";
    query += " WHERE "+tabla+".idcuenta >= id_cuenta('"+cinicial+"')   AND "+tabla+".idcuenta <= id_cuenta('"+cfinal+"') ";
    query += " AND "+tabla+".fecha >= '"+finicial+"' AND "+tabla+".fecha <= '"+ffinal+"'";
    query += tipopunteo;
    query += ccostes;
    query += ccanales;
    query += " ORDER BY codigo, fecha";
    return query;
}// end montaQuery


// *********************** PRUEBAS CON LA LIBRERIA DE REPORTS DE S.CAPEL
void ExtractoPrintView::pruebasRTK() {
#ifdef REPORTS
    	/// Mediante comandos de sistema reemplazamos lo que necesitamos para obtener un fichero deseable.
	QString cadena;
	cadena = "cp "+confpr->valor(CONF_DIR_REPORTS)+"bulma-styles.xml   /tmp/bulma-styles.xml" ;
	system (cadena.ascii());	
	cadena = "cp "+confpr->valor(CONF_DIR_REPORTS)+"extracto1.rtk   /tmp/extracto1.rtk" ;
	system (cadena.ascii());
	cadena = "sed -e \"s/###codigoinicial###/"+codigoinicial->text()+"/g\" /tmp/extracto1.rtk > /tmp/extracto2.rtk";
	system (cadena.ascii());
	cadena = " sed -e \"s/###codigofinal###/"+codigofinal->text()+"/g\"  /tmp/extracto2.rtk > /tmp/extracto1.rtk";
	system (cadena.ascii());
	cadena = " sed -e \"s&###fechainicial###&"+fechainicial1->text()+"&g\"  /tmp/extracto1.rtk > /tmp/extracto2.rtk";
	system (cadena.ascii());
	cadena = " sed -e \"s&###fechafinal###&"+fechafinal1->text()+"&g\"  /tmp/extracto2.rtk > /tmp/extracto1.rtk";
	system (cadena.ascii());
	
	cadena = "rtkview --input-sql-driver QPSQL7 --input-sql-database ";
	cadena += conexionbase->nameDB()+" ";
	cadena += "/tmp/extracto1.rtk &";
	fprintf(stderr,"%s\n",cadena.ascii());
	system (cadena.ascii());    
#endif
}// end pruebasRTK


void ExtractoPrintView::presentar(char *tipus) {

    int txt, html;
    int error;
    int pid;
    float debe, haber,saldo;
    float debeinicial=0, haberinicial=0, saldoinicial=0;
    float debefinal, haberfinal, saldofinal;
    int idcuenta;
    int idasiento;
    int contrapartida;
    int codcontrapartida;
    int activo;
    string cad;
    cursor2 *cursoraux, *cursoraux1, *cursoraux2, *cursoraux3;

    QString finicial = fechainicial1->text();
    QString ffinal = fechafinal1->text();
    QString cinicial = codigoinicial->text();
    QString cfinal = codigofinal->text();

    // tipus de presentació

    txt=!strcmp(tipus,"txt");
    html=!strcmp(tipus,"html");

    char *argstxt[]={"major.txt","major.txt",NULL};      //presentació txt normal
    char *argshtml[]={"major.html","major.html",NULL};   //presentació html normal

    ofstream fitxersortidatxt(argstxt[0]);     // creem els fitxers de sordida
    ofstream fitxersortidahtml(argshtml[0]);

    if (!fitxersortidatxt)
        txt=0;    // verifiquem que s'hagin creat correctament els fitxers
    if (!fitxersortidahtml)
        html=0;  // es pot millorar el tractament d'errors

    if (txt | html) {                 // només continuem si hem pogut crear algun fitxer
        if (txt) {
            //presentació txt

            fitxersortidatxt.setf(ios::fixed)
                ;
            fitxersortidatxt.precision(2);

            fitxersortidatxt << "                                    MAYOR \n" ;
            fitxersortidatxt << "Fecha Inicial: " << finicial.ascii() << "   Fecha Final: " << ffinal.ascii() << endl;
            fitxersortidatxt << "_________________________________________________________________________________________________________\n";
        }
        if (html) {
            //presentació html

            fitxersortidahtml.setf(ios::fixed)
                ;
            fitxersortidahtml.precision(2);
            fitxersortidahtml << "<html>\n";
            fitxersortidahtml << "<head>\n";
            fitxersortidahtml << "  <!DOCTYPE / public \"-//w3c//dtd xhtml 1.0 transitional//en\"\n";
            fitxersortidahtml << "    \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
            fitxersortidahtml << "  <LINK REL=StyleSheet HREF=\"estils.css\" TYPE=\"text/css\" MEDIA=screen>\n";
            fitxersortidahtml << "  <title> Major </title>\n";
            fitxersortidahtml << "</head>\n";
            fitxersortidahtml << "<body>\n";
            fitxersortidahtml << "<table><tr><td colspan=\"6\" class=titolmajor> Mayor <hr></td></tr>\n\n";
            fitxersortidahtml << "<tr><td colspan=\"6\" class=periodemajor> Fecha Inicial: " << finicial.ascii() << " -  Fecha Final: " << ffinal.ascii() << "<hr></td></tr>\n\n";
        }
        conexionbase->begin();
        cursoraux = conexionbase->cargacuentascodigo(-1,cinicial, cfinal);
        conexionbase->commit();
        while(!cursoraux->eof()) {
            idcuenta = atoi(cursoraux->valor("idcuenta").ascii());
            conexionbase->begin();
            cursoraux1 = conexionbase->cargaapuntesctafecha(idcuenta, finicial.ascii(), ffinal.ascii());
            conexionbase->commit();

            if (!cursoraux1->eof()) {
                activo = strcmp((char *) cursoraux->valor("activo").ascii() , "f");

                if (txt) {
                    fitxersortidatxt << "\n\n" << setw(12) << cursoraux->valor("codigo").ascii() << setw(50) << cursoraux->valor("descripcion").ascii() << endl;
                    if (activo) {
                        fitxersortidatxt << " Cuenta de activo";
                    } else {
                        fitxersortidatxt << " Cuenta de pasivo";
                    }

                }

                if (html) {
                    fitxersortidahtml << "<tr><td colspan=\"6\" class=comptemajor>" << cursoraux->valor("codigo").ascii() << " "<< cursoraux->valor("descripcion").ascii() << "</td></tr>\n";
                    if (activo) {
                        fitxersortidahtml << "<tr><td colspan=\"6\" class=tipuscomptemajor> Cuenta de activo </td></tr>\n";
                    } else {
                        fitxersortidahtml << "<tr><td colspan=\"6\" class=tipuscomptemajor> Cuenta de pasivo </td></tr>\n";
                    }

                }
                conexionbase->begin();
                cursoraux2 = conexionbase->cargasaldoscuentafecha(idcuenta, (char *)finicial.ascii());
                conexionbase->commit();
                if (!cursoraux2->eof()) {
                    debeinicial = atof(cursoraux2->valor("tdebe").ascii());
                    haberinicial = atof(cursoraux2->valor("thaber").ascii());
                    if (activo) {
                        saldoinicial = debeinicial - haberinicial;
                    } else {
                        saldoinicial = haberinicial-debeinicial;
                    }

                    if (txt) {
                        //presentació txt
                        fitxersortidatxt << "\nAsiento  Fecha   Contrapartida   Descripcion                          Debe         Haber         Saldo\n";
                        fitxersortidatxt << "                                                 SUMAS ANTERIORES...   " << setw(10) << debeinicial << setw(10) << haberinicial << setw(10) << saldoinicial << endl;
                        fitxersortidatxt << "_________________________________________________________________________________________________________\n";
                    }

                    if (html) {
                        //presentació html
                        fitxersortidahtml << "<tr><td class=titolcolumnamajor> Asiento </td><td class=titolcolumnamajor> Fecha </td><td class=titolcolumnamajor> Contrapartida </td><td class=titolcolumnamajor> Descripcion </td><td class=titolcolumnamajor> Debe </td><td class=titolcolumnamajor> Haber </td><td class=titolcolumnamajor> Saldo </td></tr>\n";
                        fitxersortidahtml << "<tr><td></td><td></td><td></td><td class=sumamajor> Sumes anteriors...</td><td class=dosdecimals> " << debeinicial << " </td><td class=dosdecimals> " << haberinicial << " </td><td class=dosdecimals> " << saldoinicial << "</td><td>\n";
                    }

                    saldo = saldoinicial;
                    debefinal = debeinicial;
                    haberfinal = haberinicial;

                    for(;!cursoraux1->eof();cursoraux1->siguienteregistro()) {
                        idasiento=atoi(cursoraux1->valor("idasiento").ascii());
                        contrapartida = atoi(cursoraux1->valor("contrapartida").ascii());
			conexionbase->begin();
			cursoraux3 = conexionbase->cargacuenta(contrapartida);
			codcontrapartida = atoi(cursoraux3->valor("codigo").ascii());;
			conexionbase->commit(); 
                        debe=atof(cursoraux1->valor("debe").ascii());
                        haber=atof(cursoraux1->valor("haber").ascii());

                        if (activo) {
                            saldo += debe - haber;
                        } else {
                            saldo += haber -debe;
                        }

                        debefinal += debe;
                        haberfinal += haber;
                        cad = cursoraux1->valor("fecha").ascii();
                        //presentació txt
                        if (txt)
                            fitxersortidatxt <<  setw(5) << idasiento << setw(14) << cad.substr(0,10).c_str() << setw(10) << codcontrapartida << "  " << setw(40)  << setiosflags(ios::left) << cursoraux1->valor("conceptocontable").ascii() << setw(10) << resetiosflags(ios::left) << debe << setw(10) << haber << setw(10) << saldo << endl;
                        //presentació html
                        if (html)
                            fitxersortidahtml << " <tr><td class=assentamentmajor> " << idasiento << " </td><td> " << cad.substr(0,10).c_str() << " </td><td class=contrapartidamajor> " << codcontrapartida << " </td><td> " << cursoraux1->valor("conceptocontable").ascii() << " </td><td class=dosdecimals> " << debe << " </td><td class=dosdecimals> " << haber << " </td><td class=dosdecimals> " << saldo << " </td></tr>\n ";
                    }

                    if (activo) {
                        saldofinal = debefinal - haberfinal;
                    } else {
                        saldofinal = haberfinal -debefinal;
                    }
                    if (txt) {
                        //presentació txt
                        fitxersortidatxt << "                                               __________________________________________________________\n";
                        fitxersortidatxt << "                                                  TOTAL SUBCUENTA...   " << setw(10) << debefinal << setw(10) << haberfinal << setw(10) << saldofinal << endl;
                    }
                    if (html) {
                        //presentació html
                        fitxersortidahtml << "<tr><td></td><td></td><td></td><td class=sumamajor> Total subcompte... </td><td class=dosdecimals>" << debefinal << " </td><td class=dosdecimals> " << haberfinal << " </td><td class=dosdecimals> " << saldofinal << "</td></tr>\n\n";
                    }
                    cursoraux2->cerrar();
                    delete cursoraux2;
                }
            }
            cursoraux1->cerrar();
            delete cursoraux1;
            cursoraux->siguienteregistro();
        }
        if (html)
            fitxersortidahtml << "\n</table></body></html>\n";     //presentació html
        conexionbase->commit();
        delete cursoraux;
    }
    if (txt) {
        //presentació txt
        fitxersortidatxt.close();
        if ((pid=fork()) < 0) {
            perror ("Fork failed");
            exit(errno);
        }
        if (!pid) {
            error = execvp(confpr->valor(CONF_EDITOR).ascii(),argstxt);
        }
    }
    if (html) {
        //presentació html
        fitxersortidahtml.close();
        if ((pid=fork()) < 0) {
            perror ("Fork failed");
            exit(errno);
        }
        if (!pid) {
            error = execvp(confpr->valor(CONF_NAVEGADOR).ascii(),argshtml);
        }
    }
}



void ExtractoPrintView::boton_codinicial() {
    listcuentasview1 *listcuentas = new listcuentasview1(empresaactual);
    listcuentas->modo=1;
    listcuentas->inicializa();
    listcuentas->exec();
    codigoinicial->setText(listcuentas->codcuenta);
    delete listcuentas;
}// end boton_codinicial

void ExtractoPrintView::boton_codfinal() {
    listcuentasview1 *listcuentas = new listcuentasview1(empresaactual);
    listcuentas->modo=1;
    listcuentas->inicializa();
    listcuentas->exec();
    codigofinal->setText(listcuentas->codcuenta);
    delete listcuentas;
}// end boton_codfinal



void ExtractoPrintView::boton_ccostes() {
    selectccosteview *selccostes = empresaactual->getselccostes();
    selccostes->exec();
}// end boton_ccostes


void ExtractoPrintView::boton_canales() {
    selectcanalview *selcanales = empresaactual->getselcanales();
    selcanales->exec();
}// end boton_ccostes

void ExtractoPrintView::s_botonFiltrar() {
	   m_filt->exec();
}// end s_botonFiltrar();




