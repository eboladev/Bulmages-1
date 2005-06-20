//
// C++ Implementation: ListDescuentoPedidoClienteView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#define COL_IDDPEDIDOCLIENTE 0
#define COL_CONCEPTDPEDIDOCLIENTE 1
#define COL_PROPORCIONDPEDIDOCLIENTE 2
#define COL_IDPEDIDOCLIENTE 3


#include "listdescpedidoclienteview.h"
#include <qtable.h>
#include <qmessagebox.h>
#include <qpopupmenu.h>


ListDescuentoPedidoClienteView::ListDescuentoPedidoClienteView(QWidget * parent, const char * name) : QTable(parent, name), ListDescuentoPedidoCliente() {
    /// Inicializamos la tabla de lineas de pedidocliente
    setNumCols(4);
    setNumRows(100);
    horizontalHeader()->setLabel( COL_IDDPEDIDOCLIENTE, tr( "N L�ea" ) );
    horizontalHeader()->setLabel( COL_CONCEPTDPEDIDOCLIENTE, tr( "Descripci�" ) );
    horizontalHeader()->setLabel( COL_PROPORCIONDPEDIDOCLIENTE, tr( "Cantidad" ) );
    horizontalHeader()->setLabel( COL_IDPEDIDOCLIENTE, tr( "Precio" ) );


    setColumnWidth(COL_IDDPEDIDOCLIENTE,100);
    setColumnWidth(COL_CONCEPTDPEDIDOCLIENTE,300);
    setColumnWidth(COL_PROPORCIONDPEDIDOCLIENTE,100);
    setColumnWidth(COL_IDPEDIDOCLIENTE,100);


    hideColumn(COL_IDDPEDIDOCLIENTE);
    hideColumn(COL_IDPEDIDOCLIENTE);


    setSelectionMode( QTable::SingleRow );

    // Establecemos el color de fondo de la rejilla. El valor lo tiene la clase configuracion que es global.
    setPaletteBackgroundColor(confpr->valor(CONF_BG_DESCPEDIDOSCLIENTE));

    connect(this, SIGNAL(valueChanged(int, int)), this, SLOT(valueBudgetLineChanged(int , int )));

    connect(this, SIGNAL(contextMenuRequested(int, int, const QPoint &)), this, SLOT(contextMenu(int, int, const QPoint &)));
}


ListDescuentoPedidoClienteView::~ListDescuentoPedidoClienteView() {}


void ListDescuentoPedidoClienteView::pintaListDescuentoPedidoCliente() {
    fprintf(stderr,"INICIO de pintaListDescuentoPedidoCliente\n");
    setNumRows(0);
    setNumRows(100);
    /// \todo Habr� que vaciar la tabla para que el pintado fuera exacto.
    DescuentoPedidoCliente *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        setText(i, COL_IDDPEDIDOCLIENTE, linea->iddpedidocliente());
        setText(i, COL_CONCEPTDPEDIDOCLIENTE, linea->conceptdpedidocliente());
        setText(i, COL_PROPORCIONDPEDIDOCLIENTE, linea->proporciondpedidocliente());
        setText(i, COL_IDPEDIDOCLIENTE, linea->idpedidocliente());
        i++;
    }// end for
    fprintf(stderr,"FIN de pintaListDescuentoPedidoCliente\n");
}



void ListDescuentoPedidoClienteView::contextMenu ( int row, int, const QPoint & pos ) {
    QPopupMenu *popup;
    int opcion;
    popup = new QPopupMenu;
    popup->insertItem(tr(tr("Borrar Linea")),1);
    opcion = popup->exec(pos);
    delete popup;
    switch(opcion) {
    case 1:
        borraDescuentoPedidoCliente(row);
    }// end switch
}// end contextMenuRequested


void ListDescuentoPedidoClienteView::borradescpedidoclienteact() {
    borraDescuentoPedidoCliente(currentRow());
}// end borralinpedidoclienteact


void ListDescuentoPedidoClienteView::pintadescListDescuentoPedidoCliente(int pos) {
fprintf(stderr,"pintalinListDescuentoPedidoCliente(%d)\n",pos);
    DescuentoPedidoCliente *linea;
    linea = m_lista.at(pos);
        setText(pos, COL_IDDPEDIDOCLIENTE, linea->iddpedidocliente());
        setText(pos, COL_CONCEPTDPEDIDOCLIENTE, linea->conceptdpedidocliente());
        setText(pos, COL_PROPORCIONDPEDIDOCLIENTE, linea->proporciondpedidocliente());
        setText(pos, COL_IDPEDIDOCLIENTE, linea->idpedidocliente());

}





void ListDescuentoPedidoClienteView::valueBudgetLineChanged(int row, int col) {
    fprintf(stderr,"valueBudgetLineChanged \n");
    DescuentoPedidoCliente *linea;
    linea = lineaat(row);
    if (linea != NULL) {
        switch (col) {
        case COL_PROPORCIONDPEDIDOCLIENTE: {
                float discountLine = text(row, COL_PROPORCIONDPEDIDOCLIENTE).replace(",",".").toFloat();
                linea->setproporciondpedidocliente(QString::number(discountLine));
                break;
            }
        case COL_CONCEPTDPEDIDOCLIENTE:
            linea->setconceptdpedidocliente(text(row,COL_CONCEPTDPEDIDOCLIENTE));
            break;
        }// end switch
        pintadescListDescuentoPedidoCliente(row);
    }// end if
} //end valueBudgetLineChanged


/// Devuelve la linea que se esta tratando actualmente
DescuentoPedidoCliente *ListDescuentoPedidoClienteView::lineaact() {
    fprintf(stderr,"ListDescuentoPedidoClienteView::lineaact()\n");
    return lineaat(currentRow());
}// end lineaact

/// Devuelve la linea especificada, y si no existe se van creando lineas hasta que exista.
DescuentoPedidoCliente *ListDescuentoPedidoClienteView::lineaat(int row) {
    fprintf(stderr,"ListDescuentoPedidoCliente::lineaat(%d)\n", row);
    DescuentoPedidoCliente *linea;
    if (row >=0) {
        while (m_lista.at(row) == 0 ) {
	    fprintf(stderr,"Creamos la linea\n");
            linea=new DescuentoPedidoCliente(companyact);
            linea->setidpedidocliente(mdb_idpedidocliente);
            m_lista.append(linea);  
        }// end while	
	      return(m_lista.at(row));
    } else {
    	fprintf(stderr,"Linea inexistente\n");
        return NULL;
    }// end if

}// end lineaat


