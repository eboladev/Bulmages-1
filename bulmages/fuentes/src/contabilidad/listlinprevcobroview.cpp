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

// Incluimos las imagenes que catalogan los tipos de cuentas.
#include "images/cactivo.xpm"
#include "images/cpasivo.xpm"
#include "images/cneto.xpm"
#include "images/cingresos.xpm"
#include "images/cgastos.xpm"

#define COL_SELECCION 0
#define COL_FCOBROPREVCOBRO 1
#define COL_CODIGOCUENTA 2
#define COL_NOMCUENTA 3
#define COL_CANTIDADPREVCOBRO 4
#define COL_IDREGISTROIVA 5
#define COL_TIPOPREVCOBRO 6
#define COL_DOCPREVCOBRO 7
#define COL_IDPREVCOBRO 8
#define COL_FPREVISTAPREVCOBRO 9
#define COL_IDFPAGO 10
#define COL_IDCUENTA 11
#define COL_IDASIENTO 12
#define COL_CANTIDADPREVISTAPREVCOBRO 13


//#include "articleslist.h"
#include "listlinprevcobroview.h"
#include <qtable.h>
#include "qtable1.h"
#include <qmessagebox.h>
#include <qpopupmenu.h>
#include "configuracion.h"
#include "calendario.h"

listlinprevcobroview::listlinprevcobroview(QWidget * parent, const char * name) : QTable(parent, name), listlinprevcobro() {
    /// Inicializamos la tabla de lineas de presupuesto
    setNumCols(14);
    setNumRows(100);
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

    setColumnWidth(COL_IDPREVCOBRO,100);
    setColumnWidth(COL_FPREVISTAPREVCOBRO,100);
    setColumnWidth(COL_FCOBROPREVCOBRO,100);
    setColumnWidth(COL_IDFPAGO,100);
    setColumnWidth(COL_IDCUENTA,74);
    setColumnWidth(COL_CODIGOCUENTA,100);
    setColumnWidth(COL_NOMCUENTA,200);
    setColumnWidth(COL_IDASIENTO,100);
    setColumnWidth(COL_CANTIDADPREVISTAPREVCOBRO,100);
    setColumnWidth(COL_CANTIDADPREVCOBRO,100);
    setColumnWidth(COL_IDREGISTROIVA,50);
    setColumnWidth(COL_TIPOPREVCOBRO,100);
    setColumnWidth(COL_DOCPREVCOBRO,100);

    /*
        hideColumn(COL_IDPREVCOBRO);
        hideColumn(COL_IDFPAGO);
        hideColumn(COL_IDCUENTA);
        hideColumn(COL_IDASIENTO);
        hideColumn(COL_IDREGISTROIVA);
        hideColumn(COL_FPREVISTAPREVCOBRO);
        hideColumn(COL_CANTIDADPREVISTAPREVCOBRO);
    */

    setSelectionMode( QTable::SingleRow );
    /*
        setFocusPolicy(QWidget::WheelFocus);
        setFrameShape(QFrame::StyledPanel);
        setFocusStyle(QTable::SpreadSheet);
    */
    setColumnReadOnly(COL_NOMCUENTA,true);

    // Establecemos el color de fondo de la rejilla. El valor lo tiene la clase configuracion que es global.
    setPaletteBackgroundColor("#FFFFFF");

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
    setText(pos, COL_CODIGOCUENTA, linea->codigocuenta());
    setText(pos, COL_NOMCUENTA, linea->nomcuenta());
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
    cursor2 *cursoraux1= conexionbase->cargacursor("SELECT tipocuenta FROM cuenta WHERE idcuenta="+linea->idcuenta());
    if (!cursoraux1->eof()) {
        // Ponemos los iconos para que la cosa parezca mas guay.
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

void listlinprevcobroview::contextMenu ( int row, int, const QPoint & pos ) {
    QPopupMenu *popup;
    int opcion;
    popup = new QPopupMenu;
    popup->insertItem(tr(tr("Borrar Linea")),1);
    opcion = popup->exec(pos);
    delete popup;
    switch(opcion) {
    case 1:
        borralinprevcobro(row);
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
    static int semaforo =0;
    //    if  (semaforo == 1)
    //        return TRUE;
    QString idcuenta;
    linprevcobro *linea;
    if ( ev->type() == QEvent::KeyRelease ) {
        QKeyEvent *k = (QKeyEvent *)ev;
        int col=currentColumn();
        int row=currentRow();
        fprintf(stderr,"eventFilter(%d, %d)\n",row, col);
        switch (k->key()) {
        case Qt::Key_Plus:
            semaforo = 1;
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
            }// end if
            semaforo = 0;
            return TRUE;
            break;
        case Qt::Key_Asterisk:
            semaforo = 1;
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
                //                pintalinlistlinprevcobro(row);
                //                arreglaPosicion(row,col);
            }// end if
            semaforo = 0;
            return TRUE;
            break;
        case Qt::Key_Return:
        case Qt::Key_Enter:
            semaforo = 1;
            arreglaPosicion(row, col);
            semaforo = 0;
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
    default:
        newcol++;
        break;
    }// end switch
    //    ensureCellVisible(newrow,newcol);
    setCurrentCell(newrow,newcol);
}

void listlinprevcobroview::valueLineChanged(int row, int col) {
    static int semaforo =0;
    //    if  (semaforo == 1)
    //        return;
    fprintf(stderr,"valueLineChanged \n");
    QString valor = text(row,col);
    linprevcobro *linea;
    linea = lineaat(row);
    if (linea != NULL) {
        semaforo = 1;
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
        semaforo = 0;
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
            linea->setcodigocuenta(mdb_codigocuentaprevcobro);
            linea->settipoprevcobro(mdb_tipoprevcobro);
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
