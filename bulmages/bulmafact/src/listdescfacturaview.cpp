//
// C++ Implementation: ListDescuentoFacturaView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#define COL_IDDFACTURA 0
#define COL_CONCEPTDFACTURA 1
#define COL_PROPORCIONDFACTURA 2
#define COL_IDFACTURA 3


#include "listdescfacturaview.h"
#include <qtable.h>
#include <qmessagebox.h>
#include <qpopupmenu.h>


ListDescuentoFacturaView::ListDescuentoFacturaView(QWidget * parent, const char * name) : QTable(parent, name), ListDescuentoFactura() {
    /// Inicializamos la tabla de lineas de factura
    setNumCols(4);
    setNumRows(100);
    horizontalHeader()->setLabel( COL_IDDFACTURA, tr( "Id Línea" ) );
    horizontalHeader()->setLabel( COL_CONCEPTDFACTURA, tr( "Descripción" ) );
    horizontalHeader()->setLabel( COL_PROPORCIONDFACTURA, tr( "Porcentaje" ) );
    horizontalHeader()->setLabel( COL_IDFACTURA, tr( "Factura" ) );


    setColumnWidth(COL_IDDFACTURA,100);
    setColumnWidth(COL_CONCEPTDFACTURA,300);
    setColumnWidth(COL_PROPORCIONDFACTURA,100);
    setColumnWidth(COL_IDFACTURA,100);


    hideColumn(COL_IDDFACTURA);
    hideColumn(COL_IDFACTURA);


    setSelectionMode( QTable::SingleRow );

    // Establecemos el color de fondo de la rejilla. El valor lo tiene la clase configuracion que es global.
    setPaletteBackgroundColor(confpr->valor(CONF_BG_DESCFACTURASCLIENTE));

    connect(this, SIGNAL(valueChanged(int, int)), this, SLOT(valueBudgetLineChanged(int , int )));

    connect(this, SIGNAL(contextMenuRequested(int, int, const QPoint &)), this, SLOT(contextMenu(int, int, const QPoint &)));
}


ListDescuentoFacturaView::~ListDescuentoFacturaView() {}


void ListDescuentoFacturaView::pintaListDescuentoFactura() {
    fprintf(stderr,"INICIO de pintaListDescuentoFactura\n");
    setNumRows(0);
    setNumRows(100);
    /// \todo Habrï¿½ que vaciar la tabla para que el pintado fuera exacto.
    DescuentoFactura *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        setText(i, COL_IDDFACTURA, linea->iddfactura());
        setText(i, COL_CONCEPTDFACTURA, linea->conceptdfactura());
        setText(i, COL_PROPORCIONDFACTURA, linea->proporciondfactura());
        setText(i, COL_IDFACTURA, linea->idfactura());
        i++;
    }// end for
    fprintf(stderr,"FIN de pintaListDescuentoFactura\n");
}



void ListDescuentoFacturaView::contextMenu ( int row, int, const QPoint & pos ) {
    QPopupMenu *popup;
    int opcion;
    popup = new QPopupMenu;
    popup->insertItem(tr(tr("Borrar Linea")),1);
    opcion = popup->exec(pos);
    delete popup;
    switch(opcion) {
    case 1:
        borraDescuentoFactura(row);
    }// end switch
}// end contextMenuRequested


void ListDescuentoFacturaView::borradescfacturaact() {
    borraDescuentoFactura(currentRow());
}// end borralinfacturaact


void ListDescuentoFacturaView::pintadescListDescuentoFactura(int pos) {
fprintf(stderr,"pintalinListDescuentoFactura(%d)\n",pos);
    DescuentoFactura *linea;
    linea = m_lista.at(pos);
        setText(pos, COL_IDDFACTURA, linea->iddfactura());
        setText(pos, COL_CONCEPTDFACTURA, linea->conceptdfactura());
        setText(pos, COL_PROPORCIONDFACTURA, linea->proporciondfactura());
        setText(pos, COL_IDFACTURA, linea->idfactura());

}





void ListDescuentoFacturaView::valueBudgetLineChanged(int row, int col) {
    fprintf(stderr,"valueBudgetLineChanged \n");
    DescuentoFactura *linea;
    linea = lineaat(row);
    if (linea != NULL) {
        switch (col) {
        case COL_PROPORCIONDFACTURA: {
                float discountLine = text(row, COL_PROPORCIONDFACTURA).replace(",",".").toFloat();
                linea->setproporciondfactura(QString::number(discountLine));
                break;
            }
        case COL_CONCEPTDFACTURA:
            linea->setconceptdfactura(text(row,COL_CONCEPTDFACTURA));
            break;
        }// end switch
        pintadescListDescuentoFactura(row);
    }// end if
} //end valueBudgetLineChanged


/// Devuelve la linea que se esta tratando actualmente
DescuentoFactura *ListDescuentoFacturaView::lineaact() {
    fprintf(stderr,"ListDescuentoFacturaView::lineaact()\n");
    return lineaat(currentRow());
}// end lineaact

/// Devuelve la linea especificada, y si no existe se van creando lineas hasta que exista.
DescuentoFactura *ListDescuentoFacturaView::lineaat(int row) {
    fprintf(stderr,"ListDescuentoFactura::lineaat(%d)\n", row);
    DescuentoFactura *linea;
    if (row >=0) {
        while (m_lista.at(row) == 0 ) {
	    fprintf(stderr,"Creamos la linea\n");
            linea=new DescuentoFactura(companyact);
            linea->setidfactura(mdb_idfactura);
            m_lista.append(linea);  
        }// end while	
	      return(m_lista.at(row));
    } else {
    	fprintf(stderr,"Linea inexistente\n");
        return NULL;
    }// end if

}// end lineaat


