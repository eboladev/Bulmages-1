//
// C++ Implementation: ListDescuentoFacturaProvView
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

#include "listdescfacturaprovview.h"
#include "articulolist.h"
#include "comparticulolistview.h"
#include "funcaux.h"


ListDescuentoFacturaProvView::ListDescuentoFacturaProvView(QWidget *parent) : SubForm2Bf(parent) {
    setDBTableName("dfacturap");
    setDBCampoId("iddfacturap");
    addSHeader("iddfacturap", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, "iddfacturap");
    addSHeader("conceptdfacturap", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, "conceptdfacturap");
    addSHeader("proporciondfacturap", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, "proporciondfacturap");
    addSHeader("idfacturap", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, "idfacturap");
    setinsercion(TRUE);
};


/*


#define COL_IDDFACTURAP 0
#define COL_CONCEPTDFACTURAP 1
#define COL_PROPORCIONDFACTURAP 2
#define COL_IDFACTURAP 3


#include "listdescfacturaprovview.h"
#include <Q3Table>
#include <QMessageBox>
#include <Q3PopupMenu>


ListDescuentoFacturaProvView::ListDescuentoFacturaProvView(QWidget * parent, const char * name) : Q3Table(parent, name), ListDescuentoFacturaProv() {
    /// Inicializamos la tabla de lineas de factura
    setNumCols(4);
    setNumRows(100);
    horizontalHeader()->setLabel( COL_IDDFACTURAP, tr( "Id L�ea" ) );
    horizontalHeader()->setLabel( COL_CONCEPTDFACTURAP, tr( "Descripci�" ) );
    horizontalHeader()->setLabel( COL_PROPORCIONDFACTURAP, tr( "Porcentaje" ) );
    horizontalHeader()->setLabel( COL_IDFACTURAP, tr( "Factura" ) );


    setColumnWidth(COL_IDDFACTURAP,100);
    setColumnWidth(COL_CONCEPTDFACTURAP,300);
    setColumnWidth(COL_PROPORCIONDFACTURAP,100);
    setColumnWidth(COL_IDFACTURAP,100);


    hideColumn(COL_IDDFACTURAP);
    hideColumn(COL_IDFACTURAP);


    setSelectionMode( Q3Table::SingleRow );

    // Establecemos el color de fondo de la rejilla. El valor lo tiene la clase configuracion que es global.
    setPaletteBackgroundColor(confpr->valor(CONF_BG_DESCFACTURASCLIENTE));

    connect(this, SIGNAL(valueChanged(int, int)), this, SLOT(valueBudgetLineChanged(int , int )));

    connect(this, SIGNAL(contextMenuRequested(int, int, const QPoint &)), this, SLOT(contextMenu(int, int, const QPoint &)));
}


ListDescuentoFacturaProvView::~ListDescuentoFacturaProvView() {}


void ListDescuentoFacturaProvView::pintaListDescuentoFacturaProv() {
    _depura("INICIO de pintaListDescuentoFactura\n");
    setNumRows(0);
    setNumRows(100);
    /// \todo Habria que vaciar la tabla para que el pintado fuera exacto.
    DescuentoFacturaProv *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        setText(i, COL_IDDFACTURAP, linea->iddfacturap());
        setText(i, COL_CONCEPTDFACTURAP, linea->conceptdfacturap());
        setText(i, COL_PROPORCIONDFACTURAP, linea->proporciondfacturap());
        setText(i, COL_IDFACTURAP, linea->idfacturap());
        i++;
    }// end for
    _depura("FIN de pintaListDescuentoFactura\n");
}



void ListDescuentoFacturaProvView::contextMenu ( int row, int, const QPoint & pos ) {
    Q3PopupMenu *popup;
    int opcion;
    popup = new Q3PopupMenu;
    popup->insertItem(tr(tr("Borrar Linea")),1);
    opcion = popup->exec(pos);
    delete popup;
    switch(opcion) {
    case 1:
        borraDescuentoFacturaProv(row);
    }// end switch
}// end contextMenuRequested


void ListDescuentoFacturaProvView::borradescfacturaact() {
    borraDescuentoFacturaProv(currentRow());
}// end borralinfacturaact


void ListDescuentoFacturaProvView::pintadescListDescuentoFacturaProv(int pos) {
_depura("pintalinListDescuentoFactura()\n");
    DescuentoFacturaProv *linea;
    linea = m_lista.at(pos);
        setText(pos, COL_IDDFACTURAP, linea->iddfacturap());
        setText(pos, COL_CONCEPTDFACTURAP, linea->conceptdfacturap());
        setText(pos, COL_PROPORCIONDFACTURAP, linea->proporciondfacturap());
        setText(pos, COL_IDFACTURAP, linea->idfacturap());

}





void ListDescuentoFacturaProvView::valueBudgetLineChanged(int row, int col) {
    _depura("valueBudgetLineChanged \n");
    DescuentoFacturaProv *linea;
    linea = lineaat(row);
    if (linea != NULL) {
        switch (col) {
        case COL_PROPORCIONDFACTURAP: {
                float discountLine = text(row, COL_PROPORCIONDFACTURAP).replace(",",".").toFloat();
                linea->setproporciondfacturap(QString::number(discountLine));
                break;
            }
        case COL_CONCEPTDFACTURAP:
            linea->setconceptdfacturap(text(row,COL_CONCEPTDFACTURAP));
            break;
        }// end switch
        pintadescListDescuentoFacturaProv(row);
    }// end if
} //end valueBudgetLineChanged


/// Devuelve la linea que se esta tratando actualmente
DescuentoFacturaProv *ListDescuentoFacturaProvView::lineaact() {
    _depura("ListDescuentoFacturaProvView::lineaact()\n");
    return lineaat(currentRow());
}// end lineaact

/// Devuelve la linea especificada, y si no existe se van creando lineas hasta que exista.
DescuentoFacturaProv *ListDescuentoFacturaProvView::lineaat(int row) {
    _depura("ListDescuentoFactura::lineaat()\n");
    DescuentoFacturaProv *linea;
    if (row >=0) {
        while (m_lista.at(row) == 0 ) {
	    _depura("Creamos la linea\n");
            linea=new DescuentoFacturaProv(companyact);
            linea->setidfacturap(mdb_idfacturap);
            m_lista.append(linea);  
        }// end while	
	      return(m_lista.at(row));
    } else {
    	_depura("Linea inexistente\n");
        return NULL;
    }// end if

}// end lineaat

*/
