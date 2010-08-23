/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#include <QTextStream>
#include <QLineEdit>
#include <QLocale>

#include "bcasientoview.h"
#include "bccuentasanualesimprimirview.h"
#include "blfunctions.h"
#include "bccompany.h"
#include "blfixed.h"
#include "blpostgresqlclient.h"
#include "bcplancontablearbol.h"

#ifndef WIN32
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif


///
/**
\param emp
\param parent
**/
BcCuentasAnualesImprimirView::BcCuentasAnualesImprimirView ( BcCompany  *emp, QWidget *parent )
        : QDialog ( parent ), BlMainCompanyPointer ( emp )
{
    blDebug ( "BcCuentasAnualesImprimirView::BcCuentasAnualesImprimirView", 0 );
    setupUi ( this );
    fichero = NULL;
    blDebug ( "END BcCuentasAnualesImprimirView::BcCuentasAnualesImprimirView", 0 );

}


///
/**
**/
BcCuentasAnualesImprimirView::~BcCuentasAnualesImprimirView()
{
    blDebug ( "BcCuentasAnualesImprimirView::~BcCuentasAnualesImprimirView", 0 );
    blDebug ( "END BcCuentasAnualesImprimirView::~BcCuentasAnualesImprimirView", 0 );
}


/// Se ha pulsado sobre el bot&oacute;n de aceptar del formulario con lo que podemos
/// pasar a hacer la impresi&oacute;n.
/** PSEUDICODIGO
    Ponemos todos los valores de las cuentas.
    Mientras haya sustituciones
        Para cada una de las formulas
            Si la formula no esta completa
                Sustituye todos los valores que se puedan sustituir
                Intenta Completar la formula
            FSi
        FPara
    FMientras */
/**
\return
**/
void BcCuentasAnualesImprimirView::on_mui_aceptar_clicked()
{
    blDebug ( "BcCuentasAnualesImprimirView::on_mui_aceptar_clicked", 0 );
    QString finicial = mui_fechainicial->text();
    QString ffinal = mui_fechafinal->text();
    QString finicial1 = mui_fechainicial1->text();
    QString ffinal1 = mui_fechafinal1->text();
    if ( finicial1 == "" ) {
        finicial1 = finicial;
    } // end if
    if ( ffinal1 == "" ) {
        ffinal1 = ffinal;
    }
    if ( finicial == "" || ffinal == "" ) {
        return;
    } // end if


    /** Version sin ARBOL

        /// Ponemos todos los valores de las cuentas. Hacemos la carga.
        QDomNodeList lcuentas = m_doc.elementsByTagName("CUENTA");
        for (int i = 0; i < lcuentas.count(); i++) {
            QDomNode cuenta = lcuentas.item(i);
            QDomElement e1 = cuenta.toElement(); /// try to convert the node to an element.
            if( !e1.isNull() ) { /// the node was really an element.
                /// Este es el c&aacute;lculo de los saldos para la cuenta.
                QString query = "SELECT saldototal('" + e1.text() + "','" + finicial + "','" + ffinal + "') AS valoract, saldototal('" + e1.text() + "','" + finicial1 + "','" + ffinal1 + "') AS valorant";
                BlDbRecordSet *cur = mainCompany()->loadQuery(query);
                if (!cur->eof()) {
                    QString valoract = cur->valor("valoract");
                    QString valorant = cur->valor("valorant");
                    QDomNode c = e1.parentNode();
                    agregaValores(c, valoract, valorant);
                } // end if
                delete cur;
            } // end if
        } // end for

    **/

    /** Version con ARBOL: mas rollo de codigo pero muuuucho mas eficiente **/
    /// Vamos a crear un arbol en la memoria dinamica con
    /// los distintos niveles de cuentas.


    /// Primero, averiguaremos la cantidad de ramas iniciales (tantos como
    /// numero de cuentas de nivel 2) y las vamos creando.
    mainCompany() ->begin();
    QString query = "SELECT *, nivel(codigo) AS nivel FROM cuenta ORDER BY codigo";
    BlDbRecordSet *ramas;
    ramas = mainCompany() ->loadQuery ( query, "Ramas" );
    BcPlanContableArbol *arbolP1, *arbolP2; /// un arbol por cada periodo
    arbolP1 = new BcPlanContableArbol;
    arbolP2 = new BcPlanContableArbol;
    while ( !ramas->eof() ) {
        if ( atoi ( ramas->valor ( "nivel" ).toAscii().constData() ) == 2 ) { /// Cuenta ra&iacute;z.
            arbolP1->nuevaRama ( ramas );
            arbolP2->nuevaRama ( ramas );
        } // end if
        ramas->nextRecord();
    } // end while
    arbolP1->inicializa ( ramas );
    arbolP2->inicializa ( ramas );
    delete ramas;
    mainCompany() ->commit();

    QRegExp rx ( "^.*perdidas y ganancias.*$" ); /// filtro para saber si es el de perdidas y ganancias
    rx.setCaseSensitivity ( Qt::CaseInsensitive );
    QString asiento;
    /// Discernimos entre Balances y Cuenta de Resultados
    if ( rx.exactMatch ( m_doc.elementsByTagName ( "TITULO" ).item ( 0 ).toElement().text() ) )
        /// Hay que excluir el asiento de Regularizacion para el calculo de beneficios o perdidas si existe ya en el periodo
        asiento = "%Asiento de Regularizaci%";
    else
        asiento = "%Asiento de Cierre%"; /// No hay que tener en cuenta el asiento de cierre para obtener los saldos

    /// OJO!! Antes de nada, hay que calcular el asiento de REGULARIZACION que nos guarda el resultado en la 129
    BcAsientoView *asientoReg;

        int resur = g_plugins->lanza ( "SNewBcAsientoView", (BcCompany *) mainCompany() );
        if ( ! resur) {
            blMsgInfo("No se pudo crear instancia de asientos");
            return;
        } // end if
        asientoReg = (BcAsientoView *) g_plugParams;
//    ( ( BcCompany * ) mainCompany() ) ->regularizaempresa ( finicial, ffinal );
    asientoReg ->asiento_regularizacion ( finicial, ffinal );

    /// Ahora, recopilamos todos los apuntes agrupados por cuenta para poder
    /// establecer as&iacute; los valores de cada cuenta para el periodo 1.
    mainCompany() ->begin();
    query = "SELECT cuenta.idcuenta, numapuntes, cuenta.codigo, saldoant, debe, haber, saldo, debeej, haberej, saldoej FROM (SELECT idcuenta, codigo FROM cuenta) AS cuenta NATURAL JOIN (SELECT idcuenta, count(idcuenta) AS numapuntes,sum(debe) AS debeej, sum(haber) AS haberej, (sum(debe)-sum(haber)) AS saldoej FROM apunte WHERE EXTRACT(year FROM fecha) = EXTRACT(year FROM timestamp '" + finicial + "') GROUP BY idcuenta) AS ejercicio LEFT OUTER JOIN (SELECT idcuenta,sum(debe) AS debe, sum(haber) AS haber, (sum(debe)-sum(haber)) AS saldo FROM apunte WHERE fecha >= '" + finicial + "' AND fecha <= '" + ffinal + "' AND conceptocontable NOT SIMILAR TO '" + asiento + "' GROUP BY idcuenta) AS periodo ON periodo.idcuenta=ejercicio.idcuenta LEFT OUTER JOIN (SELECT idcuenta, (sum(debe)-sum(haber)) AS saldoant FROM apunte WHERE fecha < '" + finicial + "' GROUP BY idcuenta) AS anterior ON cuenta.idcuenta=anterior.idcuenta ORDER BY codigo";
    BlDbRecordSet *hojas;
    hojas = mainCompany() ->loadQuery ( query, "Periodo1" );
    /// Para cada cuenta con sus saldos calculados hay que actualizar hojas del &aacute;rbol.
    while ( !hojas->eof() ) {
        arbolP1->actualizaHojas ( hojas );
        hojas->nextRecord();
    } // end while
    mainCompany() ->commit();
    asientoReg->on_mui_borrar_clicked ( FALSE ); /// borramos el asiento temporal creado indicando que no queremos confirmacion

    /// Para el segundo periodo, calculamos el asiento de REGULARIZACION que nos guarda el resultado en la 129
//    ( ( BcCompany * ) mainCompany() ) ->regularizaempresa ( finicial1, ffinal1 );
    asientoReg ->asiento_regularizacion ( finicial1, ffinal1 );
//    asientoReg = ( ( BcCompany * ) mainCompany() ) ->intapuntsempresa2();

    /// Ahora, recopilamos todos los apuntes agrupados por cuenta para poder
    /// establecer as&iacute; los valores de cada cuenta para el periodo 2.
    mainCompany() ->begin();
    query = "SELECT cuenta.idcuenta, numapuntes, cuenta.codigo, saldoant, debe, haber, saldo, debeej, haberej, saldoej FROM (SELECT idcuenta, codigo FROM cuenta) AS cuenta NATURAL JOIN (SELECT idcuenta, count(idcuenta) AS numapuntes,sum(debe) AS debeej, sum(haber) AS haberej, (sum(debe)-sum(haber)) AS saldoej FROM apunte WHERE EXTRACT(year FROM fecha) = EXTRACT(year FROM timestamp '" + finicial1 + "') GROUP BY idcuenta) AS ejercicio LEFT OUTER JOIN (SELECT idcuenta,sum(debe) AS debe, sum(haber) AS haber, (sum(debe)-sum(haber)) AS saldo FROM apunte WHERE fecha >= '" + finicial1 + "' AND fecha <= '" + ffinal1 + "' AND conceptocontable NOT SIMILAR TO '" + asiento + "' GROUP BY idcuenta) AS periodo ON periodo.idcuenta=ejercicio.idcuenta LEFT OUTER JOIN (SELECT idcuenta, (sum(debe)-sum(haber)) AS saldoant FROM apunte WHERE fecha < '" + finicial1 + "' GROUP BY idcuenta) AS anterior ON cuenta.idcuenta=anterior.idcuenta ORDER BY codigo";
    hojas = mainCompany() ->loadQuery ( query, "Periodo2" );
    /// Para cada cuenta con sus saldos calculados hay que actualizar hojas del &aacute;rbol.
    while ( !hojas->eof() ) {
        arbolP2->actualizaHojas ( hojas );
        hojas->nextRecord();
    } // end while
    delete hojas;
    mainCompany() ->commit();
    asientoReg->on_mui_borrar_clicked ( FALSE ); /// borramos indicando que no queremos confirmacion

    QDomNodeList lcuentas = m_doc.elementsByTagName ( "CUENTA" );
    for ( int i = 0; i < lcuentas.count(); i++ ) {
        QDomNode cuenta = lcuentas.item ( i );
        QDomElement e1 = cuenta.toElement();
        QString valorP1, valorP2;
        BlFixed valor = BlFixed ( "0.00" );
        if ( !e1.isNull() ) {
            if ( arbolP1->irHoja ( e1.text() ) )
                valor = BlFixed ( arbolP1->hojaActual ( "saldo" ) );
            else
                valor = BlFixed ( "0.00" );
            valorP1 = valor.toQString();
            if ( arbolP2->irHoja ( e1.text() ) )
                valor = BlFixed ( arbolP2->hojaActual ( "saldo" ) );
            else
                valor = BlFixed ( "0.00" );
            valorP2 = valor.toQString();
            QDomNode c = e1.parentNode();
            agregaValores ( c, valorP1, valorP2 );
        } // end if
    } // end for

    /// Eliminamos el &aacute;rbol y cerramos la conexi&oacute;n con la BD.
    delete arbolP1;
    delete arbolP2;

    /** Fin de la version con ARBOL **/

    /// Hacemos el calculo recursivo del balance.
    bool terminado = FALSE;
    while ( !terminado ) {
        terminado = TRUE;
        /// Recogemos los valores de cuenta.
        QDomNodeList litems = m_doc.elementsByTagName ( "FORMULA" );
        for ( int i = 0; i < litems.count(); i++ ) {
            QDomNode item = litems.item ( i );
            QDomElement e1 = item.toElement(); /// Try to convert the node to an element.
            if ( !e1.isNull() ) { /// The node was really an element.
                terminado &= procesaFormula ( item );
            } // end if
        } // end for
    } // end while

    /// Una vez que tenemos el objeto bien generado y a punto pasamos a la generacion del PDF.
    imprimir ( finicial, ffinal, finicial1, ffinal1 );
    blDebug ( "END BcCuentasAnualesImprimirView::on_mui_aceptar_clicked", 0 );
}


///
/**
\param formula
\param return
\return
**/
bool BcCuentasAnualesImprimirView::procesaFormula ( const QDomNode &formula )
{
    blDebug ( "BcCuentasAnualesImprimirView::procesaFormula", 0 );
    QDomElement valor = formula.firstChildElement ( "VALORACT" );
    //
    QString valors = valor.toElement().text();
    QString codigo = formula.parentNode().firstChildElement ( "CONCEPTO" ).toElement().text();
    //
    if ( !valor.isNull() ) {
        return TRUE;
    } // end if
    BlFixed tvaloract = BlFixed ( "0.0" );
    BlFixed tvalorant = BlFixed ( "0.0" );
    QDomElement formula3 = formula.toElement();
    QDomNodeList litems = formula3.elementsByTagName ( "OPERADOR" );
    for ( int i = 0; i < litems.count(); i++ ) {
        QDomNode item = litems.item ( i );
        QDomElement e1 = item.toElement(); /// Try to convert the node to an element.
        if ( !e1.isNull() ) { /// The node was really an element.
            if ( !procesaOperador ( item ) )
                return FALSE;
            QString valoract, valorant;
            if ( valorItem ( item, valoract, valorant ) ) {
                tvaloract = tvaloract + BlFixed ( valoract );
                tvalorant = tvalorant + BlFixed ( valorant );
            } else
                return FALSE;
        } // end if
    } // end for
    QString tvaloracts = tvaloract.toQString();
    QString tvalorants = tvalorant.toQString();
    agregaValores ( formula, tvaloracts, tvalorants );
    blDebug ( "END BcCuentasAnualesImprimirView::procesaFormula", 0 );
    return TRUE;
}

/** Pseudocodigo
    Si el operador ya tiene valor (devuelve TRUE)
    Si la formula ya tiene valor le ponemos el valor y devuelve TRUE
    devuelve FALSe */
/**
\param operador
\return
**/
bool BcCuentasAnualesImprimirView::procesaOperador ( const QDomNode &operador )
{
    blDebug ( "BcCuentasAnualesImprimirView::procesaOperador", 0, operador.toElement().text() );
    QDomElement valor = operador.firstChildElement ( "VALORACT" );
    if ( !valor.isNull() )
        return TRUE;
    /// Miramos la f&oacute;rmula.
    QDomElement lineaid = operador.firstChildElement ( "LINEAID" );

    if ( !lineaid.isNull() ) {
        QDomNodeList litems = m_doc.elementsByTagName ( "ID" );
        for ( int i = 0; i < litems.count(); i++ ) {
            QDomNode item = litems.item ( i );
            QDomElement e1 = item.toElement(); /// Try to convert the node to an element.
            if ( !e1.isNull() ) { /// The node was really an element.
                if ( e1.text() == lineaid.text() ) {
                    /// Este item es la f&oacute;rmula referenciada.
                    QDomNode formula = item.parentNode().firstChildElement ( "FORMULA" );
                    QString valoract, valorant;
                    if ( valorItem ( formula, valoract, valorant ) ) {
                        agregaValores ( operador, valoract, valorant );
                        return TRUE;
                    } else {
                        return FALSE;
                    } // end if
                } // end if
            } // end if
        } // end for
    } // end if
    blDebug ( "BcCuentasAnualesImprimirView::procesaOperador", 0 );
    return FALSE;
}


///
/**
\param formula
\param valoract
\param valorant
\return
**/
bool BcCuentasAnualesImprimirView::valorItem ( const QDomNode &formula, QString &valoract, QString &valorant )
{
    blDebug ( "BcCuentasAnualesImprimirView::valorItem", 0, formula.toElement().tagName() );
    QDomElement valor = formula.namedItem ( "VALORACT" ).toElement();
    if ( valor.isNull() ) {
        return FALSE;
    } // end if
    valoract = valor.text();
    valorant = formula.namedItem ( "VALORANT" ).toElement().text();
    blDebug ( "END BcCuentasAnualesImprimirView::valorItem", 0, formula.toElement().text() + "--" + valoract );
    return TRUE;
}


///
/**
\param nodo
\param valoract
\param valorant
**/
void BcCuentasAnualesImprimirView::agregaValores ( const QDomNode &nodo, const QString &valoract, const QString &valorant )
{
    blDebug ( "BcCuentasAnualesImprimirView::agregaValores", 0, nodo.toElement().tagName() + " " + valoract );
    QDomElement enodo = nodo.toElement();
    BlFixed fvaloract ( valoract );
    BlFixed fvalorant ( valorant );
    /// Miramos las opciones pasadas
    if ( nodo.nodeName() == QString ( "OPERADOR" ) ) {
        QDomNodeList opcs = enodo.elementsByTagName ( "OPCIONES" );
        for ( int i = 0; i < opcs.count(); i++ ) {
            QDomElement op = opcs.item ( i ).toElement();
            QString opciones = op.text();
            if ( opciones == "POSITIVO" ) {
                if ( fvaloract < 0 )
                    fvaloract = fvaloract * -1;
                if ( fvalorant < 0 )
                    fvalorant = fvalorant * -1;
            } // end if
            if ( opciones == "NEGATIVO" ) {
                if ( fvaloract > 0 )
                    fvaloract = fvaloract * -1;
                if ( fvalorant > 0 )
                    fvalorant = fvalorant * -1;
            } // end if
            if ( opciones == "RESTAR" ) {
                fvaloract = fvaloract * -1;
                fvalorant = fvalorant * -1;
            } // end if
        } // end for
    } else {
        QDomNodeList opcs = nodo.parentNode().toElement().elementsByTagName ( "OPCIONES" );
        for ( int i = 0; i < opcs.count(); i++ ) {
            QDomElement op = opcs.item ( i ).toElement();
            QString opciones = op.text();
            if ( opciones == "MAYORCERO" ) {
                if ( fvaloract < 0 )
                    fvaloract = 0;
                if ( fvalorant < 0 )
                    fvalorant = 0;
            } // end if
            if ( opciones == "MENORCERO" ) {
                if ( fvaloract > 0 )
                    fvaloract = 0;
                if ( fvalorant > 0 )
                    fvalorant = 0;
            } // end if
        } // end for
    } // end if
    QDomElement valoract1 = m_doc.createElement ( "VALORACT" );
    valoract1.setTagName ( "VALORACT" );
    QDomText etx = m_doc.createTextNode ( fvaloract.toQString() );
    valoract1.appendChild ( etx );
    QDomElement valorant1 = m_doc.createElement ( "VALORANT" );
    valorant1.setTagName ( "VALORANT" );
    QDomText etc = m_doc.createTextNode ( fvalorant.toQString() );
    valorant1.appendChild ( etc );
    QDomNode n = nodo;
    enodo.appendChild ( valoract1 );
    enodo.appendChild ( valorant1 );
    blDebug ( "END BcCuentasAnualesImprimirView::agregaValores", 0 );
}


///
/**
\param periodo1finicial
\param periodo1ffinal
\param periodo2finicial
\param periodo2ffinal
**/
void BcCuentasAnualesImprimirView::imprimir ( QString periodo1finicial, QString periodo1ffinal, QString periodo2finicial, QString periodo2ffinal )
{
    blDebug ( "BcCuentasAnualesImprimirView::imprimir", 0 );
    QString archivo = g_confpr->valor ( CONF_DIR_OPENREPORTS ) + "canuales.rml";
    QString archivod = g_confpr->valor ( CONF_DIR_USER ) + "canuales.rml";
    QString archivologo = g_confpr->valor ( CONF_DIR_OPENREPORTS ) + "logo.jpg";
    /// Copiamos el archivo.
#ifdef WINDOWS

    archivo = "copy " + archivo + " " + archivod;
#else

    archivo = "cp " + archivo + " " + archivod;
#endif

    system ( archivo.toAscii().constData() );
    /// Copiamos el logo.
#ifdef WINDOWS

    archivologo = "copy " + archivologo + " " + g_confpr->valor ( CONF_DIR_USER ) + "logo.jpg";
#else

    archivologo = "cp " + archivologo + " " + g_confpr->valor ( CONF_DIR_USER ) + "logo.jpg";
#endif

    system ( archivologo.toAscii().constData() );
    QFile file;
    file.setFileName ( archivod );
    file.open ( QIODevice::ReadOnly );
    QTextStream stream ( &file );
    QString buff = stream.readAll();
    file.close();
    QString fitxersortidatxt;
    fitxersortidatxt = "";

    /// Iteramos para cada componente del listado.
    QDomNodeList componentes = m_doc.elementsByTagName ( "COMPONENTE" );
    for ( int j = 0; j < componentes.count(); j++ ) {
        QDomElement comp = componentes.item ( j ).toElement();
        /// Escribimos el titulo del componente
        QString debeohaber = comp.namedItem ( "SUBTITULO" ).toElement().text();
        fitxersortidatxt += "" + debeohaber + "\n";
        /// Cogemos los elementos del componente y los ponemos en forma de tabla.
        fitxersortidatxt += "<blockTable style=\"tabla\" repeatRows=\"1\" colWidths=\"12cm,3cm,3cm\">\n";
        fitxersortidatxt += "<tr><td><para style=\"debeohaber\">" + debeohaber + "</para></td>\n";
        fitxersortidatxt += "<td><para style=\"periodo\">Periodo:</para><para style=\"periodo\">" + periodo1finicial + " ~ " + periodo1ffinal + "</para></td>\n";
        fitxersortidatxt += "<td><para style=\"periodo\">Periodo:</para><para style=\"periodo\">" + periodo2finicial + " ~ " + periodo2ffinal + "</para></td></tr>\n";

        QDomNodeList litems = comp.elementsByTagName ( "LBALANCE" );
        QLocale::setDefault ( QLocale ( QLocale::Spanish, QLocale::Spain ) );
        QLocale spanish;
        for ( int i = 0; i < litems.count(); i++ ) {
            QDomNode item = litems.item ( i );
            QDomElement e1 = item.toElement(); /// Try to convert the node to an element.
            if ( !e1.isNull() ) { /// The node was really an element.
                QDomNode formul = item.firstChildElement ( "FORMULA" );
                QString vact = formul.firstChildElement ( "VALORACT" ).toElement().text();
                vact = spanish.toString ( vact.toDouble(), 'f', 2 );
                QString vant = formul.firstChildElement ( "VALORANT" ).toElement().text();
                vant = spanish.toString ( vant.toDouble(), 'f', 2 );
                QString texto = item.firstChildElement ( "CONCEPTO" ).toElement().text();
                fitxersortidatxt += "<tr>\n";
                fitxersortidatxt += "<td>" + texto + "</td>\n";
                fitxersortidatxt += "<td>" + vact + "</td>\n";
                fitxersortidatxt += "<td>" + vant + "</td>\n";
                fitxersortidatxt += "</tr>\n";
            } // end if
        } // end for
        QLocale::setDefault ( QLocale::C );
        fitxersortidatxt += "</blockTable>\n";
        fitxersortidatxt += "<nextFrame/>\n";
    } // end for

    buff.replace ( "[story]", fitxersortidatxt );
    QDomElement nodo = m_doc.namedItem ( "BALANCE" ).namedItem ( "TITULO" ).toElement();
    buff.replace ( "[titulo]", nodo.text() );

    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );
        stream << buff;
        file.close();
    } // end if

    invocaPDF ( "canuales" );
    blDebug ( "END BcCuentasAnualesImprimirView::imprimir", 0 );
}


///
/**
\param id
\return
**/
void BcCuentasAnualesImprimirView::setidbalance ( QString id )
{
    blDebug ( "BcCuentasAnualesImprimirView::setidbalance", 0 );
    QFile f ( id );
    if ( !f.open ( QIODevice::ReadOnly ) )
        return;
    if ( !m_doc.setContent ( &f ) ) {
        f.close();
        return;
    } // end if
    f.close();
    m_nomBalance->setText ( id );
    blDebug ( "END BcCuentasAnualesImprimirView::setidbalance", 0 );
}

