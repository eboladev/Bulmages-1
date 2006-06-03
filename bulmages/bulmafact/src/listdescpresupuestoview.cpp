/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QMessageBox>
#include <QMenu>
#include <QKeyEvent>
#include <QEvent>

#include "listdescpresupuestoview.h"
#include "articulolist.h"
#include "comparticulolistview.h"
#include "funcaux.h"


ListDescuentoPresupuestoView::ListDescuentoPresupuestoView(QWidget *parent)
        : SubForm2Bf(parent) {
    setDBTableName("dpresupuesto");
    setDBCampoId("iddpresupuesto");
    addSHeader("iddpresupuesto", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, tr("Idd pedidocliente"));
    addSHeader("conceptdpresupuesto", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, tr("Conceptod presupuesto"));
    addSHeader("proporciondpresupuesto", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("Proporciond presupuesto"));
    addSHeader("idpresupuesto", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Id presupuesto"));
    setinsercion(TRUE);
}

/*
 
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
    horizontalHeader()->setLabel( COL_IDDPRESUPUESTO, tr( "Id L�ea" ) );
    horizontalHeader()->setLabel( COL_CONCEPTDPRESUPUESTO, tr( "Descripci�" ) );
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
    /// \todo Habr� que vaciar la tabla para que el pintado fuera exacto.
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
*/

