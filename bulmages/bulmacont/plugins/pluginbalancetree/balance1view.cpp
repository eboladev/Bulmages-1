/***************************************************************************
 *   Copyright (C) 2008 by Ricardo Diaz de la Calle                        *
 *   richard@galdi.es                                                      *
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
// #include <QLocale>

#include <busquedafecha.h>

#include "balance1view.h"
#include "listcuentasview1.h"
#include "bccompany.h"
#include "busquedacuenta.h"
#include "asiento1view.h"
#include "selectcanalview.h"
#include "selectccosteview.h"

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
// #define IDCUENTA       m_cidcuenta
// #define NIVEL          m_cnivel
// #define PADRE          m_cpadre


///
/**
\param emp
\param parent
**/
BalanceTreeView::BalanceTreeView ( BcCompany *emp, QWidget *parent, int )
        : FichaBc ( emp, parent )
{
    _depura ( "BalanceTreeView::BalanceTreeView", 0 );

    setupUi ( this );
    setAttribute ( Qt::WA_DeleteOnClose );
    setTitleName ( _( "Balance Jerarquico" ) );
    /// Establecemos cual es la tabla en la que basarse para los permisos
    setDbTableName ( "asiento" );

    unsigned int numdigitos = mainCompany() ->numdigitosempresa();

    m_codigoinicial->setMainCompany ( emp );
    m_codigofinal->setMainCompany ( emp );

    m_codigoinicial->hideNombre();
    m_codigofinal->hideNombre();
    /// Hacemos la carga de los centros de coste. Rellenamos el combobox correspondiente.
//    mui_combocoste->setMainCompany ( emp );
//    mui_combocoste->setidc_coste ( "0" );

    listado->clear();

    listado->setColumnCount ( 9 );
    QStringList nombrecolumnas;
    nombrecolumnas << _( "Cuenta" )
    << _( "Nombre de la cuenta" )
    << _( "Saldo anterior" )
    << _( "Debe" )
    << _( "Haber" )
    << _( "Saldo" )
    << _( "Debe ejercicio" )
    << _( "Haber ejercicio" )
    << _( "Saldo ejercicio" );
//     << _( "Nivel" )
//     << _( "ID Cuenta" )
//     << _( "ID Cuenta Padre" );

    listado->setHeaderLabels ( nombrecolumnas );

    CUENTA = 0;
    DENOMINACION = 1;
    SALDO_ANT = 2;
    DEBE = 3;
    HABER = 4;
    SALDO = 5;
    DEBEEJ = 6;
    HABEREJ =  7;
    SALDOEJ = 8;
//     NIVEL = 10;
//     IDCUENTA = 11;
//     PADRE = 12;

    for ( int i = numdigitos; i >= 2; i-- ) {
        /// Inicializamos la tabla de nivel.
        combonivel->insertItem ( i, QString::number ( i ) );
    } // end for


    connect ( listado, SIGNAL ( contextMenuRequested ( QTreeWidgetItem *, const QPoint &, int ) ), this, SLOT ( contextmenu ( QTreeWidgetItem *, const QPoint &, int ) ) );
//     connect ( combonivel, SIGNAL ( activated ( int ) ), this, SLOT ( nivelactivated ( int ) ) );

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
//        mainCompany() ->extractoempresa() ->inicializa1 ( listado->currentItem() ->text ( CUENTA ), listado->currentItem() ->text ( CUENTA ), fecha1.toString ( "dd/MM/yyyy" ), fecha2.toString ( "dd/MM/yyyy" ), mui_combocoste->idc_coste().toInt() );
    } // end if
    mainCompany() ->extractoempresa() ->accept();
    mainCompany() ->extractoempresa() ->show();
    mainCompany() ->extractoempresa() ->setFocus();
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
        mainCompany() ->diarioempresa() ->inicializa1 ( fecha1.toString ( "dd/MM/yyyy" ), fecha2.toString ( "dd/MM/yyyy" ), 0 );
    } // end if
    mainCompany() ->diarioempresa() ->accept();
    mainCompany() ->diarioempresa() ->show();
    mainCompany() ->diarioempresa() ->setFocus();
    _depura ( "END BalanceTreeView::boton_diario1", 0 );
}


///
/**
**/
void BalanceTreeView::boton_asiento()
{
    _depura ( "BalanceTreeView::boton_asiento", 0 );
    mainCompany() ->intapuntsempresa() ->show();
    mainCompany() ->intapuntsempresa() ->setFocus();
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


bool BalanceTreeView::generaBalance()
{
    _depura ( "BalanceTreeView::generaBalance", 0 );

    /// Vamos a crear una estructura tipo arbol (usando la clase Arbol).

    /// Primero, averiguaremos la cantidad de ramas iniciales (tantos como
    /// numero de cuentas de nivel 2) y las vamos creando.
    mainCompany() ->begin();
    QString query = "SELECT *, nivel(codigo) AS nivel FROM cuenta ORDER BY codigo";
    BlDbRecordSet *ramas;
    ramas = mainCompany() ->cargacursor ( query, "Ramas" );
    mainCompany() ->commit();
    if ( ramas == NULL ) {
        mensajeInfo ( _( "Error con la base de datos" ) );
        return 0;
    }

    arbol = new Arbol;
    while ( !ramas->eof() ) {
        if ( ramas->valor ( "nivel" ).toInt() == 2 )
            arbol->nuevaRama ( ramas );
        ramas->siguienteregistro();
    } // end while

    /// Inicializamos el arbol desde sus raices (desde sus cuentas de nivel 2)
    /// con el resto de cuentas (las hojas del arbol)
    arbol->inicializa ( ramas );

    /// Seguidamente, recopilamos todos los apuntes agrupados por cuenta para poder
    /// establecer asi los valores de cada cuenta.

    /// Hacemos la consulta de los apuntes a listar en la base de datos.
    /// Consideraciones para centros de coste y canales
    selectcanalview *scanal = mainCompany() ->getselcanales();
    SelectCCosteView *scoste = mainCompany() ->getselccostes();
    QString ccostes = scoste->cadcoste();
    QString logicand = "";
    if ( ccostes != "" ) {
        ccostes = "AND idc_coste IN (" + ccostes + ") ";

    } // end if
    QString ccanales = scanal->cadcanal();
    if ( ccanales != "" ) {
        ccanales =  " AND idcanal IN (" + ccanales + ") ";
    } // end if
    QString clauswhere = ccostes + ccanales;


    QString finicial = m_fechainicial1->text();
    if ( finicial == "" ) {
        mensajeInfo ( _( "Introduzca la Fecha Inicial" ) );
        return 0;
    }
    QString ffinal = m_fechafinal1->text();
    if ( ffinal == "" ) {
        mensajeInfo ( _( "Introduzca la Fecha Final" ) );
        return 0;
    }
    QString ejercicio = ffinal.right ( 4 );

    /// Cargamos las cuentas afectadas con sus saldos y demas cosas.
    query = "SELECT cuenta.idcuenta, numapuntes, cuenta.codigo, saldoant, debe, haber, saldo, debeej, haberej, saldoej FROM (SELECT idcuenta, codigo FROM cuenta) AS cuenta NATURAL JOIN (SELECT idcuenta, count(idcuenta) AS numapuntes, sum(debe) AS debeej, sum(haber) AS haberej, (sum(debe)-sum(haber)) AS saldoej FROM apunte WHERE EXTRACT(year FROM fecha) = '" + ejercicio + "' GROUP BY idcuenta) AS ejercicio LEFT OUTER JOIN (SELECT idcuenta,sum(debe) AS debe, sum(haber) AS haber, (sum(debe)-sum(haber)) AS saldo FROM apunte WHERE fecha >= '" + finicial + "' AND fecha <= '" + ffinal + "' AND conceptocontable !~* '.*asiento.*(cierre|regularizaci).*' " + clauswhere + " GROUP BY idcuenta) AS periodo ON periodo.idcuenta=ejercicio.idcuenta LEFT OUTER JOIN (SELECT idcuenta, (sum(debe)-sum(haber)) AS saldoant FROM apunte WHERE fecha < '" + finicial + "'" + clauswhere + " GROUP BY idcuenta) AS anterior ON cuenta.idcuenta=anterior.idcuenta ORDER BY codigo";

    BlDbRecordSet *hojas;
    hojas = mainCompany() ->cargacursor ( query);
    if ( hojas == NULL ) {
        mensajeInfo ( _( "Error con la base de datos" ) );
        return 0;
    }

    /// Para cada cuenta con sus saldos ya calculados hay que actualizar las hojas del arbol.
    while ( !hojas->eof() ) {
        arbol->actualizaHojas ( hojas );
        hojas->siguienteregistro();
    } // end while
    delete hojas;
    _depura ( "END BalanceTreeView::generaBalance", 0 );
    return 1;
}


///
/**
**/
void BalanceTreeView::presentar()
{
    _depura ( "BalanceTreeView::presentar", 0 );

    if ( generaBalance() ) {

        QTreeWidgetItem* it;
        QList <QTreeWidgetItem *> ptrList, ptrIt;
        for ( int i = 0; i <= mainCompany()->numdigitosempresa(); i++ )
            ptrIt << NULL; // Reservamos las posiciones de la lista que coincidiran con los niveles de cuentas
        BlFixed tsaldoant ( "0.00" ), tdebe ( "0.00" ), thaber ( "0.00" ), tsaldo ( "0.00" );
        QString cinicial = m_codigoinicial->codigocuenta().left ( 1 );
        if ( cinicial == "" ) cinicial = "10";
        QString cfinal = m_codigofinal->codigocuenta();
        unsigned int nivelAnt, nivelCta, nivel = combonivel->currentText().toInt();
        if ( cfinal == "" )
            cfinal = arbol->codigoCuentaMayor ( nivel );
        else
            cfinal = arbol->hijoMayor ( cfinal, nivel );
        bool jerarquico = mui_jerarquico->isChecked();
        QStringList datos;
        QLocale spain ( "es_ES" );

        listado->clear();
        arbol->inicia();
        while ( arbol->deshoja ( nivel, jerarquico ) ) {
            QString cuenta = arbol->hojaActual ( "codigo" );
            QString denominacion = arbol->hojaActual ( "descripcion" );
            QString saldoant = arbol->hojaActual ( "saldoant" );
            QString debe = arbol->hojaActual ( "debe" );
            QString haber = arbol->hojaActual ( "haber" );
            QString saldo = arbol->hojaActual ( "saldo" );
            QString debeej = arbol->hojaActual ( "debeej" );
            QString haberej = arbol->hojaActual ( "haberej" );
            QString saldoej = arbol->hojaActual ( "saldoej" );

            if ( cuenta >= cinicial and cuenta <= cfinal ) {
                /// Acumulamos los totales para al final poder escribirlos.
                if ( !jerarquico || ( jerarquico && cuenta.length() == 2 ) ) {
                    tsaldoant = tsaldoant + BlFixed ( saldoant );
                    tsaldo = tsaldo + BlFixed ( saldo );
                    tdebe = tdebe + BlFixed ( debe );
                    thaber = thaber + BlFixed ( haber );
                }

                /// Las variables de las filas en formato espanyol.
                saldoant = spain.toString ( saldoant.toDouble(), 'f', 2 );
                debe = spain.toString ( debe.toDouble(), 'f', 2 );
                haber = spain.toString ( haber.toDouble(), 'f', 2 );
                saldo = spain.toString ( saldo.toDouble(), 'f', 2 );
                debeej = spain.toString ( debeej.toDouble(), 'f', 2 );
                haberej = spain.toString ( haberej.toDouble(), 'f', 2 );
                saldoej = spain.toString ( saldoej.toDouble(), 'f', 2 );

                /// Ahora, vamos a pintar en el Widget cada linea.
                /// Formamos la l&iacute;nea.
                datos << cuenta << denominacion << saldoant << debe << haber << saldo << debeej << haberej << saldoej;

                /// Si se van mostrar tambien las cuentas superiores, habra que
                /// jerarquizar el arbol. Sino, se pinta cada linea al mismo nivel.
                if ( jerarquico ) { /// jerarquizando...
                    nivelCta = cuenta.length();
                    if ( nivelCta == 2 ) {
                        /// La hoja cuelga de la raiz principal.
                        it = new QTreeWidgetItem ( listado, datos );
                        ptrList.append ( it );
                    } else {
                        if ( nivelCta > nivelAnt )
                            it = new QTreeWidgetItem ( ptrIt.at ( nivelAnt ), datos );
                        else
                            it = new QTreeWidgetItem ( ptrIt.at ( nivelCta )->parent(), datos );
                    } // end if
                    ptrIt.replace ( nivelCta, it );
                    nivelAnt = nivelCta;
                } else { /// sin jerarquizar...
                    it = new QTreeWidgetItem ( listado, datos );
                } // end if
                datos.clear();

                /// Formateamos un poquito la informacion mostrada.
                /// Establecemos los alineados del nuevo elemento creado.

                for ( int col = 0; col < it->columnCount(); col++ ) {
                    if ( col == DEBE || col == HABER || col == SALDO ||  col == DEBEEJ || col == HABEREJ || col == SALDOEJ ) {
                        it->setFont ( col, QFont ( "SansSerif", 9, QFont::DemiBold, false ) );
                    } else {
                        it->setFont ( col, QFont ( "SansSerif", 9, QFont::Normal, false ) );
                    } // end if
                    if ( nivelCta == 2 ) {
                        it->setTextColor ( col, Qt::black );
                        it->setFont ( col, QFont ( "SansSerif", 10, QFont::Bold, false ) );
                    } else if ( nivelCta < 5 ) {
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
                            it->setFont ( col, QFont ( "SansSerif", 8, QFont::Normal, false ) );
                            it->setTextColor ( col, Qt::darkGray );
                        } // end if
                    } // end if
                } // end for
//                 int tamanyo = 10;
//                 if ( denominacion.length() > 40 ) {
//                     tamanyo -= 2;
//                 } else if ( denominacion.length() > 50 ) {
//                     tamanyo -= 3;
//                 } // end if
//                 it->setFont ( DENOMINACION, QFont ( "Serif", tamanyo, -1, false ) );
                it->setTextAlignment ( CUENTA, Qt::AlignLeft | Qt::AlignBottom );
                it->setTextAlignment ( DENOMINACION, Qt::AlignLeft | Qt::AlignBottom );
                it->setTextAlignment ( SALDO_ANT, Qt::AlignRight | Qt::AlignBottom );
                it->setTextAlignment ( DEBE, Qt::AlignRight | Qt::AlignBottom );
                it->setTextAlignment ( HABER, Qt::AlignRight | Qt::AlignBottom );
                it->setTextAlignment ( SALDO, Qt::AlignRight | Qt::AlignBottom );
                it->setTextAlignment ( DEBEEJ, Qt::AlignRight | Qt::AlignBottom );
                it->setTextAlignment ( HABEREJ, Qt::AlignRight | Qt::AlignBottom );
                it->setTextAlignment ( SALDOEJ, Qt::AlignRight | Qt::AlignBottom );
            } // end if
        } // end while

        /// Expandimos el listado.
        listado->expandAll();
        listado->resizeColumnToContents ( CUENTA );
        listado->setColumnWidth ( DENOMINACION, 300 );
        listado->resizeColumnToContents ( DEBE );
        listado->resizeColumnToContents ( HABER );
        listado->resizeColumnToContents ( SALDO );
        listado->resizeColumnToContents ( DEBEEJ );
        listado->resizeColumnToContents ( HABEREJ );
        listado->resizeColumnToContents ( SALDOEJ );

        /// Hacemos la actualizacion de los saldos totales.
        double totalSaldoAnterior = tsaldoant.toQString ( '.' ).toDouble();
        double totalDebe = tdebe.toQString ( '.' ).toDouble();
        double totalHaber = thaber.toQString ( '.' ).toDouble();
        double totalSaldo = tsaldo.toQString ( '.' ).toDouble();
        totalsaldoant->setText ( spain.toString ( totalSaldoAnterior, 'f', 2 ) );
        totaldebe->setText ( spain.toString ( totalDebe, 'f', 2 ) );
        totalhaber->setText ( spain.toString ( totalHaber, 'f', 2 ) );
        totalsaldo->setText ( spain.toString ( totalSaldo, 'f', 2 ) );
    } // end if

    _depura ( "END BalanceTreeView::presentar", 0 );
}


///
/**
\param nivel
**/
// void BalanceTreeView::nivelactivated ( int nivel )
// {
//     _depura ( "BalanceTreeView::nivelactivated", 0 );
//
//     combonivel->setCurrentIndex ( nivel );
//     int nivel1 = combonivel->currentText().toInt();
//
//     int j = 0;
//     while ( j < listado->topLevelItemCount() ) {
//         nivelactivated1 ( nivel1, listado->topLevelItem ( j ) );
//         j++;
//     } // end while
//
//     _depura ( "END BalanceTreeView::nivelactivated", 0 );
// }


///
/**
\param ot
**/
// void BalanceTreeView::nivelactivated1 ( int nivel, QTreeWidgetItem *ot )
// {
//     _depura ( "BalanceTreeView::nivelactivated1", 0 );
//     if ( ot ) {
//         if ( ot->text ( NIVEL ).toInt()  < nivel ) {
//             ot->treeWidget() ->setItemExpanded ( ot, TRUE );
//             ot->treeWidget() ->expandItem ( ot );
//         } else {
//             ot->treeWidget() ->setItemExpanded ( ot, FALSE );
//             ot->treeWidget() ->collapseItem ( ot );
//         } // end if
//
//         for ( int i = 0; i < ot->childCount(); i++ ) {
//             nivelactivated1 ( nivel, ot->child ( i ) );
//         } // end for
//
//     } // end if
//     _depura ( "END BalanceTreeView::nivelactivated1", 0 );
// }


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
    QAction *opt1 = menupopup->addAction ( _( "Ver Diario (este dia)" ) );
    QAction *opt2 = menupopup->addAction ( _( "Ver Diario (este mes)" ) );
    QAction *opt3 = menupopup->addAction ( _( "Ver Diario (este anyo)" ) );
    menupopup->addSeparator();
    QAction *opt4 = menupopup->addAction ( _( "Ver extracto (este dia)" ) );
    QAction *opt5 = menupopup->addAction ( _( "Ver extracto (este mes)" ) );
    QAction *opt6 = menupopup->addAction ( _( "Ver extracto (este anyo)" ) );
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
     QString queryalance();
//
     generaRML ( "balance.rml" );
     invocaPDF ( "balance" );

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

//     QString query;
//     int num1;
//     QTreeWidgetItem *it;
//     double tsaldoant = 0, tdebe = 0, thaber = 0, tsaldo = 0;
//     generarBalance();
//
//
// // =================================================================
//
//     QString archivod = confpr->valor ( CONF_DIR_USER ) + "listadosxc.perl";
//     BlSubForm * subf = ( BlSubForm * ) parent();
//
//     QString fitxersortidatxt = "";
//
//     fitxersortidatxt += "#!/usr/bin/perl\n";
//     fitxersortidatxt += "# Balance Export\n";
//     fitxersortidatxt += "\n";
//     fitxersortidatxt += "use ooolib;\n";
//     fitxersortidatxt += "use utf8;\n";
//     fitxersortidatxt += "\n";
//     fitxersortidatxt += "# Set variables\n";
//     fitxersortidatxt += "$doc = new ooolib(\"sxc\");\n";
//     fitxersortidatxt += "$doc->oooSet(\"builddir\", \".\");\n";
//     fitxersortidatxt += "$doc->oooSet(\"title\", \"SubFormulario Exportado\");\n";
//     fitxersortidatxt += "$doc->oooSet(\"subject\", \"SubFormulario\");\n";
//     fitxersortidatxt += "$doc->oooSet(\"comments\", \"SubFormulario Exportado\");\n";
//     fitxersortidatxt += "$doc->oooSet(\"author\", \"Tomeu Borras\");\n";
//     fitxersortidatxt += "$doc->oooSet(\"meta1-name\", \"Programmer\");\n";
//     fitxersortidatxt += "$doc->oooSet(\"meta1-value\", \"Tomeu Borras\");\n";
//     fitxersortidatxt += "\n";
//
//     fitxersortidatxt += "$doc->oooSet(\"cell-auto\", \"1,0\");\n";
//     fitxersortidatxt += "$doc->oooSet(\"bold\", \"on\");\n";
//     fitxersortidatxt += "$doc->oooSet(\"text-size\", \"18\");\n";
//     fitxersortidatxt += "$doc->oooSet(\"cell-loc\", 4, 1);\n";
//     fitxersortidatxt += "$doc->oooData(\"cell-text\", \"BALANCE DE SITUACION\");\n";
//     fitxersortidatxt += "$doc->oooSet(\"text-size\", \"10\");\n";
//     fitxersortidatxt += "$doc->oooSet(\"bold\", \"off\");\n";
//
// /// Generamos el query
//     query = "SELECT * FROM balancetemp WHERE debe <> 0  OR haber <> 0 ORDER BY padre, codigo";
//     BlDbRecordSet *cursorapt1 = mainCompany() ->cargacursor ( query );
//
//     int y = 1;
//     int x = 1;
//     /// Sacamos las cabeceras
//     for ( int h = 0; h < cursorapt1->numcampos(); h++ ) {
//
//         fitxersortidatxt += "# Fila " + QString::number ( y ) + "\n";
//         fitxersortidatxt += "$doc->oooSet(\"bold\", \"on\");\n";
//         fitxersortidatxt += "$doc->oooSet(\"cell-loc\", " + QString::number ( x++ ) + ", " + QString::number ( y ) + ");\n";
//         fitxersortidatxt += "$doc->oooData(\"cell-text\", \"" + parsearCode ( cursorapt1->nomcampo ( h ) ) + "\");\n";
//         fitxersortidatxt += "$doc->oooSet(\"bold\", \"off\");\n";
//         fitxersortidatxt += "\n";
//
//     } // end for
//
//
//     y += 2;
//
//     /// Sacamos el contenido
//     /// Calculamos cuantos registros van a crearse
//
//     while ( !cursorapt1->eof() ) {
//
//         int x = 1;
//         for ( int j = 0; j < cursorapt1->numcampos(); j++ ) {
//             fitxersortidatxt += "# Fila " + QString::number ( y ) + "\n";
//             fitxersortidatxt += "$doc->oooSet(\"bold\", \"on\");\n";
//             fitxersortidatxt += "$doc->oooSet(\"cell-loc\", " + QString::number ( x++ ) + ", " + QString::number ( y ) + ");\n";
//             fitxersortidatxt += "$doc->oooData(\"cell-text\", \"" + parsearCode ( cursorapt1->valor ( j ) ) + "\");\n";
//             fitxersortidatxt += "$doc->oooSet(\"bold\", \"off\");\n";
//             fitxersortidatxt += "\n";
//
//         } // end for
//         y++;
//         cursorapt1->siguienteregistro();
//     } // end while
//     /// Vaciamos el cursor de la base de datos.
//     delete cursorapt1;
//
//     /// Eliminamos la tabla temporal y cerramos la transacci&oacute;n.
//     query.sprintf ( "DROP TABLE balancetemp" );
//     mainCompany() ->ejecuta ( query );
//     mainCompany() ->commit();
//
//     fitxersortidatxt += "my($filename) = $doc->oooGenerate(\"listadosxc.sxc\");\n";
//     fitxersortidatxt += "\n";
//     fitxersortidatxt += "exit;\n";
//
//     QString cadena = "rm " + confpr->valor ( CONF_DIR_USER ) + "listadosxc.sxc";
//     system ( cadena.toAscii() );
//     cadena = "rm " + archivod;
//     system ( cadena.toAscii() );
//
//     QFile file ( archivod );
//     if ( file.open ( QIODevice::WriteOnly ) )  {
//         QTextStream stream ( &file );
//         stream.setCodec ( "UTF-8" );
//         stream << fitxersortidatxt;
//         file.close();
//     } // end if
//
//     cadena = " cd " + confpr->valor ( CONF_DIR_USER ) + "; perl " + archivod;
//     system ( cadena.toAscii() );
//     cadena = "kspread " + confpr->valor ( CONF_DIR_USER ) + "listadosxc.sxc &";
//     system ( cadena.toAscii() );

    _depura ( "END BalanceTreeView::on_mui_hojacalculo_clicked", 0 );
}

