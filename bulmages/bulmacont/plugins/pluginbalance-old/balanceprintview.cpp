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

#include <QWidget>
#include <QRadioButton>
#include <QLineEdit>
#include <QComboBox>
#include <QToolButton>
#include <QCheckBox>

#include <busquedafecha.h>

#include "balanceprintview.h"
#include "listcuentasview1.h"
#include "empresa.h"
#include "arbol.h"
#include "busquedacuenta.h"
#include "selectcanalview.h"
#include "selectccosteview.h"

#ifndef WIN32
#include <unistd.h>
#endif

using namespace std;


///
/**
\param emp
**/
BalancePrintView::BalancePrintView ( Empresa *emp )
        : QDialog ( 0 ), PEmpresaBase ( emp )
{
    _depura ( "BalancePrintView::BalancePrintView", 0 );
    setupUi ( this );
    m_codigoinicial->setEmpresaBase ( emp );
    m_codigofinal->setEmpresaBase ( emp );
    /// Buscamos los diferentes niveles que existen seg&uacute;n existan en la tabla
    /// de cuentas.
    empresaBase()->begin();
    QString query = "SELECT nivel(codigo) FROM cuenta GROUP BY nivel ORDER BY nivel";
    cursor2 *niveles = empresaBase()->cargacursor ( query, "Niveles" );
    int i = 0;
    while ( !niveles->eof() ) {
        /// Inicializamos la tabla de nivel.
        combonivel->insertItem ( i, niveles->valor ( "nivel" ) );
        niveles->siguienteregistro();
        i++;
    } // end while
    empresaBase()->commit();
    delete niveles;
    _depura ( "END BalancePrintView::BalancePrintView", 0 );
}


///
/**
**/
BalancePrintView::~BalancePrintView()
{
    _depura ( "BalancePrintView::~BalancePrintView", 0 );
    _depura ( "END BalancePrintView::~BalancePrintView", 0 );
}


/// Inicializa la clase para que tenga par&aacute;metros por defecto (no es obligatorio).
/** codinicial = Indica el c&oacute;digo inicial para mostrar el balance.
    cofinal = Indica el c&oacute;digo de cuenta para no mostrar m&aacute;s balances.
    finicial = Indica la fecha a partir de la cual comenzar los saldos.
    ffinal = Indica la fecha para terminar de contar los saldos.
    arbol = Indica si hay que representar el balance en forma de árbol o no. */
/**
**/
void BalancePrintView::inicializa1 ( QString codinicial1, QString codfinal1, QString finicial1, QString ffinal1, bool arbol )
{
    _depura ( "BalancePrintView::inicializa1", 0 );
    m_fechainicial1->setText ( finicial1 );
    m_fechafinal1->setText ( ffinal1 );
    m_codigoinicial->setText ( codinicial1 );
    m_codigofinal->setText ( codfinal1 );
    checksuperiores->setChecked ( arbol );
    _depura ( "END BalancePrintView::inicializa1", 0 );
}


/// Se ha pulsado sobre el bot&oacute;n aceptar del formulario.
/**
**/
void BalancePrintView::on_mui_imprimir_clicked()
{
    _depura ( "BalancePrintView::on_mui_imprimir_clicked", 0 );
    if ( radiotexto->isChecked() )
        presentar ( "txt" );
    if ( radiohtml->isChecked() )
        presentar ( "html" );
    if ( radiopropietario->isChecked() )
        presentar ( "rtk" );
    if ( radiokugar->isChecked() )
        presentar ( "kugar" );
    _depura ( "END BalancePrintView::on_mui_imprimir_clicked", 0 );
}


/// Se ha pulsado sobre el bot&oacute;n aceptar del formulario.
/**
\param tipus
**/
void BalancePrintView::presentar ( const char *tipus )
{
    _depura ( "BalancePrintView::presentar", 0 );
#ifndef WIN32
    int kugar, txt, html, txtapren, htmlapren;
    double tsaldoant, tdebe, thaber, tsaldo, debeej, haberej, saldoej;
    QString query;

    /// tipos de presentaciones.
    txt = !strcmp ( tipus, "txt" );
    html = !strcmp ( tipus, "html" );
    txtapren = !strcmp ( tipus, "txtapren" );
    htmlapren = !strcmp ( tipus, "htmlapren" );
    kugar = !strcmp ( tipus, "kugar" );

    /// Cogemos los valores del formulario.
    QString finicial = m_fechainicial1->text();
    QString ffinal = m_fechafinal1->text();
    QString cinicial = m_codigoinicial->codigocuenta();
    QString cfinal = m_codigofinal->codigocuenta();
    int nivel = combonivel->currentText().toInt();
    bool superiores = checksuperiores->isChecked();

    if ( txt | html | kugar ) {
        QString archivo = confpr->valor ( CONF_DIR_USER ) + "balance.txt";
        QString archivokugar = confpr->valor ( CONF_DIR_USER ) + "balance.kud";
        QString archivohtml = confpr->valor ( CONF_DIR_USER ) + "balance.html";
        fprintf ( stderr, "%s\n", archivo.toAscii().constData() );

        /// Creamos los ficheros de salida.
        ofstream fitxersortidakugar ( archivokugar.toAscii().constData() );
        ofstream fitxersortidatxt ( archivo.toAscii().constData() );
        ofstream fitxersortidahtml ( archivohtml.toAscii().constData() );

        if ( !fitxersortidatxt )
            txt = 0; /// Verificamos que se hayan creado correctamente los archivos.
        if ( !fitxersortidahtml )
            html = 0; /// Se puede mejorar el tratamiento de errores.
        if ( !fitxersortidakugar )
            kugar = 0;

        /// S&oacute;lo continuamos si hemos podido crear alg&uacute;n archivo.
        if ( txt | html | kugar ) {
            /// Vamos a crear un &aacute;rbol en la mem&oacute;ria din&aacute;mica con
            /// los distintos niveles de cuentas.
            /// Primero, averiguaremos la cantidad de ramas iniciales (tantos como
            /// n&uacute;mero de cuentas de nivel 2) y las vamos creando.
            empresaBase()->begin();
            query.sprintf ( "SELECT *, nivel(codigo) AS nivel FROM cuenta ORDER BY codigo" );
            cursor2 *ramas;
            ramas = empresaBase()->cargacursor ( query, "Ramas" );
            Arbol *arbol;
            arbol = new Arbol;
            while ( !ramas->eof() ) {
                if ( atoi ( ramas->valor ( "nivel" ).toAscii().constData() ) == 2 ) { /// Cuenta ra&iacute;z.
                    arbol->nuevaRama ( ramas );
                } // end if
                ramas->siguienteregistro();
            } // end while
            arbol->inicializa ( ramas );
            delete ramas;

            /// Ahora despu&eacute;s, usaremos el &aacute;rbol para poner los datos a
            /// cada hoja (cuenta) seg&uacute;n los per&iacute;odos que necesitemos acotar.
            /// Pero antes, preparamos las plantillas segun el tipo de salida seleccionado.
            if ( kugar ) {
                fitxersortidakugar.setf ( ios::fixed )
                ;
                fitxersortidakugar.precision ( 2 );
                fitxersortidakugar << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" ;
                fitxersortidakugar << "<!DOCTYPE KugarData [\n" ;
                fitxersortidakugar << "\t<!ELEMENT KugarData (Row* )>\n" ;
                fitxersortidakugar << "\t\t<!ATTLIST KugarData\n";
                fitxersortidakugar << "\t\tTemplate CDATA #REQUIRED>\n";
                fitxersortidakugar << "\t<!ELEMENT Row EMPTY>\n";
                fitxersortidakugar << "\t<!ATTLIST Row \n";
                fitxersortidakugar << "\t\tlevel CDATA #REQUIRED\n";
                fitxersortidakugar << "\t\tfinicial CDATA #IMPLIED\n";
                fitxersortidakugar << "\t\tffinal CDATA #IMPLIED\n";
                fitxersortidakugar << "\t\tcuenta CDATA #IMPLIED\n";
                fitxersortidakugar << "\t\tdescripcion CDATA #IMPLIED\n";
                fitxersortidakugar << "\t\tsaldoant CDATA #IMPLIED\n";
                fitxersortidakugar << "\t\tdebe CDATA #IMPLIED\n";
                fitxersortidakugar << "\t\thaber CDATA #IMPLIED\n";
                fitxersortidakugar << "\t\tsaldo CDATA #IMPLIED\n";
                fitxersortidakugar << "\t\tdebeej CDATA #IMPLIED\n";
                fitxersortidakugar << "\t\thaberej CDATA #IMPLIED\n";
                fitxersortidakugar << "\t\tsaldoej CDATA #IMPLIED>\n";
                fitxersortidakugar << "]>\n\n";
                fitxersortidakugar << "<KugarData Template=\"" << confpr->valor ( CONF_DIR_KUGAR ).toAscii().constData() << "balance.kut\">\n";
                fitxersortidakugar << "\t<Row";
                fitxersortidakugar << " level=\"0\"";
                fitxersortidakugar << " finicial='" << finicial.toAscii().constData() << "'";
                fitxersortidakugar << " ffinal='" << ffinal.toAscii().constData() << "'/>\n";
            } // end if

            if ( txt ) {
                /// Presentaci&oacute;n txt normal.
                fitxersortidatxt.setf ( ios::fixed )
                ;
                fitxersortidatxt.precision ( 2 );
                fitxersortidatxt << "                                        Balance \n" ;
                fitxersortidatxt << "Fecha Inicial: " << finicial.toAscii().constData() << "   Fecha Final: " << ffinal.toAscii().constData() << endl;
                fitxersortidatxt << "Cuenta            Denominación                        Saldo ant.         Debe        Haber        Saldo     Debe ej.    Haber ej.    Saldo ej.\n" ;
                fitxersortidatxt << "______________________________________________________________________________________________________________________________________________\n";
            } // end if

            if ( html ) {
                /// Presentaci&oacute;n html normal.
                fitxersortidahtml.setf ( ios::fixed )
                ;
                fitxersortidahtml.precision ( 2 );
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
            cursor2 *cuentas;
            cuentas = empresaBase()->cargacursor ( query, "Periodo" );
            /// Para cada cuenta con sus apuntes hechos hay que actualizar hojas
            /// del &aacute;rbol.
            while ( !cuentas->eof() ) {
                arbol->actualizaHojas ( cuentas );
                cuentas->siguienteregistro();
            } // end while

            /// Ir&aacute; contando las l&iacute;neas impresas en el impreso de Kugar.
            float linea = 1;
            /// Determina cu&aacute;ntas l&iacute;neas caben para el impreso de Kugar.
            float lineaskugar = 53;
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
                    fitxersortidatxt << setiosflags ( ios::left ) << setw ( 10 ) <<  lcuenta.toAscii().constData() << " " << setw ( 40 ) <<  ldenominacion.left ( 40 ).toAscii().constData() << " " << resetiosflags ( ios::left ) << setw ( 12 ) <<  lsaldoant.toAscii().constData() << " " << setw ( 12 ) <<  ldebe.toAscii().constData() << " " << setw ( 12 ) <<  lhaber.toAscii().constData() << " " << setw ( 12 ) <<  lsaldo.toAscii().constData() << " " << setw ( 12 ) <<  ldebeej.toAscii().constData() << " " << setw ( 12 ) <<  lhaberej.toAscii().constData() << " " << setw ( 12 ) <<  lsaldoej.toAscii().constData() << " " << setw ( 12 ) <<  endl;
                } // end if

                /// Presentaci&oacute;n en html normal.
                if ( html ) {
                    fitxersortidahtml << "<tr><td class=comptebalanc>" << lcuenta.toAscii().constData() << "</td><td class=assentamentbalanc>" <<  ldenominacion.left ( 40 ).toAscii().constData() << "</td><td class=dosdecimals>" << lsaldoant.toAscii().constData() << "</td><td class=dosdecimals>" << ldebe.toAscii().constData() << "</td><td class=dosdecimals>" << lhaber.toAscii().constData() << "</td><td class=dosdecimals>" << lsaldo.toAscii().constData() << "</td><td class=dosdecimals>" << ldebeej.toAscii().constData() << "</td><td class=dosdecimals>" << lhaberej.toAscii().constData() << "</td><td class=dosdecimals>" << lsaldoej.toAscii().constData() << endl;
                } // end if

                /// Presentaci&oacute;n en Kugar seg&uacute;n plantilla balance.kut
                if ( kugar ) {
                    /// Primero vamos a establecer si hay que imprimir una l&iacute;nea
                    /// de cabecera (Detail 0 en balance.kut) con los datos del
                    /// per&iacute;odo.
                    if ( fmod ( linea, lineaskugar ) == 0 ) {
                        fitxersortidakugar << "\t<Row";
                        fitxersortidakugar << " level=\"0\"";
                        fitxersortidakugar << " finicial='" << finicial.toAscii().constData() << "'";
                        fitxersortidakugar << " ffinal='" << ffinal.toAscii().constData() << "'/>\n";
                    } // end if
                    if ( linea == lineaskugar ) {
                        /// A partir de la segunda p&aacute;gina alguna l&iacute;nea m&aacute;s.
                        lineaskugar = 54;
                        linea = lineaskugar;
                    }
                    /// Ahora, imprimimos tantas l&iacute;neas como nos permite la
                    /// variable "lineaskugar" sin imprimir cabecera (Detail 1 en
                    /// balance.kut). Una l&iacute;nea por cada iteraci&oacute;n del buble.
                    fitxersortidakugar << "\t<Row";
                    fitxersortidakugar << " level=\"1\"";
                    fitxersortidakugar << " cuenta='" << lcuenta.toAscii().constData() << "'";
                    fitxersortidakugar << " descripcion='" << ldenominacion.toAscii().constData() << "'";
                    fitxersortidakugar << " saldoant='" << lsaldoant.toAscii().constData() << "'";
                    fitxersortidakugar << " debe='" << ldebe.toAscii().constData() << "'";
                    fitxersortidakugar << " haber='" << lhaber.toAscii().constData() << "'";
                    fitxersortidakugar << " saldo='" << lsaldo.toAscii().constData() << "'";
                    fitxersortidakugar << " debeej='" << ldebeej.toAscii().constData() << "'";
                    fitxersortidakugar << " haberej='" << lhaberej.toAscii().constData() << "'";
                    fitxersortidakugar << " saldoej='" << lsaldoej.toAscii().constData() << "'/>\n";
                    linea++;
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
                fitxersortidatxt << "                                            __________________________________________________________________________________________________\n";
                fitxersortidatxt << "                                            Totales " << setw ( 12 ) <<  totalsaldoant.toAscii().constData() << " " << setw ( 12 ) <<  totaldebe.toAscii().constData() << " " << setw ( 12 ) <<  totalhaber.toAscii().constData() << " " << setw ( 12 ) <<  totalsaldo.toAscii().constData()  << " " << setw ( 12 ) <<  totaldebeej.toAscii().constData() << " " << setw ( 12 ) <<  totalhaberej.toAscii().constData() << " " << setw ( 12 ) <<  totalsaldoej.toAscii().constData() << endl;
            }

            /// Presentaci&oacute;n html normal.
            if ( html ) {
                fitxersortidahtml << "<tr><td></td><td class=totalbalanc>Totals</td><td class=dosdecimals>" <<  totalsaldoant.toAscii().constData() << "</td><td class=dosdecimals>" << totaldebe.toAscii().constData() << "</td><td class=dosdecimals>" << totalhaber.toAscii().constData() << "</td><td class=dosdecimals>" << totalsaldo.toAscii().constData() << "</td></tr>\n</table>\n</body>\n</html>\n";
            }

            /// Presentaci&oacute;n Kugar.
            if ( kugar ) {
                fitxersortidakugar << "\t<Row";
                fitxersortidakugar << " level=\"2\"";
                fitxersortidakugar << " tsaldoant='" << totalsaldoant.toAscii().constData() << "'";
                fitxersortidakugar << " tdebe='" << totaldebe.toAscii().constData() << "'";
                fitxersortidakugar << " thaber='" << totalhaber.toAscii().constData() << "'";
                fitxersortidakugar << " tsaldo='" << totalsaldo.toAscii().constData() << "'";
                fitxersortidakugar << " tdebeej='" << totaldebeej.toAscii().constData() << "'";
                fitxersortidakugar << " thaberej='" << totalhaberej.toAscii().constData() << "'";
                fitxersortidakugar << " tsaldoej='" << totalsaldoej.toAscii().constData() << "'/>\n";
                fitxersortidakugar << "</KugarData>\n";
            }

            /// Eliminamos el &aacute;rbol y cerramos la conexi&oacute;n con la BD.
            delete arbol;
            empresaBase()->commit();

            fitxersortidatxt.close();
            /// Dependiendo del formato de salida ejecutaremos el programa correspondiente.
            /// Presentaci&oacxute;n txt normal.
            if ( txt ) {
                QString cadena = confpr->valor ( CONF_EDITOR ) + " " + confpr->valor ( CONF_DIR_USER ) + "balance.txt";
                system ( cadena.toAscii().constData() );
            } // end if

            /// Presentaci&oacute;n html normal.
            if ( html ) {
                QString cadena = confpr->valor ( CONF_NAVEGADOR ) + " " + confpr->valor ( CONF_DIR_USER ) + "balance.html";
                system ( cadena.toAscii().constData() );
            }

            /// Presentaci&oacute;n Kugar normal.
            if ( kugar ) {
                QString cadena = "kugar " + confpr->valor ( CONF_DIR_USER ) + "balance.kud";
                system ( cadena.toAscii().constData() );
            }
        }
    }
#endif
    _depura ( "END BalancePrintView::presentar", 0 );
}


/// SLOT que responde a la pulsaci&oacute;n del bot&oacute;n de selecci&oacute;n de canal.
/** Presenta la ventana de selecci&oacute;n de canales \ref selectcanalview. */
/**
**/
void BalancePrintView::on_mui_canales_clicked()
{
    _depura ( "BalancePrintView::on_mui_canales_clicked", 0 );
    selectcanalview *selcanales = ( ( Empresa * ) empresaBase() )->getselcanales();
    selcanales->exec();
    selcanales->firstcanal();
    _depura ( "END BalancePrintView::on_mui_canales_clicked", 0 );
}


/// SLOT que responde a la pulsaci&oacute;n del bot&oacute;n de selecci&oacute;n de
/// centros de coste.
/** Presenta la ventana de selecci&oacute;n de centros de coste \ref SelectCCosteView. */
/**
**/
void BalancePrintView::on_mui_ccostes_clicked()
{
    _depura ( "BalancePrintView::on_mui_ccostes_clicked", 0 );
    SelectCCosteView *selccostes = ( ( Empresa * ) empresaBase() )->getselccostes();
    selccostes->exec();
    selccostes->firstccoste();
    _depura ( "END BalancePrintView::on_mui_ccostes_clicked", 0 );
}

