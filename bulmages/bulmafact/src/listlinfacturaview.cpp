//
// C++ Implementation: ListLinFacturaView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#define COL_IDLFACTURA 0
#define COL_IDARTICULO 1
#define COL_CODARTICULO 2
#define COL_NOMARTICULO 3
#define COL_DESCLFACTURA 4
#define COL_CANTLFACTURA 5
#define COL_PVPLFACTURA 6
#define COL_DESCUENTOLFACTURA 7
#define COL_IDFACTURA 8
#define COL_REMOVE 9
#define COL_TASATIPO_IVA 10
#define COL_TIPO_IVA 11


#include "articleslist.h"
#include "listlinfacturaview.h"
#include <qtable.h>
#include <qmessagebox.h>
#include <qpopupmenu.h>


ListLinFacturaView::ListLinFacturaView(QWidget * parent, const char * name) : QTable(parent, name), ListLinFactura() {
    /// Inicializamos la tabla de lineas de Factura
    setNumCols(12);
    setNumRows(100);
    horizontalHeader()->setLabel( COL_IDLFACTURA, tr( "N L�ea" ) );
    horizontalHeader()->setLabel( COL_DESCLFACTURA, tr( "Descripci�" ) );
    horizontalHeader()->setLabel( COL_CANTLFACTURA, tr( "Cantidad" ) );
    horizontalHeader()->setLabel( COL_PVPLFACTURA, tr( "Precio" ) );
    horizontalHeader()->setLabel( COL_DESCUENTOLFACTURA, tr( "Descuento" ) );
    horizontalHeader()->setLabel( COL_IDFACTURA, tr( "N Pedido" ) );
    horizontalHeader()->setLabel( COL_IDARTICULO, tr( "Art�ulo" ) );
    horizontalHeader()->setLabel( COL_CODARTICULO, tr( "C�igo Art�ulo" ) );
    horizontalHeader()->setLabel( COL_NOMARTICULO, tr( "Descripci� Art�ulo" ) );
    horizontalHeader()->setLabel( COL_TASATIPO_IVA, tr( "% IVA" ) );
    horizontalHeader()->setLabel( COL_TIPO_IVA, tr( "Tipo IVA" ) );

    setColumnWidth(COL_IDLFACTURA,100);
    setColumnWidth(COL_DESCLFACTURA,300);
    setColumnWidth(COL_CANTLFACTURA,100);
    setColumnWidth(COL_PVPLFACTURA,100);
    setColumnWidth(COL_DESCUENTOLFACTURA,74);
    setColumnWidth(COL_IDFACTURA,100);
    setColumnWidth(COL_IDARTICULO,100);
    setColumnWidth(COL_CODARTICULO,100);
    setColumnWidth(COL_NOMARTICULO,300);
    setColumnWidth(COL_TASATIPO_IVA,50);
    setColumnWidth(COL_TIPO_IVA,50);

    hideColumn(COL_IDLFACTURA);
    hideColumn(COL_IDFACTURA);
    hideColumn(COL_IDARTICULO);
    hideColumn(COL_REMOVE);
    hideColumn(COL_TASATIPO_IVA);
    hideColumn(COL_TIPO_IVA);

    setSelectionMode( QTable::SingleRow );

    setColumnReadOnly(COL_NOMARTICULO,true);
    // Establecemos el color de fondo de la rejilla. El valor lo tiene la clase configuracion que es global.
    setPaletteBackgroundColor("#DDAAAA");

    connect(this, SIGNAL(valueChanged(int, int)), this, SLOT(valueBudgetLineChanged(int , int )));

    connect(this, SIGNAL(contextMenuRequested(int, int, const QPoint &)), this, SLOT(contextMenu(int, int, const QPoint &)));

    installEventFilter(this);

}


ListLinFacturaView::~ListLinFacturaView() {}


void ListLinFacturaView::pintaListLinFactura() {
    fprintf(stderr,"INICIO de pintaListLinFactura\n");
    setNumRows(0);
    setNumRows(100);
    /// \todo Habr� que vaciar la tabla para que el pintado fuera exacto.
    LinFactura *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        setText(i, COL_IDLFACTURA, linea->idlFactura());
        setText(i, COL_IDARTICULO, linea->idarticulo());
        setText(i, COL_CODARTICULO, linea->codigocompletoarticulo());
        setText(i, COL_NOMARTICULO, linea->nomarticulo());
        setText(i, COL_DESCLFACTURA, linea->desclFactura());
        setText(i, COL_CANTLFACTURA, linea->cantlFactura());
        setText(i, COL_DESCUENTOLFACTURA, linea->descuentolFactura());
        setText(i, COL_IDFACTURA, linea->idFactura());
        setText(i, COL_REMOVE, "XX");
        setText(i, COL_TASATIPO_IVA, linea->ivalFactura()+"%");
        setText(i, COL_TIPO_IVA, linea->idlFactura());
        setText(i, COL_PVPLFACTURA, linea->pvplFactura());
        i++;
    }// end for
    fprintf(stderr,"FIN de pintaListLinFactura\n");
}



void ListLinFacturaView::contextMenu ( int row, int, const QPoint & pos ) {
    QPopupMenu *popup;
    int opcion;
    popup = new QPopupMenu;
    popup->insertItem(tr(tr("Borrar Linea")),1);
    opcion = popup->exec(pos);
    delete popup;
    switch(opcion) {
    case 1:
        borraLinFactura(row);
    }// end switch
}// end contextMenuRequested


void ListLinFacturaView::borraLinFacturaact() {
    borraLinFactura(currentRow());
}// end borraLinFacturaact


void ListLinFacturaView::pintalinListLinFactura(int pos) {
fprintf(stderr,"pintalinListLinFactura(%d)\n",pos);
    LinFactura *linea;
    linea = m_lista.at(pos);
    setText(pos, COL_IDLFACTURA, linea->idlFactura());
    setText(pos, COL_IDARTICULO, linea->idarticulo());
    setText(pos, COL_CODARTICULO, linea->codigocompletoarticulo());
    setText(pos, COL_NOMARTICULO, linea->nomarticulo());
    setText(pos, COL_DESCLFACTURA, linea->desclFactura());
    setText(pos, COL_CANTLFACTURA, linea->cantlFactura());
    setText(pos, COL_DESCUENTOLFACTURA, linea->descuentolFactura());
    setText(pos, COL_IDFACTURA, linea->idFactura());
    setText(pos, COL_REMOVE, "XX");
    setText(pos, COL_TASATIPO_IVA, linea->ivalFactura()+"%");
    setText(pos, COL_TIPO_IVA, linea->idlFactura());
    setText(pos, COL_PVPLFACTURA, linea->pvplFactura());

}


bool ListLinFacturaView::eventFilter( QObject *obj, QEvent *ev ) {
	fprintf(stderr,"eventFilter()\n");
    QString idArticle;
//    LinFactura *linea=lineaact();
    LinFactura *linea;//=m_lista.at(currentRow());
    
        if ( ev->type() == QEvent::KeyRelease ) {
//        if ( ev->type() == QEvent::KeyPress ) {
            QKeyEvent *k = (QKeyEvent *)ev;
            int col=currentColumn();
            int row=currentRow();
            switch (k->key()) {
            case Qt::Key_Plus:
                break;
            case Qt::Key_Asterisk:
	    	linea = lineaact();
                idArticle = searchArticle();
                linea->setidarticulo(idArticle);
                pintalinListLinFactura(currentRow());
		return TRUE;
                break;
            case Qt::Key_Return:
            case Qt::Key_Enter:
                // Esto se hace porque en la ltima linea del qtable tiene un comportamiento raro. Se reportar�como bug a trolltech.
                switch (col) {
                case COL_CODARTICULO:
                    setCurrentCell(row, COL_DESCLFACTURA);
                    break;
                case COL_DESCLFACTURA:
                    setCurrentCell(row, COL_CANTLFACTURA);
                    break;
                case COL_CANTLFACTURA:
                    setCurrentCell(row, COL_PVPLFACTURA);
                    break;
                case COL_PVPLFACTURA:
                    setCurrentCell(row+1, COL_CODARTICULO);
                    break;
                }// end switch
                return TRUE;
                break;
            }// end switch
    }// end if
    return QTable::eventFilter( obj, ev );
} //end eventFilter



void ListLinFacturaView::valueBudgetLineChanged(int row, int col) {
    fprintf(stderr,"valueBudgetLineChanged \n");
    LinFactura *linea;
    linea = lineaat(row);
    if (linea != NULL) {
        switch (col) {
        case COL_DESCUENTOLFACTURA: {
                float discountLine = text(row, COL_DESCUENTOLFACTURA).replace(",",".").toFloat();
                linea->setdescuentolFactura(QString::number(discountLine));
                break;
            }
        case COL_CODARTICULO:
            manageArticle(row);
            break;
        case COL_DESCLFACTURA:
            linea->setdesclFactura(text(row,COL_DESCLFACTURA));
            break;
        case COL_CANTLFACTURA: {
                float cantLine = text(row, COL_CANTLFACTURA).replace(",",".").toFloat();
                linea->setcantlFactura(QString::number(cantLine));
                break;
            }
        case COL_PVPLFACTURA:  {
                float pvpLine = text(row, COL_PVPLFACTURA).replace(",",".").toFloat();
                linea->setpvplFactura(QString::number(pvpLine));
                break;
            }
        case COL_TASATIPO_IVA: {
                float ivaLine = text(row, COL_TASATIPO_IVA).replace(",",".").toFloat();
                linea->setivalFactura(QString::number(ivaLine));
                break;
            }// end case
        }// end switch
        pintalinListLinFactura(row);
    }// end if
} //end valueBudgetLineChanged


/// Devuelve la linea que se esta tratando actualmente
LinFactura *ListLinFacturaView::lineaact() {
    fprintf(stderr,"ListLinFacturaView::lineaact()\n");
    return lineaat(currentRow());
}// end lineaact


/// Devuelve la linea especificada, y si no existe se van creando lineas hasta que exista.
LinFactura *ListLinFacturaView::lineaat(int row) {
    fprintf(stderr,"ListLinFactura::lineaat(%d)\n", row);
    LinFactura *linea;
    if (row >=0) {
        while (m_lista.at(row) == 0 ) {
	    fprintf(stderr,"Creamos la linea\n");
            linea=new LinFactura(companyact);
            linea->setidFactura(mdb_idfactura);
            m_lista.append(linea);  
        }// end while	
	      return(m_lista.at(row));
    } else {
    	fprintf(stderr,"Linea inexistente\n");
        return NULL;
    }// end if

}// end lineaat


void ListLinFacturaView::manageArticle(int row) {
	fprintf(stderr,"manageArticle(%d)\n",row);
    LinFactura *linea= lineaat(row);
    QString articleCode = text(row, COL_CODARTICULO);
    linea->setcodigocompletoarticulo(text(row,COL_CODARTICULO));
    pintalinListLinFactura(row);
} //end manageArticle


QString ListLinFacturaView::searchArticle() {
    fprintf(stderr,"Busqueda de un art�ulo\n");
    articleslist *artlist = new articleslist(companyact, NULL, theApp->translate("Seleccione Art�ulo","company"));
    // , WType_Dialog| WShowModal
    artlist->modoseleccion();
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
