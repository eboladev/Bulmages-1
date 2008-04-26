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

#include <QPixmap>
#include <QMenu>
#include <QTextStream>
#include <busquedafecha.h>

#include "balance1view.h"
#include "listcuentasview1.h"
#include "empresa.h"
#include "busquedacuenta.h"
#include "asiento1view.h"

#include "selectcanalview.h"
#include "selectccosteview.h"

#include "blprogressbar.h"

/// Incluimos las imagenes que catalogan los tipos de cuentas.

#include "images/cactivo.xpm"
#include "images/cpasivo.xpm"
#include "images/cneto.xpm"
#include "images/cingresos.xpm"
#include "images/cgastos.xpm"


#define CUENTA         m_ccuenta
#define DENOMINACION   m_cdenominacion
#define SALDO_ANT      m_csaldo_ant
#define DEBE           m_cdebe
#define HABER          m_chaber
#define SALDO          m_csaldo
#define DEBEEJ         m_cdebeej
#define HABEREJ        m_chaberej
#define SALDOEJ        m_csaldoej
#define IDCUENTA       m_cidcuenta
#define NIVEL          m_cnivel
#define PADRE          m_cpadre


///
/**
\param emp
\param parent
**/
BalanceTreeView::BalanceTreeView ( Empresa *emp, QWidget *parent, int )
        : FichaBc ( emp, parent )
{
    setupUi ( this );
    _depura ( "BalanceTreeView::BalanceTreeView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setTitleName ( tr ( "Balance Jerarquico" ) );
    /// Establecemos cual es la tabla en la que basarse para los permisos
    setDBTableName ( "asiento" );

    numdigitos = empresaBase() ->numdigitosempresa();

    m_codigoinicial->setEmpresaBase ( emp );
    m_codigofinal->setEmpresaBase ( emp );

    m_codigoinicial->hideNombre();
    m_codigofinal->hideNombre();
    /// Hacemos la carga de los centros de coste. Rellenamos el combobox correspondiente.
//    mui_combocoste->setEmpresaBase ( emp );
//    mui_combocoste->setidc_coste ( "0" );

    listado->clear();

    listado->setColumnCount ( 12 );
    QStringList nombrecolumnas;
    nombrecolumnas << "J" << tr ( "Codigo cuenta" )
    << tr ( "Nombre de la cuenta" )
    << tr ( "Saldo anterior" )
    << tr ( "Debe" )
    << tr ( "Haber" )
    << tr ( "Saldo periodo" )
    << tr ( "Debe ejercicio" )
    << tr ( "Haber ejercicio" )
    << tr ( "Saldo ejercicio" )
    << tr ( "Nivel" )
    << tr ( "ID cuenta" )
    << tr ( "Padre" );

    listado->setHeaderLabels ( nombrecolumnas );

    CUENTA = 1;
    DENOMINACION = 2;
    SALDO_ANT = 3;
    DEBE = 4;
    HABER = 5;
    SALDO = 6;
    DEBEEJ = 7;
    HABEREJ =  8;
    SALDOEJ = 9;
    NIVEL = 10;
    IDCUENTA = 11;
    PADRE = 12;

    for ( int i = 2; i <= numdigitos; i++ ) {
        /// Inicializamos la tabla de nivel.
        combonivel->insertItem ( i, QString::number ( i ) );
    } // end for


    connect ( listado, SIGNAL ( contextMenuRequested ( QTreeWidgetItem *, const QPoint &, int ) ), this, SLOT ( contextmenu ( QTreeWidgetItem *, const QPoint &, int ) ) );
    connect ( combonivel, SIGNAL ( activated ( int ) ), this, SLOT ( nivelactivated ( int ) ) );

    /// Iniciamos los componentes de la fecha para que al principio aparezcan
    /// como el a&ntilde;o inicial.
    QString cadena;
    cadena.sprintf ( "%2.2d/%2.2d/%4.4d", 1, 1, QDate::currentDate().year() );
    m_fechainicial1->setText ( cadena );
    cadena.sprintf ( "%2.2d/%2.2d/%4.4d", 31, 12, QDate::currentDate().year() );
    m_fechafinal1->setText ( cadena );
    meteWindow ( windowTitle(), this, FALSE );
    _depura ( "END BalanceTreeView::BalanceTreeView", 0 );
}


///
/**
**/
BalanceTreeView::~BalanceTreeView()
{
    _depura ( "BalanceTreeView::~BalanceTreeView", 0 );
    sacaWindow();
    _depura ( "END BalanceTreeView::~BalanceTreeView", 0 );
}


/// Esta funci&oacute;n inicializa la ventana de extracto con los mismos datos
/// que la ventana de balance, cuentas, fechas y centros de coste
/// y pone la ventana de estracto como la ventana principal.
/// De esta forma cuando se pulsa sobre el bot&oacute;n extracto estando en la de balance
/// se cambia a la ventana de extracto de la forma correcta.
/// Si el parametro pasado es un:
/// 0 -> del d&iacute;a actual.
/// 1 -> del mes actual.
/// 2 -> del a&ntilde;o actual.
/**
\param tipo
**/
void BalanceTreeView::boton_extracto1 ( int tipo )
{
    _depura ( "BalanceTreeView::boton_extracto1", 0 );
    QDate fecha1, fecha2, fechaact, fechaact1;
    if ( !m_fechainicial1->text().isEmpty() ) {
        fechaact = normalizafecha ( m_fechainicial1->text() );
        fechaact1 = normalizafecha ( m_fechafinal1->text() );
        switch ( tipo ) {
        case 0:
            fecha1.setYMD ( fechaact.year(), fechaact.month(), fechaact.day() );
            fecha2.setYMD ( fechaact1.year(), fechaact1.month(), fechaact1.day() );
            break;
        case 1:
            fecha1.setYMD ( fechaact.year(), fechaact.month(), 1 );
            fecha2.setYMD ( fechaact.year(), fechaact.month(), fechaact.daysInMonth() );
            break;
        case 2:
            fecha1.setYMD ( fechaact.year(), 1, 1 );
            fecha2.setYMD ( fechaact.year(), 12, 31 );
            break;
        } // end switch
//        empresaBase() ->extractoempresa() ->inicializa1 ( listado->currentItem() ->text ( CUENTA ), listado->currentItem() ->text ( CUENTA ), fecha1.toString ( "dd/MM/yyyy" ), fecha2.toString ( "dd/MM/yyyy" ), mui_combocoste->idc_coste().toInt() );
    } // end if
    empresaBase() ->extractoempresa() ->accept();
    empresaBase() ->extractoempresa() ->show();
    empresaBase() ->extractoempresa() ->setFocus();
    _depura ( "END BalanceTreeView::boton_extracto1", 0 );
}


/// Si el par&aacute;metro pasado es un:
/// 0 -> del d&iacute;a actual.
/// 1 -> del mes actual.
/// 2 -> del a&ntilde;o actual.
/**
\param tipo
**/
void BalanceTreeView::boton_diario1 ( int tipo )
{
    _depura ( "BalanceTreeView::boton_diario1", 0 );
    QDate fecha1, fecha2, fechaact, fechaact1;
    if ( !m_fechainicial1->text().isEmpty() ) {
        fechaact = normalizafecha ( m_fechainicial1->text() );
        fechaact1 = normalizafecha ( m_fechafinal1->text() );
        switch ( tipo ) {
        case 0:
            fecha1.setYMD ( fechaact.year(), fechaact.month(), fechaact.day() );
            fecha2.setYMD ( fechaact1.year(), fechaact1.month(), fechaact1.day() );
            break;
        case 1:
            fecha1.setYMD ( fechaact.year(), fechaact.month(), 1 );
            fecha2.setYMD ( fechaact.year(), fechaact.month(), fechaact.daysInMonth() );
            break;
        case 2:
            fecha1.setYMD ( fechaact.year(), 1, 1 );
            fecha2.setYMD ( fechaact.year(), 12, 31 );
            break;
        } // end switch
        empresaBase() ->diarioempresa() ->inicializa1 ( fecha1.toString ( "dd/MM/yyyy" ), fecha2.toString ( "dd/MM/yyyy" ), 0 );
    } // end if
    empresaBase() ->diarioempresa() ->accept();
    empresaBase() ->diarioempresa() ->show();
    empresaBase() ->diarioempresa() ->setFocus();
    _depura ( "END BalanceTreeView::boton_diario1", 0 );
}


///
/**
**/
void BalanceTreeView::boton_asiento()
{
    _depura ( "BalanceTreeView::boton_asiento", 0 );
    empresaBase() ->intapuntsempresa() ->show();
    empresaBase() ->intapuntsempresa() ->setFocus();
    _depura ( "END BalanceTreeView::boton_asiento", 0 );
}


///
/**
\param codinicial
\param codfinal
\param fecha1
\param fecha2
\param idc_coste
**/
void BalanceTreeView::inicializa1 ( QString codinicial, QString codfinal, QString fecha1, QString fecha2, int idc_coste )
{
    _depura ( "BalanceTreeView::inicializa1", 0 );
    m_codigoinicial->setText ( codinicial );
    m_codigofinal->setText ( codfinal );
    m_fechainicial1->setText ( normalizafecha ( fecha1 ).toString ( "dd/MM/yyyy" ) );
    m_fechafinal1->setText ( normalizafecha ( fecha2 ).toString ( "dd/MM/yyyy" ) );
    /// Establecemos el centro de coste correspondiente.
//    mui_combocoste->setidc_coste ( QString::number ( idc_coste ) );
    _depura ( "END BalanceTreeView::inicializa1", 0 );
}


void BalanceTreeView::generarBalance()
{
    //QListViewItem1 * it;
    int num1;
    QString query;
    cursor2 *cursorapt;
    QString finicial = m_fechainicial1->text();
    QString ffinal = m_fechafinal1->text();
    QString cinicial = m_codigoinicial->codigocuenta();
    QString cfinal = m_codigofinal->codigocuenta();
    QString ejercicio = ffinal.right ( 4 );

    /// Hacemos la consulta de los apuntes a listar en la base de datos.
    // Consideraciones para centros de coste y canales
    selectcanalview *scanal = empresaBase() ->getselcanales();
    SelectCCosteView *scoste = empresaBase() ->getselccostes();
    QString ccostes = scoste->cadcoste();
    QString logicand = "";
    if ( ccostes != "" ) {
        ccostes = "AND apunte.idc_coste IN (" + ccostes + ") ";

    } // end if

    QString ccanales = scanal->cadcanal();
    if ( ccanales != "" ) {
        ccanales =  " AND apunte.idcanal IN (" + ccanales + ") ";
    } // end if

    QString clauswhere = ccostes + ccanales;


    /// La consulta es compleja, requiere la creaci&oacute;n de una tabla temporal y de
    /// cierta mandanga por lo que puede causar problemas con el motor de base de datos.
    query = "CREATE TEMPORARY TABLE balancetemp AS SELECT cuenta.idcuenta, codigo, nivel(codigo) AS nivel, cuenta.descripcion, padre, tipocuenta ,debe, haber, tdebe, thaber,(tdebe - thaber) AS tsaldo, (debe - haber) AS saldo, adebe, ahaber, (adebe - ahaber) AS asaldo, ejdebe, ejhaber, (ejdebe - ejhaber) AS ejsaldo FROM cuenta";
    query += " LEFT JOIN (SELECT idcuenta, sum(debe) AS tdebe, sum(haber) AS thaber FROM apunte WHERE fecha >= '" + finicial + "' AND fecha <= '" + ffinal + "' " + clauswhere + "  GROUP BY idcuenta) AS t1 ON t1.idcuenta = cuenta.idcuenta";
    query += " LEFT JOIN (SELECT idcuenta, sum(debe) AS adebe, sum(haber) AS ahaber FROM apunte WHERE fecha < '" + finicial + "' " + clauswhere + " GROUP BY idcuenta) AS t2 ON t2.idcuenta = cuenta.idcuenta";
    query += " LEFT JOIN (SELECT idcuenta, sum(debe) AS ejdebe, sum(haber) AS ejhaber FROM apunte WHERE EXTRACT (YEAR FROM fecha) = '" + ejercicio + "' " + clauswhere + " GROUP BY idcuenta) AS t3 ON t3.idcuenta = cuenta.idcuenta";

    empresaBase() ->begin();
    empresaBase() ->ejecuta ( query );
    query.sprintf ( "UPDATE balancetemp SET padre = 0 WHERE padre ISNULL" );
    empresaBase() ->ejecuta ( query );

    query.sprintf ( "DELETE FROM balancetemp WHERE debe = 0 AND haber = 0" );
    empresaBase() ->ejecuta ( query );
    /// Vamos a implementar el tema del c&oacute;digo.
    if ( cinicial != "" ) {
        query.sprintf ( "DELETE FROM balancetemp WHERE codigo < '%s'", cinicial.toAscii().constData() );
        empresaBase() ->ejecuta ( query );
    } // end if
    if ( cfinal != "" ) {
        query.sprintf ( "DELETE FROM balancetemp WHERE codigo > '%s'", cfinal.toAscii().constData() );
        empresaBase() ->ejecuta ( query );
    } // end if

    /// Para evitar problemas con los nulls hacemos algunos updates.
    query.sprintf ( "UPDATE balancetemp SET tsaldo = 0 WHERE tsaldo ISNULL" );
    empresaBase() ->ejecuta ( query );
    query.sprintf ( "UPDATE balancetemp SET tdebe = 0 WHERE tdebe ISNULL" );
    empresaBase() ->ejecuta ( query );
    query.sprintf ( "UPDATE balancetemp SET thaber = 0 WHERE thaber ISNULL" );
    empresaBase() ->ejecuta ( query );
    query.sprintf ( "UPDATE balancetemp SET asaldo = 0 WHERE asaldo ISNULL" );
    empresaBase() ->ejecuta ( query );
    query.sprintf ( "UPDATE balancetemp SET ejsaldo = 0 WHERE ejsaldo ISNULL" );
    empresaBase() ->ejecuta ( query );
    query.sprintf ( "UPDATE balancetemp SET ejdebe = 0 WHERE ejdebe ISNULL" );
    empresaBase() ->ejecuta ( query );
    query.sprintf ( "UPDATE balancetemp SET ejhaber = 0 WHERE ejhaber ISNULL" );
    empresaBase() ->ejecuta ( query );

    /// Cargamos el balance temporal
    query.sprintf ( "SELECT idcuenta FROM balancetemp ORDER BY padre DESC" );
    cursorapt = empresaBase() ->cargacursor ( query );
    while ( !cursorapt->eof() ) {
        query.sprintf ( "SELECT * FROM balancetemp WHERE idcuenta = %s", cursorapt->valor ( "idcuenta" ).toAscii().constData() );
        cursor2 *mycur = empresaBase() ->cargacursor ( query );
        if ( !mycur->eof() ) {
            query = "UPDATE balancetemp SET tsaldo = tsaldo + " + mycur->valor ( "tsaldo" ) + ", tdebe = tdebe + " + mycur->valor ( "tdebe" ) + ", thaber = thaber +" + mycur->valor ( "thaber" ) + ", asaldo = asaldo+" + mycur->valor ( "asaldo" ) + ", ejdebe = ejdebe + " + mycur->valor ( "ejdebe" ) + ", ejhaber = ejhaber + " + mycur->valor ( "ejhaber" ) + ", ejsaldo = ejsaldo + " + mycur->valor ( "ejsaldo" ) + " WHERE idcuenta = " + mycur->valor ( "padre" );
            empresaBase() ->ejecuta ( query );
        } /// end if
        delete mycur;
        cursorapt->siguienteregistro();
    } // end while
    delete cursorapt;


    query.sprintf ( "DELETE FROM balancetemp WHERE tdebe = 0 AND thaber = 0" );
    empresaBase() ->ejecuta ( query );

    if ( !mui_jerarquico->isChecked() ) {
        query = "DELETE FROM balancetemp WHERE nivel <> " + combonivel->currentText();
        empresaBase()->ejecuta ( query );
    } // end if

}

///
/**
**/
void BalanceTreeView::presentar()
{
    _depura ( "BalanceTreeView::presentar", 0 );
    QString query;
    int num1;
    QTreeWidgetItem *it;
    double tsaldoant = 0, tdebe = 0, thaber = 0, tsaldo = 0;
    generarBalance();

    /// Barra de progreso pq el tema tarda.
    BLProgressBar barra;
    barra.setText ( tr ( "Generando Balance" ) );
    barra.show();
    barra.setValue ( 0 );

    query = "SELECT * FROM balancetemp WHERE debe <> 0  OR haber <> 0 ORDER BY padre, codigo";
    cursor2 *cursorapt1 = empresaBase() ->cargacursor ( query );
    barra.setRange ( 0, cursorapt1->numregistros() );
    /// Calculamos cuantos registros van a crearse y dimensionamos la tabla.
    num1 = cursorapt1->numregistros();
    listado->clear();
    while ( !cursorapt1->eof() ) {

        QString padre1 = cursorapt1->valor ( "padre" );

        QTreeWidgetItem *padre = NULL;
        if ( ! listado->findItems ( padre1, Qt::MatchRecursive | Qt::MatchCaseSensitive, IDCUENTA ).isEmpty() )
            padre = listado->findItems ( padre1, Qt::MatchRecursive | Qt::MatchCaseSensitive, IDCUENTA ).first();

        /// Si hemos encontrado el padre de la lista lo ponemos, si no lo hemos encontrado
        /// no lo ponemos.
        if ( padre != NULL ) {
            it = new QTreeWidgetItem ( padre );
        } else {
            it = new QTreeWidgetItem ( listado );
        } // end if

        /// Acumulamos los totales para al final poder escribirlos.
        if ( cursorapt1->valor ( "nivel" ) == combonivel->currentText() ) {
            tsaldoant += atof ( cursorapt1->valor ( "asaldo" ).toAscii() );
            tsaldo += atof ( cursorapt1->valor ( "tsaldo" ).toAscii() );
            tdebe += atof ( cursorapt1->valor ( "tdebe" ).toAscii() );
            thaber += atof ( cursorapt1->valor ( "thaber" ).toAscii() );
        } // end if

        it->setText ( CUENTA, cursorapt1->valor ( "codigo" ) );
        if ( cursorapt1->valor ( "tipocuenta" ) == "1" ) {
            it->setIcon ( CUENTA, QIcon ( QPixmap ( cactivo ) ) );
        } else if ( cursorapt1->valor ( "tipocuenta" ) == "2" ) {
            it->setIcon ( CUENTA, QIcon ( QPixmap ( cpasivo ) ) );
        } else if ( cursorapt1->valor ( "tipocuenta" ) == "3" ) {
            it->setIcon ( CUENTA, QIcon ( QPixmap ( cneto ) ) );
        } else if ( cursorapt1->valor ( "tipocuenta" ) == "4" ) {
            it->setIcon ( CUENTA, QIcon ( QPixmap ( cingresos ) ) );
        } else if ( cursorapt1->valor ( "tipocuenta" ) == "5" ) {
            it->setIcon ( CUENTA, QIcon ( QPixmap ( cgastos ) ) );
        } // end if

        it->setText ( DENOMINACION, cursorapt1->valor ( "descripcion" ) );
        it->setText ( SALDO_ANT, QString::number ( cursorapt1->valor ( "asaldo" ).toDouble(), 'f', 2 ) );
        it->setText ( DEBE, QString::number ( cursorapt1->valor ( "tdebe" ).toDouble(), 'f', 2 ) );
        it->setText ( HABER, QString::number ( cursorapt1->valor ( "thaber" ).toDouble(), 'f', 2 ) );
        it->setText ( SALDO, QString::number ( cursorapt1->valor ( "tsaldo" ).toDouble(), 'f', 2 ) );
        it->setText ( DEBEEJ, QString::number ( cursorapt1->valor ( "ejdebe" ).toDouble(), 'f', 2 ) );
        it->setText ( HABEREJ, QString::number ( cursorapt1->valor ( "ejhaber" ).toDouble(), 'f', 2 ) );
        it->setText ( SALDOEJ, QString::number ( cursorapt1->valor ( "ejsaldo" ).toDouble(), 'f', 2 ) );

        it->setText ( NIVEL, cursorapt1->valor ( "nivel" ) );
        it->setText ( IDCUENTA, cursorapt1->valor ( "idcuenta" ) );
        it->setText ( PADRE, cursorapt1->valor ( "padre" ) );

        /// Formateamos un poquito la informaci&oacute;n mostrada.

        /// Establecemos los alineados del nuevo elemento creado.



        for ( int col = 0; col < it->columnCount(); col++ ) {

            if ( col == DEBE || col == HABER || col == SALDO ||  col == DEBEEJ || col == HABEREJ || col == SALDOEJ ) {
                it->setFont ( col, QFont ( "SansSerif", 10, QFont::DemiBold, false ) );
            } else {
                it->setFont ( col, QFont ( "SansSerif", 10, QFont::Normal, false ) );
            } // end if

            if ( cursorapt1->valor ( "nivel" ).toInt() == 2 ) {
                it->setTextColor ( col, Qt::black );
                it->setFont ( col, QFont ( "SansSerif", 12, QFont::Bold, false ) );
            } else if ( cursorapt1->valor ( "nivel" ).toInt() < 5 ) {
                it->setTextColor ( col, Qt::blue );
            } else  {

                if ( col == DEBE || col == HABER || col == DEBEEJ || col == HABEREJ ) {
                    it->setTextColor ( col, Qt::darkGray );
                } else if ( col == SALDO || col == SALDOEJ )  {
                    it->setTextColor ( col, Qt::darkGreen );
                    if ( it->text ( col ).left ( 1 ) == "-" ) {
                        it->setTextColor ( col, Qt::darkRed );
                    } // end if

                } else {
                    it->setFont ( col, QFont ( "SansSerif", 10, QFont::Normal, false ) );
                    it->setTextColor ( col, Qt::darkGray );
                } // end if

            } // end if
        } // end for

        int tamanyo = 10;
        if ( cursorapt1->valor ( "descripcion" ).length() > 40 ) {
            tamanyo -= 1;
        } else if ( cursorapt1->valor ( "descripcion" ).length() > 50 ) {
            tamanyo -= 2;
        } // end if
        it->setFont ( DENOMINACION, QFont ( "Serif", tamanyo, -1, false ) );

        it->setTextAlignment ( CUENTA, Qt::AlignLeft );
        it->setTextAlignment ( DENOMINACION, Qt::AlignLeft );
        it->setTextAlignment ( SALDO_ANT, Qt::AlignRight );
        it->setTextAlignment ( DEBE, Qt::AlignRight );
        it->setTextAlignment ( HABER, Qt::AlignRight );
        it->setTextAlignment ( SALDO, Qt::AlignRight );
        it->setTextAlignment ( DEBEEJ, Qt::AlignRight );
        it->setTextAlignment ( HABEREJ, Qt::AlignRight );
        it->setTextAlignment ( SALDOEJ, Qt::AlignRight );
        it->setTextAlignment ( IDCUENTA, Qt::AlignRight );
        it->setTextAlignment ( NIVEL, Qt::AlignRight );
        it->setTextAlignment ( PADRE, Qt::AlignRight );



        cursorapt1->siguienteregistro();
        barra.setValue ( barra.value() + 1 );
    } // end while

    /// Vaciamos el cursor de la base de datos.
    delete cursorapt1;

    /// Expandimos el listado.
    listado->expandAll();

    /// Eliminamos la tabla temporal y cerramos la transacci&oacute;n.
    query.sprintf ( "DROP TABLE balancetemp" );
    empresaBase() ->ejecuta ( query );
    empresaBase() ->commit();

    /// Hacemos la actualizacion de los saldos totales.
    totalsaldoant->setText ( QString::number ( tsaldoant, 'f', 2 ) );
    totaldebe->setText ( QString::number ( tdebe, 'f', 2 ) );
    totalhaber->setText ( QString::number ( thaber, 'f', 2 ) );
    totalsaldo->setText ( QString::number ( tsaldo, 'f', 2 ) );

    /// Activamos la parte de nivel para que se filtre el listado.
    nivelactivated ( combonivel->currentIndex() );
    _depura ( "END BalanceTreeView::presentar", 0 );
}


///
/**
\param nivel
**/
void BalanceTreeView::nivelactivated ( int nivel )
{
    _depura ( "BalanceTreeView::nivelactivated", 0 );
    combonivel->setCurrentIndex ( nivel );
    int nivel1 = combonivel->currentText().toInt();

    int j = 0;
    while ( j < listado->topLevelItemCount() ) {
        nivelactivated1 ( nivel1, listado->topLevelItem ( j ) );
        j++;
    } // end while
    _depura ( "END BalanceTreeView::nivelactivated", 0 );
}


///
/**
\param ot
**/
void BalanceTreeView::nivelactivated1 ( int nivel, QTreeWidgetItem *ot )
{
    _depura ( "BalanceTreeView::nivelactivated1", 0 );
    if ( ot ) {
        if ( ot->text ( NIVEL ).toInt()  < nivel ) {
            ot->treeWidget() ->setItemExpanded ( ot, TRUE );
            ot->treeWidget() ->expandItem ( ot );
        } else {
            ot->treeWidget() ->setItemExpanded ( ot, FALSE );
            ot->treeWidget() ->collapseItem ( ot );
        } // end if

        for ( int i = 0; i < ot->childCount(); i++ ) {
            nivelactivated1 ( nivel, ot->child ( i ) );
        } // end for

    } // end if
    _depura ( "END BalanceTreeView::nivelactivated1", 0 );
}


/// SLOT que responde a la petici&oacute;n de un men&uacute; contextual para un elemento
/// del balance.
/** @param poin Punto en el que se ha hecho la pulsaci&oacute;n del rat&oacute;n y que,
    por tanto, es donde queremos hacer aparecer el men&uacute; contextual.
    Creamos el objeto QPopupMenu con las opciones que queremos que aparezcan.
    Lo invocamos y seg&uacute;n la opci&oacute;n que haya elegido el usuario llamamos
    a la funci&oacute;n que da respuesta a dicha petici&oacute;n. */
void BalanceTreeView::contextmenu ( const QPoint &point )
{
    _depura ( "BalanceTreeView::contextmenu", 0 );

    QMenu *menupopup = new QMenu ( this );
    QAction *opt1 = menupopup->addAction ( tr ( "Ver Diario (este dia)" ) );
    QAction *opt2 = menupopup->addAction ( tr ( "Ver Diario (este mes)" ) );
    QAction *opt3 = menupopup->addAction ( tr ( "Ver Diario (este anyo)" ) );
    menupopup->addSeparator();
    QAction *opt4 = menupopup->addAction ( tr ( "Ver extracto (este dia)" ) );
    QAction *opt5 = menupopup->addAction ( tr ( "Ver extracto (este mes)" ) );
    QAction *opt6 = menupopup->addAction ( tr ( "Ver extracto (este anyo)" ) );
    QAction *opcion = menupopup->exec ( point );

    if ( opcion == opt1 ) {
        boton_diario1 ( 0 );
    } // end if
    if ( opcion == opt2 ) {
        boton_diario1 ( 1 );
    } // end if
    if ( opcion == opt3 ) {
        boton_diario1 ( 2 );
    } // end if
    if ( opcion == opt4 ) {
        boton_extracto1 ( 0 );
    } // end if
    if ( opcion == opt5 ) {
        boton_extracto1 ( 1 );
    } // end if
    if ( opcion == opt6 ) {
        boton_extracto1 ( 2 );
    } // end if

    delete menupopup;
    _depura ( "END BalanceTreeView::contextmenu", 0 );
}


/// SLOT que responde a la pulsaci&oacute;n del bot&oacute;n de imprimir.
/** Crea el objeto \ref BalancePrintView lo inicializa con los mismos valores del balance
    y lo ejecuta en modo Modal. */
/**
**/
void BalanceTreeView::imprimir()
{
    _depura ( "BalanceTreeView::on_mui_imprimir_clicked", 0 );
    QString query;
    generarBalance();

    generaRML ( "balance.rml" );
    invocaPDF ( "balance" );

    /// Eliminamos la tabla temporal y cerramos la transacci&oacute;n.
    query = "DROP TABLE balancetemp" ;
    empresaBase() ->ejecuta ( query );
    empresaBase() ->commit();

    _depura ( "END BalanceTreeView::on_mui_imprimir_clicked", 0 );
}


///
/**
**/
void BalanceTreeView::on_mui_actualizar_clicked()
{
    _depura ( "BalanceTreeView::on_mui_actualizar_clicked", 0 );
    presentar();
    _depura ( "END BalanceTreeView::on_mui_actualizar_clicked", 0 );
}

///
/**
**/
void BalanceTreeView::on_mui_hojacalculo_clicked()
{
    _depura ( "BalanceTreeView::on_mui_hojacalculo_clicked", 0 );

    QString query;
    int num1;
    QTreeWidgetItem *it;
    double tsaldoant = 0, tdebe = 0, thaber = 0, tsaldo = 0;
    generarBalance();


// =================================================================

    QString archivod = confpr->valor ( CONF_DIR_USER ) + "listadosxc.perl";
    SubForm3 * subf = ( SubForm3 * ) parent();

    QString fitxersortidatxt = "";

    fitxersortidatxt += "#!/usr/bin/perl\n";
    fitxersortidatxt += "# Balance Export\n";
    fitxersortidatxt += "\n";
    fitxersortidatxt += "use ooolib;\n";
    fitxersortidatxt += "use utf8;\n";
    fitxersortidatxt += "\n";
    fitxersortidatxt += "# Set variables\n";
    fitxersortidatxt += "$doc = new ooolib(\"sxc\");\n";
    fitxersortidatxt += "$doc->oooSet(\"builddir\", \".\");\n";
    fitxersortidatxt += "$doc->oooSet(\"title\", \"SubFormulario Exportado\");\n";
    fitxersortidatxt += "$doc->oooSet(\"subject\", \"SubFormulario\");\n";
    fitxersortidatxt += "$doc->oooSet(\"comments\", \"SubFormulario Exportado\");\n";
    fitxersortidatxt += "$doc->oooSet(\"author\", \"Tomeu Borras\");\n";
    fitxersortidatxt += "$doc->oooSet(\"meta1-name\", \"Programmer\");\n";
    fitxersortidatxt += "$doc->oooSet(\"meta1-value\", \"Tomeu Borras\");\n";
    fitxersortidatxt += "\n";

    fitxersortidatxt += "$doc->oooSet(\"cell-auto\", \"1,0\");\n";
    fitxersortidatxt += "$doc->oooSet(\"bold\", \"on\");\n";
    fitxersortidatxt += "$doc->oooSet(\"text-size\", \"18\");\n";
    fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 4, 1);\n";
    fitxersortidatxt += "$doc->oooData(\"cell-text\", \"BALANCE DE SITUACION\");\n";
    fitxersortidatxt += "$doc->oooSet(\"text-size\", \"10\");\n";
    fitxersortidatxt += "$doc->oooSet(\"bold\", \"off\");\n";

/// Generamos el query
    query = "SELECT * FROM balancetemp WHERE debe <> 0  OR haber <> 0 ORDER BY padre, codigo";
    cursor2 *cursorapt1 = empresaBase() ->cargacursor ( query );

    int y = 1;
    int x = 1;
    /// Sacamos las cabeceras
    for ( int h = 0; h < cursorapt1->numcampos(); h++ ) {

        fitxersortidatxt += "# Fila " + QString::number ( y ) + "\n";
        fitxersortidatxt += "$doc->oooSet(\"bold\", \"on\");\n";
        fitxersortidatxt += "$doc->oooSet(\"cell-loc\", " + QString::number ( x++ ) + ", " + QString::number ( y ) + ");\n";
        fitxersortidatxt += "$doc->oooData(\"cell-text\", \"" + parsearCode ( cursorapt1->nomcampo ( h ) ) + "\");\n";
        fitxersortidatxt += "$doc->oooSet(\"bold\", \"off\");\n";
        fitxersortidatxt += "\n";

    } // end for


    y += 2;

    /// Sacamos el contenido
    /// Calculamos cuantos registros van a crearse

    while ( !cursorapt1->eof() ) {

        int x = 1;
        for ( int j = 0; j < cursorapt1->numcampos(); j++ ) {
            fitxersortidatxt += "# Fila " + QString::number ( y ) + "\n";
            fitxersortidatxt += "$doc->oooSet(\"bold\", \"on\");\n";
            fitxersortidatxt += "$doc->oooSet(\"cell-loc\", " + QString::number ( x++ ) + ", " + QString::number ( y ) + ");\n";
            fitxersortidatxt += "$doc->oooData(\"cell-text\", \"" + parsearCode ( cursorapt1->valor ( j ) ) + "\");\n";
            fitxersortidatxt += "$doc->oooSet(\"bold\", \"off\");\n";
            fitxersortidatxt += "\n";

        } // end for
        y++;
        cursorapt1->siguienteregistro();
    } // end while
    /// Vaciamos el cursor de la base de datos.
    delete cursorapt1;

    /// Eliminamos la tabla temporal y cerramos la transacci&oacute;n.
    query.sprintf ( "DROP TABLE balancetemp" );
    empresaBase() ->ejecuta ( query );
    empresaBase() ->commit();

    fitxersortidatxt += "my($filename) = $doc->oooGenerate(\"listadosxc.sxc\");\n";
    fitxersortidatxt += "\n";
    fitxersortidatxt += "exit;\n";

    QString cadena = "rm " + confpr->valor ( CONF_DIR_USER ) + "listadosxc.sxc";
    system ( cadena.toAscii() );
    cadena = "rm " + archivod;
    system ( cadena.toAscii() );

    QFile file ( archivod );
    if ( file.open ( QIODevice::WriteOnly ) )  {
        QTextStream stream ( &file );
        stream.setCodec ( "UTF-8" );
        stream << fitxersortidatxt;
        file.close();
    } // end if

    cadena = " cd " + confpr->valor ( CONF_DIR_USER ) + "; perl " + archivod;
    system ( cadena.toAscii() );
    cadena = "kspread " + confpr->valor ( CONF_DIR_USER ) + "listadosxc.sxc &";
    system ( cadena.toAscii() );

    _depura ( "END BalanceTreeView::on_mui_hojacalculo_clicked", 0 );
}
