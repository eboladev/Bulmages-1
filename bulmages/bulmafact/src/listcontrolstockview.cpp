//
// C++ Implementation: ListControlStockView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#define COL_IDINVENTARIO 0
#define COL_IDARTICULO 1
#define COL_CODARTICULO 2
#define COL_NOMARTICULO 3
#define COL_IDALMACEN 4
#define COL_CODALMACEN 5
#define COL_NOMALMACEN 6
#define COL_STOCKANTCONTROLSTOCK 7
#define COL_STOCKNEWCONTROLSTOCK 8



#include "articleslist.h"
#include "listcontrolstockview.h"
#include "listcontrolstock.h"
#include "funcaux.h"

#include <q3table.h>
#include <qmessagebox.h>
#include <q3popupmenu.h>
//Added by qt3to4:
#include <QKeyEvent>
#include <QEvent>


ListControlStockView::ListControlStockView(QWidget * parent, const char * name) : Q3Table(parent, name), ListControlStock() {
    /// Inicializamos la tabla de lineas de FacturaProveedor
    setNumCols(9);
    setNumRows(10000);

    horizontalHeader()->setLabel( COL_IDINVENTARIO, tr( "COL_IDINVENTARIO" ) );
    horizontalHeader()->setLabel( COL_IDARTICULO, tr( "COL_IDARTICULO" ) );
    horizontalHeader()->setLabel( COL_CODARTICULO, tr( "COL_CODARTICULO" ) );
    horizontalHeader()->setLabel( COL_NOMARTICULO, tr( "COL_NOMARTICULO" ) );
    horizontalHeader()->setLabel( COL_IDALMACEN, tr( "COL_IDALMACEN" ) );
    horizontalHeader()->setLabel( COL_STOCKNEWCONTROLSTOCK, tr( "COL_STOCKNEWCONTROLSTOCK" ) );
    horizontalHeader()->setLabel( COL_CODALMACEN, tr( "COL_CODALMACEN" ) );
    horizontalHeader()->setLabel( COL_STOCKANTCONTROLSTOCK, tr( "COL_STOCKANTCONTROLSTOCK" ) );
    horizontalHeader()->setLabel( COL_NOMALMACEN, tr( "COL_NOMALMACEN" ) );

    hideColumn(COL_IDINVENTARIO);
    hideColumn(COL_IDARTICULO);
    hideColumn(COL_IDALMACEN);

    setColumnWidth(COL_NOMARTICULO,250);
    setColumnWidth(COL_NOMALMACEN,250);
    setColumnWidth(COL_CODARTICULO,70);
    setColumnWidth(COL_CODALMACEN,70);
    setColumnWidth(COL_STOCKANTCONTROLSTOCK,100);
    setColumnWidth(COL_STOCKNEWCONTROLSTOCK,100);

    setSelectionMode( Q3Table::SingleRow );

    setColumnReadOnly(COL_NOMARTICULO,true);
    setColumnReadOnly(COL_NOMALMACEN,true);
    setColumnReadOnly(COL_STOCKANTCONTROLSTOCK,true);

    // Establecemos el color de fondo de la rejilla. El valor lo tiene la clase configuracion que es global.
    setPaletteBackgroundColor(confpr->valor(CONF_BG_LINFACTURASCLIENTE));

    connect(this, SIGNAL(valueChanged(int, int)), this, SLOT(valueBudgetLineChanged(int , int )));

    connect(this, SIGNAL(contextMenuRequested(int, int, const QPoint &)), this, SLOT(contextMenu(int, int, const QPoint &)));

    installEventFilter(this);

}


ListControlStockView::~ListControlStockView() {}


void ListControlStockView::pintaListControlStock() {
    _depura("ListControlStockView::pintaListControlStock",0);
    setNumRows(0);
    setNumRows(10000);
    /// \todo Habr� que vaciar la tabla para que el pintado fuera exacto.
    ControlStock *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        setText(i, COL_IDINVENTARIO, linea->idinventario());
	setText(i, COL_IDARTICULO, linea->idarticulo());
	setText(i, COL_NOMARTICULO, linea->nomarticulo());
	setText(i, COL_NOMALMACEN, linea->nomalmacen());
	setText(i, COL_CODARTICULO, linea->codigocompletoarticulo());
	setText(i, COL_CODALMACEN, linea->codigoalmacen());
	setText(i, COL_STOCKNEWCONTROLSTOCK, linea->stocknewcontrolstock());
	setText(i, COL_STOCKANTCONTROLSTOCK, linea->stockantcontrolstock());
	adjustRow(i);
        i++;
    }// end for
    fprintf(stderr,"FIN de pintaListControlStock\n");
}



void ListControlStockView::contextMenu ( int row, int, const QPoint & pos ) {
    Q3PopupMenu *popup;
    int opcion;
    popup = new Q3PopupMenu;
    popup->insertItem(tr(tr("Borrar Linea")),1);
    opcion = popup->exec(pos);
    delete popup;
    switch(opcion) {
    case 1:
        borrarControlStock(row);
    }// end switch
}// end contextMenuRequested

void ListControlStockView::pintalinListControlStock(int pos) {
    fprintf(stderr,"pintalinListControlStock(%d)\n",pos);
    ControlStock *linea;
    linea = m_lista.at(pos);
    setText(pos, COL_IDINVENTARIO, linea->idinventario());
	setText(pos, COL_IDARTICULO, linea->idarticulo());
	setText(pos, COL_NOMARTICULO, linea->nomarticulo());
	setText(pos, COL_NOMALMACEN, linea->nomalmacen());
	setText(pos, COL_CODARTICULO, linea->codigocompletoarticulo());
	setText(pos, COL_CODALMACEN, linea->codigoalmacen());
	setText(pos, COL_STOCKNEWCONTROLSTOCK, linea->stocknewcontrolstock());
	setText(pos, COL_STOCKANTCONTROLSTOCK, linea->stockantcontrolstock());
	adjustRow(pos);
}


bool ListControlStockView::eventFilter( QObject *obj, QEvent *ev ) {
    fprintf(stderr,"eventFilter()\n");
    QString idArticle;
    ControlStock *linea;
    if ( ev->type() == QEvent::KeyRelease ) {
        QKeyEvent *k = (QKeyEvent *)ev;
        int col=currentColumn();
        int row=currentRow();
        switch (k->key()) {
        case Qt::Key_Asterisk:
            linea = lineaact();
            idArticle = searchArticle();
            linea->setidarticulo(idArticle);
            pintalinListControlStock(row);
            return TRUE;
            break;

        case Qt::Key_Return:
        case Qt::Key_Enter:
            // Esto se hace porque en la ltima linea del qtable tiene un comportamiento raro. Se reportar�como bug a trolltech.
            switch (col) {
            case COL_CODARTICULO:
                setCurrentCell(row, COL_CODALMACEN);
                break;
            case COL_CODALMACEN:
                setCurrentCell(row, COL_STOCKNEWCONTROLSTOCK);
                break;
            case COL_STOCKNEWCONTROLSTOCK:
                setCurrentCell(row+1, COL_CODARTICULO);
                break;
            }// end switch
            return TRUE;
            break;

        }// end switch
    }// end if
    return Q3Table::eventFilter( obj, ev );
} //end eventFilter



void ListControlStockView::valueBudgetLineChanged(int row, int col) {
    fprintf(stderr,"valueBudgetLineChanged \n");
    ControlStock *linea;
    linea = lineaat(row);
    if (linea != NULL) {
        switch (col) {
        case COL_CODARTICULO:
            manageArticle(row);
            break;

        case COL_STOCKNEWCONTROLSTOCK:
            linea->setstocknewcontrolstock(text(row,COL_STOCKNEWCONTROLSTOCK));
            break;
        case COL_CODALMACEN:
            manageAlmacen(row);
            break;
        }// end switch
        pintalinListControlStock(row);
    }// end if
} //end valueBudgetLineChanged


/// Devuelve la linea que se esta tratando actualmente
ControlStock *ListControlStockView::lineaact() {
    return lineaat(currentRow());
}// end lineaact


/// Devuelve la linea especificada, y si no existe se van creando lineas hasta que exista.
ControlStock *ListControlStockView::lineaat(int row) {
    _depura("ListControlStock::lineaat()\n", 0);
    ControlStock *linea;
    if (row >=0) {
        while (m_lista.at(row) == 0 ) {
            fprintf(stderr,"Creamos la linea\n");
            linea=new ControlStock(companyact);
            linea->setidinventario(mdb_idinventario);
            m_lista.append(linea);
        }// end while
        return(m_lista.at(row));
    } else {
        fprintf(stderr,"Linea inexistente\n");
        return NULL;
    }// end if
}// end lineaat


void ListControlStockView::manageArticle(int row) {
    _depura("manageArticle()\n",0);
    ControlStock *linea= lineaat(row);
    QString articleCode = text(row, COL_CODARTICULO);
    linea->setcodigocompletoarticulo(text(row,COL_CODARTICULO));
    pintalinListControlStock(row);
} //end manageArticle


QString ListControlStockView::searchArticle() {
    _depura("Busqueda de un art�ulo\n",0);
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

void ListControlStockView::manageAlmacen(int row) {
    _depura("manageAlmacen()\n",0);
    ControlStock *linea= lineaat(row);
    linea->setcodigoalmacen(text(row,COL_CODALMACEN));
    pintalinListControlStock(row);
} //end manageArticle
