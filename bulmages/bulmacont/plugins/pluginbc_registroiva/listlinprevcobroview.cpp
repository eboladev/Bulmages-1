/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#ifdef TOMEU


/// @todo Hay que hacer dos lineas de presentacion (una para el listado en las facturas y otro para el listado por si solo)
/// Incluimos las imagenes que catalogan los tipos de cuentas.
#include "registroivaview.h"
#include "pluginbc_asiento.h"


#define COL_SELECCION                   0
#define COL_FCOBROPREVCOBRO             1
#define COL_CODIGOCTACLIENTE            2
#define COL_NOMCTACLIENTE               3
#define COL_CODIGOCUENTA                4
#define COL_NOMCUENTA                   5
#define COL_CANTIDADPREVCOBRO           6
#define COL_IDREGISTROIVA               7
#define COL_TIPOPREVCOBRO               8
#define COL_DOCPREVCOBRO                9
#define COL_IDPREVCOBRO                10
#define COL_FPREVISTAPREVCOBRO         11
#define COL_IDFPAGO                    12
#define COL_IDCUENTA                   13
#define COL_IDASIENTO                  14
#define COL_CANTIDADPREVISTAPREVCOBRO  15
#define COL_IDCTACLIENTE               16

#include <QMessageBox>
#include <QPixmap>
#include <QKeyEvent>
#include <QEvent>
#include <QMenu>

#include "blconfiguration.h"
#include "bcasientoview.h"
#include "listlinprevcobroview.h"
#include "bcimages.h"


///
/**
**/
void ListLinPrevCobroView::presentacionFactura()
{
    BL_FUNC_DEBUG
    hideColumn ( COL_SELECCION );
    hideColumn ( COL_IDPREVCOBRO );
    hideColumn ( COL_IDFPAGO );
    hideColumn ( COL_IDCUENTA );
    hideColumn ( COL_IDASIENTO );
    hideColumn ( COL_IDREGISTROIVA );
    hideColumn ( COL_FPREVISTAPREVCOBRO );
    hideColumn ( COL_CANTIDADPREVISTAPREVCOBRO );
    hideColumn ( COL_IDCTACLIENTE );
    
}


///
/**
**/
void ListLinPrevCobroView::presentacionListado()
{
    BL_FUNC_DEBUG
    showColumn ( COL_SELECCION );
    hideColumn ( COL_IDPREVCOBRO );
    hideColumn ( COL_IDFPAGO );
    hideColumn ( COL_IDCUENTA );
    hideColumn ( COL_IDASIENTO );
    hideColumn ( COL_IDREGISTROIVA );
    hideColumn ( COL_FPREVISTAPREVCOBRO );
    hideColumn ( COL_CANTIDADPREVISTAPREVCOBRO );
    hideColumn ( COL_IDCTACLIENTE );
    
}


///
/**
\param parent
**/
ListLinPrevCobroView::ListLinPrevCobroView ( QWidget * parent )
        : QTableWidget ( parent ), ListLinPrevCobro()
{
    BL_FUNC_DEBUG
    /// Inicializamos la tabla de lineas de presupuesto
    setColumnCount ( 17 );
    setRowCount ( 10000 );
    QStringList etiquetas;

    etiquetas << "COL_SELECCION" << "COL_FCOBROPREVCOBRO" << "COL_CODIGOCTACLIENTE" << "COL_NOMCTACLIENTE" << "COL_CODIGOCUENTA" << "COL_NOMCUENTA" << "COL_CANTIDADPREVCOBRO" << "COL_IDREGISTROIVA" << "COL_TIPOPREVCOBRO" << "COL_DOCPREVCOBRO" << "COL_IDPREVCOBRO" << "COL_FPREVISTAPREVCOBRO" << "COL_IDFPAGO" << "COL_IDCUENTA" << "COL_IDASIENTO" << "COL_CANTIDADPREVISTAPREVCOBRO" << "COL_IDCTACLIENTE";
    setHorizontalHeaderLabels ( etiquetas );

    setColumnWidth ( COL_SELECCION, 25 );
    setColumnWidth ( COL_IDPREVCOBRO, 100 );
    setColumnWidth ( COL_FPREVISTAPREVCOBRO, 100 );
    setColumnWidth ( COL_FCOBROPREVCOBRO, 100 );
    setColumnWidth ( COL_IDFPAGO, 100 );
    setColumnWidth ( COL_IDCUENTA, 74 );
    setColumnWidth ( COL_CODIGOCUENTA, 100 );
    setColumnWidth ( COL_CODIGOCTACLIENTE, 100 );
    setColumnWidth ( COL_NOMCUENTA, 150 );
    setColumnWidth ( COL_NOMCTACLIENTE, 150 );
    setColumnWidth ( COL_IDASIENTO, 100 );
    setColumnWidth ( COL_CANTIDADPREVISTAPREVCOBRO, 100 );
    setColumnWidth ( COL_CANTIDADPREVCOBRO, 100 );
    setColumnWidth ( COL_IDREGISTROIVA, 50 );
    setColumnWidth ( COL_TIPOPREVCOBRO, 100 );
    setColumnWidth ( COL_DOCPREVCOBRO, 100 );

    setSelectionMode ( QAbstractItemView::SingleSelection );

    //    setColumnReadOnly(COL_NOMCUENTA, true);
    //    setColumnReadOnly(COL_NOMCTACLIENTE, true);

    /// Establecemos el color de fondo de la rejilla. El valor lo tiene la clase
    /// configuraci&oacute;n que es global.
    QPalette palette;
    palette.setColor ( this->backgroundRole(), QColor ( "#FFFFFF" ) );
    this->setPalette ( palette );

    presentacionListado();
    connect ( this, SIGNAL ( valueChanged ( int, int ) ), this, SLOT ( valueLineChanged ( int, int ) ) );
    connect ( this, SIGNAL ( contextMenuRequested ( int, int, const QPoint & ) ), this, SLOT ( contextMenu ( int, int, const QPoint & ) ) );
    installEventFilter ( this );
    
}


///
/**
**/
ListLinPrevCobroView::~ListLinPrevCobroView()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param linea
\param pos
**/
void ListLinPrevCobroView::pintalistlinprevcobro ( linprevcobro *linea, int pos )
{
    BL_FUNC_DEBUG
    item ( pos, COL_IDPREVCOBRO ) ->setText ( linea->idprevcobro() );
    item ( pos, COL_FPREVISTAPREVCOBRO ) ->setText ( linea->fprevistaprevcobro() );
    item ( pos, COL_FCOBROPREVCOBRO ) ->setText ( linea->fcobroprevcobro() );
    item ( pos, COL_IDFPAGO ) ->setText ( linea->idfpago() );
    item ( pos, COL_IDCUENTA ) ->setText ( linea->idcuenta() );
    item ( pos, COL_IDCTACLIENTE ) ->setText ( linea->idctacliente() );
    item ( pos, COL_CODIGOCUENTA ) ->setText ( linea->codigocuenta() );
    item ( pos, COL_CODIGOCTACLIENTE ) ->setText ( linea->codigoctacliente() );
    item ( pos, COL_NOMCUENTA ) ->setText ( linea->nomcuenta() );
    item ( pos, COL_NOMCTACLIENTE ) ->setText ( linea->nomctacliente() );
    item ( pos, COL_IDASIENTO ) ->setText ( linea->idasiento() );

    if ( linea->idasiento() == "" ) {
        item ( pos, COL_SELECCION ) ->setFlags ( Qt::ItemIsUserCheckable );
    } // end if

    item ( pos, COL_CANTIDADPREVISTAPREVCOBRO ) ->setText ( linea->cantidadprevistaprevcobro() );
    item ( pos, COL_CANTIDADPREVCOBRO ) ->setText ( linea->cantidadprevcobro() );
    item ( pos, COL_IDREGISTROIVA ) ->setText ( linea->idregistroiva() );

    if ( linea->tipoprevcobro() == "t" ) {
        item ( pos, COL_TIPOPREVCOBRO ) ->setText ( _ ( "Cobro" ) );
    } else {
        item ( pos, COL_TIPOPREVCOBRO ) ->setText ( _ ( "Pago" ) );
    } // end if

    item ( pos, COL_DOCPREVCOBRO ) ->setText ( linea->docprevcobro() );

    /// Ponemos los iconos para que la cosa parezca mas guay.
    BlDbRecordSet *cursoraux1 = m_companyact->loadQuery ( "SELECT tipocuenta FROM cuenta WHERE idcuenta = " + linea->idcuenta() );
    if ( !cursoraux1->eof() ) {
        if ( cursoraux1->value( "tipocuenta" ) == "1" ) {
            item ( pos, COL_CODIGOCUENTA ) ->setIcon ( QIcon ( QPixmap ( cactivo ) ) );
        } else if ( cursoraux1->value( "tipocuenta" ) == "2" ) {
            item ( pos, COL_CODIGOCUENTA ) ->setIcon ( QIcon ( QPixmap ( cpasivo ) ) );
        } else if ( cursoraux1->value( "tipocuenta" ) == "3" ) {
            item ( pos, COL_CODIGOCUENTA ) ->setIcon ( QIcon ( QPixmap ( cneto ) ) );
        } else if ( cursoraux1->value( "tipocuenta" ) == "4" ) {
            item ( pos, COL_CODIGOCUENTA ) ->setIcon ( QIcon ( QPixmap ( cingresos ) ) );
        } else if ( cursoraux1->value( "tipocuenta" ) == "5" ) {
            item ( pos, COL_CODIGOCUENTA ) ->setIcon ( QIcon ( QPixmap ( cgastos ) ) );
        } // end if
    } // end if
    delete cursoraux1;

    /// Ponemos los iconos para que la cosa parezca mas guay.
    cursoraux1 = m_companyact->loadQuery ( "SELECT tipocuenta FROM cuenta WHERE idcuenta = " + linea->idctacliente() );
    if ( !cursoraux1->eof() ) {
        if ( cursoraux1->value( "tipocuenta" ) == "1" ) {
            item ( pos, COL_CODIGOCTACLIENTE ) ->setIcon ( QIcon ( QPixmap ( cactivo ) ) );
        } else if ( cursoraux1->value( "tipocuenta" ) == "2" ) {
            item ( pos, COL_CODIGOCTACLIENTE ) ->setIcon ( QIcon ( QPixmap ( cpasivo ) ) );
        } else if ( cursoraux1->value( "tipocuenta" ) == "3" ) {
            item ( pos, COL_CODIGOCTACLIENTE ) ->setIcon ( QIcon ( QPixmap ( cneto ) ) );
        } else if ( cursoraux1->value( "tipocuenta" ) == "4" ) {
            item ( pos, COL_CODIGOCTACLIENTE ) ->setIcon ( QIcon ( QPixmap ( cingresos ) ) );
        } else if ( cursoraux1->value( "tipocuenta" ) == "5" ) {
            item ( pos, COL_CODIGOCTACLIENTE ) ->setIcon ( QIcon ( QPixmap ( cgastos ) ) );
        } // end if
    } // end if
    delete cursoraux1;
    
}


///
/**
**/
void ListLinPrevCobroView::pintalistlinprevcobro()
{
    BL_FUNC_DEBUG
    setRowCount ( 0 );
    setRowCount ( 100 );
    /// TODO: Habra que vaciar la tabla para que el pintado fuera exacto.
    linprevcobro *linea;
    uint i = 0;
    QMutableListIterator<linprevcobro*> m_ilista ( m_lista );
    /// Vamos delante del primer elemento de la lista.
    m_ilista.toFront();
    /// Comprobamos que el primer elemento y siguientes existan.
    while ( m_ilista.hasNext() ) {
        /// Si existe el elemento nos desplazamos a el moviendo el cursor.
        linea = m_ilista.next();
        pintalistlinprevcobro ( linea, i );
    } // end while

    
}


///
/**
\param row
\param col
\param pos
\return
**/
void ListLinPrevCobroView::contextMenu ( int row, int col, const QPoint & pos )
{
    BL_FUNC_DEBUG
    QMenu *popup = new QMenu();
    QAction *menuborrarlinea = popup->addAction ( _ ( "Borrar linea" ) );
    QAction *menugenerarasiento = popup->addAction ( _ ( "Generar asiento de cobro/pago" ) );
    QAction *menudesvinculaasiento = popup->addAction ( _ ( "Desvincular asiento" ) );
    QAction *menuverasiento = popup->addAction ( _ ( "Ver asiento" ) );
    QAction *menuverfact = popup->addAction ( _ ( "Ver factura" ) );
    QAction *menucobro = popup->addAction ( _ ( "Cobro" ) );
    QAction *menupago = popup->addAction ( _ ( "Pago" ) );

    menugenerarasiento->setVisible ( FALSE );
    menudesvinculaasiento->setVisible ( FALSE );
    menuverasiento->setVisible ( FALSE );
    menuverfact->setVisible ( FALSE );
    menucobro->setVisible ( FALSE );
    menupago->setVisible ( FALSE );

    BlDbRecordSet *cur;
    QString query;
    linprevcobro *linea = lineaact();
    if ( linea == NULL ) {
        return;
    } // end if
    if ( linea->idprevcobro() == "" ) {
        return;
    } // end if
    if ( col == COL_TIPOPREVCOBRO ) {
        menucobro->setVisible ( TRUE );
        menupago->setVisible ( TRUE );
    } // end if

    if ( linea->idasiento() == "" ) {
        menugenerarasiento->setVisible ( TRUE );
    } else {
        menudesvinculaasiento->setVisible ( TRUE );
        menuverasiento->setVisible ( TRUE );
    } // end if
    if ( linea->idregistroiva() != "" ) {
        menuverfact->setVisible ( TRUE );
    } // end if

    QAction *opcion = popup->exec ( pos );
    delete popup;

    /// Comborobamos que opcio se ha seleccionado del menu contextual.

    if ( opcion == menuborrarlinea ) {
        borralinprevcobro ( row );
    } // end if
    if ( opcion == menugenerarasiento ) {
        /// Poner aqu&iacute; el c&oacute;digo necesario para generar el asiento.
        /// Intentamos la creacion del asiento y si funciona repintamos todo.
        if ( linea->creaPago() ) {
            pintalistlinprevcobro();
        } // end if
    } // end if
    if ( opcion == menudesvinculaasiento ) {
        if ( linea->idasiento() != "" ) {
            query = "UPDATE prevcobro SET idasiento = NULL WHERE idprevcobro = " + linea->idprevcobro();
            m_companyact->runQuery ( query );
            linea->setidasiento ( "" );
            pintalistlinprevcobro();
        } // end if
    } // end if
    if ( opcion == menuverasiento ) {
        if ( linea->idasiento() != "" ) {
//            m_companyact->intapuntsempresa() ->muestraAsiento ( linea->idasiento().toInt() );
            g_asiento ->muestraAsiento ( linea->idasiento().toInt() );


        } // end if
    } // end if
    if ( opcion == menuverfact ) {
        query = "SELECT idborrador FROM registroiva WHERE idregistroiva = " + linea->idregistroiva();
        cur = m_companyact->loadQuery ( query );
        if ( linea->idregistroiva() != "" ) {
            RegistroIvaView * iva = new RegistroIvaView ( m_companyact, 0 );
            iva->inicializa1 ( cur->value( "idborrador" ).toInt() );
            m_companyact->pWorkspace() ->addSubWindow ( iva );
            iva->show();
        } // end if
        delete cur;
    } // end if
    if ( opcion == menucobro ) {
        linea->settipoprevcobro ( "t" );
        pintalistlinprevcobro ( linea, row );
    } // end if
    if ( opcion == menupago ) {
        linea->settipoprevcobro ( "f" );
        pintalistlinprevcobro ( linea, row );
    } // end if
    
}


///
/**
**/
void ListLinPrevCobroView::borralinprevcobroact()
{
    BL_FUNC_DEBUG
    borralinprevcobro ( currentRow() );
    
}


///
/**
\param pos
**/
void ListLinPrevCobroView::pintalinlistlinprevcobro ( int pos )
{
    BL_FUNC_DEBUG
    linprevcobro *linea;
    linea = m_lista.at ( pos );
    pintalistlinprevcobro ( linea, pos );
}


/// Este m&eacute;todo se encarga de asegurar que la posicion del foco es la correcta para
/// la tabla.
/**
\param row
\param col
**/
void ListLinPrevCobroView::arreglaPosicion ( int row, int col )
{
    BL_FUNC_DEBUG
    int newcol = col;
    int newrow = row;
    switch ( col ) {
    case COL_DOCPREVCOBRO:
        newcol = COL_FPREVISTAPREVCOBRO;
        newrow++;
        break;
    case COL_CODIGOCUENTA:
        newcol = COL_CANTIDADPREVCOBRO;
        break;
    case COL_CODIGOCTACLIENTE:
        newcol = COL_CODIGOCUENTA;
        break;
    default:
        newcol++;
        break;
    } // end switch
    setCurrentCell ( newrow, newcol );
    
}

///
/**
\param row
\param col
**/
void ListLinPrevCobroView::valueLineChanged ( int row, int col )
{
    BL_FUNC_DEBUG
    QString valor = item ( row, col ) ->text();
    linprevcobro *linea;
    linea = lineaat ( row );
    if ( linea != NULL ) {
        switch ( col ) {
        case COL_FPREVISTAPREVCOBRO:
            linea->setfprevistaprevcobro ( blNormalizeDate ( valor ).toString ( "dd/MM/yyyy" ) );
            item ( row, col ) ->setText ( linea->fprevistaprevcobro() );
            break;
        case COL_FCOBROPREVCOBRO:
            linea->setfcobroprevcobro ( blNormalizeDate ( valor ).toString ( "dd/MM/yyyy" ) );
            linea->setfprevistaprevcobro ( blNormalizeDate ( valor ).toString ( "dd/MM/yyyy" ) );
            item ( row, col ) ->setText ( linea->fcobroprevcobro() );
            item ( row, COL_FPREVISTAPREVCOBRO ) ->setText ( linea->fprevistaprevcobro() );
            break;
        case COL_CODIGOCUENTA:
            linea->setcodigocuenta ( valor );
            item ( row, col ) ->setText ( linea->codigocuenta() );
            item ( row, COL_NOMCUENTA ) ->setText ( linea->nomcuenta() );
            item ( row, COL_IDCUENTA ) ->setText ( linea->idcuenta() );
            break;
        case COL_CODIGOCTACLIENTE:
            linea->setcodigoctacliente ( valor );
            item ( row, col ) ->setText ( linea->codigocuenta() );
            item ( row, COL_NOMCTACLIENTE ) ->setText ( linea->nomctacliente() );
            item ( row, COL_IDCTACLIENTE ) ->setText ( linea->idctacliente() );
            break;
        case COL_CANTIDADPREVISTAPREVCOBRO:
            linea->setcantidadprevistaprevcobro ( valor );
            item ( row, col ) ->setText ( linea->cantidadprevistaprevcobro() );
            break;
        case COL_CANTIDADPREVCOBRO:
            linea->setcantidadprevcobro ( valor );
            linea->setcantidadprevistaprevcobro ( valor );
            item ( row, col ) ->setText ( linea->cantidadprevcobro() );
            item ( row, COL_CANTIDADPREVISTAPREVCOBRO ) ->setText ( linea->cantidadprevistaprevcobro() );
            break;
        case COL_DOCPREVCOBRO:
            linea->setdocprevcobro ( valor );
            item ( row, col ) ->setText ( linea->docprevcobro() );
            break;
        case COL_TIPOPREVCOBRO:
            if ( valor == "COBRO" ) {
                linea->settipoprevcobro ( "t" );
                item ( row, col ) ->setText ( _ ( "Cobro" ) );
            } else {
                linea->settipoprevcobro ( "f" );
                item ( row, col ) ->setText ( _ ( "Pago" ) );
            } // end if
            break;
        } // end switch
    } // end if
}


/// Devuelve la l&iacute;nea que se est&aacute; tratando actualmente.
/**
\return
**/
linprevcobro *ListLinPrevCobroView::lineaact()
{
    fprintf ( stderr, "ListLinPrevCobroView::lineaact()\n" );
    return lineaat ( currentRow() );
}


/// Devuelve la l&iacute;nea especificada, y si no existe se van creando l&iacute;neas
/// hasta que exista.
/**
\param row
\return
**/
linprevcobro *ListLinPrevCobroView::lineaat ( int row )
{
    BL_FUNC_DEBUG
    linprevcobro *linea;
    if ( row >= 0 ) {
        while ( m_lista.value ( row ) == 0 ) {
	    BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Creamos la linea.") );
            linea = new linprevcobro ( m_companyact );
            linea->setidregistroiva ( mdb_idregistroiva );
            m_lista.append ( linea );
        } // end while
        return m_lista.at ( row );
    } else {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Linea inexistente.") );
        return NULL;
    } // end if
    
}


///
/**
\return
**/
QString ListLinPrevCobroView::searchCuenta()
{
    BL_FUNC_DEBUG

    /*
        QString idcuenta;
        BcPlanContableListView *listcuentas = new BcPlanContableListView(m_companyact);
        listcuentas->setModoLista();
        listcuentas->inicializa();
        listcuentas->exec();
        idcuenta = listcuentas->idcuenta();
        delete listcuentas;
        return idcuenta;
    */
    
    return 0;
}


/**
  * SLOT que respoonde a la creacion de un asiento de cobro o pago a partir de la gestion de
  * cobros y pagos.
  * Descripcion:
  * 1.- Calculamos los campos Total, Tipo de Asiento (compra/venta), Cuenta bancaria y cuenta de cliente
  * 2.- Determinamos si es un cobro o un pago.
  * 3.- Cargamos la plantilla de cobro o pago y le metemos los valores necesarios
  * 4.- Generamos el asiento a partir del asiento inteligente.
  */


///
/**
**/
void ListLinPrevCobroView::s_creaPago()
{
    BL_FUNC_DEBUG
    /// Calculamos los campos necesarios.
    /// El calculo de los campos requeridos es una iteracion por la tabla.
    /// Actualizamos los campos que haga falta.
    for ( int i = 0; i < rowCount(); i++ ) {
        /*
                Q3TableItem *check = item(i,COL_SELECCION);
                if (check != NULL) {
                    Q3CheckTableItem *check1 = (Q3CheckTableItem *) check;
                    if (check1->isChecked()) {
                        linprevcobro *linea = lineaat(i);
                        linea->creaPago();
                    } // end if
                } // end if
        */
    } // end for
    /// Inicializamos para que se muestren las cosas estas.
    pintalistlinprevcobro();
    
}

#endif
