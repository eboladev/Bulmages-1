//
// C++ Implementation: ListCompArticuloView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#define COL_IDARTICULO 0
#define COL_IDCOMPONENTE 1
#define COL_CODCOMPLETOCOMPONENTE 2
#define COL_NOMCOMPONENTE 3
#define COL_CANTCOMPARTICULO 4

#include "articulolist.h"
#include "listcomparticuloview.h"
#include "funcaux.h"
#include <Q3Table>
#include <QMessageBox>
#include <Q3PopupMenu>
//Added by qt3to4:
#include <QKeyEvent>
#include <QEvent>


ListCompArticuloView::ListCompArticuloView(QWidget * parent, const char * name) : Q3Table(parent, name), ListCompArticulo() {
    _depura("ListCompArticuloView::INIT_ListCompArticuloView()\n",0);

    /// Inicializamos la tabla de lineas de presupuesto
    setNumCols(5);
    setNumRows(100);
    horizontalHeader()->setLabel( COL_IDARTICULO, tr( "COL_IDARTICULO" ) );
    horizontalHeader()->setLabel( COL_IDCOMPONENTE, tr( "COL_IDCOMPONENTE" ) );
    horizontalHeader()->setLabel( COL_CODCOMPLETOCOMPONENTE, tr( "COL_CODCOMPLETOCOMPONENTE" ) );
    horizontalHeader()->setLabel( COL_NOMCOMPONENTE, tr( "COL_NOMCOMPONENTE" ) );
    horizontalHeader()->setLabel( COL_CANTCOMPARTICULO, tr( "COL_CANTCOMPARTICULO" ) );


    setColumnWidth(COL_IDARTICULO,100);
    setColumnWidth(COL_IDCOMPONENTE,300);
    setColumnWidth(COL_CODCOMPLETOCOMPONENTE,100);
    setColumnWidth(COL_NOMCOMPONENTE,100);
    setColumnWidth(COL_CANTCOMPARTICULO,74);

    hideColumn(COL_IDARTICULO);
    hideColumn(COL_IDCOMPONENTE);


    setSelectionMode( Q3Table::SingleRow );

    setColumnReadOnly(COL_NOMCOMPONENTE,true);
    // Establecemos el color de fondo de la rejilla. El valor lo tiene la clase configuracion que es global.
    setPaletteBackgroundColor(confpr->valor(CONF_BG_LINPRESUPUESTOS));

    connect(this, SIGNAL(valueChanged(int, int)), this, SLOT(valueListCompArticuloViewChanged(int , int )));

    connect(this, SIGNAL(contextMenuRequested(int, int, const QPoint &)), this, SLOT(contextMenu(int, int, const QPoint &)));

    installEventFilter(this);

    _depura("ListCompArticuloView::END_ListCompArticuloView()\n",0);
}


ListCompArticuloView::~ListCompArticuloView() {}


void ListCompArticuloView::pintaListCompArticulo() {
    fprintf(stderr,"INICIO de pintalistCompArticulo\n");
    setNumRows(0);
    setNumRows(100);
    /// \todo Habr� que vaciar la tabla para que el pintado fuera exacto.
    CompArticulo *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        setText(i, COL_IDARTICULO, linea->idarticulo());
        setText(i, COL_IDCOMPONENTE, linea->idcomponente());
        setText(i, COL_CODCOMPLETOCOMPONENTE, linea->codigocompletocomponente());
        setText(i, COL_NOMCOMPONENTE, linea->nomcomponente());
        setText(i, COL_CANTCOMPARTICULO, linea->cantcomparticulo());
        i++;
    }// end for
    fprintf(stderr,"FIN de pintalistCompArticulo\n");
}



void ListCompArticuloView::contextMenu ( int row, int, const QPoint & pos ) {
    Q3PopupMenu *popup;
    int opcion;
    popup = new Q3PopupMenu;
    popup->insertItem(tr(tr("Borrar Linea")),1);
    opcion = popup->exec(pos);
    delete popup;
    switch(opcion) {
    case 1:
        borraCompArticulo(row);
    }// end switch
}// end contextMenuRequested


void ListCompArticuloView::borraCompArticuloact() {
    borraCompArticulo(currentRow());
}// end borraCompArticuloact


void ListCompArticuloView::pintalinListCompArticulo(int pos) {
fprintf(stderr,"pintalinlistCompArticulo(%d)\n",pos);
    CompArticulo *linea;
    linea = m_lista.at(pos);
        setText(pos, COL_IDARTICULO, linea->idarticulo());
        setText(pos, COL_IDCOMPONENTE, linea->idcomponente());
        setText(pos, COL_CODCOMPLETOCOMPONENTE, linea->codigocompletocomponente());
        setText(pos, COL_NOMCOMPONENTE, linea->nomcomponente());
        setText(pos, COL_CANTCOMPARTICULO, linea->cantcomparticulo());

}


bool ListCompArticuloView::eventFilter( QObject *obj, QEvent *ev ) {
	_depura("ListCompArticuloView::INIT_eventFilter()\n",1);
    QString idArticle;
    CompArticulo *linea;//=m_lista.at(currentRow());
    
        if ( ev->type() == QEvent::KeyRelease ) {
            QKeyEvent *k = (QKeyEvent *)ev;
            int col=currentColumn();
            int row=currentRow();
            switch (k->key()) {
            case Qt::Key_Plus:
                break;
            case Qt::Key_Asterisk:
	    	linea = lineaact();
                idArticle = searchArticle();
                linea->setidcomponente(idArticle);
                pintalinListCompArticulo(currentRow());
		return TRUE;
                break;
            case Qt::Key_Return:
            case Qt::Key_Enter:
                // Esto se hace porque en la ltima linea del qtable tiene un comportamiento raro. Se reportar�como bug a trolltech.
                switch (col) {
                case COL_CODCOMPLETOCOMPONENTE:
                    setCurrentCell(row, COL_CANTCOMPARTICULO);
                    break;
                case COL_CANTCOMPARTICULO:
                    setCurrentCell(row+1, COL_CODCOMPLETOCOMPONENTE);
                    break;
                }// end switch
                return TRUE;
                break;
            }// end switch
    }// end if
	_depura("ListCompArticuloView::END_eventFilter()\n",1);
    return Q3Table::eventFilter( obj, ev );
} //end eventFilter



void ListCompArticuloView::valueListCompArticuloViewChanged(int row, int col) {
    fprintf(stderr,"valueBudgetLineChanged \n");
    CompArticulo *linea;
    linea = lineaat(row);
    if (linea != NULL) {
        switch (col) {

        case COL_CODCOMPLETOCOMPONENTE:
            manageArticle(row);
            break;
        case COL_CANTCOMPARTICULO: {
                float cantLine = text(row, COL_CANTCOMPARTICULO).replace(",",".").toFloat();
                linea->setcantcomparticulo(QString::number(cantLine));
                break;
            }
        }// end switch
        pintalinListCompArticulo(row);
    }// end if
} //end valueBudgetLineChanged


/// Devuelve la linea que se esta tratando actualmente
CompArticulo *ListCompArticuloView::lineaact() {
    fprintf(stderr,"ListCompArticuloView::lineaact()\n");
    return lineaat(currentRow());
}// end lineaact

/// Devuelve la linea especificada, y si no existe se van creando lineas hasta que exista.
CompArticulo *ListCompArticuloView::lineaat(int row) {
    fprintf(stderr,"listCompArticulo::lineaat(%d)\n", row);
    CompArticulo *linea;
    if (row >=0) {
        while (m_lista.at(row) == 0 ) {
	    fprintf(stderr,"Creamos la linea\n");
            linea=new CompArticulo(companyact);
            linea->setidarticulo(mdb_idarticulo);
            m_lista.append(linea);  
        }// end while	
	      return(m_lista.at(row));
    } else {
    	fprintf(stderr,"Linea inexistente\n");
        return NULL;
    }// end if

}// end lineaat


void ListCompArticuloView::manageArticle(int row) {
	fprintf(stderr,"manageArticle(%d)\n",row);
    CompArticulo *linea= lineaat(row);
    QString articleCode = text(row, COL_CODCOMPLETOCOMPONENTE);
    linea->setcodigocompletocomponente(text(row,COL_CODCOMPLETOCOMPONENTE));
    pintalinListCompArticulo(row);
} //end manageArticle


QString ListCompArticuloView::searchArticle() {
    _depura("ListCompArticuloView::searchArticle",0);
    ArticuloList *artlist = new ArticuloList(companyact, NULL, theApp->translate("Seleccione Art�ulo","company"),0,ArticuloList::SelectMode);
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
