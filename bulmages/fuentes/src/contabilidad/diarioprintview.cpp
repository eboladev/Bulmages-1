/***************************************************************************
                          diarioprintview.cpp  -  description
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
 *                                                                         *
 ***************************************************************************/

#include "diarioprintview.h"
#include "empresa.h"

#include <unistd.h>
#include <qcheckbox.h>




DiarioPrintView::DiarioPrintView(empresa *emp,QWidget *parent, const char *name ) : DiarioPrintDlg(parent,name) {
    empresaactual = emp;
    conexionbase = empresaactual->bdempresa();
    numdigitos = empresaactual->numdigitosempresa();
}// end DiarioPrintView



DiarioPrintView::~DiarioPrintView() {}

/*
#ifdef REPORTS
#include "rtkinputbges.h"
#include "rtkqtreportviewer.h"
using namespace RTK;
#endif
*/

// *********************** PRUEBAS CON LA LIBRERIA DE REPORTS DE S.CAPEL
void DiarioPrintView::pruebasRTK() {
#ifdef REPORTS
    	/// Mediante comandos de sistema reemplazamos lo que necesitamos para obtener un fichero deseable.
	QString cadena;
	// ACORDARSE DE CAMBIAR LAS RUTAS POR LAS DEL ARCHIVO DE CONFIGURACION.
	cadena = "cp "+confpr->valor(CONF_DIR_REPORTS)+"bulma-styles.xml   /tmp/bulma-styles.xml" ;
	system (cadena.ascii());	
	cadena = "cp "+confpr->valor(CONF_DIR_REPORTS)+"diario.rtk   /tmp/diario.rtk" ;
	system (cadena.ascii());
	cadena = " sed -e \"s&###fechainicial###&"+fechainicial1->text()+"&g\"  /tmp/diario.rtk > /tmp/diario1.rtk";
	system (cadena.ascii());
	cadena = " sed -e \"s&###fechafinal###&"+fechafinal1->text()+"&g\"  /tmp/diario1.rtk > /tmp/diario.rtk";
	system (cadena.ascii());
	
	cadena = "rtkview --input-sql-driver QPSQL7 --input-sql-database ";
	cadena += conexionbase->nameDB()+" ";
	cadena += "/tmp/diario.rtk &";
	fprintf(stderr,"%s\n",cadena.ascii());
	system (cadena.ascii()); 
#endif
}// end pruebasRTK

// *********************** PRUEBAS CON LA LIBRERIA DE REPORTS DE S.CAPEL
void DiarioPrintView::pruebasRTKoo() {
#ifdef REPORTS

/*
    cursor2 *cursoraux;
    conexionbase->begin();
    cursoraux=conexionbase->cargacursor("SELECT ordenasiento, asiento.idasiento AS idasiento, cuenta.descripcion AS descripcion, apunte.debe AS debe , apunte.haber AS haber, conceptocontable, idc_coste, codigo, cuenta.descripcion AS desc1, apunte.fecha AS fecha FROM (asiento LEFT JOIN apunte ON asiento.idasiento=apunte.idasiento) LEFT JOIN cuenta ON apunte.idcuenta=cuenta.idcuenta ORDER BY ordenasiento", "unquery");

    conexionbase->commit();

    RTK::Report unReport;
    unReport.readXml(confpr->valor(CONF_DIR_REPORTS)+"diario.rtk");
    InputBGes *inp = static_cast<InputBGes *>(unReport.getInput());
    inp->set
    (InputBGes::diario, empresaactual, cursoraux);
    OutputOpenOffice *oo = new OutputOpenOffice("/tmp/pruebasbulmages.sxc");
    unReport.print(*oo);
    
*/
#endif
}// end pruebasRTKoo


/** \brief Esta función monta la consulta que se va a realizar contra la base de datos
  * La consulta es de bastante detalle y por eso es conveniente dedicar una función a realizarla
  * Además dicha consulta puede ser invocada desde distintos sitios
  */
QString DiarioPrintView::montaQuery() {
    QString query;
    QString fecha;
    QString fechaasiento;
    QString descripcion;
    QString concepto;
    QString codigocuenta;
    QString nomcuenta;
    QString cadaux;

    QString finicial = fechainicial1->text();
    QString ffinal = fechafinal1->text();
    QString cad;

    // Consideraciones para centros de coste y canales
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

    query= "SELECT asiento.ordenasiento, "+tabla+".contrapartida, "+tabla+".fecha, asiento.fecha AS fechaasiento,cuenta.tipocuenta, cuenta.descripcion, "+tabla+".conceptocontable,"+tabla+".descripcion AS descapunte, "+tabla+".debe, "+tabla+".haber, cuenta.idcuenta, asiento.idasiento, "+tabla+".idc_coste, "+tabla+".idcanal, cuenta.codigo AS codigocuenta FROM asiento, "+tabla+", cuenta WHERE asiento.idasiento="+tabla+".idasiento AND "+tabla+".idcuenta = cuenta.idcuenta AND "+tabla+".fecha >= '"+finicial+"' AND "+tabla+".fecha <= '"+ffinal+"' "+ccostes+" "+ccanales+" ORDER BY "+tabla+".fecha, asiento.idasiento, "+tabla+".orden";
    return query;
}// end montaQuery

void DiarioPrintView::inicializa1(QString fechai, QString fechaf) {
    fechainicial1->setText(fechai);
    fechafinal1->setText(fechaf);
}// end inicializa1

/**************************************************************
 * Se ha pulsado sobre el boton aceptar del formulario
 **************************************************************/
void DiarioPrintView::accept() {
    // Versió per si només permetem escollir una opció

    if (radiotexto->isChecked()) {
        if (radionormal->isChecked()) {
            presentar("txt");
        } else if (radioaprendizaje->isChecked()) {
            presentar("txtapren");
        }// end if
    } else  if (radiohtml->isChecked()) {
        if (radionormal->isChecked()) {
            presentar("html");
        } else if (radioaprendizaje->isChecked()) {
            presentar("htmlapren");
        }// end if
    } else if (radiopropietario->isChecked()) {
        // El formato propietario es de momento el de RTK.
        pruebasRTK();
        //     pruebasRTKoo();
    } else if (radiokugar->isChecked()) {
        presentakugar();
    }// end if
}// end accept

void DiarioPrintView::presentakugar() {
    int txt=1;
    float debe, haber;
    int idasiento;
    string fecha;
    string fechaasiento;
    string descripcion;
    string concepto;
    string codigocuenta;
    string cad;
    cursor2 *cursoraux;

    // Cogemos los valores del formulario.
    QString finicial = fechainicial1->text();
    QString ffinal = fechafinal1->text();


    char *argstxt[]={"diari.kud","diari.kud",NULL};      //presentació txt normal
    ofstream fitxersortidatxt(argstxt[0]);     // creem els fitxers de sordida
    if (!fitxersortidatxt)
        txt=0;    // verifiquem que s'hagin creat correctament els fitxers

    if (txt) {
        //presentació txt normal
        fitxersortidatxt.setf(ios::fixed);
        fitxersortidatxt.precision(2);
        fitxersortidatxt << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" ;
        fitxersortidatxt << "<!DOCTYPE KugarData [\n" ;
        fitxersortidatxt << "\t<!ELEMENT KugarData (Row* )>\n" ;
        fitxersortidatxt << "\t\t<!ATTLIST KugarData\n";
        fitxersortidatxt << "\t\tTemplate CDATA #REQUIRED>\n";
        fitxersortidatxt << "\t<!ELEMENT Row EMPTY>\n";
        fitxersortidatxt << "\t<!ATTLIST Row \n";
        fitxersortidatxt << "\t\tlevel CDATA #REQUIRED\n";
        fitxersortidatxt << "\t\tasiento CDATA #REQUIRED\n";
        fitxersortidatxt << "\t\tfecha CDATA #REQUIRED\n";
        fitxersortidatxt << "\t\tcuenta CDATA #REQUIRED\n";
        fitxersortidatxt << "\t\tdescripcion CDATA #REQUIRED\n";
        fitxersortidatxt << "\t\tdebe CDATA #REQUIRED\n";
        fitxersortidatxt << "\t\thaber CDATA #REQUIRED>\n";
        fitxersortidatxt << "]>\n\n";
        fitxersortidatxt << "<KugarData Template=\"" << confpr->valor(CONF_DIR_KUGAR).ascii()<<"diari.kut\">\n";
    }// end if

    QString query = montaQuery();
    conexionbase->begin();
    cursoraux = conexionbase->cargacursor(query,"elquery");
    conexionbase->commit();
    for(;!cursoraux->eof();cursoraux->siguienteregistro()) {
        fechaasiento = cursoraux->valor("fechaasiento").ascii();
        idasiento = atoi(cursoraux->valor("ordenasiento").ascii());
        fecha = cursoraux->valor("fecha").left(10).ascii();
        descripcion = cursoraux->valor("descripcion").ascii();
        concepto = cursoraux->valor("conceptocontable").ascii();
        debe = atof(cursoraux->valor("debe").ascii());
        haber = atof(cursoraux->valor("haber").ascii());
        codigocuenta = cursoraux->valor("codigocuenta").ascii();

        if (txt) {
            //presentació txt normal
            fitxersortidatxt << "\t<Row level=\"0\" asiento=\""<< idasiento <<"\"";
            fitxersortidatxt << " fecha=\""<< fecha <<"\"";
            fitxersortidatxt << " cuenta=\""<< codigocuenta.c_str() <<"\"";
            fitxersortidatxt << " descripcion =\""<< concepto.c_str() <<"\"";
            fitxersortidatxt << " debe=\""<< debe <<"\"";
            fitxersortidatxt << " haber=\""<< haber <<"\"/>\n" ;
        }
    }// end for

    delete cursoraux;
    fitxersortidatxt <<"</KugarData>\n";
    fitxersortidatxt.close();

    system("kugar diari.kud");
}// end presentakugar



void DiarioPrintView::presentar(char * tipus) {
    int txt, html, txtapren, htmlapren;
    int error;
    int pid;
    float debe, haber;
    int idcuenta;
    int idasiento;
    char data[15];
    string fecha;
    string fechaasiento;
    string descripcion;
    string concepto;
    string codigocuenta;
    string cad;
    cursor2 *cursoraux;

    // tipus de presentació
    txt=!strcmp(tipus,"txt");
    html=!strcmp(tipus,"html");
    txtapren=!strcmp(tipus,"txtapren");
    htmlapren=!strcmp(tipus,"htmlapren");

    // Cogemos los valores del formulario.
    QString finicial = fechainicial1->text();
    QString ffinal = fechafinal1->text();

    if (txt | html) {
        char *argstxt[]={"diari.txt","diari.txt",NULL};      //presentació txt normal
        char *argshtml[]={"diari.html","diari.html",NULL};   //presentació html normal

        ofstream fitxersortidatxt(argstxt[0]);     // creem els fitxers de sordida
        ofstream fitxersortidahtml(argshtml[0]);

        if (!fitxersortidatxt)
            txt=0;    // verifiquem que s'hagin creat correctament els fitxers
        if (!fitxersortidahtml)
            html=0;  // es pot millorar el tractament d'erro
	    
        if (txt | html) {                // només continuem si hem pogut crear algun fitxer
            if (txt) {
                //presentació txt normal

                fitxersortidatxt.setf(ios::fixed);
                fitxersortidatxt.precision(2);

                fitxersortidatxt << "                                        LLIBRE DIARI \n" ;
                fitxersortidatxt << "Data Inicial: " << finicial.ascii() << "   Data Final: " << ffinal.ascii() << endl;
                fitxersortidatxt << "ASIENTO   FECHA    SUBCUENTA   DESCRIPCION                                   DEBE     HABER \n" ;
                fitxersortidatxt << "_______________________________________________________________________________________________\n";
            }// end if
            if (html) {
                //presentació html normal
                fitxersortidahtml.setf(ios::fixed);
                fitxersortidahtml.precision(2);
                fitxersortidahtml << "<html>\n";
                fitxersortidahtml << "<head>\n";
                fitxersortidahtml << "  <!DOCTYPE / public \"-//w3c//dtd xhtml 1.0 transitional//en\"\n";
                fitxersortidahtml << "    \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
                fitxersortidahtml << "  <LINK REL=StyleSheet HREF=\""<< confpr->valor(CONF_PLANTILLA) <<"\" TYPE=\"text/css\" MEDIA=screen>\n";
                fitxersortidahtml << "  <title> Llibre Diari </title>\n";
                fitxersortidahtml << "</head>\n";
                fitxersortidahtml << "<body>\n";
                fitxersortidahtml << "<table><tr><td colspan=\"6\" class=titoldiari> Llibre diari <hr></td></tr>\n\n";
                fitxersortidahtml << "<tr><td colspan=\"6\" class=periodediari> Data Inicial: " << finicial.ascii() << " -  Data Final: " << ffinal.ascii() << "<hr></td></tr>\n\n";
                fitxersortidahtml << "<tr><td class=titolcolumnadiari>Assentament</td><td class=titolcolumnadiari>Data</td><td class=titolcolumnadiari>Subcompte</td><td class=titolcolumnadiari>Descripció</td><td class=titolcolumnadiari>Deure</td><td class=titolcolumnadiari>Haver</td></tr>\n";
            }

            QString query = montaQuery();
            conexionbase->begin();
            cursoraux = conexionbase->cargacursor(query,"elquerydb");
            conexionbase->commit();

            for(;!cursoraux->eof();cursoraux->siguienteregistro()) {
                fechaasiento = cursoraux->valor("fechaasiento").ascii();
                idasiento = atoi(cursoraux->valor("ordenasiento").ascii());
                fecha = cursoraux->valor("fecha").ascii();
                descripcion = cursoraux->valor("descripcion").ascii();
                concepto = cursoraux->valor("conceptocontable").ascii();
                debe = atof(cursoraux->valor("debe").ascii());
                haber = atof(cursoraux->valor("haber").ascii());
                idcuenta = atoi(cursoraux->valor("idcuenta").ascii());
                codigocuenta = cursoraux->valor("codigocuenta").ascii();
                strncpy(data,fecha.c_str(),10);
                data[10]='\0';
                if (txt) {
                    //presentació txt normal
                    fitxersortidatxt << setw(5) << idasiento << "  " << setw(10) << data << " " << setw(10) << codigocuenta.c_str() <<  "  " << setw(40) <<  setiosflags(ios::left) << descripcion.c_str() <<  resetiosflags(ios::left)<< " " << setw(10) << debe << " " << setw(10) << haber << endl;
                }// end if
                if (html) {
                    //presentació html normal
                    fitxersortidahtml << "<tr><td class=assentamentdiari>" << idasiento << "</td><td class=datadiari>" << data << "</td><td class=codicomptediari>" << codigocuenta.c_str() << "</td><td class=descripciodiari>" <<  descripcion.c_str() << "</td><td class=dosdecimals>" << debe << "</td><td class=dosdecimals>" << haber << "</td></tr>\n";
                }// end if
            }// end for
            delete cursoraux;
            if (html) {
                fitxersortidahtml << "</table>\n<hr>\n</body>\n</html>\n";     //presentació html normal
                fitxersortidahtml.close();
            }// end if
            if (txt) {
                fitxersortidatxt << "_______________________________________________________________________________________________\n";
                fitxersortidatxt.close();
            }// end if
        }// end if

        if ((pid=fork()) < 0) {
            perror ("Fork failed");
            exit(errno);
        }

        if (txt) {
            //presentació txt normal
            if (!pid) {
                error = execvp(confpr->valor(CONF_EDITOR).ascii(),argstxt);
            }
        }
        if (html) {
            //presentació html normal
            if (!pid) {
                error = execvp(confpr->valor(CONF_NAVEGADOR).ascii(),argshtml);
            }// end if
        }// end if
    }
    if (txtapren | htmlapren ) {

        char *argstxt[]={"diariaprenentatge.txt","diariaprenentatge.txt",NULL};      //presentació txt normal
        char *argshtml[]={"diariaprenentatge.html","diariaprenentatge.html",NULL};   //presentació html normal

        ofstream fitxersortidatxt(argstxt[0]);     // creem els fitxers de sordida
        ofstream fitxersortidahtml(argshtml[0]);

        if (!fitxersortidatxt)
            txtapren=0;    // verifiquem que s'hagin creat correctament els fitxers
        if (!fitxersortidahtml)
            htmlapren=0;  // es pot millorar el tractament d'errors

        if (txtapren | htmlapren) {       // només continuem si hem pogut crear algun fitxer
            if (txtapren) {
                //presentació txt format aprenentatge

                fitxersortidatxt.setf(ios::fixed);
                fitxersortidatxt.precision(2);

                fitxersortidatxt << "                                                      LLIBRE DIARI \n" ;
                fitxersortidatxt << "Data Inicial: " << finicial.ascii() << "   Data Final: " << ffinal.ascii() << endl;
                fitxersortidatxt << "___________________________________________________________________________________________________________________________________\n";

            }
            if (htmlapren) {
                //presentació html format aprenentatge

                fitxersortidahtml.setf(ios::fixed)
                    ;
                fitxersortidahtml.precision(2);

                fitxersortidahtml << "<html>\n";
                fitxersortidahtml << "<head>\n";
                fitxersortidahtml << "  <!DOCTYPE / public \"-//w3c//dtd xhtml 1.0 transitional//en\"\n";
                fitxersortidahtml << "    \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
                fitxersortidahtml << "  <LINK REL=StyleSheet HREF=\"estils.css\" TYPE=\"text/css\" MEDIA=screen>\n";
                fitxersortidahtml << "  <title> Llibre Diari </title>\n";
                fitxersortidahtml << "</head>\n";
                fitxersortidahtml << "<body>\n";
                fitxersortidahtml << "<table><tr><td colspan=\"7\" class=titoldiariapren> Llibre diari <hr></td></tr>\n\n";
                fitxersortidahtml << "<tr><td colspan=\"7\" class=periodediariapren> Data Inicial: " << finicial.ascii() << " -  Data Final: " << ffinal.ascii() << "<hr></td></tr>\n\n";
            }
            conexionbase->begin();
            cursoraux = conexionbase->cargaasientosfecha((char *)finicial.ascii(),(char *)ffinal.ascii());

            for(;!cursoraux->eof();cursoraux->siguienteregistro()) {
                int idasiento=atoi(cursoraux->valor("idasiento").ascii());

                if (txtapren) {
                    //presentació txt format aprenentatge
                    fitxersortidatxt << "_________________________________________________________   " << idasiento << "   ___________________________________________________________________\n";
                }
                if (htmlapren) {
                    //presentació html format aprenentatge
                    fitxersortidahtml << "\n<tr><td colspan=\"7\" class=liniadiariapren>____________________________________________________   " << idasiento << "   _____________________________________________________</td></tr>\n" ;
                }

                char consulta[400];
                char codicompte[20];
                sprintf(consulta,"SELECT cuenta.codigo, cuenta.descripcion AS nomcuenta, apunte.debe AS debe, apunte.haber AS haber FROM apunte, cuenta WHERE apunte.idasiento=%d AND apunte.haber=0 AND cuenta.idcuenta=apunte.idcuenta",idasiento);
                conexionbase->begin();
                cursor2 *cursasiento = conexionbase->cargacursor(consulta,"asiento");
                conexionbase->commit();

                while (!cursasiento->eof()) {

                    if (txtapren) {
                        //presentació txt format aprenentatge
                        sprintf(codicompte,"( %s )",cursasiento->valor("codigo").ascii());
                        fitxersortidatxt << setw(12) << atof(cursasiento->valor("debe").ascii()) << "  " << setw(10) << codicompte << "  " << cursasiento->valor("nomcuenta").ascii() << endl;
                    }
                    if (htmlapren) {
                        //presentació html format aprenentatge
                        fitxersortidahtml << " <tr><td class=deurediariapren> " << atof(cursasiento->valor("debe").ascii()) << " </td><td class=codidiariapren> ( " << cursasiento->valor("codigo").ascii() << " ) </td><td class=nomcomptediariapren> " << cursasiento->valor("nomcuenta").ascii() << " </td><td> </td><td> </td><td> </td><td> </td></tr>\n";
                    }
                    cursasiento->siguienteregistro();

                }

                delete cursasiento;
                sprintf(consulta,"SELECT cuenta.codigo, cuenta.descripcion AS nomcuenta, apunte.debe AS debe, apunte.haber AS haber FROM apunte, cuenta WHERE apunte.idasiento=%d AND apunte.debe=0 AND cuenta.idcuenta=apunte.idcuenta",idasiento);
                conexionbase->begin();
                cursasiento = conexionbase->cargacursor(consulta,"asiento");
                conexionbase->commit();

                while (!cursasiento->eof()) {

                    if (txtapren) {
                        //presentació txt format aprenentatge
                        sprintf(codicompte,"( %s )",cursasiento->valor("codigo").ascii());
                        fitxersortidatxt << "                                                                 a  " << setw(30) << cursasiento->valor("nomcuenta").ascii() << "  " << setw(10) << codicompte << "  " << setw(12) <<  atof(cursasiento->valor("haber").ascii()) <<endl;
                    }// end if
                    if (htmlapren) {
                        //presentació html format aprenentatge
                        fitxersortidahtml << " <tr><td> </td><td> </td><td> </td><td class=adiariapren>  a  </td><td class=nomcomptediariapren> " << cursasiento->valor("nomcuenta").ascii() << " </td><td class=codidiariapren> ( " << cursasiento->valor("codigo").ascii() << " ) </td><td class=haverdiariapren> " <<  atof(cursasiento->valor("haber").ascii()) << " </td></tr>\n";
                    }// end if
                    cursasiento->siguienteregistro();
                }// end while
                delete cursasiento;
            }// end for

            delete cursoraux;
            conexionbase->commit();

            if (txtapren) {
                fitxersortidatxt << "____________________________________________________________________________________________________________________________________\n" ;//presentació text format aprenentatge
                fitxersortidatxt.close();
            }// end if

            if (htmlapren) {
                fitxersortidahtml << "<tr><td colspan=\"7\"  class=liniadiariapren>\n_____________________________________________________________________________________________________________\n<hr></td></tr>\n</table>\n</body></html>\n";     //presentació html format aprenentatge
                fitxersortidahtml.close();
            }// end if



            if (txtapren) {
                //presentació txt format aprenentatge
                if ((pid=fork()) < 0) {
                    perror ("Fork failed");
                    exit(errno);
                }// end if
                if (!pid) {
                    error = execvp(confpr->valor(CONF_EDITOR).ascii(),argstxt);
                }// end if
            }// end if

            if (htmlapren) {
                //presentació html format aprenentatge
                if ((pid=fork()) < 0) {
                    perror ("Fork failed");
                    exit(errno);
                }// end if
                if (!pid) {
                    error = execvp(confpr->valor(CONF_NAVEGADOR).ascii(),argshtml);
                }// end if
            }// end if
        }// end if
    }// end if
}// end presentar


/** \brief SLOT que responde a la pulsación del botón de canales.
  * Sirve para que se puedan establecer en las opciones de filtrado los canales.
  * En realidad no hace una nueva intancia de los canales sino que llama a una clase global.
  */
void DiarioPrintView::boton_canales() {
    fprintf(stderr,"Boton canales\n");
    selectcanalview *selcanales = empresaactual->getselcanales();
    selcanales->exec();
    selcanales->firstcanal();
}// end boton_canales


/** \brief SLOT que responde a la pulsación del botón de centros de coste.
  */
void DiarioPrintView::boton_ccostes() {
    fprintf(stderr,"Boton ccostes\n");
    selectccosteview *selccostes = empresaactual->getselccostes();
    selccostes->exec();
    selccostes->firstccoste();
}// end boton_ccostes

/** \brief SLOT que responde a la pulsación del botón de filtrado
  * Muestra la ventana de filtrado.
  */
void DiarioPrintView::s_botonFiltrar() {
    m_filt->exec();
}// end s_botonFiltrar


