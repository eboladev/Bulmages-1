/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2003 by Antoni Mirabete i Teres                         *
 *   amirabet@biada.org                                                    *
 *   Copyright (C) 2012 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
 *                                                                         *
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

#include <QWidget>
#include <QRadioButton>
#include <QLineEdit>
#include <QComboBox>
#include <QToolButton>
#include <QCheckBox>
#include <QFile>
#include <QTextStream>

#include <bldatesearch.h>

#include "balanceprintview.h"
#include "bcplancontablelistview.h"
#include "bccompany.h"
#include "bcplancontablearbol.h"
#include "bccanalseleccionarview.h"
#include "bccentrocosteseleccionarview.h"

#ifndef Q_OS_WIN32
#include <unistd.h>
#endif


///
/**
\param emp
**/
BalancePrintView::BalancePrintView ( BcCompany *emp )
        : QDialog ( 0 ), BlMainCompanyPointer ( emp )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    mui_cuentaInicial->setMainCompany ( emp );
    mui_cuentaInicial->setLabel ( _ ( "Cuenta inicial:" ) );
    mui_cuentaInicial->setTableName ( "cuenta" );
    mui_cuentaInicial->setFieldId("idcuenta");
    mui_cuentaInicial->m_valores["descripcion"] = "";
    mui_cuentaInicial->m_valores["codigo"] = "";
    mui_cuentaInicial->hideLabel();

    mui_cuentaFinal->setMainCompany ( emp );
    mui_cuentaFinal->setLabel ( _ ( "Cuenta final:" ) );
    mui_cuentaFinal->setTableName ( "cuenta" );
    mui_cuentaFinal->setFieldId("idcuenta");
    mui_cuentaFinal->m_valores["descripcion"] = "";
    mui_cuentaFinal->m_valores["codigo"] = "";
    mui_cuentaFinal->hideLabel();

    /// Buscamos los diferentes niveles que existen seg&uacute;n existan en la tabla
    /// de cuentas.
    mainCompany() ->begin();
    QString query = "SELECT nivel(codigo) FROM cuenta GROUP BY nivel ORDER BY nivel";
    BlDbRecordSet *niveles = mainCompany() ->loadQuery ( query, "Niveles" );
    int i = 0;
    while ( !niveles->eof() ) {
        /// Inicializamos la tabla de nivel.
        mui_nivel->insertItem ( i, niveles->value( "nivel" ) );
        niveles->nextRecord();
        i++;
    } // end while
    mainCompany() ->commit();
    delete niveles;
    
}


///
/**
**/
BalancePrintView::~BalancePrintView()
{
    BL_FUNC_DEBUG
}


/// Inicializa la clase para que tenga par&aacute;metros por defecto (no es obligatorio).
/** cuentaInicial = Indica el c&oacute;digo inicial para mostrar el balance.
    cuentaFinal = Indica el c&oacute;digo de cuenta para no mostrar m&aacute;s balances.
    fechaInicial = Indica la fecha a partir de la cual comenzar los saldos.
    fechaFinal = Indica la fecha para terminar de contar los saldos.
    arbol = Indica si hay que representar el balance en forma de arbol o no. */
/**
**/
void BalancePrintView::inicializa ( QString cuentaInicial, QString cuentaFinal, QString fechaInicial, QString fechaFinal, bool arbol )
{
    BL_FUNC_DEBUG
    mui_fechaInicial->setText ( fechaInicial );
    mui_fechaFinal->setText ( fechaFinal );
    mui_cuentaInicial->setText ( cuentaInicial );
    mui_cuentaFinal->setText ( cuentaFinal );
    mui_nivelSuperior->setChecked ( arbol );
}


/// Se ha pulsado sobre el bot&oacute;n aceptar del formulario.8
/**
**/
void BalancePrintView::on_mui_imprimir_clicked()
{
    BL_FUNC_DEBUG
    if ( mui_textoPlano->isChecked() )
        presentar ( "txt" );
    if ( mui_html->isChecked() )
        presentar ( "html" );
}


/// Se ha pulsado sobre el bot&oacute;n aceptar del formulario.
/**
\param tipus
**/
void BalancePrintView::presentar ( const char* tipus )
{
    BL_FUNC_DEBUG
#ifndef Q_OS_WIN32
    int txt, html, txtapren, htmlapren;
    double tsaldoant, tdebe, thaber, tsaldo, debeej, haberej, saldoej;
    QString query;

    /// tipos de presentaciones.
    txt = !strcmp ( tipus, "txt" );
    html = !strcmp ( tipus, "html" );
    txtapren = !strcmp ( tipus, "txtapren" );
    htmlapren = !strcmp ( tipus, "htmlapren" );

    /// Cogemos los valores del formulario.
    QString finicial = mui_fechaInicial->text();
    QString ffinal = mui_fechaFinal->text();
    QString cinicial = mui_cuentaInicial->fieldValue("codigo");
    QString cfinal = mui_cuentaFinal->fieldValue("codigo");
    int nivel = mui_nivel->currentText().toInt();
    bool superiores = mui_nivelSuperior->isChecked();

    if ( txt | html ) {
        QString archivo = g_confpr->value( CONF_DIR_USER ) + "balance.txt";
        QString archivohtml = g_confpr->value( CONF_DIR_USER ) + "balance.html";

        QFile filehtml;
        filehtml.setFileName ( archivohtml );
        filehtml.open ( QIODevice::WriteOnly );
        QTextStream fitxersortidahtml ( &filehtml );


        QFile filetxt;
        filetxt.setFileName ( archivo );
        filetxt.open ( QIODevice::WriteOnly );
        QTextStream fitxersortidatxt ( &filetxt );

        /// S&oacute;lo continuamos si hemos podido crear alg&uacute;n archivo.
        if ( txt | html ) {
            /// Vamos a crear un &aacute;rbol en la mem&oacute;ria din&aacute;mica con
            /// los distintos niveles de cuentas.
            /// Primero, averiguaremos la cantidad de ramas iniciales (tantos como
            /// n&uacute;mero de cuentas de nivel 2) y las vamos creando.
            mainCompany() ->begin();
            query.sprintf ( "SELECT *, nivel(codigo) AS nivel FROM cuenta ORDER BY codigo" );
            BlDbRecordSet *ramas;
            ramas = mainCompany() ->loadQuery ( query, "Ramas" );
            BcPlanContableArbol *arbol;
            arbol = new BcPlanContableArbol;
            while ( !ramas->eof() ) {
                if ( atoi ( ramas->value( "nivel" ).toAscii().constData() ) == 2 ) { /// Cuenta ra&iacute;z.
                    arbol->nuevaRama ( ramas );
                } // end if
                ramas->nextRecord();
            } // end while
            arbol->inicializa ( ramas );
            delete ramas;

            /// Ahora despu&eacute;s, usaremos el &aacute;rbol para poner los datos a
            /// cada hoja (cuenta) seg&uacute;n los per&iacute;odos que necesitemos acotar.
            /// Pero antes, preparamos las plantillas segun el tipo de salida seleccionado.

            if ( txt ) {
                /// Presentaci&oacute;n txt normal.
                fitxersortidatxt << "Balance de sumas y saldos. \n\n" ;
                fitxersortidatxt << "Fecha inicial: " << finicial.toAscii().constData() << "   Fecha final: " << ffinal.toAscii().constData() << endl << endl;
                fitxersortidatxt << "Cuenta            Denominacion                        Saldo ant.        Debe        Haber        Saldo     Debe ej.    Haber ej.    Saldo ej.\n" ;
                fitxersortidatxt << "_____________________________________________________________________________________________________________________________________________\n";
            } // end if

            if ( html ) {
                /// Presentaci&oacute;n html normal.
                fitxersortidahtml << "<html>\n";
                fitxersortidahtml << "<head>\n";
                fitxersortidahtml << "  <!DOCTYPE / public \"-//w3c//dtd xhtml 1.0 transitional//en\"\n";
                fitxersortidahtml << "    \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
                fitxersortidahtml << "  <LINK REL=StyleSheet HREF=\"estils.css\" TYPE=\"text/css\" MEDIA=screen>\n";
                fitxersortidahtml << "  <title> Balance </title>\n";
                fitxersortidahtml << "</head>\n";
                fitxersortidahtml << "<body>\n";
                fitxersortidahtml << "<table><tr><td colspan=\"9\" class=titolbalanc> Balance <hr></td></tr>\n\n";
                fitxersortidahtml << "<tr><td colspan=\"9\" class=periodebalanc> Data Inicial: " << finicial.toAscii().constData() << " -  Data Final: " << ffinal.toAscii().constData() << "<hr></td></tr>\n\n";
                fitxersortidahtml << "<tr><td class=titolcolumnabalanc>lcuenta</td><td class=titolcolumnabalanc> ldenominacion</td><td class=titolcolumnabalanc>lsaldoant</td><td class=titolcolumnabalanc>ldebe</td><td class=titolcolumnabalanc>lhaber</td><td class=titolcolumnabalanc>lsaldo</td><td class=titolcolumnabalanc> ldebeej  </td><td class=titolcolumnabalanc> lhaberej </td><td class=titolcolumnabalanc> lsaldoej </td></tr>\n";
            } // end if

            /// Vamos a recopilar todos los apuntes agrupados por cuenta para poder
            /// establecer as&iacute; los valores de cada cuenta.
            query.sprintf ( "SELECT cuenta.idcuenta, numapuntes, cuenta.codigo, saldoant, debe, haber, saldo, debeej, haberej, saldoej FROM (SELECT idcuenta, codigo FROM cuenta) AS cuenta NATURAL JOIN (SELECT idcuenta, count(idcuenta) AS numapuntes,sum(debe) AS debeej, sum(haber) AS haberej, (sum(debe)-sum(haber)) AS saldoej FROM apunte WHERE EXTRACT(year FROM fecha) = EXTRACT(year FROM timestamp '%s') GROUP BY idcuenta) AS ejercicio LEFT OUTER JOIN (SELECT idcuenta,sum(debe) AS debe, sum(haber) AS haber, (sum(debe)-sum(haber)) AS saldo FROM apunte WHERE fecha >= '%s' AND fecha <= '%s' GROUP BY idcuenta) AS periodo ON periodo.idcuenta=ejercicio.idcuenta LEFT OUTER JOIN (SELECT idcuenta, (sum(debe)-sum(haber)) AS saldoant FROM apunte WHERE fecha < '%s' GROUP BY idcuenta) AS anterior ON cuenta.idcuenta=anterior.idcuenta ORDER BY codigo", finicial.toAscii().constData(), finicial.toAscii().constData(), ffinal.toAscii().constData(), finicial.toAscii().constData() );
            BlDbRecordSet *cuentas;
            cuentas = mainCompany() ->loadQuery ( query, "Periodo" );
            /// Para cada cuenta con sus apuntes hechos hay que actualizar hojas
            /// del &aacute;rbol.
            while ( !cuentas->eof() ) {
                arbol->actualizaHojas ( cuentas );
                cuentas->nextRecord();
            } // end while

            tsaldoant = tdebe = thaber = tsaldo = debeej = haberej = saldoej = 0;
            /// Ahora imprimimos los valores.
            arbol->inicia();
            while ( arbol->deshoja ( nivel, superiores ) ) {
                QString lcuenta = arbol->hojaActual ( "codigo" );
                QString ldenominacion = arbol->hojaActual ( "descripcion" );
                QString lsaldoant = arbol->hojaActual ( "saldoant" );
                QString ldebe = arbol->hojaActual ( "debe" );
                QString lhaber = arbol->hojaActual ( "haber" );
                QString lsaldo = arbol->hojaActual ( "saldo" );
                QString ldebeej = arbol->hojaActual ( "debeej" );
                QString lhaberej = arbol->hojaActual ( "haberej" );
                QString lsaldoej = arbol->hojaActual ( "saldoej" );

                /// Acumulamos los totales para al final poder escribirlos.
                tsaldoant += lsaldoant.toDouble();
                tsaldo += lsaldo.toDouble();
                tdebe += ldebe.toDouble();
                thaber += lhaber.toDouble();
                debeej += ldebeej.toDouble();
                haberej += lhaberej.toDouble();
                saldoej += lsaldoej.toDouble();

                /// Las variables de las filas en formato espa&ntilde;ol.
                lsaldoant = QString::number ( lsaldoant.toDouble(), 'f', 2 );
                ldebe = QString::number ( ldebe.toDouble(), 'f', 2 );
                lhaber = QString::number ( lhaber.toDouble(), 'f', 2 );
                lsaldo = QString::number ( lsaldo.toDouble(), 'f', 2 );
                ldebeej = QString::number ( ldebeej.toDouble(), 'f', 2 );
                lhaberej = QString::number ( lhaberej.toDouble(), 'f', 2 );
                lsaldoej = QString::number ( lsaldoej.toDouble(), 'f', 2 );

                /// Imprimimos l&iacute;nea seg&uacute;n formato.
                /// Presentaci&oacute;n en txt normal.
                if ( txt ) {
                    fitxersortidatxt << lcuenta.leftJustified(17, ' ').toAscii().constData() <<  ldenominacion.leftJustified(26, ' ', true).toAscii().constData() << " " << lsaldoant.rightJustified(19, ' ').toAscii().constData() << " " <<  ldebe.rightJustified(12, ' ').toAscii().constData() << " " <<  lhaber.rightJustified(12, ' ').toAscii().constData() << " " << lsaldo.rightJustified(12, ' ').toAscii().constData() << " " << ldebeej.rightJustified(12, ' ').toAscii().constData() << " " << lhaberej.rightJustified(12, ' ').toAscii().constData() << " " << lsaldoej.rightJustified(12, ' ').toAscii().constData() << endl;
                } // end if

                /// Presentaci&oacute;n en html normal.
                if ( html ) {
                    fitxersortidahtml << "<tr><td class=comptebalanc>" << lcuenta.toAscii().constData() << "</td><td class=assentamentbalanc>" <<  ldenominacion.left ( 40 ).toAscii().constData() << "</td><td class=dosdecimals>" << lsaldoant.toAscii().constData() << "</td><td class=dosdecimals>" << ldebe.toAscii().constData() << "</td><td class=dosdecimals>" << lhaber.toAscii().constData() << "</td><td class=dosdecimals>" << lsaldo.toAscii().constData() << "</td><td class=dosdecimals>" << ldebeej.toAscii().constData() << "</td><td class=dosdecimals>" << lhaberej.toAscii().constData() << "</td><td class=dosdecimals>" << lsaldoej.toAscii().constData() << endl;
                } // end if

            } // end while

            /// Hacemos la actualizaci&oacute;n de los saldos totales en
            /// formato espa&ntilde;ol.
            QString totalsaldoant = QString::number ( tsaldoant, 'f', 2 );
            QString totaldebe = QString::number ( tdebe, 'f', 2 );
            QString totalhaber = QString::number ( thaber, 'f', 2 );
            QString totalsaldo = QString::number ( tsaldo, 'f', 2 );
            QString totaldebeej = QString::number ( debeej, 'f', 2 );
            QString totalhaberej = QString::number ( haberej, 'f', 2 );
            QString totalsaldoej = QString::number ( saldoej, 'f', 2 );

            /// Imprimimos la l&iacute;nea con los resultados totalizados.
            /// Presentaci&oacute;n txt normal.
            if ( txt ) {
                fitxersortidatxt << "_____________________________________________________________________________________________________________________________________________\n";
                fitxersortidatxt << QString("Totales:").leftJustified(51, ' ').toAscii().constData() << totalsaldoant.rightJustified(12, ' ').toAscii().constData() << " " << totaldebe.rightJustified(12, ' ').toAscii().constData() << " " <<  totalhaber.rightJustified(12, ' ').toAscii().constData() << " " <<  totalsaldo.rightJustified(12, ' ').toAscii().constData()  << " " << totaldebeej.rightJustified(12, ' ').toAscii().constData() << " " <<  totalhaberej.rightJustified(12, ' ').toAscii().constData() << " " <<  totalsaldoej.rightJustified(12, ' ').toAscii().constData() << endl;
            } // end if

            /// Presentaci&oacute;n html normal.
            if ( html ) {
                fitxersortidahtml << "<tr><td></td><td class=totalbalanc>Totals</td><td class=dosdecimals>" <<  totalsaldoant.toAscii().constData() << "</td><td class=dosdecimals>" << totaldebe.toAscii().constData() << "</td><td class=dosdecimals>" << totalhaber.toAscii().constData() << "</td><td class=dosdecimals>" << totalsaldo.toAscii().constData() << "</td></tr>\n</table>\n</body>\n</html>\n";
            } // end if

            /// Eliminamos el &aacute;rbol y cerramos la conexi&oacute;n con la BD.
            delete arbol;
            mainCompany() ->commit();

            filetxt.close();
            filehtml.close();
            /// Dependiendo del formato de salida ejecutaremos el programa correspondiente.
            /// Presentaci&oacxute;n txt normal.
            if ( txt ) {
                QString cadena = g_confpr->value( CONF_EDITOR ) + " " + g_confpr->value( CONF_DIR_USER ) + "balance.txt";
                system ( cadena.toAscii().constData() );
            } // end if

            /// Presentaci&oacute;n html normal.
            if ( html ) {
                blWebBrowser(g_confpr->value( CONF_DIR_USER ) + "balance.html");
            } // end if

        } // end if
    } // end if
#endif

}


/// SLOT que responde a la pulsaci&oacute;n del bot&oacute;n de selecci&oacute;n de canal.
/** Presenta la ventana de selecci&oacute;n de canales \ref BcCanalSeleccionarView. */
/**
**/
void BalancePrintView::on_mui_canales_clicked()
{
    BL_FUNC_DEBUG
    BcCanalSeleccionarView *selcanales = ( ( BcCompany * ) mainCompany() ) ->getselcanales();
    selcanales->exec();
    selcanales->firstCanal();
}


/// SLOT que responde a la pulsaci&oacute;n del bot&oacute;n de selecci&oacute;n de
/// centros de coste.
/** Presenta la ventana de selecci&oacute;n de centros de coste \ref BcCentroCosteSeleccionarView. */
/**
**/
void BalancePrintView::on_mui_centroCostes_clicked()
{
    BL_FUNC_DEBUG
    BcCentroCosteSeleccionarView *selccostes = ( ( BcCompany * ) mainCompany() ) ->getselccostes();
    selccostes->exec();
    selccostes->firstccoste();
}


void BalancePrintView::on_mui_cerrar_clicked()
{
    close();
}

