//
// C++ Implementation: ListDescuentoAlbaranProvView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#define COL_IDDALBARANP 0
#define COL_CONCEPTDALBARANP 1
#define COL_PROPORCIONDALBARANP 2
#define COL_IDALBARANP 3


#include "listdescalbaranprovview.h"
#include <q3table.h>
#include <qmessagebox.h>
#include <q3popupmenu.h>


ListDescuentoAlbaranProvView::ListDescuentoAlbaranProvView(QWidget * parent, const char * name) : Q3Table(parent, name), ListDescuentoAlbaranProv() {
    /// Inicializamos la tabla de lineas de albaran
    setNumCols(4);
    setNumRows(100);
    horizontalHeader()->setLabel( COL_IDDALBARANP, tr( "N Linea" ) );
    horizontalHeader()->setLabel( COL_CONCEPTDALBARANP, tr( "Descripcion" ) );
    horizontalHeader()->setLabel( COL_PROPORCIONDALBARANP, tr( "Cantidad" ) );
    horizontalHeader()->setLabel( COL_IDALBARANP, tr( "Precio" ) );


    setColumnWidth(COL_IDDALBARANP,100);
    setColumnWidth(COL_CONCEPTDALBARANP,300);
    setColumnWidth(COL_PROPORCIONDALBARANP,100);
    setColumnWidth(COL_IDALBARANP,100);


    hideColumn(COL_IDDALBARANP);
    hideColumn(COL_IDALBARANP);


    setSelectionMode( Q3Table::SingleRow );

    // Establecemos el color de fondo de la rejilla. El valor lo tiene la clase configuracion que es global.
    setPaletteBackgroundColor(confpr->valor(CONF_BG_DESCALBARANESCLIENTE));

    connect(this, SIGNAL(valueChanged(int, int)), this, SLOT(valueBudgetLineChanged(int , int )));
    connect(this, SIGNAL(contextMenuRequested(int, int, const QPoint &)), this, SLOT(contextMenu(int, int, const QPoint &)));
}


ListDescuentoAlbaranProvView::~ListDescuentoAlbaranProvView() {}


void ListDescuentoAlbaranProvView::pintaListDescuentoAlbaranProv() {
    _depura("INICIO de pintaListDescuentoAlbaranProv\n");
    setNumRows(0);
    setNumRows(100);
    /// \todo Habr� que vaciar la tabla para que el pintado fuera exacto.
    DescuentoAlbaranProv *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        setText(i, COL_IDDALBARANP, linea->iddalbaranp());
        setText(i, COL_CONCEPTDALBARANP, linea->conceptdalbaranp());
        setText(i, COL_PROPORCIONDALBARANP, linea->proporciondalbaranp());
        setText(i, COL_IDALBARANP, linea->idalbaranp());
        i++;
    }// end for
    _depura("FIN de pintaListDescuentoAlbaranProv\n");
}



void ListDescuentoAlbaranProvView::contextMenu ( int row, int, const QPoint & pos ) {
    Q3PopupMenu *popup;
    int opcion;
    popup = new Q3PopupMenu;
    popup->insertItem(tr(tr("Borrar Linea")),1);
    opcion = popup->exec(pos);
    delete popup;
    switch(opcion) {
    case 1:
        borraDescuentoAlbaranProv(row);
    }// end switch
}// end contextMenuRequested


void ListDescuentoAlbaranProvView::borradescalbaranact() {
    borraDescuentoAlbaranProv(currentRow());
}// end borralinalbaranact


void ListDescuentoAlbaranProvView::pintadescListDescuentoAlbaranProv(int pos) {
_depura("pintalinListDescuentoAlbaranCliente()\n");
    DescuentoAlbaranProv *linea;
    linea = m_lista.at(pos);
        setText(pos, COL_IDDALBARANP, linea->iddalbaranp());
        setText(pos, COL_CONCEPTDALBARANP, linea->conceptdalbaranp());
        setText(pos, COL_PROPORCIONDALBARANP, linea->proporciondalbaranp());
        setText(pos, COL_IDALBARANP, linea->idalbaranp());

}





void ListDescuentoAlbaranProvView::valueBudgetLineChanged(int row, int col) {
    _depura("valueBudgetLineChanged \n");
    DescuentoAlbaranProv *linea;
    linea = lineaat(row);
    if (linea != NULL) {
        switch (col) {
        case COL_PROPORCIONDALBARANP: {
                float discountLine = text(row, COL_PROPORCIONDALBARANP).replace(",",".").toFloat();
                linea->setproporciondalbaranp(QString::number(discountLine));
                break;
            }
        case COL_CONCEPTDALBARANP:
            linea->setconceptdalbaranp(text(row,COL_CONCEPTDALBARANP));
            break;
        }// end switch
        pintadescListDescuentoAlbaranProv(row);
    }// end if
} //end valueBudgetLineChanged


/// Devuelve la linea que se esta tratando actualmente
DescuentoAlbaranProv *ListDescuentoAlbaranProvView::lineaact() {
    _depura("ListDescuentoAlbaranProvView::lineaact()\n");
    return lineaat(currentRow());
}// end lineaact

/// Devuelve la linea especificada, y si no existe se van creando lineas hasta que exista.
DescuentoAlbaranProv *ListDescuentoAlbaranProvView::lineaat(int row) {
    _depura("ListDescuentoAlbaranCliente::lineaat()\n");
    DescuentoAlbaranProv *linea;
    if (row >=0) {
        while (m_lista.at(row) == 0 ) {
	    _depura("Creamos la linea\n");
            linea=new DescuentoAlbaranProv(companyact);
            linea->setidalbaranp(mdb_idalbaranp);
            m_lista.append(linea);  
        }// end while	
	      return(m_lista.at(row));
    } else {
    	_depura("Linea inexistente\n");
        return NULL;
    }// end if

}// end lineaat


