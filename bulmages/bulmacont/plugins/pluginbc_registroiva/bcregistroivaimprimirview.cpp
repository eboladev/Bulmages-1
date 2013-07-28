/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras                                    *
 *   tborras@conetxia.com                                                  *
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

#include "bcregistroivaimprimirview.h"
#include "bccompany.h"

#include "bcmodelo300imprimir2ps.h"
#ifndef Q_OS_WIN32
#include <unistd.h>
#endif

#include "blfixed.h"


extern BcModelo300Imprimir2PS *modelo;


///
/**
\param emp
\param parent
**/
BcRegistroIVAImprimirView::BcRegistroIVAImprimirView ( BcCompany *emp, QWidget *parent )
        : QDialog ( parent ), BlMainCompanyPointer ( emp )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    fichero = NULL;
    
}


///
/**
**/
BcRegistroIVAImprimirView::~BcRegistroIVAImprimirView()
{
    BL_FUNC_DEBUG
    
}


/// Se ha pulsado sobre el bot&oacute;n aceptar del formulario.
/**
**/
void BcRegistroIVAImprimirView::accept()
{
    BL_FUNC_DEBUG
    if ( radiotexto->isChecked() )
        presentar ( "txt" );
    if ( radiohtml->isChecked() )
        presentar ( "html" );
    if ( radioPS->isChecked() ) {
        /// Lanza dialogo de generacion de modelo 300
        /// Deberia revisarse el Widget parent de la variable modelo
        /// (listivaview--->modelo, en vez de BcRegistroIVAImprimirView--->modelo)
        /// modelo->exec();
    } // end if
    
}


///
/**
\param finicial1
\param ffinal1
**/
void BcRegistroIVAImprimirView::inicializa1 ( QString finicial1, QString ffinal1 )
{
    BL_FUNC_DEBUG
    fechainicial1->setText ( finicial1 );
    fechafinal1->setText ( ffinal1 );
    
}


/// Se ha pulsado sobre el bot&oacute;n aceptar del formulario.
/**
\param tipus
**/
void BcRegistroIVAImprimirView::presentar ( const char *tipus )
{
    BL_FUNC_DEBUG
#ifndef Q_OS_WIN32
    int txt, html, txtapren, htmlapren;
    QString data;
    QString datahora;

    QString query;

    /// tipo de presentaci&oacute;n.
    txt = ! strcmp ( tipus, "txt" );
    html = ! strcmp ( tipus, "html" );
    txtapren = ! strcmp ( tipus, "txtapren" );
    htmlapren = ! strcmp ( tipus, "htmlapren" );

    /// Cogemos los valores del formulario.
    QString finicial = fechainicial1->text();
    QString ffinal = fechafinal1->text();

    if ( txt | html ) {

        QString archivo = g_confpr->value( CONF_DIR_USER ) + "iva.txt";
        QString archivohtml = g_confpr->value( CONF_DIR_USER ) + "iva.html";


        QFile filehtml;
        filehtml.setFileName ( archivohtml );
        filehtml.open ( QIODevice::WriteOnly );
        QTextStream fitxersortidahtml ( &filehtml );


        QFile filetxt;
        filetxt.setFileName ( archivo );
        filetxt.open ( QIODevice::WriteOnly );
        QTextStream fitxersortidatxt ( &filetxt );


        if ( txt | html ) { /// S&oacute;lo continuamos si hemos podido crear alg&uacute;n archivo.
            int num1;
            BlDbRecordSet *cursorapt;
            mainCompany() ->begin();
            query.sprintf ( "SELECT * FROM registroiva, cuenta, borrador, asiento  where cuenta.idcuenta = borrador.idcuenta AND borrador.idborrador = registroiva.idborrador AND asiento.idasiento = borrador.idasiento AND (cuenta.codigo LIKE '43%%' OR cuenta.codigo LIKE '600%%') AND borrador.fecha >= '%s' AND borrador.fecha <= '%s' ORDER BY asiento.ordenasiento", fechainicial1->text().toLatin1().constData(), fechafinal1->text().toLatin1().constData() );
            fprintf ( stderr, "%s\n", query.toLatin1().constData() );
            cursorapt = mainCompany() ->loadQuery ( query, "mycursor" );
            mainCompany() ->commit();
            /// Calculamos cuantos registros van a crearse y dimensionamos la tabla.
            num1 = cursorapt->numregistros();
            int hoja = 0;

            if ( txt ) {
                /// Presentaci&oacute;n txt normal.
                fitxersortidatxt << "                                        IVA Repercutido \n" ;
                fitxersortidatxt << "Fecha Inicial: " << finicial.toLatin1().constData() <<
                "   Fecha Final: " << ffinal.toLatin1().constData() << endl;
                fitxersortidatxt << "Asiento  Fecha   Cuenta   Descripcion                        Base imponible    Factura \n" ;
                fitxersortidatxt << "__________________________________________________________________________________________________________\n";
            } // end if
            if ( html ) {
                /// Presentaci&oacute;n html normal.
                fitxersortidahtml << "<html>\n";
                fitxersortidahtml << "<head>\n";
                fitxersortidahtml << "  <!DOCTYPE / public \"-//w3c//dtd xhtml 1.0 transitional//en\"\n";
                fitxersortidahtml << "    \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
                fitxersortidahtml << "  <LINK REL=StyleSheet HREF=\"estils.css\" TYPE=\"text/css\" MEDIA=screen>\n";
                fitxersortidahtml << "  <title> IVA Repercutit </title>\n";
                fitxersortidahtml << "</head>\n";
                fitxersortidahtml << "<body>\n";
                fitxersortidahtml << "<table><tr><td colspan=\"10\" class=titoliva> IVA Repercutido <hr></td></tr>\n\n";
                fitxersortidahtml << "<tr><td colspan=\"10\" class periodeiva> Data Inicial: " << finicial.toLatin1().constData() << " -  Data Final: " << ffinal.toLatin1().constData() << "<hr></td></tr>\n\n";
                fitxersortidahtml << "<tr><td class=titolcolumnaiva> Asiento </td><td class=titolcolumnaiva> Data </td><td class=titolcolumnaiva> Cuenta </td><td class=titolcolumnaiva> Descripcion</td><td class=titolcolumnaiva> Base Imponible </td><td class=titolcolumnaiva> % IVA </td><td class=titolcolumnaiva> Quota IVA </td><td class=titolcolumnaiva> Factura </td><td class=titolcolumnaiva> CIF </td><td class=titolcolumnaiva> Cuenta IVA </td></tr>\n";
            } // end if
            while ( !cursorapt->eof() ) {
                datahora = cursorapt->value( "fecha" );
                data = datahora.mid ( 0, 10 );
                int baseimp = cursorapt->value( "baseimp" ).replace ( ".", "" ).toInt();
                int total = baseimp * cursorapt->value( "baseimp" ).replace ( ".", "" ).toInt() / 10000;
                QString numberstr = QString::number ( total );
                numberstr = numberstr.left ( numberstr.length() - 2 ) + "." + numberstr.right ( 2 );
                if ( txt ) {
                    /// Presentaci&oacute;n txt normal.
                    fitxersortidatxt << cursorapt->value( "ordenasiento" ).toLatin1().data() << " " <<
                    data.toLatin1().constData() << " " <<
                    cursorapt->value( "contrapartida" ).toLatin1().constData() << " " << cursorapt->value( "descripcion" ).toLatin1().constData() << " " << cursorapt->value( "baseimp" ).toLatin1().constData() << " " <<
                    cursorapt->value( "iva" ).toLatin1().constData() << " " <<
                    numberstr.toLatin1().constData() << " " <<
                    cursorapt->value( "factura" ).toLatin1().constData() << " " <<
                    cursorapt->value( "cif" ).toLatin1().constData() << " " <<
                    endl;
                } // end if
                if ( html ) {
                    /// Presentaci&oacute;n html normal.
                    fitxersortidahtml << "<tr><td class=assentamentiva>" << cursorapt->value( "idasiento" ).toLatin1().constData() << "</td><td class=dataiva>" << data.toLatin1().constData() << "</td><td class=contrapartidaiva>" << cursorapt->value( "contrapartida" ).toLatin1().constData() << "</td><td class=descripcioiva>" << cursorapt->value( "descripcion" ).toLatin1().constData() << "</td><td class=baseimponibleiva>" << cursorapt->value( "baseimp" ).toLatin1().constData() << "</td><td class=tipusiva>" << cursorapt->value( "iva" ).toLatin1().constData() << "</td><td class=quotaiva>" << numberstr.toLatin1().constData() << "</td><td class=facturaiva>" << cursorapt->value( "factura" ).toLatin1().constData() << "</td><td class=cifiva>" << cursorapt->value( "cif" ).toLatin1().constData() << "</td></tr> \n";
                } // end if
                /// Calculamos la siguiente cuenta registro y finalizamos el bucle.
                cursorapt->nextRecord();
            } // end while
            /// Vaciamos el cursor de la base de datos.
            delete cursorapt;

            /// AHORA PONEMOS EL RESUMEN DEL IVA REPERCUTIDO.
            QString SQLQuery = "SELECT * FROM cuenta, tipoiva LEFT JOIN (SELECT idtipoiva, SUM(baseiva) AS tbaseiva FROM iva iva.idregistroiva IN (SELECT idregistroiva FROM registroiva WHERE ffactura >='" + fechainicial1->text() + "' AND ffactura <='" + fechafinal1->text() + "' ) GROUP BY idtipoiva) AS dd ON dd.idtipoiva=tipoiva.idtipoiva WHERE tipoiva.idcuenta = cuenta.idcuenta AND cuenta.codigo LIKE '477%'";
            mainCompany() ->begin();
            BlDbRecordSet* cur = mainCompany() ->loadQuery ( SQLQuery, "elcursor" );
            mainCompany() ->commit();
            int j = 0;
            BlFixed tivar ( "0" );
            BlFixed tbaseimpr ( "0" );
            while ( !cur->eof() ) {
                BlFixed baseiva ( cur->value( "tbaseiva" ).replace ( ".", "" ).toLatin1().constData() );
                BlFixed porcent ( cur->value( "porcentajetipoiva" ).replace ( ".", "" ).toLatin1().constData() );
                BlFixed baseimp = baseiva * 1000000 / porcent;
                QString numberstr = baseimp.toQString();

                /// Pasamos al formato de representaci&oacute;n espa&ntilde;ol las
                /// cantidades a mostrar.
                QString iva = QString::number ( cursorapt->value( "tbaseiva" ).toDouble(), 'f', 2 );
                QString bi = QString::number ( numberstr.toDouble(), 'f', 2 );

                fitxersortidatxt << cur->value( "nombretipoiva" ).toLatin1().constData() << " IVA: ";
                fitxersortidatxt << iva.toLatin1().constData() << " BI: ";
                fitxersortidatxt << bi.toLatin1().constData() << endl;

                tivar = tivar + baseiva;
                tbaseimpr = tbaseimpr + baseimp;
                cur->nextRecord();
                j++;
            } // end while
            delete cur;

            mainCompany() ->begin();
            query.sprintf ( "SELECT *, (baseimp + iva) AS total, (iva / baseimp * 100)::INTEGER AS cuota FROM registroiva, cuenta, borrador, asiento  WHERE cuenta.idcuenta=borrador.idcuenta AND borrador.idborrador=registroiva.idborrador AND asiento.idasiento=borrador.idasiento AND (cuenta.codigo NOT LIKE '43%%' AND cuenta.codigo NOT LIKE '600%%') AND borrador.fecha >= '%s' AND borrador.fecha <= '%s' ORDER BY cuota, borrador.fecha", fechainicial1->text().toLatin1().constData(), fechafinal1->text().toLatin1().constData() );
            fprintf ( stderr, "%s\n", query.toLatin1().constData() );
            cursorapt = mainCompany() ->loadQuery ( query, "mycursor" );
            mainCompany() ->commit();
            /// Calculamos cuantos registros van a crearse y dimensionamos la tabla.
            num1 = cursorapt->numregistros();
            hoja = 0;
            if ( txt ) {
                /// Presentaci&oacute;n txt normal.
                fitxersortidatxt << "                                        IVA Soportado \n" ;
                fitxersortidatxt << "Asiento   Fecha     Cuenta   Descripcion                        Subtotal Cuota   IVA       Total   Factura    CIF\n" ;
                fitxersortidatxt << "_______________________________________________________________________________________________________________________\n";
            }
            if ( html ) {
                /// Presentaci&oacute;n html normal.
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
            while ( !cursorapt->eof() ) {
                datahora = cursorapt->value( "fecha" );
                data = datahora.mid ( 0, 10 );
                /// Pasamos al formato de representacion espanyol las cantidades a mostrar.
                QString bi = QString::number ( cursorapt->value( "baseimp" ).toDouble(), 'f', 2 );
                QString iva = QString::number ( cursorapt->value( "iva" ).toDouble(), 'f', 2 );
                QString total = QString::number ( cursorapt->value( "total" ).toDouble(), 'f', 2 );
                /// Hacemos el c&aacute;lculo de la cuota (as&iacute; nos sirve para ver
                /// si se c&aacute;lculo el % correcto).
                // int calculo = round(cursorapt->value("iva").toDouble()/cursorapt->value("baseimp").toDouble()*100);
                // QString cuota = QString::number(calculo);
                QString cuota = QString::number ( cursorapt->value( "cuota" ).toInt() );
                /// Acumulamos los totales para al final poder escribirlos.
                if ( txt ) {
                    /// Presentaci&oacute;n txt normal.
                    fitxersortidatxt << cursorapt->value( "ordenasiento" ).toLatin1().constData() <<  data.toLatin1().constData() <<  cursorapt->value( "codigo" ).toLatin1().constData() <<  cursorapt->value( "descripcion" ).left ( 30 ).toLatin1().constData() << " " <<   bi.toLatin1().constData() << " " <<  cuota.toLatin1().constData() << "%" <<  iva.toLatin1().constData() <<  total.toLatin1().constData() <<  "  " <<  cursorapt->value( "factura" ).right ( 8 ).toLatin1().constData() <<   cursorapt->value( "cif" ).toLatin1().constData() << endl;
                }
                if ( html ) {
                    /// Presentaci&oacute;n html normal.
                    fitxersortidahtml << "<tr><td class=assentamentiva>" << cursorapt->value( "ordenasiento" ).toLatin1().constData() << "</td><td class=dataiva>" << data.toLatin1().constData() << "</td><td class=contrapartidaiva>" << cursorapt->value( "contrapartida" ).toLatin1().constData() << "</td><td class=descripcioiva>" << cursorapt->value( "descripcion" ).toLatin1().constData() << "</td><td class=baseimponibleiva>" << cursorapt->value( "baseimp" ).toLatin1().constData() << "</td><td class=tipusiva>" << cursorapt->value( "iva" ).toLatin1().constData() << "</td><td class=quotaiva>" << total.toLatin1().constData() << "</td><td class=facturaiva>" << cursorapt->value( "factura" ).toLatin1().constData() << "</td><td class=cifiva>" << cursorapt->value( "cif" ).toLatin1().constData() << "</td></tr> \n";
                }
                /// Calculamos la siguiente cuenta registro y finalizamos el bucle.
                cursorapt->nextRecord();
            } // end while
            /// Vaciamos el cursor de la base de datos.
            delete cursorapt;

            /// Ahora ponemos el resumen del IVA soportado.
            SQLQuery = "SELECT * FROM cuenta, tipoiva LEFT JOIN (SELECT idtipoiva, SUM(baseiva) AS tbaseiva FROM iva  WHERE iva.idregistroiva IN (SELECT idregistroiva FROM registroiva WHERE ffactura >='" + fechainicial1->text() + "' AND ffactura <='" + fechafinal1->text() + "' ) GROUP BY idtipoiva) AS dd ON dd.idtipoiva=tipoiva.idtipoiva WHERE tipoiva.idcuenta = cuenta.idcuenta AND cuenta.codigo LIKE '472%'";
            mainCompany() ->begin();
            cur = mainCompany() ->loadQuery ( SQLQuery, "elcursor" );
            mainCompany() ->commit();
            j = 0;
            BlFixed tivas ( "0" );
            BlFixed tbaseimps ( "0" );
            while ( !cur->eof() ) {
                BlFixed baseiva ( cur->value( "tbaseiva" ).toLatin1().constData() );
                BlFixed porcent ( cur->value( "porcentajetipoiva" ).toLatin1().constData() );
                BlFixed ivacalculado = baseiva * porcent / 100;
                QString numberstr = ivacalculado.toQString();

                /// Pasamos al formato de representaci&oacute;n espa&ntilde;ol las
                /// cantidades a mostrar.
                QString bi = QString::number ( cur->value( "tbaseiva" ).toDouble(), 'f', 2 );
                QString iva = QString::number ( numberstr.toDouble(), 'f', 2 );

                if ( j == 0 ) {
                    fitxersortidatxt << endl;
                } // end if
                fitxersortidatxt << cur->value( "nombretipoiva" ).toLatin1().constData() << " BI: ";
                fitxersortidatxt <<  bi.toLatin1().constData() << " IVA: ";
                fitxersortidatxt << iva.toLatin1().constData() << endl;

                tivas = tivas + ivacalculado;
                tbaseimps = tbaseimps + baseiva;
                cur->nextRecord();
                j++;
            } // end while
            delete cur;

            /// Ahora cerramos los ficheros.
            if ( txt ) {
                filetxt.close();
                QString comando = g_confpr->value( CONF_EDITOR ) + " " + archivo + " &";
                system ( comando.toLatin1() );
            } // end if
            if ( html ) {
                fitxersortidahtml << "\n</table>\n</body>\n</html>\n";
                filehtml.close();
                /// Presentaci&oacute;n html normal.
                blWebBrowser(archivohtml);
            } // end if
        }
    }

#endif
    
}

