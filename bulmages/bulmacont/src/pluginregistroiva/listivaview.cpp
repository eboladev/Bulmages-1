//
// C++ Implementation: ListIvaView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#define COL_IDIVA 0
#define COL_IDREGISTROIVA 1
#define COL_IDTIPOIVA 2
#define COL_IDCUENTA 3
#define COL_NOMBRETIPOIVA 4
#define COL_CODIGO 5
#define COL_BASEIVA 6
#define COL_IVAIVA 7

#include "listivaview.h"
#include "funcaux.h"
#include <Q3Table>
#include <QMessageBox>
#include <Q3PopupMenu>
//Added by qt3to4:
#include <QKeyEvent>
#include <QEvent>
//Added by qt3to4:
#include <QTextStream>
#include "empresa.h"


void ListIvaView::guardaconfig() {
    _depura("ListIvaView::guardaconfig",0);
    QString aux = "";

    QFile file( confpr->valor(CONF_DIR_USER)+"confListIvaView.cfn" );
    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        for (int i = 0; i < numCols(); i++) {
            showColumn(i);
            stream << columnWidth(i) << "\n";
        }// end for
        file.close();
    }// end if
}// end guardaconfig()

void ListIvaView::cargaconfig() {
    _depura("ListIvaView::cargaconfig",0);
    QFile file( confpr->valor(CONF_DIR_USER)+"confListIvaView.cfn" );
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




ListIvaView::ListIvaView(QWidget * parent, const char * name) : Q3Table(parent, name), ListIva() {
    _depura("ListIvaView::ListIvaView",0);
    /// Inicializamos la tabla de lineas de presupuesto
    setNumCols(8);
    setNumRows(0);

    horizontalHeader()->setLabel( COL_IDIVA, tr( "COL_IDIVA" ) );
    horizontalHeader()->setLabel( COL_IDTIPOIVA, tr( "COL_IDTIPOIVA" ) );
    horizontalHeader()->setLabel( COL_IDCUENTA, tr( "COL_IDCUENTA" ) );
    horizontalHeader()->setLabel( COL_CODIGO, tr( "COL_CODIGO" ) );
    horizontalHeader()->setLabel( COL_NOMBRETIPOIVA, tr( "COL_NOMBRETIPOIVA" ) );
    horizontalHeader()->setLabel( COL_IDREGISTROIVA, tr( "COL_IDREGISTROIVA" ) );
    horizontalHeader()->setLabel( COL_BASEIVA, tr( "COL_BASEIVA" ) );
    horizontalHeader()->setLabel( COL_IVAIVA, tr( "COL_IVAIVA" ) );

    setColumnWidth(COL_IDIVA,100);
    setColumnWidth(COL_IDTIPOIVA,100);
    setColumnWidth(COL_IDCUENTA,100);
    setColumnWidth(COL_CODIGO,100);
    setColumnWidth(COL_NOMBRETIPOIVA,100);
    setColumnWidth(COL_IDREGISTROIVA,100);
    setColumnWidth(COL_BASEIVA,100);
    setColumnWidth(COL_IVAIVA,74);

    hideColumn(COL_IDIVA);
    hideColumn(COL_IDTIPOIVA);
    hideColumn(COL_IDCUENTA);
    hideColumn(COL_IDREGISTROIVA);

    setSelectionMode( Q3Table::SingleRow );
    // Establecemos el color de fondo de la rejilla. El valor lo tiene la clase configuracion que es global.
    setPaletteBackgroundColor(confpr->valor(CONF_BG_LINPRESUPUESTOS));

    connect(this, SIGNAL(valueChanged(int, int)), this, SLOT(valueBudgetLineChanged(int , int )));
    connect(this, SIGNAL(contextMenuRequested(int, int, const QPoint &)), this, SLOT(contextMenu(int, int, const QPoint &)));
    installEventFilter(this);
    cargaconfig();
}


ListIvaView::~ListIvaView() {
    _depura("ListIvaView::~ListIvaView()",0);
    guardaconfig();
}


void ListIvaView::pintaListIva() {
    _depura("ListIvaView::pintaListIva\n",0);
    setNumRows(0);
    setNumRows(m_lista.count());
    /// \todo Habr� que vaciar la tabla para que el pintado fuera exacto.
    Iva *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        pintaIva(i);
        adjustRow(i);
        i++;
    }// end for
    _depura("END ListIvaView::pintaListIva\n",0);
}



void ListIvaView::contextMenu ( int row, int, const QPoint & pos ) {
    Q3PopupMenu *popup;
    int opcion;
    popup = new Q3PopupMenu;
    popup->insertItem(tr(tr("Borrar Linea")),1);
    opcion = popup->exec(pos);
    delete popup;
    switch(opcion) {
    case 1:
        borraIva(row);
    }// end switch
}// end contextMenuRequested


void ListIvaView::borraIvaAct() {
    borraIva(currentRow());
}// end borralinpresupuestoact


void ListIvaView::pintaIva(int pos) {
    _depura("ListIvaView::pintaIva\n",0);
    Iva *linea;
    linea = m_lista.at(pos);
    setText(pos, COL_IDIVA, linea->idiva());
    setText(pos, COL_IDTIPOIVA, linea->idtipoiva());
    setText(pos, COL_IDCUENTA, linea->idcuenta());
    setText(pos, COL_CODIGO, linea->codigo());
    setText(pos, COL_NOMBRETIPOIVA, linea->nombretipoiva());
    setText(pos, COL_IDREGISTROIVA, linea->idregistroiva());
    setText(pos, COL_BASEIVA, linea->baseiva());
    setText(pos, COL_IVAIVA, linea->ivaiva());
    adjustRow(pos);
    _depura("END ListIvaView::pintaIva\n",0);
}// end pintaIva


bool ListIvaView::eventFilter( QObject *obj, QEvent *ev ) {
    _depura("eventFilter()\n",0);
    /*
        QString idArticle;
        //    linpresupuesto *linea=lineaact();
        Iva *linea;//=m_lista.at(currentRow());
     
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



void ListIvaView::valueBudgetLineChanged(int row, int col) {
    _depura("valueBudgetLineChanged \n",0);
    Iva *linea;
    linea = lineaat(row);
        if (linea != NULL) {
            switch (col) {
            case COL_BASEIVA: {
                    float baseiva = text(row, COL_BASEIVA).replace(",",".").toFloat();
                    linea->setbaseiva(QString::number(baseiva));
                    break;
                }
            case COL_IVAIVA: {
                    float ivaiva = text(row, COL_IVAIVA).replace(",",".").toFloat();
                    linea->setivaiva(QString::number(ivaiva));
                    break;
                }// end case
            }// end switch
            pintaIva(row);
        }// end if
    _depura("END valueBudgetLineChanged \n",0);
} //end valueBudgetLineChanged


/// Devuelve la linea que se esta tratando actualmente
Iva *ListIvaView::lineaact() {
    _depura("ListIvaView::lineaact()\n",0);
    return lineaat(currentRow());
}// end lineaact

/// Devuelve la linea especificada, y si no existe se van creando lineas hasta que exista.
Iva *ListIvaView::lineaat(int row) {
    _depura("ListIvaView::lineaat\n", 0);
    Iva *linea;
    if (row >=0) {
        while (m_lista.at(row) == 0 ) {
            fprintf(stderr,"Creamos la linea\n");
            linea=new Iva(companyact);
            linea->setidregistroiva(mdb_idregistroiva);
            m_lista.append(linea);
        }// end while
        return(m_lista.at(row));
    } else {
        _depura("Linea inexistente\n",0);
        return NULL;
    }// end if

}// end lineaat



