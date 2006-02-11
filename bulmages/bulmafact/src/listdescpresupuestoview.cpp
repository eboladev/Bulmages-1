//
// C++ Implementation: ListDescuentoPresupuestoView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#define COL_IDDPRESUPUESTO 0
#define COL_CONCEPTDPRESUPUESTO 1
#define COL_PROPORCIONDPRESUPUESTO 2
#define COL_IDPRESUPUESTO 3


#include "listdescpresupuestoview.h"
#include <Q3Table>
#include <QMessageBox>
#include <Q3PopupMenu>


ListDescuentoPresupuestoView::ListDescuentoPresupuestoView(QWidget * parent, const char * name) : Q3Table(parent, name), ListDescuentoPresupuesto() {
    /// Inicializamos la tabla de lineas de presupuesto
    setNumCols(4);
    setNumRows(100);
    horizontalHeader()->setLabel( COL_IDDPRESUPUESTO, tr( "Id Línea" ) );
    horizontalHeader()->setLabel( COL_CONCEPTDPRESUPUESTO, tr( "Descripción" ) );
    horizontalHeader()->setLabel( COL_PROPORCIONDPRESUPUESTO, tr( "Porcentaje" ) );
    horizontalHeader()->setLabel( COL_IDPRESUPUESTO, tr( "Presupuesto" ) );


    setColumnWidth(COL_IDDPRESUPUESTO,100);
    setColumnWidth(COL_CONCEPTDPRESUPUESTO,300);
    setColumnWidth(COL_PROPORCIONDPRESUPUESTO,100);
    setColumnWidth(COL_IDPRESUPUESTO,100);


    hideColumn(COL_IDDPRESUPUESTO);
    hideColumn(COL_IDPRESUPUESTO);


    setSelectionMode( Q3Table::SingleRow );

    // Establecemos el color de fondo de la rejilla. El valor lo tiene la clase configuracion que es global.
    setPaletteBackgroundColor(confpr->valor(CONF_BG_DESCPRESUPUESTOS));

    connect(this, SIGNAL(valueChanged(int, int)), this, SLOT(valueBudgetLineChanged(int , int )));

    connect(this, SIGNAL(contextMenuRequested(int, int, const QPoint &)), this, SLOT(contextMenu(int, int, const QPoint &)));
}


ListDescuentoPresupuestoView::~ListDescuentoPresupuestoView() {}


void ListDescuentoPresupuestoView::pintaListDescuentoPresupuesto() {
    fprintf(stderr,"INICIO de pintaListDescuentoPresupuesto\n");
    setNumRows(0);
    setNumRows(100);
    /// \todo Habrï¿½ que vaciar la tabla para que el pintado fuera exacto.
    DescuentoPresupuesto *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        setText(i, COL_IDDPRESUPUESTO, linea->iddpresupuesto());
        setText(i, COL_CONCEPTDPRESUPUESTO, linea->conceptdpresupuesto());
        setText(i, COL_PROPORCIONDPRESUPUESTO, linea->proporciondpresupuesto());
        setText(i, COL_IDPRESUPUESTO, linea->idpresupuesto());
        i++;
    }// end for
    fprintf(stderr,"FIN de pintaListDescuentoPresupuesto\n");
}



void ListDescuentoPresupuestoView::contextMenu ( int row, int, const QPoint & pos ) {
    Q3PopupMenu *popup;
    int opcion;
    popup = new Q3PopupMenu;
    popup->insertItem(tr(tr("Borrar Linea")),1);
    opcion = popup->exec(pos);
    delete popup;
    switch(opcion) {
    case 1:
        borraDescuentoPresupuesto(row);
    }// end switch
}// end contextMenuRequested


void ListDescuentoPresupuestoView::borradescpresupuestoact() {
    borraDescuentoPresupuesto(currentRow());
}// end borralinpresupuestoact


void ListDescuentoPresupuestoView::pintadescListDescuentoPresupuesto(int pos) {
fprintf(stderr,"pintalinListDescuentoPresupuesto(%d)\n",pos);
    DescuentoPresupuesto *linea;
    linea = m_lista.at(pos);
        setText(pos, COL_IDDPRESUPUESTO, linea->iddpresupuesto());
        setText(pos, COL_CONCEPTDPRESUPUESTO, linea->conceptdpresupuesto());
        setText(pos, COL_PROPORCIONDPRESUPUESTO, linea->proporciondpresupuesto());
        setText(pos, COL_IDPRESUPUESTO, linea->idpresupuesto());

}





void ListDescuentoPresupuestoView::valueBudgetLineChanged(int row, int col) {
    fprintf(stderr,"valueBudgetLineChanged \n");
    DescuentoPresupuesto *linea;
    linea = lineaat(row);
    if (linea != NULL) {
        switch (col) {
        case COL_PROPORCIONDPRESUPUESTO: {
                float discountLine = text(row, COL_PROPORCIONDPRESUPUESTO).replace(",",".").toFloat();
                linea->setproporciondpresupuesto(QString::number(discountLine));
                break;
            }
        case COL_CONCEPTDPRESUPUESTO:
            linea->setconceptdpresupuesto(text(row,COL_CONCEPTDPRESUPUESTO));
            break;
        }// end switch
        pintadescListDescuentoPresupuesto(row);
    }// end if
} //end valueBudgetLineChanged


/// Devuelve la linea que se esta tratando actualmente
DescuentoPresupuesto *ListDescuentoPresupuestoView::lineaact() {
    fprintf(stderr,"ListDescuentoPresupuestoView::lineaact()\n");
    return lineaat(currentRow());
}// end lineaact

/// Devuelve la linea especificada, y si no existe se van creando lineas hasta que exista.
DescuentoPresupuesto *ListDescuentoPresupuestoView::lineaat(int row) {
    fprintf(stderr,"ListDescuentoPresupuesto::lineaat(%d)\n", row);
    DescuentoPresupuesto *linea;
    if (row >=0) {
        while (m_lista.at(row) == 0 ) {
	    fprintf(stderr,"Creamos la linea\n");
            linea=new DescuentoPresupuesto(companyact);
            linea->setidpresupuesto(mdb_idpresupuesto);
            m_lista.append(linea);  
        }// end while	
	      return(m_lista.at(row));
    } else {
    	fprintf(stderr,"Linea inexistente\n");
        return NULL;
    }// end if

}// end lineaat


