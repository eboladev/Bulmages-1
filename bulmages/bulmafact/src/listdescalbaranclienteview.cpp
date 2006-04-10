//
// C++ Implementation: ListDescuentoAlbaranClienteView
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

#include "listdescalbaranclienteview.h"
#include "articulolist.h"
#include "comparticulolistview.h"
#include "funcaux.h"


ListDescuentoAlbaranClienteView::ListDescuentoAlbaranClienteView(QWidget *parent) : SubForm2Bf(parent) {
    setDBTableName("dalbaran");
    setDBCampoId("iddalbaran");
    addSHeader("iddalbaran", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, "iddalbaran");
    addSHeader("conceptdalbaran", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, "conceptdalbaran");
    addSHeader("proporciondalbaran", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, "proporciondalbaran");
    addSHeader("idalbaran", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, "idalbaran");
    setinsercion(TRUE);
};


/*


#define COL_IDDALBARAN 0
#define COL_CONCEPTDALBARAN 1
#define COL_PROPORCIONDALBARAN 2
#define COL_IDALBARAN 3


#include "listdescalbaranclienteview.h"
#include <Q3Table>
#include <QMessageBox>
#include <Q3PopupMenu>


ListDescuentoAlbaranClienteView::ListDescuentoAlbaranClienteView(QWidget * parent, const char * name) : Q3Table(parent, name), ListDescuentoAlbaranCliente() {
    /// Inicializamos la tabla de lineas de albaran
    setNumCols(4);
    setNumRows(100);
    horizontalHeader()->setLabel( COL_IDDALBARAN, tr( "N L�ea" ) );
    horizontalHeader()->setLabel( COL_CONCEPTDALBARAN, tr( "Descripci�" ) );
    horizontalHeader()->setLabel( COL_PROPORCIONDALBARAN, tr( "Cantidad" ) );
    horizontalHeader()->setLabel( COL_IDALBARAN, tr( "Precio" ) );


    setColumnWidth(COL_IDDALBARAN,100);
    setColumnWidth(COL_CONCEPTDALBARAN,300);
    setColumnWidth(COL_PROPORCIONDALBARAN,100);
    setColumnWidth(COL_IDALBARAN,100);


    hideColumn(COL_IDDALBARAN);
    hideColumn(COL_IDALBARAN);


    setSelectionMode( Q3Table::SingleRow );

    // Establecemos el color de fondo de la rejilla. El valor lo tiene la clase configuracion que es global.
    setPaletteBackgroundColor(confpr->valor(CONF_BG_DESCALBARANESCLIENTE));

    connect(this, SIGNAL(valueChanged(int, int)), this, SLOT(valueBudgetLineChanged(int , int )));

    connect(this, SIGNAL(contextMenuRequested(int, int, const QPoint &)), this, SLOT(contextMenu(int, int, const QPoint &)));
}


ListDescuentoAlbaranClienteView::~ListDescuentoAlbaranClienteView() {}


void ListDescuentoAlbaranClienteView::pintaListDescuentoAlbaranCliente() {
    fprintf(stderr,"INICIO de pintaListDescuentoAlbaranCliente\n");
    setNumRows(0);
    setNumRows(100);
    /// \todo Habr� que vaciar la tabla para que el pintado fuera exacto.
    DescuentoAlbaranCliente *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        setText(i, COL_IDDALBARAN, linea->iddalbaran());
        setText(i, COL_CONCEPTDALBARAN, linea->conceptdalbaran());
        setText(i, COL_PROPORCIONDALBARAN, linea->proporciondalbaran());
        setText(i, COL_IDALBARAN, linea->idalbaran());
        i++;
    }// end for
    fprintf(stderr,"FIN de pintaListDescuentoAlbaranCliente\n");
}



void ListDescuentoAlbaranClienteView::contextMenu ( int row, int, const QPoint & pos ) {
    Q3PopupMenu *popup;
    int opcion;
    popup = new Q3PopupMenu;
    popup->insertItem(tr(tr("Borrar Linea")),1);
    opcion = popup->exec(pos);
    delete popup;
    switch(opcion) {
    case 1:
        borraDescuentoAlbaranCliente(row);
    }// end switch
}// end contextMenuRequested


void ListDescuentoAlbaranClienteView::borradescalbaranact() {
    borraDescuentoAlbaranCliente(currentRow());
}// end borralinalbaranact


void ListDescuentoAlbaranClienteView::pintadescListDescuentoAlbaranCliente(int pos) {
fprintf(stderr,"pintalinListDescuentoAlbaranCliente(%d)\n",pos);
    DescuentoAlbaranCliente *linea;
    linea = m_lista.at(pos);
        setText(pos, COL_IDDALBARAN, linea->iddalbaran());
        setText(pos, COL_CONCEPTDALBARAN, linea->conceptdalbaran());
        setText(pos, COL_PROPORCIONDALBARAN, linea->proporciondalbaran());
        setText(pos, COL_IDALBARAN, linea->idalbaran());

}





void ListDescuentoAlbaranClienteView::valueBudgetLineChanged(int row, int col) {
    fprintf(stderr,"valueBudgetLineChanged \n");
    DescuentoAlbaranCliente *linea;
    linea = lineaat(row);
    if (linea != NULL) {
        switch (col) {
        case COL_PROPORCIONDALBARAN: {
                float discountLine = text(row, COL_PROPORCIONDALBARAN).replace(",",".").toFloat();
                linea->setproporciondalbaran(QString::number(discountLine));
                break;
            }
        case COL_CONCEPTDALBARAN:
            linea->setconceptdalbaran(text(row,COL_CONCEPTDALBARAN));
            break;
        }// end switch
        pintadescListDescuentoAlbaranCliente(row);
    }// end if
} //end valueBudgetLineChanged


/// Devuelve la linea que se esta tratando actualmente
DescuentoAlbaranCliente *ListDescuentoAlbaranClienteView::lineaact() {
    fprintf(stderr,"ListDescuentoAlbaranClienteView::lineaact()\n");
    return lineaat(currentRow());
}// end lineaact

/// Devuelve la linea especificada, y si no existe se van creando lineas hasta que exista.
DescuentoAlbaranCliente *ListDescuentoAlbaranClienteView::lineaat(int row) {
    fprintf(stderr,"ListDescuentoAlbaranCliente::lineaat(%d)\n", row);
    DescuentoAlbaranCliente *linea;
    if (row >=0) {
        while (m_lista.at(row) == 0 ) {
	    fprintf(stderr,"Creamos la linea\n");
            linea=new DescuentoAlbaranCliente(companyact);
            linea->setidalbaran(mdb_idalbaran);
            m_lista.append(linea);  
        }// end while	
	      return(m_lista.at(row));
    } else {
    	fprintf(stderr,"Linea inexistente\n");
        return NULL;
    }// end if

}// end lineaat

*/
