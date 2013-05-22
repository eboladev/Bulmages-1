/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2003 by Antoni Mirabete i Teres                         *
 *   amirabet@biada.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <unistd.h>
#include <QCheckBox>
#include <QTextStream>

#include "bcdiarioprintview.h"
#include "bfcompany.h"
#include "pluginbf_diario.h"
#include "bcdiarioview.h"



///
/**
\param emp
\param parent
**/
BcDiarioPrintView::BcDiarioPrintView ( BfCompany  *emp, QWidget *parent )
        : QDialog ( parent ), BlMainCompanyPointer ( emp )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    numDigitos = g_confpr->value(CONF_CONT_NUMDIGITOSEMPRESA).toInt();
    
}


///
/**
**/
BcDiarioPrintView::~BcDiarioPrintView()
{
    BL_FUNC_DEBUG
    
}


/// Esta funci&oacute;n monta la consulta que se va a realizar contra la base de datos.
/// La consulta es de bastante detalle y por eso es conveniente dedicar una funci&oacute;n
/// a realizarla. Adem&aacute;s dicha consulta puede ser invocada desde distintos sitios.
/**
\return
**/
QString BcDiarioPrintView::montaQuery()
{
    BL_FUNC_DEBUG
    BcDiarioView *diario = g_diario;
    QString query;
    QString fecha;
    QString fechaasiento;
    QString descripcion;
    QString concepto;
    QString codigocuenta;
    QString nomcuenta;
    QString cadaux;
    QString finicial = diario->mui_fechainicial->text();
    QString ffinal = diario->mui_fechafinal->text();
    QString cad;

    QString ccostes = "";
    QString ccanales = "";

/*
    /// Consideraciones para centros de coste y canales.
    BcCanalSeleccionarView *scanal = ( ( BfCompany * ) mainCompany() ) ->getselcanales();
    BcCentroCosteSeleccionarView *scoste = ( ( BfCompany * ) mainCompany() ) ->getselccostes();
    QString ccostes = scoste->cadcoste();
    if ( ccostes != "" ) {
        ccostes.sprintf ( " AND idc_coste IN (%s) ", ccostes.toAscii().constData() );
    } // end if
    QString ccanales = scanal->cadCanal();
    if ( ccanales != "" ) {
        ccanales.sprintf ( " AND idcanal IN (%s) ", ccanales.toAscii().constData() );
    } // end if
*/
    QString tabla;
    if ( diario->mui_asAbiertos->isChecked() ) {
        tabla = "borrador";
    } else {
        tabla = "apunte";
    } // end if
    query = "SELECT asiento.ordenasiento, " + tabla + ".contrapartida, " + tabla + ".fecha, asiento.fecha AS fechaasiento,cuenta.tipocuenta, cuenta.descripcion, " + tabla + ".conceptocontable," + tabla + ".descripcion AS descapunte, to_char(" + tabla + ".debe,'MI999G999G990D99') AS debe, to_char(" + tabla + ".haber,'MI999G999G990D99') AS haber, cuenta.idcuenta, asiento.idasiento, " + tabla + ".idc_coste, " + tabla + ".idcanal, cuenta.codigo AS codigocuenta FROM asiento, " + tabla + ", cuenta WHERE asiento.idasiento=" + tabla + ".idasiento AND " + tabla + ".idcuenta = cuenta.idcuenta AND " + tabla + ".fecha >= '" + finicial + "' AND " + tabla + ".fecha <= '" + ffinal + "' " + ccostes + " " + ccanales + " ORDER BY asiento.ordenasiento, " + tabla + ".orden" + tabla;
    
    return query;
}


/// Se ha pulsado sobre el bot&oacute;n aceptar del formulario.
/**
\parent
**/
void BcDiarioPrintView::accept()
{
    BL_FUNC_DEBUG
    /// Versi&oacute;n por si s&oacute;lo permitimos elegir una opci&oacute;n.
    if ( radiotexto->isChecked() ) {
        if ( radionormal->isChecked() ) {
            presentar ( "txt" );
        } else if ( radioaprendizaje->isChecked() ) {
            presentar ( "txtapren" );
        } // end if
    } else if ( radiohtml->isChecked() ) {
        if ( radionormal->isChecked() ) {
            presentar ( "html" );
        } else if ( radioaprendizaje->isChecked() ) {
            presentar ( "htmlapren" );
        } // end if
    } // end if
    
}


///
/**
\param tipus
**/
void BcDiarioPrintView::presentar ( const char *tipus )
{
    BL_FUNC_DEBUG
    BcDiarioView *diario = g_diario;
    int txt, html, txtapren, htmlapren;
    float debe, haber;
    int idcuenta;
    int idasiento;
    QString data;
    QString fecha;
    QString fechaasiento;
    QString descripcion;
    QString concepto;
    QString codigocuenta;
    QString cad;
    BlDbRecordSet *cursoraux;

    /// Tipo de presentaci&oacute;n.
    txt = ! strcmp ( tipus, "txt" );
    html = ! strcmp ( tipus, "html" );
    txtapren = ! strcmp ( tipus, "txtapren" );
    htmlapren = ! strcmp ( tipus, "htmlapren" );

    /// Cogemos los valores del formulario.
    QString finicial = diario->mui_fechainicial->text();
    QString ffinal = diario->mui_fechafinal->text();

    if ( txt | html ) {
        /// Creamos los archivos de salida.

        QString archivo = g_confpr->value( CONF_DIR_USER ) + "diario.txt";
        QString archivohtml = g_confpr->value( CONF_DIR_USER ) + "diario.html";


        QFile filehtml;
        filehtml.setFileName ( archivohtml );
        filehtml.open ( QIODevice::WriteOnly );
        QTextStream fitxersortidahtml ( &filehtml );


        QFile filetxt;
        filetxt.setFileName ( archivo );
        filetxt.open ( QIODevice::WriteOnly );
        QTextStream fitxersortidatxt ( &filetxt );



        if ( txt | html ) {
            /// S&oacute;lo continuamos se hemos podido crear alg&uacute;n archivo.
            if ( txt ) {
                /// Presentaci&oacute;n txt normal.
                fitxersortidatxt << "                                        LLIBRE DIARI \n" ;
                fitxersortidatxt << "Data Inicial: " << finicial.toAscii().constData() << "   Data Final: " << ffinal.toAscii().constData() << endl;
                fitxersortidatxt << "ASIENTO   FECHA    SUBCUENTA   DESCRIPCION                                   DEBE     HABER \n" ;
                fitxersortidatxt << "_______________________________________________________________________________________________\n";
            } // end if
            if ( html ) {
                /// Presentaci&oacute;n html normal.
                fitxersortidahtml << "<html>\n";
                fitxersortidahtml << "<head>\n";
                fitxersortidahtml << "  <!DOCTYPE / public \"-//w3c//dtd xhtml 1.0 transitional//en\"\n";
                fitxersortidahtml << "    \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
                fitxersortidahtml << "  <LINK REL=StyleSheet HREF=\"" << g_confpr->value( CONF_PLANTILLA ).toAscii().constData() << "\" TYPE=\"text/css\" MEDIA=screen>\n";
                fitxersortidahtml << "  <title> Llibre Diari </title>\n";
                fitxersortidahtml << "</head>\n";
                fitxersortidahtml << "<body>\n";
                fitxersortidahtml << "<table><tr><td colspan=\"6\" class=titoldiari> Llibre diari <hr></td></tr>\n\n";
                fitxersortidahtml << "<tr><td colspan=\"6\" class=periodediari> Data Inicial: " << finicial.toAscii().constData() << " -  Data Final: " << ffinal.toAscii().constData() << "<hr></td></tr>\n\n";
                fitxersortidahtml << "<tr><td class=titolcolumnadiari>Assentament</td><td class=titolcolumnadiari>Data</td><td class=titolcolumnadiari>Subcompte</td><td class=titolcolumnadiari>Descripcio</td><td class=titolcolumnadiari>Deure</td><td class=titolcolumnadiari>Haver</td></tr>\n";
            } // end if
            QString query = montaQuery();
            mainCompany() ->begin();
            cursoraux = mainCompany() ->loadQuery ( query, "elquerydb" );
            mainCompany() ->commit();

            for ( ; !cursoraux->eof(); cursoraux->nextRecord() ) {
                fechaasiento = cursoraux->value( "fechaasiento" ).toAscii().constData();
                idasiento = atoi ( cursoraux->value( "ordenasiento" ).toAscii() );
                fecha = cursoraux->value( "fecha" ).toAscii().constData();
                descripcion = cursoraux->value( "descripcion" ).toAscii().constData();
                concepto = cursoraux->value( "conceptocontable" ).toAscii().constData();
                debe = atof ( cursoraux->value( "debe" ).toAscii() );
                haber = atof ( cursoraux->value( "haber" ).toAscii() );
                idcuenta = atoi ( cursoraux->value( "idcuenta" ).toAscii() );
                codigocuenta = cursoraux->value( "codigocuenta" ).toAscii().constData();
                data = fecha.left ( 10 );
                if ( txt ) {
                    /// Presentaci&oacute;n txt normal.
                    fitxersortidatxt << idasiento << "  " << data << " " << codigocuenta <<  "  " << descripcion <<  " " << debe << " " << haber << endl;
                } // end if
                if ( html ) {
                    /// Presentaci&oacute; html normal.
                    fitxersortidahtml << "<tr><td class=assentamentdiari>" << idasiento << "</td><td class=datadiari>" << data << "</td><td class=codicomptediari>" << codigocuenta << "</td><td class=descripciodiari>" <<  descripcion << "</td><td class=dosdecimals>" << debe << "</td><td class=dosdecimals>" << haber << "</td></tr>\n";
                } // end if
            } // end for
            delete cursoraux;
            if ( html ) {
                /// Presentaci&oacute;n html normal.
                fitxersortidahtml << "</table>\n<hr>\n</body>\n</html>\n";
                filehtml.close();
            } // end if
            if ( txt ) {
                fitxersortidatxt << "_______________________________________________________________________________________________\n";
                filetxt.close();
            } // end if
        } // end if
        if ( txt ) {
            /// Presentaci&oacute; txt normal.
            QString cadaux = g_confpr->value( CONF_EDITOR ) + " diairo.txt";
            system ( cadaux.toAscii() );
        }
        if ( html ) {
            /// Presentaci&oacute; html normal.
            blWebBrowser("diario.html");
        } // end if
    } // end if


    if ( txtapren | htmlapren ) {

        QString archivo = g_confpr->value( CONF_DIR_USER ) + "diario.txt";
        QString archivohtml = g_confpr->value( CONF_DIR_USER ) + "diario.html";


        QFile filehtml;
        filehtml.setFileName ( archivohtml );
        filehtml.open ( QIODevice::WriteOnly );
        QTextStream fitxersortidahtml ( &filehtml );


        QFile filetxt;
        filetxt.setFileName ( archivo );
        filetxt.open ( QIODevice::WriteOnly );
        QTextStream fitxersortidatxt ( &filetxt );


        if ( txtapren | htmlapren ) {
            /// S&oacute;lo continuamos si hemos podido crear alg&uacute;n archivo.
            if ( txtapren ) {
                /// Presentaci&oacute;n txt formato aprendizaje.
                fitxersortidatxt << "                                                      LLIBRE DIARI \n" ;
                fitxersortidatxt << "Data Inicial: " << finicial.toAscii().constData() << "   Data Final: " << ffinal.toAscii().constData() << endl;
                fitxersortidatxt << "___________________________________________________________________________________________________________________________________\n";
            } // end if
            if ( htmlapren ) {
                /// Presentaci&oacute;n html formato aprendizaje.
                fitxersortidahtml << "<html>\n";
                fitxersortidahtml << "<head>\n";
                fitxersortidahtml << "  <!DOCTYPE / public \"-//w3c//dtd xhtml 1.0 transitional//en\"\n";
                fitxersortidahtml << "    \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
                fitxersortidahtml << "  <LINK REL=StyleSheet HREF=\"estils.css\" TYPE=\"text/css\" MEDIA=screen>\n";
                fitxersortidahtml << "  <title> Llibre Diari </title>\n";
                fitxersortidahtml << "</head>\n";
                fitxersortidahtml << "<body>\n";
                fitxersortidahtml << "<table><tr><td colspan=\"7\" class=titoldiariapren> Llibre diari <hr></td></tr>\n\n";
                fitxersortidahtml << "<tr><td colspan=\"7\" class=periodediariapren> Data Inicial: " << finicial.toAscii().constData() << " -  Data Final: " << ffinal.toAscii().constData() << "<hr></td></tr>\n\n";
            } // end if
            mainCompany() ->begin();
            cursoraux = mainCompany() -> loadQuery ( "SELECT * FROM asiento WHERE fecha >= '" + finicial + "' AND fecha <= '" + ffinal + "' ORDER BY fecha" );

            for ( ; !cursoraux->eof(); cursoraux->nextRecord() ) {
                int idasiento = atoi ( cursoraux->value( "idasiento" ).toAscii() );
                if ( txtapren ) {
                    /// Presentaci&oacute;n txt formato aprendizaje.
                    fitxersortidatxt << "_________________________________________________________   " << idasiento << "   ___________________________________________________________________\n";
                } // end if
                if ( htmlapren ) {
                    /// Presentaci&oacute;n html formato aprendizaje.
                    fitxersortidahtml << "\n<tr><td colspan=\"7\" class=liniadiariapren>____________________________________________________   " << idasiento << "   _____________________________________________________</td></tr>\n" ;
                } // end if
                char consulta[400];
                char codicompte[20];
                sprintf ( consulta, "SELECT cuenta.codigo, cuenta.descripcion AS nomcuenta, apunte.debe AS debe, apunte.haber AS haber FROM apunte, cuenta WHERE apunte.idasiento=%d AND apunte.haber=0 AND cuenta.idcuenta=apunte.idcuenta", idasiento );
                mainCompany() ->begin();
                BlDbRecordSet *cursasiento = mainCompany() ->loadQuery ( consulta, "asiento" );
                mainCompany() ->commit();

                while ( !cursasiento->eof() ) {
                    if ( txtapren ) {
                        /// Presentaci&oacute;n txt formato aprendizaje.
                        sprintf ( codicompte, "( %s )", cursasiento->value( "codigo" ).toAscii().constData() );
                        fitxersortidatxt <<  atof ( cursasiento->value( "debe" ).toAscii() ) << "  " <<  codicompte << "  " << cursasiento->value( "nomcuenta" ).toAscii().constData() << endl;
                    } // end if
                    if ( htmlapren ) {
                        /// Presentaci&oacute;n html formato aprendizaje.
                        fitxersortidahtml << " <tr><td class=deurediariapren> " << atof ( cursasiento->value( "debe" ).toAscii() ) << " </td><td class=codidiariapren> ( " << cursasiento->value( "codigo" ).toAscii().constData() << " ) </td><td class=nomcomptediariapren> " << cursasiento->value( "nomcuenta" ).toAscii().constData() << " </td><td> </td><td> </td><td> </td><td> </td></tr>\n";
                    } // end if
                    cursasiento->nextRecord();
                } // end while

                delete cursasiento;
                sprintf ( consulta, "SELECT cuenta.codigo, cuenta.descripcion AS nomcuenta, apunte.debe AS debe, apunte.haber AS haber FROM apunte, cuenta WHERE apunte.idasiento = %d AND apunte.debe = 0 AND cuenta.idcuenta = apunte.idcuenta", idasiento );
                mainCompany() ->begin();
                cursasiento = mainCompany() ->loadQuery ( consulta, "asiento" );
                mainCompany() ->commit();

                while ( !cursasiento->eof() ) {
                    if ( txtapren ) {
                        /// Presentaci&oacute;n txt formato aprendizaje.
                        sprintf ( codicompte, "( %s )", cursasiento->value( "codigo" ).toAscii().constData() );
                        fitxersortidatxt << "                                                                 a  " <<  cursasiento->value( "nomcuenta" ).toAscii().constData() << "  " << codicompte << "  " <<  atof ( cursasiento->value( "haber" ).toAscii() ) << endl;
                    } // end if
                    if ( htmlapren ) {
                        /// Presentaci&oacute;n html formato aprendizaje.
                        fitxersortidahtml << " <tr><td> </td><td> </td><td> </td><td class=adiariapren>  a  </td><td class=nomcomptediariapren> " << cursasiento->value( "nomcuenta" ).toAscii().constData() << " </td><td class=codidiariapren> ( " << cursasiento->value( "codigo" ).toAscii().constData() << " ) </td><td class=haverdiariapren> " <<  atof ( cursasiento->value( "haber" ).toAscii() ) << " </td></tr>\n";
                    } // end if
                    cursasiento->nextRecord();
                } // end while
                delete cursasiento;
            } // end for

            delete cursoraux;
            mainCompany() ->commit();

            if ( txtapren ) {
                fitxersortidatxt << "____________________________________________________________________________________________________________________________________\n" ;//presentacio text format aprenentatge
                filetxt.close();
            } // end if
            if ( htmlapren ) {
                fitxersortidahtml << "<tr><td colspan=\"7\"  class=liniadiariapren>\n_____________________________________________________________________________________________________________\n<hr></td></tr>\n</table>\n</body></html>\n";     //presentacio html format aprenentatge
                filehtml.close();
            } // end if
            if ( txtapren ) {
                /// Presentaci&oacute;n txt formato aprendizaje.
                QString cadaux = g_confpr->value( CONF_EDITOR ) + " diario.txt";
                system ( cadaux.toAscii() );
            } // end if
            if ( htmlapren ) {
                /// Presentaci&oacute;n html formato aprendizaje.
                QString cadaux = g_confpr->value( CONF_NAVEGADOR ) + " diario.txt";
                system ( cadaux.toAscii() );
            } // end if
        } // end if
    } // end if
    
}

