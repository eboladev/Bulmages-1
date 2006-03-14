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

#include <QMessageBox>
#include <QMenu>
#include <QKeyEvent>
#include <QEvent>

#include "articulolist.h"
#include "comparticulolistview.h"
#include "funcaux.h"

#define COL_IDARTICULO 0
#define COL_IDCOMPONENTE 1
#define COL_CODCOMPLETOCOMPONENTE 2
#define COL_NOMCOMPONENTE 3
#define COL_CANTCOMPARTICULO 4





ListCompArticuloView::ListCompArticuloView(QWidget * parent, const char * name) : QTableWidget2(parent), ListCompArticulo() {
    _depura("ListCompArticuloView::INIT_ListCompArticuloView()\n",0);

    /// Inicializamos la tabla de lineas de presupuesto
    setColumnCount(5);
    setRowCount(100);


    QStringList headers;
    headers << tr( "COL_IDARTICULO" ) << tr( "COL_IDCOMPONENTE" ) << tr( "COL_CODCOMPLETOCOMPONENTE" ) << tr( "COL_NOMCOMPONENTE" ) << tr( "COL_CANTCOMPARTICULO" );

    setHorizontalHeaderLabels (headers);


    setColumnWidth(COL_IDARTICULO,100);
    setColumnWidth(COL_IDCOMPONENTE,100);
    setColumnWidth(COL_CODCOMPLETOCOMPONENTE,250);
    setColumnWidth(COL_NOMCOMPONENTE,100);
    setColumnWidth(COL_CANTCOMPARTICULO,100);

    hideColumn(COL_IDARTICULO);
    hideColumn(COL_IDCOMPONENTE);



    setSelectionMode( QAbstractItemView::SingleSelection );
    setSelectionBehavior( QAbstractItemView::SelectRows);
    setPaletteBackgroundColor(confpr->valor(CONF_BG_LINPRESUPUESTOS));
    setAlternatingRowColors(TRUE);
    setSortingEnabled(TRUE);

    installEventFilter(this);
    connect (this, SIGNAL(cellChanged(int,int)), this, SLOT(s_cellChanged(int,int)));
    connect (this, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(s_currentCellChanged(int,int,int,int)));

    _depura("ListCompArticuloView::END_ListCompArticuloView()\n",0);
}


ListCompArticuloView::~ListCompArticuloView() {}


void ListCompArticuloView::pintar(int pos) {
    _depura("ListCompArticuloView::pintar(int)\n",0);
    CompArticulo *linea;
    linea = lineaat(pos);
    item(pos, COL_IDARTICULO)->setText(linea->idarticulo());
    item(pos, COL_IDCOMPONENTE)->setText(linea->idcomponente());
    item(pos, COL_CODCOMPLETOCOMPONENTE)->setText(linea->codigocompletocomponente());
    item(pos, COL_NOMCOMPONENTE)->setText(linea->nomcomponente());
    item(pos, COL_CANTCOMPARTICULO)->setText(linea->cantcomparticulo());
}


void ListCompArticuloView::pintar() {
    _depura("ListCompArticuloView::pintar",0);
    setRowCount(0);
    setRowCount(100);
    generar();
    CompArticulo *linea;
    uint i = 0;
    for ( linea = m_lista.at(0); linea; linea = m_lista.at(i) ) {
        pintar(i);
        i++;
    }// end for
    _depura("FIN de pintalistCompArticulo\n",0);
}


void ListCompArticuloView::contextMenuEvent (QContextMenuEvent *) {
    _depura("ListLTarifaView::contextMenuEvent",0);
    int a = currentRow();
    if ( a < 0)
        return;
    QMenu *popup = new QMenu(this);
    QAction *del = popup->addAction(tr("Borrar"));
    QAction *opcion = popup->exec(QCursor::pos());
    if (opcion == del)
        borrar(a);
    delete popup;
}







bool ListCompArticuloView::eventFilter( QObject *obj, QEvent *ev ) {
    _depura("ListCompArticuloView::INIT_eventFilter()\n",0);
    QString idArticle;
    CompArticulo *linea;
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
            pintar(currentRow());
            return TRUE;
            break;
        case Qt::Key_Return:
        case Qt::Key_Enter:
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
    _depura("ListCompArticuloView::END_eventFilter()\n",0);
    return QTableWidget2::eventFilter( obj, ev );
} //end eventFilter

void ListCompArticuloView::s_currentCellChanged(int row, int col, int prow, int pcol) {
    if (prow >=0)
        pintar(prow);
}

void ListCompArticuloView::s_cellChanged(int row, int col) {
    if(item(row,col)->text() == "")
        return;
    CompArticulo *linea = lineaat(row);
    switch (col) {
    case COL_CODCOMPLETOCOMPONENTE:
        linea->setcodigocompletocomponente(item(row,col)->text());
        break;
    case COL_CANTCOMPARTICULO:
        linea->setcantcomparticulo(item(row,col)->text());
        break;
    }// end switch
}



/// Devuelve la linea que se esta tratando actualmente
CompArticulo *ListCompArticuloView::lineaact() {
    _depura("ListCompArticuloView::lineaact()\n",0);
    return lineaat(currentRow());
}// end lineaact


/// Devuelve la linea especificada, y si no existe se van creando lineas hasta que exista.
CompArticulo *ListCompArticuloView::lineaat(int row) {
    _depura("listCompArticulo::lineaat()\n", 0);
    CompArticulo *linea;
    if (row >=0) {
        while (m_lista.at(row) == 0 ) {
            linea=new CompArticulo(companyact);
            linea->setidarticulo(mdb_idarticulo);
            m_lista.append(linea);
        }// end while
        return(m_lista.at(row));
    } else {
        _depura("Linea inexistente\n",0);
        return NULL;
    }// end if
    _depura("listCompArticulo::lineaat()\n", 0);
}// end lineaat


void ListCompArticuloView::manageArticle(int row) {
    _depura("manageArticle()\n",0);
    CompArticulo *linea= lineaat(row);
    QString articleCode = item(row, COL_CODCOMPLETOCOMPONENTE)->text();
    linea->setcodigocompletocomponente(item(row,COL_CODCOMPLETOCOMPONENTE)->text());
    pintar(row);
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


void ListCompArticuloView:: generar() {
    for(int pos=0 ; pos < 100; pos++) {
        setText(pos, COL_IDARTICULO,"");
        setText(pos, COL_IDCOMPONENTE,"");
        setText(pos, COL_CODCOMPLETOCOMPONENTE,"");
        setText(pos, COL_NOMCOMPONENTE,"");
        setText(pos, COL_CANTCOMPARTICULO,"");
    }//end for
}
