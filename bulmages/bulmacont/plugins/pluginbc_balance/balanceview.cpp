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

#include <QWidget>
#include <QLineEdit>
#include <QToolButton>
#include <QPixmap>
#include <QFont>

#include "balanceview.h"
#include "bccompany.h"
#include "bcplancontablearbol.h"
#include "blpostgresqlclient.h"
#include "balanceprintview.h"
#include "bldatesearch.h"
#include "bcbuscarcentrocoste.h"
#include "bccentrocosteseleccionarview.h"
#include "bccanalseleccionarview.h"


#define CUENTA          0
#define DENOMINACION    1
#define SALDO_ANT       2
#define DEBE            3
#define HABER           4
#define SALDO           5
#define DEBEEJ          6
#define HABEREJ         7
#define SALDOEJ         8


/// Se prepara el combobox de niveles a mostrar y se ponen las fechas de balance.
/** \bug No es necesario borrar la tabla de designer para que esto funcione. */
/**
\param emp
\param parent
**/
BalanceView::BalanceView ( BcCompany *emp, QWidget *parent, int )
        : BcForm ( emp, parent )
{
    BL_FUNC_DEBUG
    setupUi ( this );

    setTitleName ( _ ( "Balance" ) );
    /// EStablezco cual es la tabla en la que basarse para el sistema de permisos.
    setDbTableName ( "asiento" );

    numDigitos = mainCompany() ->numDigitosEmpresa();
    m_codigoinicial->setMainCompany ( emp );
    /// Arreglamos la cuenta
    m_codigoinicial->setLabel ( _ ( "Cuenta inicial:" ) );
    m_codigoinicial->setTableName ( "cuenta" );
    m_codigoinicial->setFieldId("idcuenta");
    m_codigoinicial->m_valores["descripcion"] = "";
    m_codigoinicial->m_valores["codigo"] = "";
    m_codigoinicial->hideLabel();

    m_codigofinal->setMainCompany ( emp );
    /// Arreglamos la cuenta
    m_codigofinal->setLabel ( _ ( "Cuenta final:" ) );
    m_codigofinal->setTableName ( "cuenta" );
    m_codigofinal->setFieldId("idcuenta");
    m_codigofinal->m_valores["descripcion"] = "";
    m_codigofinal->m_valores["codigo"] = "";
    m_codigofinal->hideLabel();

    /// Inicializamos la tabla de nivel.
    combonivel->insertItem ( 0, "2" );
    combonivel->insertItem ( 1, "3" );
    combonivel->insertItem ( 2, "4" );
    combonivel->insertItem ( 3, "5" );
    combonivel->insertItem ( 4, "6" );
    combonivel->insertItem ( 5, "7" );
    combonivel->insertItem ( 6, "8" );
    combonivel->insertItem ( 7, "9" );
    combonivel->insertItem ( 8, "10" );
    combonivel->insertItem ( 9, "11" );
    combonivel->insertItem ( 10, "12" );
    combonivel->insertItem ( 11, "13" );
    combonivel->insertItem ( 12, "14" );
    combonivel->insertItem ( 13, "15" );
    combonivel->insertItem ( 14, "16" );
    combonivel->insertItem ( 15, "17" );


    /// Iniciamos los componentes de la fecha para que al principio aparezcan
    /// como el a&ntilde;o inicial.
    QString cadena;
    cadena.sprintf ( "%2.2d/%2.2d/%4.4d", 1, 1, QDate::currentDate().year() );
    m_fechainicial1->setText ( cadena );
    cadena.sprintf ( "%2.2d/%2.2d/%4.4d", 31, 12, QDate::currentDate().year() );
    m_fechafinal1->setText ( cadena );


    mainCompany() ->insertWindow ( windowTitle(), this );
    /// Llamamos a los scripts
    blScript(this);

    
}


///
/**
**/
BalanceView::~BalanceView()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void BalanceView::on_mui_actualizar_clicked()
{
    BL_FUNC_DEBUG
    accept();
    
}


/// Se encarga de inicializar la clase con los par&aacute;metros que se le han pasado.
/** Esta funci&ocute;n sirve para que desde fuera se pueda preparar a la clase para
    presentar un balance predeterminado. */
/**
\param codinicial
\param codfinal
\param fecha1
\param fecha2
\param idc_coste
**/
void BalanceView::inicializa1 ( QString codinicial, QString codfinal, QString fecha1, QString fecha2, QString idc_coste )
{
    BL_FUNC_DEBUG
    m_codigoinicial->setText ( codinicial );
    m_codigofinal->setText ( codfinal );
    m_fechainicial1->setText ( blNormalizeDate ( fecha1 ).toString ( "dd/MM/yyyy" ) );
    m_fechafinal1->setText ( blNormalizeDate ( fecha2 ).toString ( "dd/MM/yyyy" ) );
    
}


/// Se encarga de hacer la consulta en la base de datos y mostrar el resultado.
/** \bug Hay que eliminar el uso de double y usar un sistema de punto fijo.
    Crea una tabla auxiliar de balance y hace en ella todos los c&aacute;lculos necesarios
    para concretar los resultados. */
/**
**/
void BalanceView::presentar()
{
    BL_FUNC_DEBUG
    QString finicial = m_fechainicial1->text();
    QString ffinal = m_fechafinal1->text();
    QString cinicial = m_codigoinicial->fieldValue("codigo").left ( 2 );
    if ( cinicial == "" ) cinicial = "1";
    QString cfinal = m_codigofinal->fieldValue("codigo");
    if ( cfinal == "" ) cfinal = "9";
    int nivel = combonivel->currentText().toInt();
    bool jerarquico = checksuperiores->isChecked();

    /// A partir de ahora ya no hay tablas temporales ni accesos a disco que merman la
    /// ejecuci&oacute;n del programa.
    /// Se genera un &aacute;rbol din&aacute;mico en la memoria RAM que contendra todas
    /// y cada una de las cuentas del PGC con sus saldos.
    //if (sumasysaldosButton->isChecked()) {
    /// Balance de Sumas y Saldos.
    presentarSyS ( finicial, ffinal, cinicial, cfinal, nivel, 0, jerarquico );
    //} // end if
    
}


///
/**
\param finicial
\param ffinal
\param cinicial
\param cfinal
\param nivel
\param jerarquico
**/
void BalanceView::presentarSyS ( QString finicial, QString ffinal, QString cinicial, QString cfinal, int nivel, int, bool jerarquico )
{
    BL_FUNC_DEBUG
    BlFixed tsaldoant ( "0.00" ), tdebe ( "0.00" ), thaber ( "0.00" ), tsaldo ( "0.00" );
    BlDbRecordSet *ramas = NULL;
    BlDbRecordSet *hojas = NULL;

    try {
        /// Primero, averiguaremos la cantidad de ramas iniciales que nacen de la ra&iacute;z
        /// (tantas como n&uacute;mero de cuentas de nivel 2) y las vamos creando.
        mainCompany() ->begin();
        QString query = "SELECT *, nivel(codigo) AS nivel FROM cuenta ORDER BY codigo";

        ramas = mainCompany() ->loadQuery ( query );

        /// Creamos el arbol y lo inicializamos con todas las cuentas.
        BcPlanContableArbol *arbol;
        arbol = new BcPlanContableArbol;
        while ( !ramas->eof() ) {
            if ( ramas->value( "nivel" ).toInt() == 2 ) { /// Cuenta ra&iacute;z.
                /// Se crea una rama en el &aacute;rbol (de las primeritas que salen
                /// de una ra&iacute;z).
                arbol->nuevaRama ( ramas );
            } // end if
            ramas->nextRecord();
        } // end while
        arbol->inicializa ( ramas );
        delete ramas;

        /// Sacamos la subcadena para centros de coste y canales.
        // Consideraciones para centros de coste y canales
        QString cadand = "";
        BcCanalSeleccionarView *scanal = mainCompany() ->getselcanales();
        BcCentroCosteSeleccionarView *scoste = mainCompany() ->getselccostes();
        QString ccostes = scoste->cadcoste();
        if ( ccostes != "" ) {
            ccostes = " AND apunte.idc_coste IN (" + ccostes + ") ";
            cadand = " AND ";
        } // end if

        QString ccanales = scanal->cadCanal();
        if ( ccanales != "" ) {
            ccanales = cadand + " apunte.idcanal IN (" + ccanales + ") ";
        } // end if

        QString wherecostesycanales = ccostes + ccanales;

        /// Ahora, usaremos el &aacute;rbol para poner los datos a cada hoja (cuenta)
        /// seg&uacute;n los per&iacute;odos que necesitemos acotar.
        /// Para ello, vamos a recopilar todos los apuntes introducidos agrupados por cuenta
        /// para poder averiguar el estado contable de cada cuenta.
        query = "SELECT cuenta.idcuenta, numapuntes, cuenta.codigo, saldoant, debe, haber, ( COALESCE(saldop,0) + COALESCE(saldoant,0)) AS saldo, debeej, haberej, saldoej FROM ";

        query += "(SELECT idcuenta, codigo FROM cuenta) AS cuenta ";

        query += " LEFT JOIN (SELECT idcuenta, COALESCE(sum(debe),0) AS debeej, COALESCE(sum(haber),0) AS haberej, (COALESCE(sum(debe),0) - COALESCE(sum(haber),0)) AS saldoej FROM apunte WHERE EXTRACT(year FROM fecha) = '" + ffinal.right ( 4 ) + "' " + wherecostesycanales + " GROUP BY idcuenta) AS ejercicio ON ejercicio.idcuenta = cuenta.idcuenta";

        query += " LEFT OUTER JOIN (SELECT idcuenta, count(idcuenta) AS numapuntes, COALESCE(sum(debe),0) AS debe, COALESCE(sum(haber),0) AS haber, (COALESCE(sum(debe),0) - COALESCE(sum(haber),0)) AS saldop FROM apunte WHERE fecha >= '" + finicial + "' AND fecha <= '" + ffinal + "' " + wherecostesycanales + " GROUP BY idcuenta) AS periodo ON periodo.idcuenta = cuenta.idcuenta ";

        query += " LEFT OUTER JOIN (SELECT idcuenta, (COALESCE(sum(debe),0) - COALESCE(sum(haber),0)) AS saldoant FROM apunte WHERE  fecha < '" + finicial + "' " + wherecostesycanales +  " GROUP BY idcuenta) AS anterior ON cuenta.idcuenta = anterior.idcuenta ORDER BY codigo";


        /// Poblamos el &aacute;rbol de hojas (cuentas).
        hojas = mainCompany() ->loadQuery ( query );
        while ( !hojas->eof() ) {
            /// Para cada cuenta con apuntes introducidos hay que actualizar hojas del &aacute;rbol.
            arbol->actualizaHojas ( hojas );
            hojas->nextRecord();
        } // end while
        delete hojas;
        /// Establecemos la hoja activa
        arbol->inicia();

        /// Por fin, presentamos los valores.
        /// Aunque antes, prepararemos el Widget con la cabecera a mostrar.
        QStringList headers, datos;
        headers << _ ( "Codigo" ) << _ ( "Denominacion" ) << _ ( "Saldo anterior" ) << _ ( "Debe" ) << _ ( "Haber" ) << _ ( "Saldo" ) << _ ( "Debe " ) + ffinal.right ( 4 ) << _ ( "Haber " ) + ffinal.right ( 4 ) << _ ( "Saldo " ) + ffinal.right ( 4 );
        mui_list->setHeaderLabels ( headers );
        mui_list->clear();

        /// Ponemos las columnas
        mui_list->setColumnWidth ( CUENTA, 100 );
        mui_list->setColumnWidth ( DENOMINACION, 150 );
        mui_list->setColumnWidth ( SALDO_ANT, 100 );
        mui_list->setColumnWidth ( DEBE, 100 );
        mui_list->setColumnWidth ( HABER, 100 );
        mui_list->setColumnWidth ( SALDO, 100 );
        mui_list->setColumnWidth ( DEBEEJ, 100 );
        mui_list->setColumnWidth ( HABEREJ, 100 );
        mui_list->setColumnWidth ( SALDOEJ, 100 );

        /// Cada hoja del &aacute;rbol usara uno de estos Widgets para mostrarse.
        QTreeWidgetItem *it;
        int nivelActual;
        /// Mantenemos una tabla con &iacute;ndices de niveles del &aacute;rbol.
        QMap <int, QTreeWidgetItem *> ptrList;
        /// Y el iterador para controlar donde accedemos, as&iacute; como un &iacute;ndice
        /// adicional.
        QMap <int, QTreeWidgetItem *>::const_iterator ptrIt, i;
        ptrList.clear();


        while ( arbol->deshoja ( nivel, jerarquico ) ) {
            QString lcuenta = arbol->hojaActual ( "codigo" );
            QString ldenominacion = arbol->hojaActual ( "descripcion" );
            QString lsaldoant = arbol->hojaActual ( "saldoant" );
            QString ldebe = arbol->hojaActual ( "debe" );
            QString lhaber = arbol->hojaActual ( "haber" );
            QString lsaldo = arbol->hojaActual ( "saldo" );
            QString ldebeej = arbol->hojaActual ( "debeej" );
            QString lhaberej = arbol->hojaActual ( "haberej" );
            QString lsaldoej = arbol->hojaActual ( "saldoej" );

            if ( lcuenta >= cinicial and lcuenta <= cfinal ) {
                /// Acumulamos los totales para al final poder escribirlos.
                tsaldoant = tsaldoant + BlFixed ( lsaldoant );
                tsaldo = tsaldo + BlFixed ( lsaldo );
                tdebe = tdebe + BlFixed ( ldebe );
                thaber = thaber + BlFixed ( lhaber );

                /// Las variables de las filas en formato espa&ntilde;ol.
                lsaldoant = QString::number ( lsaldoant.toDouble(), 'f', 2 );
                ldebe = QString::number ( ldebe.toDouble(), 'f', 2 );
                lhaber = QString::number ( lhaber.toDouble(), 'f', 2 );
                lsaldo = QString::number ( lsaldo.toDouble(), 'f', 2 );
                ldebeej = QString::number ( ldebeej.toDouble(), 'f', 2 );
                lhaberej = QString::number ( lhaberej.toDouble(), 'f', 2 );
                lsaldoej = QString::number ( lsaldoej.toDouble(), 'f', 2 );

                /// Ahora, vamos a pintar en el Widget cada l&iacute;nea.
                /// Formamos la l&iacute;nea.
                datos << lcuenta << ldenominacion << lsaldoant << ldebe << lhaber << lsaldo << ldebeej << lhaberej << lsaldoej;

                /// Si se van mostrar tambi&eacute;n las cuentas superiores, habr&aacute; que
                /// jerarquizar el &aacute;rbol. Sino, se pinta cada l&iacute;nea al mismo nivel.
                if ( jerarquico ) { /// jerarquizando...
                    nivelActual = lcuenta.length();
                    if ( nivelActual == 2 ) {
                        /// La hoja cuelga de la ra&iacute;z principal.
                        it = new QTreeWidgetItem ( mui_list, datos );
                    } else {
                        while ( ptrIt.key() >= nivelActual )
                            /// Ascendemos por el &aacute;rbol para colgar la hoja en el
                            /// lugar correcto.
                            ptrIt--;
                        it = new QTreeWidgetItem ( ptrIt.value(), datos );
                    } // end if
                    /// Insertamos el widget (hoja actual) en la tabla controlada y obtenemos
                    /// su puntero iterador.
                    ptrIt = ptrList.insert ( nivelActual, it );
                    mui_list->setItemExpanded ( it, true );
                    /// Borramos el resto de niveles que cuelgan, para no seguir colgando por
                    /// esa rama.
                    i = ptrIt + 1;
                    while ( i != ptrList.constEnd() ) {
                        /// Borra todas las entradas con la misma clave.
                        ptrList.remove ( i.key() );
                        ++i;
                    } // end while
                } else { /// sin jerarquizar...
                    it = new QTreeWidgetItem ( mui_list, datos );
                } // end if
                datos.clear();

                /// Formateamos un poquito la informaci&oacute;n mostrada.
                int tamanyo = 10;
                if ( ldenominacion.length() > 40 ) {
                    tamanyo -= 1;
                } else if ( ldenominacion.length() > 50 ) {
                    tamanyo -= 2;
                } // end if
                it->setFont ( DENOMINACION, QFont ( "Serif", tamanyo, -1, false ) );
                for ( int col = 0; col < it->columnCount(); col++ ) {

                    if ( col == DEBE or col == HABER or col == SALDO ) {
                        it->setFont ( col, QFont ( "SansSerif", 10, QFont::DemiBold, false ) );
                    } else {
                        it->setFont ( col, QFont ( "SansSerif", 10, QFont::Normal, false ) );
                    } // end if
                    it->setTextAlignment ( col, Qt::AlignRight );

                    if ( lcuenta.size() == 2 ) {
                        it->setTextColor ( col, Qt::darkGray );
                    } else if ( lcuenta.size() < 5 ) {
                        it->setTextColor ( col, Qt::blue );
                    } else {
                        if ( it->text ( col ).left ( 1 ) == "-" ) {
                            it->setTextColor ( col, Qt::darkRed );
                        } else {
                            it->setTextColor ( col, Qt::black );
                        } // end if
                    } // end if
                } // end for
            } // end if
        } // end while
        mui_list->resizeColumnToContents ( 1 );

        /// Hacemos la actualizaci&oacute;n de los saldos totales en formato
        /// espa&ntilde;ol y los ponemos en su Widget.
        QString totsaldoant = tsaldoant.toQString();
        QString totdebe = tdebe.toQString();
        QString tothaber = thaber.toQString();
        QString totsaldo = tsaldo.toQString();
        totalsaldoant->setText ( totsaldoant );
        totaldebe->setText ( totdebe );
        totalhaber->setText ( tothaber );
        totalsaldo->setText ( totsaldo );

        /// Eliminamos el &aacute;rbol de la mem&oacute;ria y cerramos la conexi&oacute;n
        /// con la BD.
        delete arbol;
        mainCompany() ->commit();
    } catch ( ... ) {
        blMsgInfo ( "error en los calculos" );
        mainCompany() ->rollback();
        /// Libermos posible memoria utilizada
        if ( ramas ) delete ramas;
        if ( hojas ) delete hojas;
    } // end try
    
}


///
/**
**/
void BalanceView::accept()
{
    BL_FUNC_DEBUG
    presentar();
    
}



/// SLOT que responde a la pulsaci&oacute;n del bot&oacute;n de imprimir.
/** Crea el objeto \ref BalancePrintView lo inicializa con los mismos valores del
    balance y lo ejecuta en modo Modal. */
/**
**/
void BalanceView::imprimir()
{
    BL_FUNC_DEBUG
    BalancePrintView *balan = new BalancePrintView ( mainCompany() );
    balan->inicializa1 ( m_codigoinicial->fieldValue("codigo"), m_codigofinal->fieldValue("codigo"), m_fechainicial1->text(), m_fechafinal1->text(), FALSE );
    balan->exec();
    
}

