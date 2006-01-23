//
// C++ Implementation: ListLinAlbaranClienteView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#define COL_NUMLALBARAN 0
#define COL_IDARTICULO 1
#define COL_CODARTICULO 2
#define COL_NOMARTICULO 3
#define COL_DESCLALBARAN 4
#define COL_CANTLALBARAN 5
#define COL_PVPLALBARAN 6
#define COL_DESCONTLALBARAN 7
#define COL_IDALBARAN 8
#define COL_REMOVE 9
#define COL_TASATIPO_IVA 10
#define COL_TIPO_IVA 11

#include <q3table.h>
#include <qmessagebox.h>
#include <q3popupmenu.h>
//Added by qt3to4:
#include <QKeyEvent>
#include <QEvent>

#include "listlinalbaranclienteview.h"
#include "articleslist.h"
#include "funcaux.h"


void ListLinAlbaranClienteView::guardaconfig() {
    _depura("ListLinAlbaranClienteView::guardaconfig",0);
    QString aux = "";

    QFile file( confpr->valor(CONF_DIR_USER)+"conflistlinalbaranclienteview.cfn" );
    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        for (int i = 0; i < numCols(); i++) {
            stream << columnWidth(i) << "\n";
        }// end for
        file.close();
    }// end if
}// end guardaconfig()

void ListLinAlbaranClienteView::cargaconfig() {
	_depura("ListLinAlbaranClienteView::cargaconfig",0);
    QFile file( confpr->valor(CONF_DIR_USER)+"conflistlinalbaranclienteview.cfn" );
    QString line;
    if ( file.open( QIODevice::ReadOnly ) ) {
        QTextStream stream( &file );
        for (int i = 0; i < numCols(); i++) {
            QString linea = stream.readLine();
            setColumnWidth(i, linea.toInt());
        }// end for
        file.close();
    }
}// end cargaconfig

ListLinAlbaranClienteView::ListLinAlbaranClienteView(QWidget * parent, const char * name) : Q3Table(parent, name), ListLinAlbaranCliente() {
    /// Inicializamos la tabla de lineas de Factura
    setNumCols(12);
    setNumRows(100);
    horizontalHeader()->setLabel( COL_NUMLALBARAN, tr( "N L�ea" ) );
    horizontalHeader()->setLabel( COL_DESCLALBARAN, tr( "Descripci�" ) );
    horizontalHeader()->setLabel( COL_CANTLALBARAN, tr( "Cantidad" ) );
    horizontalHeader()->setLabel( COL_PVPLALBARAN, tr( "Precio" ) );
    horizontalHeader()->setLabel( COL_DESCONTLALBARAN, tr( "Descuento" ) );
    horizontalHeader()->setLabel( COL_IDALBARAN, tr( "N Pedido" ) );
    horizontalHeader()->setLabel( COL_IDARTICULO, tr( "Art�ulo" ) );
    horizontalHeader()->setLabel( COL_CODARTICULO, tr( "C�igo Art�ulo" ) );
    horizontalHeader()->setLabel( COL_NOMARTICULO, tr( "Descripci� Art�ulo" ) );
    horizontalHeader()->setLabel( COL_TASATIPO_IVA, tr( "% IVA" ) );
    horizontalHeader()->setLabel( COL_TIPO_IVA, tr( "Tipo IVA" ) );

    hideColumn(COL_NUMLALBARAN);
    hideColumn(COL_IDALBARAN);
    hideColumn(COL_IDARTICULO);
    hideColumn(COL_REMOVE);
    hideColumn(COL_TASATIPO_IVA);
    hideColumn(COL_TIPO_IVA);

    setSelectionMode( Q3Table::SingleRow );

    setColumnReadOnly(COL_NOMARTICULO,true);
    // Establecemos el color de fondo de la rejilla. El valor lo tiene la clase configuracion que es global.
    setPaletteBackgroundColor(confpr->valor(CONF_BG_LINALBARANESCLIENTE));

    connect(this, SIGNAL(valueChanged(int, int)), this, SLOT(valueBudgetLineChanged(int , int )));
    connect(this, SIGNAL(contextMenuRequested(int, int, const QPoint &)), this, SLOT(contextMenu(int, int, const QPoint &)));
    installEventFilter(this);
	cargaconfig();
}


ListLinAlbaranClienteView::~ListLinAlbaranClienteView() {
	guardaconfig();
}


void ListLinAlbaranClienteView::pintaListLinAlbaranCliente() {
    fprintf(stderr,"INICIO de pintaListLinAlbaranCliente\n");
    setNumRows(0);
    setNumRows(100);
    /// \todo Habr� que vaciar la tabla para que el pintado fuera exacto.
    LinAlbaranCliente *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        setText(i, COL_NUMLALBARAN, linea->numlalbaran());
        setText(i, COL_IDARTICULO, linea->idarticulo());
        setText(i, COL_CODARTICULO, linea->codigocompletoarticulo());
        setText(i, COL_NOMARTICULO, linea->nomarticulo());
        setText(i, COL_DESCLALBARAN, linea->desclalbaran());
        setText(i, COL_CANTLALBARAN, linea->cantlalbaran());
        setText(i, COL_DESCONTLALBARAN, linea->descontlalbaran());
        setText(i, COL_IDALBARAN, linea->idalbaran());
        setText(i, COL_REMOVE, "XX");
        setText(i, COL_TASATIPO_IVA, linea->ivalalbaran());
        setText(i, COL_TIPO_IVA, linea->ivalalbaran());
        setText(i, COL_PVPLALBARAN, linea->pvplalbaran());
	adjustRow(i);
        i++;
    }// end for
    fprintf(stderr,"FIN de pintaListLinAlbaranCliente\n");
}



void ListLinAlbaranClienteView::contextMenu ( int row, int, const QPoint & pos ) {
    Q3PopupMenu *popup;
    int opcion;
    popup = new Q3PopupMenu;
    popup->insertItem(tr(tr("Borrar Linea")),1);
    opcion = popup->exec(pos);
    delete popup;
    switch(opcion) {
    case 1:
        borraLinAlbaranCliente(row);
    }// end switch
}// end contextMenuRequested


void ListLinAlbaranClienteView::borraLinAlbaranClienteact() {
    borraLinAlbaranCliente(currentRow());
}// end borraLinAlbaranClienteact


void ListLinAlbaranClienteView::pintalinListLinAlbaranCliente(int pos) {
fprintf(stderr,"pintalinListLinAlbaranCliente(%d)\n",pos);
    LinAlbaranCliente *linea;
    linea = m_lista.at(pos);
    setText(pos, COL_NUMLALBARAN, linea->numlalbaran());
    setText(pos, COL_IDARTICULO, linea->idarticulo());
    setText(pos, COL_CODARTICULO, linea->codigocompletoarticulo());
    setText(pos, COL_NOMARTICULO, linea->nomarticulo());
    setText(pos, COL_DESCLALBARAN, linea->desclalbaran());
    setText(pos, COL_CANTLALBARAN, linea->cantlalbaran());
    setText(pos, COL_DESCONTLALBARAN, linea->descontlalbaran());
    setText(pos, COL_IDALBARAN, linea->idalbaran());
    setText(pos, COL_REMOVE, "XX");
    setText(pos, COL_TASATIPO_IVA, linea->ivalalbaran());
    setText(pos, COL_TIPO_IVA, linea->ivalalbaran());
    setText(pos, COL_PVPLALBARAN, linea->pvplalbaran());
    adjustRow(pos);

}


bool ListLinAlbaranClienteView::eventFilter( QObject *obj, QEvent *ev ) {
	fprintf(stderr,"eventFilter()\n");
    QString idArticle;
//    LinAlbaranCliente *linea=lineaact();
    LinAlbaranCliente *linea;//=m_lista.at(currentRow());
    
        if ( ev->type() == QEvent::KeyRelease ) {
//        if ( ev->type() == QEvent::KeyPress ) {
            QKeyEvent *k = (QKeyEvent *)ev;
            int col=currentColumn();
            int row=currentRow();
            switch (k->key()) {
            case Qt::Key_Plus:
		if (col == COL_DESCLALBARAN) {
                setText(row,col,editaTexto(text(row,col)));
		valueBudgetLineChanged(row,col);
                return TRUE;
		}// end if
                break;
            case Qt::Key_Asterisk:
	    	linea = lineaact();
                idArticle = searchArticle();
                linea->setidarticulo(idArticle);
                pintalinListLinAlbaranCliente(currentRow());
		return TRUE;
                break;
            case Qt::Key_Return:
            case Qt::Key_Enter:
                // Esto se hace porque en la ltima linea del qtable tiene un comportamiento raro. Se reportar�como bug a trolltech.
                switch (col) {
                case COL_CODARTICULO:
                    setCurrentCell(row, COL_DESCLALBARAN);
                    break;
                case COL_DESCLALBARAN:
                    setCurrentCell(row, COL_CANTLALBARAN);
                    break;
                case COL_CANTLALBARAN:
                    setCurrentCell(row, COL_PVPLALBARAN);
                    break;
                case COL_PVPLALBARAN:
                    setCurrentCell(row+1, COL_CODARTICULO);
                    break;
                }// end switch
                return TRUE;
                break;
            }// end switch
    }// end if
    return Q3Table::eventFilter( obj, ev );
} //end eventFilter



void ListLinAlbaranClienteView::valueBudgetLineChanged(int row, int col) {
    fprintf(stderr,"valueBudgetLineChanged \n");
    LinAlbaranCliente *linea;
    linea = lineaat(row);
    if (linea != NULL) {
        switch (col) {
        case COL_DESCONTLALBARAN: {
                float discountLine = text(row, COL_DESCONTLALBARAN).replace(",",".").toFloat();
                linea->setdescontlalbaran(QString::number(discountLine));
                break;
            }
        case COL_CODARTICULO:
            manageArticle(row);
            break;
        case COL_DESCLALBARAN:
            linea->setdesclalbaran(text(row,COL_DESCLALBARAN));
            break;
        case COL_CANTLALBARAN: {
                float cantLine = text(row, COL_CANTLALBARAN).replace(",",".").toFloat();
                linea->setcantlalbaran(QString::number(cantLine));
                break;
            }
        case COL_PVPLALBARAN:  {
                float pvpLine = text(row, COL_PVPLALBARAN).replace(",",".").toFloat();
                linea->setpvplalbaran(QString::number(pvpLine));
                break;
            }
        }// end switch
        pintalinListLinAlbaranCliente(row);
    }// end if
} //end valueBudgetLineChanged


/// Devuelve la linea que se esta tratando actualmente
LinAlbaranCliente *ListLinAlbaranClienteView::lineaact() {
    fprintf(stderr,"ListLinAlbaranClienteView::lineaact()\n");
    return lineaat(currentRow());
}// end lineaact


/// Devuelve la linea especificada, y si no existe se van creando lineas hasta que exista.
LinAlbaranCliente *ListLinAlbaranClienteView::lineaat(int row) {
    fprintf(stderr,"ListLinAlbaranCliente::lineaat(%d)\n", row);
    LinAlbaranCliente *linea;
    if (row >=0) {
        while (m_lista.at(row) == 0 ) {
	    fprintf(stderr,"Creamos la linea\n");
            linea=new LinAlbaranCliente(companyact);
            linea->setidalbaran(mdb_idalbaran);
            m_lista.append(linea);  
        }// end while	
	      return(m_lista.at(row));
    } else {
    	fprintf(stderr,"Linea inexistente\n");
        return NULL;
    }// end if

}// end lineaat


void ListLinAlbaranClienteView::manageArticle(int row) {
	fprintf(stderr,"manageArticle(%d)\n",row);
    LinAlbaranCliente *linea= lineaat(row);
    QString articleCode = text(row, COL_CODARTICULO);
    linea->setcodigocompletoarticulo(text(row,COL_CODARTICULO));
    pintalinListLinAlbaranCliente(row);
} //end manageArticle


QString ListLinAlbaranClienteView::searchArticle() {
    fprintf(stderr,"Busqueda de un art�ulo\n");
    articleslist *artlist = new articleslist(companyact, NULL, theApp->translate("Seleccione Art�ulo","company"),0,articleslist::SelectMode);
    // Esto es convertir un QWidget en un sistema modal de dialogo.
    this->setEnabled(false);
    artlist->show();
    while(!artlist->isHidden())
        theApp->processEvents();
    this->setEnabled(true);
    QString idArticle = artlist->idArticle();
    delete artlist;
    return idArticle;
}// end searchArticle
