//
// C++ Implementation: listlinprevcobroview
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//


/// @todo Hay que meter toda esta gestión como un plugin independiente de la aplicación.

/// @todo Hay que hacer dos líneas de presentación (una para el listado en las facturas y otro para el listado por si solo)



// Incluimos las imagenes que catalogan los tipos de cuentas.
#include "images/cactivo.xpm"
#include "images/cpasivo.xpm"
#include "images/cneto.xpm"
#include "images/cingresos.xpm"
#include "images/cgastos.xpm"

#include "ivaview.h"

#define COL_SELECCION 0
#define COL_FCOBROPREVCOBRO 1
#define COL_CODIGOCTACLIENTE 2
#define COL_NOMCTACLIENTE 3
#define COL_CODIGOCUENTA 4
#define COL_NOMCUENTA 5
#define COL_CANTIDADPREVCOBRO 6
#define COL_IDREGISTROIVA 7
#define COL_TIPOPREVCOBRO 8
#define COL_DOCPREVCOBRO 9
#define COL_IDPREVCOBRO 10
#define COL_FPREVISTAPREVCOBRO 11
#define COL_IDFPAGO 12
#define COL_IDCUENTA 13
#define COL_IDASIENTO 14
#define COL_CANTIDADPREVISTAPREVCOBRO 15
#define COL_IDCTACLIENTE 16


//#include "articleslist.h"
#include "listlinprevcobroview.h"
#include <qtable.h>
#include "qtable1.h"
#include <qmessagebox.h>
#include <qpopupmenu.h>
#include "configuracion.h"
#include "calendario.h"


void listlinprevcobroview::presentacionFactura() {
    hideColumn(COL_SELECCION);
    hideColumn(COL_IDPREVCOBRO);
    hideColumn(COL_IDFPAGO);
    hideColumn(COL_IDCUENTA);
    hideColumn(COL_IDASIENTO);
    hideColumn(COL_IDREGISTROIVA);
    hideColumn(COL_FPREVISTAPREVCOBRO);
    hideColumn(COL_CANTIDADPREVISTAPREVCOBRO);
    hideColumn(COL_IDCTACLIENTE);
}// end presentacionFactura


void listlinprevcobroview::presentacionListado() {
    showColumn(COL_SELECCION);
    hideColumn(COL_IDPREVCOBRO);
    hideColumn(COL_IDFPAGO);
    hideColumn(COL_IDCUENTA);
    hideColumn(COL_IDASIENTO);
    hideColumn(COL_IDREGISTROIVA);
    hideColumn(COL_FPREVISTAPREVCOBRO);
    hideColumn(COL_CANTIDADPREVISTAPREVCOBRO);
    hideColumn(COL_IDCTACLIENTE);
}// end presentacionListado


listlinprevcobroview::listlinprevcobroview(QWidget * parent, const char * name) : QTable(parent, name), listlinprevcobro() {
    /// Inicializamos la tabla de lineas de presupuesto
    setNumCols(17);
    setNumRows(10000);
    horizontalHeader()->setLabel( COL_IDPREVCOBRO, tr( "COL_IDPREVCOBRO" ) );
    horizontalHeader()->setLabel( COL_FPREVISTAPREVCOBRO, tr( "COL_FPREVISTAPREVCOBRO" ) );
    horizontalHeader()->setLabel( COL_FCOBROPREVCOBRO, tr( "COL_FCOBROPREVCOBRO" ) );
    horizontalHeader()->setLabel( COL_IDFPAGO, tr( "COL_IDFPAGO" ) );
    horizontalHeader()->setLabel( COL_IDCUENTA, tr( "COL_IDCUENTA" ) );
    horizontalHeader()->setLabel( COL_CODIGOCUENTA, tr( "COL_CODIGOCUENTA" ) );
    horizontalHeader()->setLabel( COL_NOMCUENTA, tr( "COL_NOMCUENTA" ) );
    horizontalHeader()->setLabel( COL_IDASIENTO, tr( "COL_IDASIENTO" ) );
    horizontalHeader()->setLabel( COL_CANTIDADPREVISTAPREVCOBRO, tr( "COL_CANTIDADPREVISTAPREVCOBRO" ) );
    horizontalHeader()->setLabel( COL_CANTIDADPREVCOBRO, tr( "COL_CANTIDADPREVCOBRO" ) );
    horizontalHeader()->setLabel( COL_IDREGISTROIVA, tr( "COL_IDREGISTROIVA" ) );
    horizontalHeader()->setLabel( COL_TIPOPREVCOBRO, tr( "COL_TIPOPREVCOBRO" ) );
    horizontalHeader()->setLabel( COL_DOCPREVCOBRO, tr( "COL_DOCPREVCOBRO" ) );
    horizontalHeader()->setLabel( COL_CODIGOCTACLIENTE, tr( "COL_CODIGOCTACLIENTE" ) );
    horizontalHeader()->setLabel( COL_NOMCTACLIENTE, tr( "COL_NOMCTACLIENTE" ) );

    setColumnWidth(COL_SELECCION, 25);
    setColumnWidth(COL_IDPREVCOBRO,100);
    setColumnWidth(COL_FPREVISTAPREVCOBRO,100);
    setColumnWidth(COL_FCOBROPREVCOBRO,100);
    setColumnWidth(COL_IDFPAGO,100);
    setColumnWidth(COL_IDCUENTA,74);
    setColumnWidth(COL_CODIGOCUENTA,100);
    setColumnWidth(COL_CODIGOCTACLIENTE, 100);
    setColumnWidth(COL_NOMCUENTA,150);
    setColumnWidth(COL_NOMCTACLIENTE, 150);
    setColumnWidth(COL_IDASIENTO,100);
    setColumnWidth(COL_CANTIDADPREVISTAPREVCOBRO,100);
    setColumnWidth(COL_CANTIDADPREVCOBRO,100);
    setColumnWidth(COL_IDREGISTROIVA,50);
    setColumnWidth(COL_TIPOPREVCOBRO,100);
    setColumnWidth(COL_DOCPREVCOBRO,100);

    setSelectionMode( QTable::SingleRow );
    setColumnReadOnly(COL_NOMCUENTA,true);
    setColumnReadOnly(COL_NOMCTACLIENTE,true);

    // Establecemos el color de fondo de la rejilla. El valor lo tiene la clase configuracion que es global.
    setPaletteBackgroundColor("#FFFFFF");

    presentacionListado();

    connect(this, SIGNAL(valueChanged(int, int)), this, SLOT(valueLineChanged(int , int )));
    connect(this, SIGNAL(contextMenuRequested(int, int, const QPoint &)), this, SLOT(contextMenu(int, int, const QPoint &)));
    installEventFilter(this);
}


listlinprevcobroview::~listlinprevcobroview() {}


void listlinprevcobroview::pintalistlinprevcobro(linprevcobro *linea, int pos) {
    setText(pos, COL_IDPREVCOBRO, linea->idprevcobro());
    setText(pos, COL_FPREVISTAPREVCOBRO, linea->fprevistaprevcobro());
    setText(pos, COL_FCOBROPREVCOBRO, linea->fcobroprevcobro());
    setText(pos, COL_IDFPAGO, linea->idfpago());
    setText(pos, COL_IDCUENTA, linea->idcuenta());
    setText(pos, COL_IDCTACLIENTE, linea->idctacliente());
    setText(pos, COL_CODIGOCUENTA, linea->codigocuenta());
    setText(pos, COL_CODIGOCTACLIENTE, linea->codigoctacliente());
    setText(pos, COL_NOMCUENTA, linea->nomcuenta());
    setText(pos, COL_NOMCTACLIENTE, linea->nomctacliente());
    setText(pos, COL_IDASIENTO, linea->idasiento());

    if (linea->idasiento() == "") {
        QCheckTableItem *item = new QCheckTableItem(this, "");
        setItem(pos,COL_SELECCION,item);
    }// end if

    setText(pos, COL_CANTIDADPREVISTAPREVCOBRO, linea->cantidadprevistaprevcobro());
    setText(pos, COL_CANTIDADPREVCOBRO, linea->cantidadprevcobro());
    setText(pos, COL_IDREGISTROIVA, linea->idregistroiva());
    if (linea->tipoprevcobro() == "t")
        setText(pos, COL_TIPOPREVCOBRO,"COBRO");
    else
        setText(pos, COL_TIPOPREVCOBRO,"PAGO");
    setText(pos, COL_DOCPREVCOBRO, linea->docprevcobro());

    /// Ponemos los iconos para que la cosa parezca mas guay.
    cursor2 *cursoraux1= conexionbase->cargacursor("SELECT tipocuenta FROM cuenta WHERE idcuenta="+linea->idcuenta());
    if (!cursoraux1->eof()) {

        if (cursoraux1->valor("tipocuenta") == "1")
            setPixmap(pos, COL_CODIGOCUENTA, QPixmap(cactivo));
        else if (cursoraux1->valor("tipocuenta") == "2")
            setPixmap(pos, COL_CODIGOCUENTA, QPixmap(cpasivo));
        else if (cursoraux1->valor("tipocuenta") == "3")
            setPixmap(pos, COL_CODIGOCUENTA, QPixmap(cneto));
        else if (cursoraux1->valor("tipocuenta") == "4")
            setPixmap(pos, COL_CODIGOCUENTA, QPixmap(cingresos));
        else if (cursoraux1->valor("tipocuenta") == "5")
            setPixmap(pos, COL_CODIGOCUENTA, QPixmap(cgastos));
    }// end if
    delete cursoraux1;


    /// Ponemos los iconos para que la cosa parezca mas guay.
    cursoraux1= conexionbase->cargacursor("SELECT tipocuenta FROM cuenta WHERE idcuenta="+linea->idctacliente());
    if (!cursoraux1->eof()) {
        if (cursoraux1->valor("tipocuenta") == "1")
            setPixmap(pos, COL_CODIGOCTACLIENTE, QPixmap(cactivo));
        else if (cursoraux1->valor("tipocuenta") == "2")
            setPixmap(pos, COL_CODIGOCTACLIENTE, QPixmap(cpasivo));
        else if (cursoraux1->valor("tipocuenta") == "3")
            setPixmap(pos, COL_CODIGOCTACLIENTE, QPixmap(cneto));
        else if (cursoraux1->valor("tipocuenta") == "4")
            setPixmap(pos, COL_CODIGOCTACLIENTE, QPixmap(cingresos));
        else if (cursoraux1->valor("tipocuenta") == "5")
            setPixmap(pos, COL_CODIGOCTACLIENTE, QPixmap(cgastos));
    }// end if
    delete cursoraux1;

}

void listlinprevcobroview::pintalistlinprevcobro() {
    fprintf(stderr,"INICIO de pintalistlinprevcobro\n");
    setNumRows(0);
    setNumRows(100);
    /// \todo Habr� que vaciar la tabla para que el pintado fuera exacto.
    linprevcobro *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        pintalistlinprevcobro(linea, i);
        i++;
    }// end for
    fprintf(stderr,"FIN de pintalistlinprevcobro\n");
}

void listlinprevcobroview::contextMenu ( int row, int col, const QPoint & pos ) {
    QPopupMenu *popup;
    int opcion;
    cursor2 *cur;
QString query;
    linprevcobro *linea = lineaact();
    if (linea == NULL)
        return;
    if (linea->idprevcobro() == "")
        return;
    popup = new QPopupMenu;

    if (col == COL_TIPOPREVCOBRO) {
        popup->insertItem(tr("COBRO"),6);
        popup->insertItem(tr("PAGO"),7);
    }// end if

    popup->insertItem(tr("Borrar Linea"),1);
    if (linea->idasiento() == "") {
        popup->insertItem(tr("Generar Asiento de Cobro/Pago"),2);
    } else {
        popup->insertItem(tr("Desvincular Asiento"),3);
        popup->insertItem(tr("Ver Asiento"),4);
    }// end if
    if (linea->idregistroiva() != "")
        popup->insertItem(tr("Ver Factura"),5);
    opcion = popup->exec(pos);
    delete popup;
    switch(opcion) {
    case 1:
        borralinprevcobro(row);
        break;
    case 2:
        /// Poner aqui el código necesario para generar el asiento.
        /// Intentamos la creación del asiento y si funciona repintamos todo.
        if (linea->creaPago())
            pintalistlinprevcobro();
        break;
    case 3:
        if(linea->idasiento() != "") {
            query = "UPDATE prevcobro SET idasiento = NULL WHERE idprevcobro="+linea->idprevcobro();
            conexionbase->ejecuta(query);
            linea->setidasiento("");
            pintalistlinprevcobro();
        }// end if
        break;
    case 4:
        if (linea->idasiento() != "") {
            empresaactual->intapuntsempresa()->muestraasiento(linea->idasiento().toInt());
        }// end if
        break;
    case 5:
        query = "SELECT idborrador FROM registroiva WHERE idregistroiva = "+linea->idregistroiva();
        cur = conexionbase->cargacursor(query);
        if (linea->idregistroiva() != "") {
            ivaview *iva = new ivaview(empresaactual, NULL, "iva");
            iva->inicializa1(cur->valor("idborrador").toInt());
            iva->exec();
            delete iva;
        }// end if
        delete cur;
        break;
    case 6:
        linea->settipoprevcobro("t");
        pintalistlinprevcobro(linea,row);
        break;
    case 7:
        linea->settipoprevcobro("f");
        pintalistlinprevcobro(linea, row);
        break;
    }// end switch
}// end contextMenuRequested


void listlinprevcobroview::borralinprevcobroact() {
    borralinprevcobro(currentRow());
}// end borralinprevcobroact


void listlinprevcobroview::pintalinlistlinprevcobro(int pos) {
    fprintf(stderr,"pintalinlistlinprevcobro(%d)\n",pos);
    linprevcobro *linea;
    linea = m_lista.at(pos);
    pintalistlinprevcobro(linea, pos);
}


bool listlinprevcobroview::eventFilter( QObject *obj, QEvent *ev ) {
    QString idcuenta;
    linprevcobro *linea;
    if ( ev->type() == QEvent::KeyRelease ) {
        QKeyEvent *k = (QKeyEvent *)ev;
        int col=currentColumn();
        int row=currentRow();
        fprintf(stderr,"eventFilter(%d, %d)\n",row, col);
        switch (k->key()) {
        case Qt::Key_Plus:
            if ((col == COL_FCOBROPREVCOBRO)) {
                QList<QDate> a;
                calendario *cal = new calendario(0,0);
                cal->exec();
                a = cal->dn->selectedDates();
                delete cal;
                linea = lineaact();
                linea->setfcobroprevcobro(a.first()->toString("dd/MM/yyyy"));
                pintalinlistlinprevcobro(currentRow());
            } else if (col == COL_CODIGOCUENTA) {
                linea = lineaact();
                idcuenta = searchCuenta();
                linea->setidcuenta(idcuenta);
                pintalinlistlinprevcobro(row);
            } else if (col == COL_CODIGOCTACLIENTE) {
                linea = lineaact();
                idcuenta = searchCuenta();
                linea->setidctacliente(idcuenta);
                pintalinlistlinprevcobro(row);
            }// end if
            return TRUE;
            break;
        case Qt::Key_Asterisk:
            linea = lineaact();
            if (row > 0) {
                QString valor= text(row-1,col);
                switch (col) {
                case COL_FPREVISTAPREVCOBRO:
                    linea->setfprevistaprevcobro(normalizafecha(valor).toString("dd/MM/yyyy"));
                    break;
                case COL_FCOBROPREVCOBRO:
                    linea->setfcobroprevcobro(normalizafecha(valor).toString("dd/MM/yyyy"));
                    linea->setfprevistaprevcobro(normalizafecha(valor).toString("dd/MM/yyyy"));
                    setText(row,col,linea->fprevistaprevcobro());
                    break;
                case COL_CODIGOCUENTA:
                    linea->setcodigocuenta(valor);
                    setText(row,col,linea->codigocuenta());
                    break;
                case COL_CODIGOCTACLIENTE:
                    linea->setcodigoctacliente(valor);
                    setText(row,col,linea->codigoctacliente());
                    break;
                case COL_CANTIDADPREVISTAPREVCOBRO:
                    linea->setcantidadprevistaprevcobro(valor);
                    break;
                case COL_CANTIDADPREVCOBRO:
                    linea->setcantidadprevcobro(valor);
                    linea->setcantidadprevistaprevcobro(valor);
                    break;
                case COL_DOCPREVCOBRO:
                    linea->setdocprevcobro(valor);
                    break;
                case COL_TIPOPREVCOBRO:
                    if (valor == "COBRO")
                        linea->settipoprevcobro("t");
                    else
                        linea->settipoprevcobro("f");
                    break;
                }// end switch
            }// end if
            return TRUE;
            break;
        case Qt::Key_Return:
        case Qt::Key_Enter:
            arreglaPosicion(row, col);
            return TRUE;
        }// end switch
    }// end if
    return QTable::eventFilter( obj, ev );
} //end eventFilter



/// Este método se encarga de asegurar que la posición del foco es la correcta para la tabla.

void listlinprevcobroview::arreglaPosicion(int row, int col) {
    int newcol = col;
    int newrow = row;
    switch (col) {
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
    }// end switch
    setCurrentCell(newrow,newcol);
}

void listlinprevcobroview::valueLineChanged(int row, int col) {
    fprintf(stderr,"valueLineChanged \n");
    QString valor = text(row,col);
    linprevcobro *linea;
    linea = lineaat(row);
    if (linea != NULL) {
        switch (col) {
        case COL_FPREVISTAPREVCOBRO:
            linea->setfprevistaprevcobro(normalizafecha(valor).toString("dd/MM/yyyy"));
            setText(row, col, linea->fprevistaprevcobro());
            break;
        case COL_FCOBROPREVCOBRO:
            linea->setfcobroprevcobro(normalizafecha(valor).toString("dd/MM/yyyy"));
            linea->setfprevistaprevcobro(normalizafecha(valor).toString("dd/MM/yyyy"));
            setText(row,col, linea->fcobroprevcobro());
            setText(row, COL_FPREVISTAPREVCOBRO, linea->fprevistaprevcobro());
            break;
        case COL_CODIGOCUENTA:
            linea->setcodigocuenta(valor);
            setText(row, col, linea->codigocuenta());
            setText(row, COL_NOMCUENTA, linea->nomcuenta());
            setText(row, COL_IDCUENTA, linea->idcuenta());
            break;
        case COL_CODIGOCTACLIENTE:
            linea->setcodigoctacliente(valor);
            setText(row, col, linea->codigoctacliente());
            setText(row, COL_NOMCTACLIENTE, linea->nomctacliente());
            setText(row, COL_IDCTACLIENTE, linea->idctacliente());
            break;
        case COL_CANTIDADPREVISTAPREVCOBRO:
            linea->setcantidadprevistaprevcobro(valor);
            setText(row,col, linea->cantidadprevistaprevcobro());
            break;
        case COL_CANTIDADPREVCOBRO:
            linea->setcantidadprevcobro(valor);
            linea->setcantidadprevistaprevcobro(valor);
            setText(row,col, linea->cantidadprevcobro());
            setText(row, COL_CANTIDADPREVISTAPREVCOBRO, linea->cantidadprevistaprevcobro());
            break;
        case COL_DOCPREVCOBRO:
            linea->setdocprevcobro(valor);
            setText(row,col, linea->docprevcobro());
            break;
        case COL_TIPOPREVCOBRO:
            if (valor == "COBRO") {
                linea->settipoprevcobro("t");
                setText(row, col, "COBRO");
            } else {
                linea->settipoprevcobro("f");
                setText(row, col, "PAGO");
            }// end if
            break;

        }// end switch
    }// end if
} //end valueBudgetLineChanged


/// Devuelve la linea que se esta tratando actualmente
linprevcobro *listlinprevcobroview::lineaact() {
    fprintf(stderr,"listlinprevcobroview::lineaact()\n");
    return lineaat(currentRow());
}// end lineaact

/// Devuelve la linea especificada, y si no existe se van creando lineas hasta que exista.
linprevcobro *listlinprevcobroview::lineaat(int row) {
    fprintf(stderr,"listlinprevcobro::lineaat(%d)\n", row);
    linprevcobro *linea;
    if (row >=0) {
        while (m_lista.at(row) == 0 ) {
            fprintf(stderr,"Creamos la linea\n");
            linea=new linprevcobro(empresaactual);
            linea->setidregistroiva(mdb_idregistroiva);
            m_lista.append(linea);
        }// end while
        return(m_lista.at(row));
    } else {
        fprintf(stderr,"Linea inexistente\n");
        return NULL;
    }// end if

}// end lineaat

QString listlinprevcobroview::searchCuenta() {
    QString idcuenta;
    fprintf(stderr,"Busqueda de una cuenta\n");
    listcuentasview1 *listcuentas = new listcuentasview1(empresaactual);
    listcuentas->setModoLista();
    listcuentas->inicializa();
    listcuentas->exec();
    idcuenta = listcuentas->idcuenta();
    delete listcuentas;
    return idcuenta;
}// end searchArticle


/**
  * \brief SLOT que respoonde a la creación de un asiento de cobro o pago a partir de la gestion de cobros y pagos.
  * Descripción:
  * 1.- Calculamos los campos Total, Tipo de Asiento (compra/venta), Cuenta bancaria y cuenta de cliente
  * 2.- Determinamos si es un cobro o un pago.
  * 3.- Cargamos la plantilla de cobro o pago y le metemos los valores necesarios
  * 4.- Generamos el asiento a partir del asiento inteligente.
  */
void listlinprevcobroview::s_creaPago() {
    /// Calculamos los campos necesarios.
    /* El cálculo de los campos requeridos es una iteración por la tabla. */

    /// Actualizamos los campos que haga falta.
    for (int i=0; i< numRows(); i++) {
        fprintf(stderr,"Iteración para los elementos de la lista %d\n", i);
        QTableItem *check = item(i,COL_SELECCION);
        fprintf(stderr,"Vamos a testear \n");
        if (check != NULL) {
            QCheckTableItem *check1 = (QCheckTableItem *) check;
            if (check1->isChecked()) {
                linprevcobro *linea = lineaat(i);
                linea->creaPago();
            }// end if
        }// end if
    }// end for
    /// Inicializamos para que se muestren las cosas estas.
    pintalistlinprevcobro();
}// end s_creaPago

