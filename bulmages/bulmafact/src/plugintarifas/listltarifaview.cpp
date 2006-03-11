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
#define COL_PVPLTARIFA 4
#define COL_CODIGOCOMPLETOARTICULO 5
#define COL_NOMARTICULO 6
#define COL_NOMTARIFA 7
#define COL_NOMALMACEN 8


#include "listltarifaview.h"
#include "funcaux.h"
#include <Q3Table>
#include <QMessageBox>
#include <Q3PopupMenu>
#include <QFile>

//Added by qt3to4:
#include <QKeyEvent>
#include <QEvent>
//Added by qt3to4:
#include <QTextStream>
#include "company.h"


void ListLTarifaView::guardaconfig() {
    _depura("ListLTarifaView::guardaconfig",0);
    QString aux = "";

    QFile file( confpr->valor(CONF_DIR_USER)+"confListLTarifaView.cfn" );
    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        for (int i = 0; i < numCols(); i++) {
            showColumn(i);
            stream << columnWidth(i) << "\n";
        }// end for
        file.close();
    }// end if
}// end guardaconfig()

void ListLTarifaView::cargaconfig() {
    _depura("ListLTarifaView::cargaconfig",0);
    QFile file( confpr->valor(CONF_DIR_USER)+"confListLTarifaView.cfn" );
    QString line;
    if ( file.open( QIODevice::ReadOnly ) ) {
        QTextStream stream( &file );
        for (int i = 0; i < numCols(); i++) {
            QString linea = stream.readLine();
            setColumnWidth(i, linea.toInt());
        }// end for
        file.close();
    }// end if
}// end cargaconfig




ListLTarifaView::ListLTarifaView(QWidget * parent, const char * name) : Q3Table(parent, name), ListLTarifa() {
    _depura("ListLTarifaView::ListLTarifaView",0);
    /// Inicializamos la tabla de lineas de presupuesto
    setNumCols(9);
    setNumRows(0);

    horizontalHeader()->setLabel( COL_IDLTARIFA, tr( "COL_IDLTARIFA" ) );
    horizontalHeader()->setLabel( COL_IDALMACEN, tr( "COL_IDALMACEN" ) );
    horizontalHeader()->setLabel( COL_IDTARIFA, tr( "COL_IDTARIFA" ) );
    horizontalHeader()->setLabel( COL_CODIGOCOMPLETOARTICULO, tr( "COL_CODIGOCOMPLETOARTICULO" ) );
    horizontalHeader()->setLabel( COL_PVPLTARIFA, tr( "COL_PVPLTARIFA" ) );
    horizontalHeader()->setLabel( COL_IDARTICULO, tr( "COL_IDARTICULO" ) );
    horizontalHeader()->setLabel( COL_NOMARTICULO, tr( "COL_NOMARTICULO" ) );
    horizontalHeader()->setLabel( COL_NOMTARIFA, tr( "COL_NOMTARIFA" ) );
    horizontalHeader()->setLabel( COL_NOMALMACEN, tr( "COL_NOMALMACEN" ) );


/*
    hideColumn(COL_IDLTARIFA);
    hideColumn(COL_IDALMACEN);
    hideColumn(COL_IDTARIFA);
    hideColumn(COL_IDARTICULO);
*/

    setSelectionMode( Q3Table::SingleRow );
    // Establecemos el color de fondo de la rejilla. El valor lo tiene la clase configuracion que es global.
    setPaletteBackgroundColor(confpr->valor(CONF_BG_LINPRESUPUESTOS));

    connect(this, SIGNAL(valueChanged(int, int)), this, SLOT(valueBudgetLineChanged(int , int )));
    connect(this, SIGNAL(contextMenuRequested(int, int, const QPoint &)), this, SLOT(contextMenu(int, int, const QPoint &)));
    installEventFilter(this);
    cargaconfig();
}


ListLTarifaView::~ListLTarifaView() {
    _depura("ListLTarifaView::~ListLTarifaView()",0);
    guardaconfig();
}


void ListLTarifaView::pintar() {
    _depura("ListLTarifaView::pintaListLTarifa\n",0);
    setNumRows(0);
    setNumRows(lista()->count());
    /// \todo Habr� que vaciar la tabla para que el pintado fuera exacto.
    LTarifa *linea;
    uint i = 0;
    for ( linea = lista()->first(); linea; linea = lista()->next() ) {
        pintar(i);
        adjustRow(i);
        i++;
    }// end for
    _depura("END ListLTarifaView::pintaListLTarifa\n",0);
}



void ListLTarifaView::contextMenu ( int row, int, const QPoint & pos ) {
    Q3PopupMenu *popup;
    int opcion;
    popup = new Q3PopupMenu;
    popup->insertItem(tr(tr("Borrar Linea")),1);
    opcion = popup->exec(pos);
    delete popup;
    switch(opcion) {
    case 1:
        borrar(row);
    }// end switch
}// end contextMenuRequested


void ListLTarifaView::borraLTarifaAct() {
    borrar(currentRow());

}// end borralinpresupuestoact


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

    adjustRow(pos);
    _depura("END ListLTarifaView::pintaLTarifa\n",0);
}// end pintaLTarifa


bool ListLTarifaView::eventFilter( QObject *obj, QEvent *ev ) {
    _depura("eventFilter()\n",0);
    /*
        QString idArticle;
        //    linpresupuesto *linea=lineaact();
        LTarifa *linea;//=m_lista.at(currentRow());
     
        if ( ev->type() == QEvent::KeyRelease ) {
            QKeyEvent *k = (QKeyEvent *)ev;
            int col=currentColumn();
            int row=currentRow();
            switch (k->key()) {
            case Qt::Key_Plus:
                if (col == COL_DESCLPRESUPUESTO) {
                    setText(row,col,editaTexto(text(row,col)));
                    valueBudgetLineChanged(row,col);
                    return TRUE;
                }// end if
                break;
            case Qt::Key_Return:
            case Qt::Key_Enter:
                // Esto se hace porque en la ltima linea del qtable tiene un comportamiento raro. Se reportar�como bug a trolltech.
                switch (col) {
                case COL_CODARTICULO:
                    setCurrentCell(row, COL_DESCLPRESUPUESTO);
                    break;
                case COL_DESCLPRESUPUESTO:
                    setCurrentCell(row, COL_CANTLPRESUPUESTO);
                    break;
                case COL_CANTLPRESUPUESTO:
                    setCurrentCell(row, COL_PVPLPRESUPUESTO);
                    break;
                case COL_PVPLPRESUPUESTO:
                    setCurrentCell(row+1, COL_CODARTICULO);
                    break;
                }// end switch
                return TRUE;
                break;
            }// end switch
        }// end if
    */
    return Q3Table::eventFilter( obj, ev );
} //end eventFilter



void ListLTarifaView::valueBudgetLineChanged(int row, int col) {
    _depura("valueBudgetLineChanged \n",0);
/*
    LTarifa *linea;
    linea = lineaat(row);
        if (linea != NULL) {
            switch (col) {
            case COL_NOMARTICULO: {
                    float baseiva = text(row, COL_NOMARTICULO).replace(",",".").toFloat();
                    linea->setbaseiva(QString::number(baseiva));
                    break;
                }
            case COL_NOMTARIFA: {
                    float ivaiva = text(row, COL_NOMTARIFA).replace(",",".").toFloat();
                    linea->setivaiva(QString::number(ivaiva));
                    break;
                }// end case
            }// end switch
            pintaLTarifa(row);
        }// end if
*/
    _depura("END valueBudgetLineChanged \n",0);
} //end valueBudgetLineChanged


/// Devuelve la linea que se esta tratando actualmente
LTarifa *ListLTarifaView::lineaact() {
    _depura("ListLTarifaView::lineaact()\n",0);
    return lineaat(currentRow());
}// end lineaact

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
}// end lineaat



