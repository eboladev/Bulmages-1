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

#define COL_IDPREVCOBRO 0
#define COL_FPREVISTAPREVCOBRO 1
#define COL_FCOBROPREVCOBRO 2
#define COL_IDFPAGO 3
#define COL_IDCUENTA 4
#define COL_CODIGOCUENTA 5
#define COL_NOMCUENTA 6
#define COL_IDASIENTO 7
#define COL_CANTIDADPREVISTAPREVCOBRO 8
#define COL_CANTIDADPREVCOBRO 9
#define COL_IDREGISTROIVA 10
#define COL_TIPOPREVCOBRO 11
#define COL_DOCPREVCOBRO 12

//#include "articleslist.h"
#include "listlinprevcobroview.h"
#include <qtable.h>
#include <qmessagebox.h>
#include <qpopupmenu.h>
#include "configuracion.h"
#include "calendario.h"

listlinprevcobroview::listlinprevcobroview(QWidget * parent, const char * name) : QTable(parent, name), listlinprevcobro() {
    /// Inicializamos la tabla de lineas de presupuesto
    setNumCols(13);
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

    hideColumn(COL_IDPREVCOBRO);
    hideColumn(COL_IDFPAGO);
    hideColumn(COL_IDCUENTA);
    hideColumn(COL_IDASIENTO);
    hideColumn(COL_IDREGISTROIVA);
    //        hideColumn(COL_TIPOPREVCOBRO);

    setSelectionMode( QTable::SingleRow );

    setColumnReadOnly(COL_NOMCUENTA,true);

    // Establecemos el color de fondo de la rejilla. El valor lo tiene la clase configuracion que es global.
    setPaletteBackgroundColor("#FFFFFF");

    connect(this, SIGNAL(valueChanged(int, int)), this, SLOT(valueLineChanged(int , int )));

    connect(this, SIGNAL(contextMenuRequested(int, int, const QPoint &)), this, SLOT(contextMenu(int, int, const QPoint &)));

    installEventFilter(this);

}


listlinprevcobroview::~listlinprevcobroview() {}


void listlinprevcobroview::pintalistlinprevcobro() {
    fprintf(stderr,"INICIO de pintalistlinprevcobro\n");
    setNumRows(0);
    setNumRows(100);
    /// \todo Habr� que vaciar la tabla para que el pintado fuera exacto.
    linprevcobro *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        setText(i, COL_IDPREVCOBRO, linea->idprevcobro());
        setText(i, COL_FPREVISTAPREVCOBRO, linea->fprevistaprevcobro());
        setText(i, COL_FCOBROPREVCOBRO, linea->fcobroprevcobro());
        setText(i, COL_IDFPAGO, linea->idfpago());
        setText(i, COL_IDCUENTA, linea->idcuenta());
        setText(i, COL_CODIGOCUENTA, linea->codigocuenta());
        setText(i, COL_NOMCUENTA, linea->nomcuenta());
        setText(i, COL_IDASIENTO, linea->idasiento());
        setText(i, COL_CANTIDADPREVISTAPREVCOBRO, linea->cantidadprevistaprevcobro());
        setText(i, COL_CANTIDADPREVCOBRO, linea->cantidadprevcobro());
        setText(i, COL_IDREGISTROIVA, linea->idregistroiva());
        if (linea->tipoprevcobro() == "t")
            setText(i, COL_TIPOPREVCOBRO,"COBRO");
        else
            setText(i, COL_TIPOPREVCOBRO,"PAGO");
        setText(i, COL_DOCPREVCOBRO, linea->docprevcobro());
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
    setText(pos, COL_IDPREVCOBRO, linea->idprevcobro());
    setText(pos, COL_FPREVISTAPREVCOBRO, linea->fprevistaprevcobro());
    setText(pos, COL_FCOBROPREVCOBRO, linea->fcobroprevcobro());
    setText(pos, COL_IDFPAGO, linea->idfpago());
    setText(pos, COL_IDCUENTA, linea->idcuenta());
    setText(pos, COL_CODIGOCUENTA, linea->codigocuenta());
    setText(pos, COL_NOMCUENTA, linea->nomcuenta());
    setText(pos, COL_IDASIENTO, linea->idasiento());
    setText(pos, COL_CANTIDADPREVISTAPREVCOBRO, linea->cantidadprevistaprevcobro());
    setText(pos, COL_CANTIDADPREVCOBRO, linea->cantidadprevcobro());
    setText(pos, COL_IDREGISTROIVA, linea->idregistroiva());
    if (linea->tipoprevcobro() == "t")
        setText(pos, COL_TIPOPREVCOBRO,"COBRO");
    else
        setText(pos, COL_TIPOPREVCOBRO,"PAGO");
    setText(pos, COL_DOCPREVCOBRO, linea->docprevcobro());
}


bool listlinprevcobroview::eventFilter( QObject *obj, QEvent *ev ) {
    fprintf(stderr,"eventFilter()\n");
    QString idcuenta;
    linprevcobro *linea;
    if ( ev->type() == QEvent::KeyRelease ) {
        QKeyEvent *k = (QKeyEvent *)ev;
        int col=currentColumn();
        int row=currentRow();
        switch (k->key()) {
        case Qt::Key_Plus:
            if ((col == COL_FCOBROPREVCOBRO)) {
                QList<QDate> a;
                calendario *cal = new calendario(0,0);
                cal->exec();
                a = cal->dn->selectedDates();
                linea = lineaact();
                linea->setfcobroprevcobro(a.first()->toString("dd/MM/yyyy"));
                pintalinlistlinprevcobro(currentRow());
                delete cal;
                return TRUE;
            }// end if
            if ((col == COL_FPREVISTAPREVCOBRO)) {
                QList<QDate> a;
                calendario *cal = new calendario(0,0);
                cal->exec();
                a = cal->dn->selectedDates();
                linea = lineaact();
                linea->setfprevistaprevcobro(a.first()->toString("dd/MM/yyyy"));
                pintalinlistlinprevcobro(row);
                delete cal;
                return TRUE;
            }// end if
            if (col == COL_CODIGOCUENTA) {
                linea = lineaact();
                idcuenta = searchCuenta();
                linea->setidcuenta(idcuenta);
                pintalinlistlinprevcobro(row);
                return TRUE;
            }// end if
            break;
        case Qt::Key_Asterisk:
                linea = lineaact();
            if (row > 1) {
		QString valor= text(row-1,col);
                switch (col) {
                case COL_FPREVISTAPREVCOBRO:
                    linea->setfprevistaprevcobro(normalizafecha(valor).toString("dd/MM/yyyy"));
                    break;
                case COL_FCOBROPREVCOBRO:
                    linea->setfcobroprevcobro(normalizafecha(valor).toString("dd/MM/yyyy"));
                    break;
                case COL_CODIGOCUENTA:
                    linea->setcodigocuenta(valor);
                    break;
                case COL_CANTIDADPREVISTAPREVCOBRO:
                    linea->setcantidadprevistaprevcobro(valor);
                    break;
                case COL_CANTIDADPREVCOBRO:
                    linea->setcantidadprevcobro(valor);
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
                pintalinlistlinprevcobro(row);
            }// end if
		return TRUE;
	break;
        case Qt::Key_Return:
        case Qt::Key_Enter:
            // Esto se hace porque en la ltima linea del qtable tiene un comportamiento raro. Se reportar�como bug a trolltech.
            switch (col) {
            case COL_FPREVISTAPREVCOBRO:
                setCurrentCell(row, COL_FCOBROPREVCOBRO);
                return TRUE;
                break;
            case COL_FCOBROPREVCOBRO:
                setCurrentCell(row, COL_CODIGOCUENTA);
                return TRUE;
                break;
            case COL_CODIGOCUENTA:
                setCurrentCell(row, COL_CANTIDADPREVISTAPREVCOBRO);
                return TRUE;
                break;
            case COL_CANTIDADPREVISTAPREVCOBRO:
                setCurrentCell(row, COL_CANTIDADPREVCOBRO);
                return TRUE;
                break;
            case COL_CANTIDADPREVCOBRO:
                setCurrentCell(row, COL_TIPOPREVCOBRO);
                return TRUE;
                break;
            case COL_DOCPREVCOBRO:
                setCurrentCell(row+1, COL_FPREVISTAPREVCOBRO);
                return TRUE;
                break;
            case COL_TIPOPREVCOBRO:
                setCurrentCell(row, COL_DOCPREVCOBRO);
                return TRUE;
                break;
            }// end switch
            break;
        }// end switch
    }// end if
    return QTable::eventFilter( obj, ev );
} //end eventFilter



void listlinprevcobroview::valueLineChanged(int row, int col) {
    fprintf(stderr,"valueLineChanged \n");
    QString valor = text(row,col);
    linprevcobro *linea;
    linea = lineaat(row);
    if (linea != NULL) {
        switch (col) {
        case COL_FPREVISTAPREVCOBRO:
            linea->setfprevistaprevcobro(normalizafecha(valor).toString("dd/MM/yyyy"));
            break;
        case COL_FCOBROPREVCOBRO:
            linea->setfcobroprevcobro(normalizafecha(valor).toString("dd/MM/yyyy"));
            break;
        case COL_CODIGOCUENTA:
            linea->setcodigocuenta(valor);
            break;
        case COL_CANTIDADPREVISTAPREVCOBRO:
            linea->setcantidadprevistaprevcobro(valor);
            break;
        case COL_CANTIDADPREVCOBRO:
            linea->setcantidadprevcobro(valor);
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
        pintalinlistlinprevcobro(row);
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
