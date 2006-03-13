//
// C++ Implementation: ListLTarifaView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//



#define COL_IDLTARIFA 0
#define COL_IDARTICULO 1
#define COL_IDALMACEN 2
#define COL_IDTARIFA 3
#define COL_CODIGOCOMPLETOARTICULO 4
#define COL_NOMARTICULO 5
#define COL_NOMTARIFA 6
#define COL_NOMALMACEN 7
#define COL_PVPLTARIFA 8




#include <QMessageBox>
#include <QKeyEvent>
#include <QEvent>
#include <QFile>
#include <QTextStream>
#include <QMenu>

#include "company.h"
#include "listltarifaview.h"
#include "funcaux.h"


void ListLTarifaView::guardaconfig() {
    _depura("ListLTarifaView::guardaconfig",0);
    QString aux = "";

    QFile file( confpr->valor(CONF_DIR_USER)+"confListLTarifaView.cfn" );
    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        for (int i = 0; i < columnCount(); i++) {
            showColumn(i);
            stream << columnWidth(i) << "\n";
        }// end for
        file.close();
    }// end if
}

void ListLTarifaView::cargaconfig() {
    _depura("ListLTarifaView::cargaconfig",0);
    QFile file( confpr->valor(CONF_DIR_USER)+"confListLTarifaView.cfn" );
    QString line;
    if ( file.open( QIODevice::ReadOnly ) ) {
        QTextStream stream( &file );
        for (int i = 0; i < columnCount(); i++) {
            QString linea = stream.readLine();
            setColumnWidth(i, linea.toInt());
        }// end for
        file.close();
    }// end if
}




ListLTarifaView::ListLTarifaView(QWidget * parent) : QTableWidget2(parent), ListLTarifa() {
    _depura("ListLTarifaView::ListLTarifaView",0);

    /// Inicializamos la tabla de lineas de presupuesto
    setColumnCount(9);
    setRowCount(0);

    QStringList headers;
    headers << tr( "COL_IDLTARIFA" ) << tr( "COL_IDARTICULO" ) << tr( "COL_IDALMACEN" ) << tr( "COL_IDTARIFA" ) << tr( "COL_CODIGOCOMPLETOARTICULO" ) << tr( "COL_NOMARTICULO" )<< tr( "COL_NOMTARIFA" )<< tr( "COL_NOMALMACEN" )<< tr( "COL_PVPLTARIFA" ) ;

    setHorizontalHeaderLabels (headers);


    hideColumn(COL_IDLTARIFA);
    hideColumn(COL_IDALMACEN);
    hideColumn(COL_IDTARIFA);
    hideColumn(COL_IDARTICULO);
    hideColumn(COL_CODIGOCOMPLETOARTICULO);
    hideColumn(COL_NOMARTICULO);

    setSelectionMode( QAbstractItemView::SingleSelection );
    setSelectionBehavior( QAbstractItemView::SelectRows);
    setPaletteBackgroundColor(confpr->valor(CONF_BG_LINPRESUPUESTOS));
    setAlternatingRowColors(TRUE);
    setSortingEnabled(TRUE);


	connect (this, SIGNAL(cellChanged(int,int)), this, SLOT(s_cellChanged(int,int)));

    cargaconfig();
}


ListLTarifaView::~ListLTarifaView() {
    _depura("ListLTarifaView::~ListLTarifaView()",0);
    guardaconfig();
}



void ListLTarifaView::pintar() {
    _depura("ListLTarifaView::pintaListLTarifa\n",0);
    setRowCount(0);
    setRowCount(lista()->count());
    /// \todo Habr� que vaciar la tabla para que el pintado fuera exacto.
    LTarifa *linea;
    uint i = 0;
    for ( linea = lista()->first(); linea; linea = lista()->next() ) {
        pintar(i);
        i++;
    }// end for
    _depura("END ListLTarifaView::pintaListLTarifa\n",0);
}



void ListLTarifaView::contextMenuEvent (QContextMenuEvent *e) {
    _depura("ListLTarifaView::contextMenuEvent",0);
    int a = currentRow();
    if ( a < 0)
        return;
    QMenu *popup = new QMenu(this);
    QAction *del = popup->addAction(tr("Borrar Articulo"));
    QAction *opcion = popup->exec(QCursor::pos());
    if (opcion == del)
        borrar(a);
    delete popup;
}



void ListLTarifaView::borraLTarifaAct() {
    borrar(currentRow());
}



void ListLTarifaView::pintar(int pos) {
    _depura("ListLTarifaView::pintaLTarifa\n",0);
    LTarifa *linea;
    linea = lista()->at(pos);

    setText(pos, COL_IDLTARIFA, linea->idltarifa());
    setText(pos, COL_IDALMACEN, linea->idalmacen());
    setText(pos, COL_IDTARIFA, linea->idtarifa());
    setText(pos, COL_CODIGOCOMPLETOARTICULO, linea->codigocompletoarticulo());
    setText(pos, COL_PVPLTARIFA, linea->pvpltarifa());
    setText(pos, COL_IDARTICULO, linea->idarticulo());
    setText(pos, COL_NOMARTICULO, linea->nomarticulo());
    setText(pos, COL_NOMTARIFA, linea->nomtarifa());
    setText(pos, COL_NOMALMACEN, linea->nomalmacen());

    _depura("END ListLTarifaView::pintaLTarifa\n",0);
}





/// Devuelve la linea que se esta tratando actualmente
LTarifa *ListLTarifaView::lineaact() {
    _depura("ListLTarifaView::lineaact()\n",0);
    return lineaat(currentRow());
}





/// Devuelve la linea especificada, y si no existe se van creando lineas hasta que exista.
LTarifa *ListLTarifaView::lineaat(int row) {
    _depura("ListLTarifaView::lineaat\n", 0);
    LTarifa *linea;
    if (row >=0) {
        while (lista()->at(row) == 0 ) {
            fprintf(stderr,"Creamos la linea\n");
            linea=new LTarifa(m_companyact);
            linea->setidarticulo(mdb_idarticulo);
            lista()->append(linea);
        }// end while
        return(lista()->at(row));
    } else {
        _depura("Linea inexistente\n",0);
        return NULL;
    }// end if
}


void ListLTarifaView::s_cellChanged(int row, int col) {
	if (col == COL_PVPLTARIFA) {
		lineaat(row)->setpvpltarifa(item(row,col)->text());
	}// end if
	
}

