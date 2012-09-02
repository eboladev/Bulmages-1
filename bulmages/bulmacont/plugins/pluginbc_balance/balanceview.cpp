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

    setAttribute(Qt::WA_DeleteOnClose);
    setTitleName ( _ ( "Balance" ) );
    /// Establezco cual es la tabla en la que basarse para el sistema de permisos.
    setDbTableName ( "asiento" );
    
    /// Para imprimir usaremos la plantilla balance
    setTemplateName("balance");

    mui_cuentaInicial->setMainCompany ( emp );
    /// Arreglamos la cuenta.
    mui_cuentaInicial->setLabel ( _ ( "Cuenta inicial:" ) );
    mui_cuentaInicial->setTableName ( "cuenta" );
    mui_cuentaInicial->setFieldId("idcuenta");
    mui_cuentaInicial->m_valores["descripcion"] = "";
    mui_cuentaInicial->m_valores["codigo"] = "";
    mui_cuentaInicial->hideLabel();

    mui_cuentaFinal->setMainCompany ( emp );
    /// Arreglamos la cuenta.
    mui_cuentaFinal->setLabel ( _ ( "Cuenta final:" ) );
    mui_cuentaFinal->setTableName ( "cuenta" );
    mui_cuentaFinal->setFieldId("idcuenta");
    mui_cuentaFinal->m_valores["descripcion"] = "";
    mui_cuentaFinal->m_valores["codigo"] = "";
    mui_cuentaFinal->hideLabel();

    QString query = "select distinct length(codigo) AS orden from cuenta ORDER BY orden DESC";
    BlDbRecordSet *niveles = NULL;
    /// Primero, averiguaremos la cantidad de ramas iniciales que nacen de la ra&iacute;z
    /// (tantas como n&uacute;mero de cuentas de nivel 2) y las vamos creando.
    niveles = mainCompany() ->loadQuery ( query );

    while ( !niveles->eof() ) {
	mui_nivel->insertItem ( 0, niveles->value("orden") );
	niveles->nextRecord();
    } // end while
    delete niveles;

    /// Iniciamos los componentes de la fecha para que al principio aparezcan
    /// como el a&ntilde;o inicial.
    QString cadena;
    cadena.sprintf ( "%2.2d/%2.2d/%4.4d", 1, 1, QDate::currentDate().year() );
    mui_fechaInicial->setText ( cadena );
    cadena.sprintf ( "%2.2d/%2.2d/%4.4d", 31, 12, QDate::currentDate().year() );
    mui_fechaFinal->setText ( cadena );

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
\param cuentaInicial
\param cuentaFinal
\param fechaInicial
\param fechaFinal
\param idCentroCoste
**/
void BalanceView::inicializa ( QString cuentaInicial, QString cuentaFinal, QString fechaInicial, QString fechaFinal, QString idCentroCoste )
{
    BL_FUNC_DEBUG
    mui_cuentaInicial->setText ( cuentaInicial );
    mui_cuentaFinal->setText ( cuentaFinal );
    mui_fechaInicial->setText ( blNormalizeDate ( fechaInicial ).toString ( "dd/MM/yyyy" ) );
    mui_fechaFinal->setText ( blNormalizeDate ( fechaFinal ).toString ( "dd/MM/yyyy" ) );
}


/// Se encarga de hacer la consulta en la base de datos y mostrar el resultado.
/** \bug Hay que eliminar el uso de double y usar un sistema de punto fijo.
    Crea una tabla auxiliar de balance y hace en ella todos los c&aacute;lculos necesarios
    para concretar los resultados.
**/
void BalanceView::presentar()
{
    BL_FUNC_DEBUG
    QString fechaInicial = mui_fechaInicial->text();
    QString fechaFinal = mui_fechaFinal->text();
    QString cuentaInicial = mui_cuentaInicial->fieldValue("codigo").left ( 2 );

    if ( cuentaInicial == "" ) cuentaInicial = "1";

    QString cuentaFinal = mui_cuentaFinal->fieldValue("codigo");

    if ( cuentaFinal == "" ) cuentaFinal = "9";

    int nivel = mui_nivel->currentText().toInt();
    bool jerarquico = mui_balanceJerarquico->isChecked();

    /// Balance de Sumas y Saldos.
    presentarSyS ( fechaInicial, fechaFinal, cuentaInicial, cuentaFinal, nivel, 0, jerarquico );
}


///
/**
\param fechaInicial
\param fechaFinal
\param cuentaInicial
\param cuentaFinal
\param nivel
\param jerarquico
**/
void BalanceView::presentarSyS ( QString fechaInicial, QString fechaFinal, QString cuentaInicial, QString cuentaFinal, int nivel, int, bool jerarquico )
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
        BcCanalSeleccionarView *scanal = mainCompany() ->getselcanales();
        BcCentroCosteSeleccionarView *scoste = mainCompany() ->getselccostes();
        QString ccostes = scoste->cadcoste();
        if ( ccostes != "" ) {
            ccostes = " AND apunte.idc_coste IN (" + ccostes + ") ";
 ;
        } // end if

        QString ccanales = scanal->cadCanal();
        if (scanal->sinCanal()) {
	  if ( ccanales != "" ) {
	      ccanales = " AND (apunte.idcanal ISNULL OR apunte.idcanal IN (" + ccanales + ")) ";
	  } else {
	      ccanales = " AND apunte.idcanal ISNULL ";	    
	  } // end if
	} else {
	  if ( ccanales != "" ) {
	      ccanales = " AND (apunte.idcanal <> NULL OR apunte.idcanal IN (" + ccanales + ")) ";
	  } else {
	      ccanales = " AND apunte.idcanal <> NULL ";	    
	  } // end if
	} // end if

        QString wherecostesycanales = ccostes + ccanales;

        /// Ahora, usaremos el &aacute;rbol para poner los datos a cada hoja (cuenta)
        /// seg&uacute;n los per&iacute;odos que necesitemos acotar.
        /// Para ello, vamos a recopilar todos los apuntes introducidos agrupados por cuenta
        /// para poder averiguar el estado contable de cada cuenta.
        query = "SELECT cuenta.idcuenta, numapuntes, cuenta.codigo, saldoant, debe, haber, ( COALESCE(saldop,0) + COALESCE(saldoant,0)) AS saldo, debeej, haberej, saldoej FROM ";

        query += "(SELECT idcuenta, codigo FROM cuenta) AS cuenta ";

        query += " LEFT JOIN (SELECT idcuenta, COALESCE(sum(debe),0) AS debeej, COALESCE(sum(haber),0) AS haberej, (COALESCE(sum(debe),0) - COALESCE(sum(haber),0)) AS saldoej FROM apunte WHERE EXTRACT(year FROM fecha) = '" + fechaFinal.right ( 4 ) + "' " + wherecostesycanales + " GROUP BY idcuenta) AS ejercicio ON ejercicio.idcuenta = cuenta.idcuenta";

        query += " LEFT OUTER JOIN (SELECT idcuenta, count(idcuenta) AS numapuntes, COALESCE(sum(debe),0) AS debe, COALESCE(sum(haber),0) AS haber, (COALESCE(sum(debe),0) - COALESCE(sum(haber),0)) AS saldop FROM apunte WHERE fecha >= '" + fechaInicial + "' AND fecha <= '" + fechaFinal + "' " + wherecostesycanales + " GROUP BY idcuenta) AS periodo ON periodo.idcuenta = cuenta.idcuenta ";

        query += " LEFT OUTER JOIN (SELECT idcuenta, (COALESCE(sum(debe),0) - COALESCE(sum(haber),0)) AS saldoant FROM apunte WHERE  fecha < '" + fechaInicial + "' " + wherecostesycanales +  " GROUP BY idcuenta) AS anterior ON cuenta.idcuenta = anterior.idcuenta ORDER BY codigo";


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
        headers << _ ( "Codigo" ) << _ ( "Denominacion" ) << _ ( "Saldo anterior" ) << _ ( "Debe" ) << _ ( "Haber" ) << _ ( "Saldo" ) << _ ( "Debe " ) + fechaFinal.right ( 4 ) << _ ( "Haber " ) + fechaFinal.right ( 4 ) << _ ( "Saldo " ) + fechaFinal.right ( 4 );
        mui_list->setHeaderLabels ( headers );
        mui_list->clear();

        /// Ponemos las columnas

        mui_list->setColumnWidth ( CUENTA, 250 );
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

            if ( lcuenta >= cuentaInicial and lcuenta <= cuentaFinal ) {
                /// Acumulamos los totales para al final poder escribirlos.
                /// Solo si son del nivel seleccionado o son hojas finales ya que en un jerarquico no se puede acumular
                /// TODO: No funciona bien en todos los niveles. Principalmente si el arbol no es normalizado
                if (lcuenta.length() == nivel) {
		    tsaldoant = tsaldoant + BlFixed ( lsaldoant );
		    tsaldo = tsaldo + BlFixed ( lsaldo );
		    tdebe = tdebe + BlFixed ( ldebe );
		    thaber = thaber + BlFixed ( lhaber );
	        } // end if

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

		    if ( col == DEBE || col == HABER || col == SALDO ||  col == DEBEEJ || col == HABEREJ || col == SALDOEJ ) {
                        it->setFont ( col, QFont ( "SansSerif", 9, QFont::DemiBold, false ) );
                    } else {
                        it->setFont ( col, QFont ( "SansSerif", 9, QFont::Normal, false ) );
                    } // end if
                    it->setTextAlignment ( col, Qt::AlignRight );

                    if ( lcuenta.size() == 2 ) {
                        it->setTextColor ( col, Qt::darkGray );
			it->setFont ( col, QFont ( "SansSerif", 10, QFont::Bold, false ) );
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
        mui_totalSaldoAnterior->setText ( totsaldoant );
        mui_totalDebe->setText ( totdebe );
        mui_totalHaber->setText ( tothaber );
        mui_totalSaldo->setText ( totsaldo );

        /// Eliminamos el &aacute;rbol de la mem&oacute;ria y cerramos la conexi&oacute;n
        /// con la BD.
        delete arbol;
        mainCompany() ->commit();
    } catch ( ... ) {
        blMsgInfo ( _("Error en los calculos.") );
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


