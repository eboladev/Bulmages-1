//
// C++ Implementation: listlinpresupuestoview
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "listlinpresupuestoview.h"

#include <qtable.h>

listlinpresupuestoview::listlinpresupuestoview(company *comp,  QWidget * parent, const char * name) : QTable(parent, name) , listlinpresupuesto (comp) {

	/// Inicializamos la tabla de lineas de presupuesto
	setNumRows( 0 );
	setNumCols( 0 );
	setSelectionMode( QTable::SingleRow );
	setSorting( TRUE );
	setSelectionMode( QTable::SingleRow );
	setColumnMovingEnabled( TRUE );
	setNumCols(12);
	horizontalHeader()->setLabel( COL_IDLPRESUPUESTO, tr( "Nº Línea" ) );
	horizontalHeader()->setLabel( COL_DESCLPRESUPUESTO, tr( "Descripción" ) );
	horizontalHeader()->setLabel( COL_CANTLPRESUPUESTO, tr( "Cantidad" ) );
	horizontalHeader()->setLabel( COL_PVPLPRESUPUESTO, tr( "Precio" ) );
	horizontalHeader()->setLabel( COL_DESCUENTOLPRESUPUESTO, tr( "Descuento" ) );
	horizontalHeader()->setLabel( COL_IDPRESUPUESTO, tr( "Nº Pedido" ) );
	horizontalHeader()->setLabel( COL_IDARTICULO, tr( "Artículo" ) );
	horizontalHeader()->setLabel( COL_CODARTICULO, tr( "Código Artículo" ) );
	horizontalHeader()->setLabel( COL_NOMARTICULO, tr( "Descripción Artículo" ) );
	horizontalHeader()->setLabel( COL_TASATIPO_IVA, tr( "% IVA" ) );
	horizontalHeader()->setLabel( COL_TIPO_IVA, tr( "Tipo IVA" ) );
   
	setColumnWidth(COL_IDLPRESUPUESTO,100);
	setColumnWidth(COL_DESCLPRESUPUESTO,300);
	setColumnWidth(COL_CANTLPRESUPUESTO,100);
	setColumnWidth(COL_PVPLPRESUPUESTO,100);
	setColumnWidth(COL_DESCUENTOLPRESUPUESTO,100);
	setColumnWidth(COL_IDPRESUPUESTO,100);
	setColumnWidth(COL_IDARTICULO,100);
	setColumnWidth(COL_CODARTICULO,100);
	setColumnWidth(COL_NOMARTICULO,300);
	setColumnWidth(COL_TASATIPO_IVA,50);
	setColumnWidth(COL_TIPO_IVA,50);
	
	hideColumn(COL_IDLPRESUPUESTO);
	hideColumn(COL_IDPRESUPUESTO);
	hideColumn(COL_IDARTICULO);
	hideColumn(COL_REMOVE);
	hideColumn(COL_TASATIPO_IVA);
	hideColumn(COL_TIPO_IVA);
	
	setNumRows(10);
	
//   listado->setPaletteBackgroundColor(QColor(150,230,230));
	setColumnReadOnly(COL_NOMARTICULO,true);
	// Establecemos el color de fondo de la rejilla. El valor lo tiene la clase configuracion que es global.
	setPaletteBackgroundColor("#AFFAFA");   
	setReadOnly(FALSE);
}
    listlinpresupuestoview::~listlinpresupuestoview() {
    }
    
    
    
    void listlinpresupuestoview::pintalistlinpresupuesto() {
    	fprintf(stderr,"INICIO de pintalistlinpresupuesto\n");

	/// \todo Habría que vaciar la tabla para que el pintado fuera exacto.
        linpresupuesto *linea;
        uint i = 0;
        for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
	    setText(i, COL_IDLPRESUPUESTO, linea->idlpresupuesto());
	    setText(i, COL_IDARTICULO, linea->idarticulo());
	    setText(i, COL_CODARTICULO, linea->codigocompletoarticulo());
	    setText(i, COL_DESCLPRESUPUESTO, linea->nomarticulo());
	    setText(i, COL_CANTLPRESUPUESTO, linea->cantlpresupuesto());
	    setText(i, COL_DESCUENTOLPRESUPUESTO, linea->idlpresupuesto());
	    setText(i, COL_IDPRESUPUESTO, linea->idpresupuesto());
	    setText(i, COL_REMOVE, linea->descuentolpresupuesto());
	    setText(i, COL_TASATIPO_IVA, linea->pvplpresupuesto());
	    setText(i, COL_TIPO_IVA, linea->idlpresupuesto());	    
	    i++;
        }// end for
	fprintf(stderr,"FIN de pintalistlinpresupuesto\n");

    }    

