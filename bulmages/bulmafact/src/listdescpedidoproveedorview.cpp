//
// C++ Implementation: ListDescuentoPedidoProveedorView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#define COL_IDDPEDIDOPROVEEDOR 0
#define COL_CONCEPTDPEDIDOPROVEEDOR 1
#define COL_PROPORCIONDPEDIDOPROVEEDOR 2
#define COL_IDPEDIDOPROVEEDOR 3


#include "listdescpedidoproveedorview.h"
#include <q3table.h>
#include <qmessagebox.h>
#include <q3popupmenu.h>


ListDescuentoPedidoProveedorView::ListDescuentoPedidoProveedorView(QWidget * parent, const char * name) : Q3Table(parent, name), ListDescuentoPedidoProveedor() {
    /// Inicializamos la tabla de lineas de pedidoproveedor
    setNumCols(4);
    setNumRows(100);
    horizontalHeader()->setLabel( COL_IDDPEDIDOPROVEEDOR, tr( "N Línea" ) );
    horizontalHeader()->setLabel( COL_CONCEPTDPEDIDOPROVEEDOR, tr( "Descripción" ) );
    horizontalHeader()->setLabel( COL_PROPORCIONDPEDIDOPROVEEDOR, tr( "Cantidad" ) );
    horizontalHeader()->setLabel( COL_IDPEDIDOPROVEEDOR, tr( "Precio" ) );


    setColumnWidth(COL_IDDPEDIDOPROVEEDOR,100);
    setColumnWidth(COL_CONCEPTDPEDIDOPROVEEDOR,300);
    setColumnWidth(COL_PROPORCIONDPEDIDOPROVEEDOR,100);
    setColumnWidth(COL_IDPEDIDOPROVEEDOR,100);


    hideColumn(COL_IDDPEDIDOPROVEEDOR);
    hideColumn(COL_IDPEDIDOPROVEEDOR);


    setSelectionMode( Q3Table::SingleRow );

    // Establecemos el color de fondo de la rejilla. El valor lo tiene la clase configuracion que es global.
    setPaletteBackgroundColor(confpr->valor(CONF_BG_DESCPEDIDOSCLIENTE));

    connect(this, SIGNAL(valueChanged(int, int)), this, SLOT(valueBudgetLineChanged(int , int )));

    connect(this, SIGNAL(contextMenuRequested(int, int, const QPoint &)), this, SLOT(contextMenu(int, int, const QPoint &)));
}


ListDescuentoPedidoProveedorView::~ListDescuentoPedidoProveedorView() {}


void ListDescuentoPedidoProveedorView::pintaListDescuentoPedidoProveedor() {
    fprintf(stderr,"INICIO de pintaListDescuentoPedidoProveedor\n");
    setNumRows(0);
    setNumRows(100);
    /// \todo Habrï¿½ que vaciar la tabla para que el pintado fuera exacto.
    DescuentoPedidoProveedor *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        setText(i, COL_IDDPEDIDOPROVEEDOR, linea->iddpedidoproveedor());
        setText(i, COL_CONCEPTDPEDIDOPROVEEDOR, linea->conceptdpedidoproveedor());
        setText(i, COL_PROPORCIONDPEDIDOPROVEEDOR, linea->proporciondpedidoproveedor());
        setText(i, COL_IDPEDIDOPROVEEDOR, linea->idpedidoproveedor());
        i++;
    }// end for
    fprintf(stderr,"FIN de pintaListDescuentoPedidoProveedor\n");
}



void ListDescuentoPedidoProveedorView::contextMenu ( int row, int, const QPoint & pos ) {
    Q3PopupMenu *popup;
    int opcion;
    popup = new Q3PopupMenu;
    popup->insertItem(tr(tr("Borrar Linea")),1);
    opcion = popup->exec(pos);
    delete popup;
    switch(opcion) {
    case 1:
        borraDescuentoPedidoProveedor(row);
    }// end switch
}// end contextMenuRequested


void ListDescuentoPedidoProveedorView::borradescpedidoproveedoract() {
    borraDescuentoPedidoProveedor(currentRow());
}// end borralinpedidoproveedoract


void ListDescuentoPedidoProveedorView::pintadescListDescuentoPedidoProveedor(int pos) {
fprintf(stderr,"pintalinListDescuentoPedidoProveedor(%d)\n",pos);
    DescuentoPedidoProveedor *linea;
    linea = m_lista.at(pos);
        setText(pos, COL_IDDPEDIDOPROVEEDOR, linea->iddpedidoproveedor());
        setText(pos, COL_CONCEPTDPEDIDOPROVEEDOR, linea->conceptdpedidoproveedor());
        setText(pos, COL_PROPORCIONDPEDIDOPROVEEDOR, linea->proporciondpedidoproveedor());
        setText(pos, COL_IDPEDIDOPROVEEDOR, linea->idpedidoproveedor());

}





void ListDescuentoPedidoProveedorView::valueBudgetLineChanged(int row, int col) {
    fprintf(stderr,"valueBudgetLineChanged \n");
    DescuentoPedidoProveedor *linea;
    linea = lineaat(row);
    if (linea != NULL) {
        switch (col) {
        case COL_PROPORCIONDPEDIDOPROVEEDOR: {
                float discountLine = text(row, COL_PROPORCIONDPEDIDOPROVEEDOR).replace(",",".").toFloat();
                linea->setproporciondpedidoproveedor(QString::number(discountLine));
                break;
            }
        case COL_CONCEPTDPEDIDOPROVEEDOR:
            linea->setconceptdpedidoproveedor(text(row,COL_CONCEPTDPEDIDOPROVEEDOR));
            break;
        }// end switch
        pintadescListDescuentoPedidoProveedor(row);
    }// end if
} //end valueBudgetLineChanged


/// Devuelve la linea que se esta tratando actualmente
DescuentoPedidoProveedor *ListDescuentoPedidoProveedorView::lineaact() {
    fprintf(stderr,"ListDescuentoPedidoProveedorView::lineaact()\n");
    return lineaat(currentRow());
}// end lineaact

/// Devuelve la linea especificada, y si no existe se van creando lineas hasta que exista.
DescuentoPedidoProveedor *ListDescuentoPedidoProveedorView::lineaat(int row) {
    fprintf(stderr,"ListDescuentoPedidoProveedor::lineaat(%d)\n", row);
    DescuentoPedidoProveedor *linea;
    if (row >=0) {
        while (m_lista.at(row) == 0 ) {
	    fprintf(stderr,"Creamos la linea\n");
            linea=new DescuentoPedidoProveedor(companyact);
            linea->setidpedidoproveedor(mdb_idpedidoproveedor);
            m_lista.append(linea);  
        }// end while	
	      return(m_lista.at(row));
    } else {
    	fprintf(stderr,"Linea inexistente\n");
        return NULL;
    }// end if

}// end lineaat


