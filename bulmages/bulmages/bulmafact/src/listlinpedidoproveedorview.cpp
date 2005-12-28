//
// C++ Implementation: ListLinPedidoProveedorView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

/*
CREATE TABLE lpedidoproveedor (
   numlpedidoproveedor serial PRIMARY KEY,
   desclpedidoproveedor character varying(150),
   cantlpedidoproveedor float,
   pvplpedidoproveedor float,
   prevlpedidoproveedor date,
   ivalpedidoproveedor numeric(5,2),
   descuentolpedidoproveedor float,   
   idpedidoproveedor integer NOT NULL REFERENCES pedidoproveedor(idpedidoproveedor),
   idarticulo integer REFERENCES articulo(idarticulo)
);
*/

#define COL_PUNTEO 0
#define COL_NUMLPEDIDOPROVEEDOR 1
#define COL_IDARTICULO 2
#define COL_CODARTICULO 3
#define COL_NOMARTICULO 4
#define COL_DESCLPEDIDOPROVEEDOR 5
#define COL_CANTLPEDIDOPROVEEDOR 6
#define COL_PVPLPEDIDOPROVEEDOR 7
#define COL_DESCUENTOLPEDIDOPROVEEDOR 8
#define COL_IDPEDIDOPROVEEDOR 9
#define COL_REMOVE 10
#define COL_TASATIPO_IVA 11
#define COL_TIPO_IVA 12
#define COL_PREVLPEDIDOPROVEEDOR 13
#define COL_IVALPEDIDOPROVEEDOR 14

#include "articleslist.h"
#include "listlinpedidoproveedorview.h"
#include "linpedidoproveedor.h"
#include "funcaux.h"
#include <q3table.h>
#include <qmessagebox.h>
#include <q3popupmenu.h>
//Added by qt3to4:
#include <QKeyEvent>
#include <QEvent>


ListLinPedidoProveedorView::ListLinPedidoProveedorView(QWidget * parent, const char * name) : Q3Table(parent, name), ListLinPedidoProveedor() {
    /// Inicializamos la tabla de lineas de Factura
    setNumCols(15);
    setNumRows(100);
    horizontalHeader()->setLabel( COL_PUNTEO, tr( "P" ) );
    horizontalHeader()->setLabel( COL_NUMLPEDIDOPROVEEDOR, tr( "N L�ea" ) );
    horizontalHeader()->setLabel( COL_DESCLPEDIDOPROVEEDOR, tr( "Descripci�" ) );
    horizontalHeader()->setLabel( COL_CANTLPEDIDOPROVEEDOR, tr( "Cantidad" ) );
    horizontalHeader()->setLabel( COL_PVPLPEDIDOPROVEEDOR, tr( "Precio" ) );
    horizontalHeader()->setLabel( COL_DESCUENTOLPEDIDOPROVEEDOR, tr( "Descuento" ) );
    horizontalHeader()->setLabel( COL_IDPEDIDOPROVEEDOR, tr( "N Pedido" ) );
    horizontalHeader()->setLabel( COL_IDARTICULO, tr( "Art�ulo" ) );
    horizontalHeader()->setLabel( COL_CODARTICULO, tr( "C�igo Art�o" ) );
    horizontalHeader()->setLabel( COL_NOMARTICULO, tr( "Descripci� Art�ulo" ) );
    horizontalHeader()->setLabel( COL_TASATIPO_IVA, tr( "% IVA" ) );
    horizontalHeader()->setLabel( COL_TIPO_IVA, tr( "Tipo IVA" ) );
    horizontalHeader()->setLabel( COL_PREVLPEDIDOPROVEEDOR, tr( "COL_PREVLPEDIDOPROVEEDOR" ) );
    horizontalHeader()->setLabel( COL_IVALPEDIDOPROVEEDOR, tr( "COL_IVALPEDIDOPROVEEDOR" ) );

    setColumnWidth(COL_PUNTEO,15);
    setColumnWidth(COL_NUMLPEDIDOPROVEEDOR,100);
    setColumnWidth(COL_DESCLPEDIDOPROVEEDOR,300);
    setColumnWidth(COL_CANTLPEDIDOPROVEEDOR,100);
    setColumnWidth(COL_PVPLPEDIDOPROVEEDOR,100);
    setColumnWidth(COL_DESCUENTOLPEDIDOPROVEEDOR,74);
    setColumnWidth(COL_IDPEDIDOPROVEEDOR,100);
    setColumnWidth(COL_IDARTICULO,100);
    setColumnWidth(COL_CODARTICULO,100);
    setColumnWidth(COL_NOMARTICULO,300);
    setColumnWidth(COL_TASATIPO_IVA,50);
    setColumnWidth(COL_TIPO_IVA,50);

    hideColumn(COL_NUMLPEDIDOPROVEEDOR);
    hideColumn(COL_IDPEDIDOPROVEEDOR);
    hideColumn(COL_IDARTICULO);
    hideColumn(COL_REMOVE);
    hideColumn(COL_TASATIPO_IVA);
    hideColumn(COL_TIPO_IVA);

    setSelectionMode( Q3Table::SingleRow );

    setColumnReadOnly(COL_NOMARTICULO,true);
    // Establecemos el color de fondo de la rejilla. El valor lo tiene la clase configuracion que es global.
    setPaletteBackgroundColor(confpr->valor(CONF_BG_LINPEDIDOSCLIENTE));

    connect(this, SIGNAL(valueChanged(int, int)), this, SLOT(valueBudgetLineChanged(int , int )));

    connect(this, SIGNAL(contextMenuRequested(int, int, const QPoint &)), this, SLOT(contextMenu(int, int, const QPoint &)));

    installEventFilter(this);

}


ListLinPedidoProveedorView::~ListLinPedidoProveedorView() {}


void ListLinPedidoProveedorView::pintaListLinPedidoProveedor() {
    fprintf(stderr,"INICIO de pintaListLinPedidoProveedor\n");
    setNumRows(0);
    setNumRows(100);

   for (int j=0;j<100;j++) {
	Q3CheckTableItem *check = new Q3CheckTableItem(this,0);
        setItem(j,COL_PUNTEO,check);
   }// end for

    /// \todo Habr� que vaciar la tabla para que el pintado fuera exacto.
    LinPedidoProveedor *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        setText(i, COL_NUMLPEDIDOPROVEEDOR, linea->numlpedidoproveedor());
        setText(i, COL_IDARTICULO, linea->idarticulo());
        setText(i, COL_CODARTICULO, linea->codigocompletoarticulo());
        setText(i, COL_NOMARTICULO, linea->nomarticulo());
        setText(i, COL_DESCLPEDIDOPROVEEDOR, linea->desclpedidoproveedor());
        setText(i, COL_CANTLPEDIDOPROVEEDOR, linea->cantlpedidoproveedor());
        setText(i, COL_DESCUENTOLPEDIDOPROVEEDOR, linea->descuentolpedidoproveedor());
        setText(i, COL_IDPEDIDOPROVEEDOR, linea->idpedidoproveedor());
        setText(i, COL_REMOVE, "XX");
        setText(i, COL_TASATIPO_IVA, "XX");
        setText(i, COL_TIPO_IVA, "XX");
        setText(i, COL_PVPLPEDIDOPROVEEDOR, linea->pvplpedidoproveedor());
	setText(i, COL_PREVLPEDIDOPROVEEDOR, linea->prevlpedidoproveedor());
	setText(i, COL_IVALPEDIDOPROVEEDOR, linea->ivalpedidoproveedor());

        /// Ponemos un checkbox para el punteo.
        Q3CheckTableItem *check =(Q3CheckTableItem *) item(i,COL_PUNTEO);
        if (linea->puntlpedidoproveedor() == "TRUE")
            check->setChecked(TRUE);
	adjustRow(i);
        i++;
    }// end for
    _depura("FIN de pintaListLinPedidoProveedor\n");
}



void ListLinPedidoProveedorView::contextMenu ( int row, int, const QPoint & pos ) {
    Q3PopupMenu *popup;
    int opcion;
    popup = new Q3PopupMenu;
    popup->insertItem(tr(tr("Borrar Linea")),1);
    opcion = popup->exec(pos);
    delete popup;
    switch(opcion) {
    case 1:
        borraLinPedidoProveedor(row);
    }// end switch
}// end contextMenuRequested


void ListLinPedidoProveedorView::borraLinPedidoProveedoract() {
    borraLinPedidoProveedor(currentRow());
}// end borraLinPedidoProveedoract


void ListLinPedidoProveedorView::pintalinListLinPedidoProveedor(int pos) {
fprintf(stderr,"pintalinListLinPedidoProveedor(%d)\n",pos);
    LinPedidoProveedor *linea;
    linea = m_lista.at(pos);
        setText(pos, COL_NUMLPEDIDOPROVEEDOR, linea->numlpedidoproveedor());
        setText(pos, COL_IDARTICULO, linea->idarticulo());
        setText(pos, COL_CODARTICULO, linea->codigocompletoarticulo());
        setText(pos, COL_NOMARTICULO, linea->nomarticulo());
        setText(pos, COL_DESCLPEDIDOPROVEEDOR, linea->desclpedidoproveedor());
        setText(pos, COL_CANTLPEDIDOPROVEEDOR, linea->cantlpedidoproveedor());
        setText(pos, COL_DESCUENTOLPEDIDOPROVEEDOR, linea->descuentolpedidoproveedor());
        setText(pos, COL_IDPEDIDOPROVEEDOR, linea->idpedidoproveedor());
        setText(pos, COL_REMOVE, "XX");
        setText(pos, COL_TASATIPO_IVA, "XX");
        setText(pos, COL_TIPO_IVA, "XX");
        setText(pos, COL_PVPLPEDIDOPROVEEDOR, linea->pvplpedidoproveedor());
	setText(pos, COL_PREVLPEDIDOPROVEEDOR, linea->prevlpedidoproveedor());
	setText(pos, COL_IVALPEDIDOPROVEEDOR, linea->ivalpedidoproveedor());
    Q3CheckTableItem *check = (Q3CheckTableItem *) item(pos, COL_PUNTEO);
    if (linea->puntlpedidoproveedor() == "TRUE")
        check->setChecked(TRUE);
    else
        check->setChecked(FALSE);
	adjustRow(pos);
}


bool ListLinPedidoProveedorView::eventFilter( QObject *obj, QEvent *ev ) {
	fprintf(stderr,"eventFilter()\n");
    QString idArticle;
//    LinPedidoProveedor *linea=lineaact();
    LinPedidoProveedor *linea;//=m_lista.at(currentRow());
    
        if ( ev->type() == QEvent::KeyRelease ) {
//        if ( ev->type() == QEvent::KeyPress ) {
            QKeyEvent *k = (QKeyEvent *)ev;
            int col=currentColumn();
            int row=currentRow();
            switch (k->key()) {
            case Qt::Key_Plus:
		if (col == COL_DESCLPEDIDOPROVEEDOR) {
                setText(row,col,editaTexto(text(row,col)));
		valueBudgetLineChanged(row,col);
                return TRUE;
		}// end if
                break;
            case Qt::Key_Asterisk:
	    	linea = lineaact();
                idArticle = searchArticle();
                linea->setidarticulo(idArticle);
                pintalinListLinPedidoProveedor(currentRow());
		return TRUE;
                break;
            case Qt::Key_Return:
            case Qt::Key_Enter:
                // Esto se hace porque en la ltima linea del qtable tiene un comportamiento raro. Se reportar�como bug a trolltech.
                switch (col) {
                case COL_CODARTICULO:
                    setCurrentCell(row, COL_DESCLPEDIDOPROVEEDOR);
                    break;
                case COL_DESCLPEDIDOPROVEEDOR:
                    setCurrentCell(row, COL_CANTLPEDIDOPROVEEDOR);
                    break;
                case COL_CANTLPEDIDOPROVEEDOR:
                    setCurrentCell(row, COL_PVPLPEDIDOPROVEEDOR);
                    break;
                case COL_PVPLPEDIDOPROVEEDOR:
                    setCurrentCell(row+1, COL_CODARTICULO);
                    break;
                }// end switch
                return TRUE;
                break;
            }// end switch
    }// end if
    return Q3Table::eventFilter( obj, ev );
} //end eventFilter



void ListLinPedidoProveedorView::valueBudgetLineChanged(int row, int col) {
    fprintf(stderr,"valueBudgetLineChanged \n");
    LinPedidoProveedor *linea;
    linea = lineaat(row);
    if (linea != NULL) {
        switch (col) {
        case COL_DESCUENTOLPEDIDOPROVEEDOR: {
                float discountLine = text(row, COL_DESCUENTOLPEDIDOPROVEEDOR).replace(",",".").toFloat();
                linea->setdescuentolpedidoproveedor(QString::number(discountLine));
                break;
            }
        case COL_CODARTICULO:
            manageArticle(row);
            break;
        case COL_DESCLPEDIDOPROVEEDOR:
            linea->setdesclpedidoproveedor(text(row,COL_DESCLPEDIDOPROVEEDOR));
            break;
        case COL_CANTLPEDIDOPROVEEDOR: {
                float cantLine = text(row, COL_CANTLPEDIDOPROVEEDOR).replace(",",".").toFloat();
                linea->setcantlpedidoproveedor(QString::number(cantLine));
                break;
            }
        case COL_PVPLPEDIDOPROVEEDOR:  {
                float pvpLine = text(row, COL_PVPLPEDIDOPROVEEDOR).replace(",",".").toFloat();
                linea->setpvplpedidoproveedor(QString::number(pvpLine));
                break;
            }
        case COL_TASATIPO_IVA: {
                float ivaLine = text(row, COL_TASATIPO_IVA).replace(",",".").toFloat();
                linea->setivalpedidoproveedor(QString::number(ivaLine));
                break;
            }// end case
        case COL_PUNTEO: {
                Q3CheckTableItem *check = (Q3CheckTableItem *) item(row, COL_PUNTEO);
                if (check->isChecked())
                    linea->setpuntlpedidoproveedor("TRUE");
                else
                    linea->setpuntlpedidoproveedor("FALSE");
            }// end case
        }// end switch
        pintalinListLinPedidoProveedor(row);
    }// end if
} //end valueBudgetLineChanged


/// Devuelve la linea que se esta tratando actualmente
LinPedidoProveedor *ListLinPedidoProveedorView::lineaact() {
    fprintf(stderr,"ListLinPedidoProveedorView::lineaact()\n");
    return lineaat(currentRow());
}// end lineaact


/// Devuelve la linea especificada, y si no existe se van creando lineas hasta que exista.
LinPedidoProveedor *ListLinPedidoProveedorView::lineaat(int row) {
    fprintf(stderr,"ListLinPedidoProveedor::lineaat(%d)\n", row);
    LinPedidoProveedor *linea;
    if (row >=0) {
        while (m_lista.at(row) == 0 ) {
	    fprintf(stderr,"Creamos la linea\n");
            linea=new LinPedidoProveedor(companyact);
            linea->setidpedidoproveedor(mdb_idpedidoproveedor);
            m_lista.append(linea);  
        }// end while	
	      return(m_lista.at(row));
    } else {
    	fprintf(stderr,"Linea inexistente\n");
        return NULL;
    }// end if

}// end lineaat


void ListLinPedidoProveedorView::manageArticle(int row) {
	fprintf(stderr,"manageArticle(%d)\n",row);
    LinPedidoProveedor *linea= lineaat(row);
    QString articleCode = text(row, COL_CODARTICULO);
    linea->setcodigocompletoarticulo(text(row,COL_CODARTICULO));
    pintalinListLinPedidoProveedor(row);
} //end manageArticle


QString ListLinPedidoProveedorView::searchArticle() {
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
