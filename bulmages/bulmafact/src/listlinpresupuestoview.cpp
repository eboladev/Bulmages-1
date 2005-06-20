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

#define COL_IDLPRESUPUESTO 0
#define COL_IDARTICULO 1
#define COL_CODARTICULO 2
#define COL_NOMARTICULO 3
#define COL_DESCLPRESUPUESTO 4
#define COL_CANTLPRESUPUESTO 5
#define COL_PVPLPRESUPUESTO 6
#define COL_DESCUENTOLPRESUPUESTO 7
#define COL_IDPRESUPUESTO 8
#define COL_REMOVE 9
#define COL_TASATIPO_IVA 10
#define COL_TIPO_IVA 11


#include "articleslist.h"
#include "listlinpresupuestoview.h"
#include <qtable.h>
#include <qmessagebox.h>
#include <qpopupmenu.h>


listlinpresupuestoview::listlinpresupuestoview(QWidget * parent, const char * name) : QTable(parent, name), listlinpresupuesto() {
    /// Inicializamos la tabla de lineas de presupuesto
    setNumCols(12);
    setNumRows(100);
    horizontalHeader()->setLabel( COL_IDLPRESUPUESTO, tr( "N. Línea" ) );
    horizontalHeader()->setLabel( COL_DESCLPRESUPUESTO, tr( "Descripción" ) );
    horizontalHeader()->setLabel( COL_CANTLPRESUPUESTO, tr( "Cantidad" ) );
    horizontalHeader()->setLabel( COL_PVPLPRESUPUESTO, tr( "Precio" ) );
    horizontalHeader()->setLabel( COL_DESCUENTOLPRESUPUESTO, tr( "Descuento" ) );
    horizontalHeader()->setLabel( COL_IDPRESUPUESTO, tr( "N Pedido" ) );
    horizontalHeader()->setLabel( COL_IDARTICULO, tr( "Artículo" ) );
    horizontalHeader()->setLabel( COL_CODARTICULO, tr( "Código Artículo" ) );
    horizontalHeader()->setLabel( COL_NOMARTICULO, tr( "Descripción Artículo" ) );
    horizontalHeader()->setLabel( COL_TASATIPO_IVA, tr( "% IVA" ) );
    horizontalHeader()->setLabel( COL_TIPO_IVA, tr( "Tipo IVA" ) );

    setColumnWidth(COL_IDLPRESUPUESTO,100);
    setColumnWidth(COL_DESCLPRESUPUESTO,300);
    setColumnWidth(COL_CANTLPRESUPUESTO,100);
    setColumnWidth(COL_PVPLPRESUPUESTO,100);
    setColumnWidth(COL_DESCUENTOLPRESUPUESTO,74);
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

    setSelectionMode( QTable::SingleRow );

    setColumnReadOnly(COL_NOMARTICULO,true);
    // Establecemos el color de fondo de la rejilla. El valor lo tiene la clase configuracion que es global.
    setPaletteBackgroundColor(confpr->valor(CONF_BG_LINPRESUPUESTOS));

    connect(this, SIGNAL(valueChanged(int, int)), this, SLOT(valueBudgetLineChanged(int , int )));

    connect(this, SIGNAL(contextMenuRequested(int, int, const QPoint &)), this, SLOT(contextMenu(int, int, const QPoint &)));

    installEventFilter(this);

}


listlinpresupuestoview::~listlinpresupuestoview() {}


void listlinpresupuestoview::pintalistlinpresupuesto() {
    fprintf(stderr,"INICIO de pintalistlinpresupuesto\n");
    setNumRows(0);
    setNumRows(100);
    /// \todo Habrï¿½ que vaciar la tabla para que el pintado fuera exacto.
    linpresupuesto *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        setText(i, COL_IDLPRESUPUESTO, linea->idlpresupuesto());
        setText(i, COL_IDARTICULO, linea->idarticulo());
        setText(i, COL_CODARTICULO, linea->codigocompletoarticulo());
        setText(i, COL_NOMARTICULO, linea->nomarticulo());
        setText(i, COL_DESCLPRESUPUESTO, linea->desclpresupuesto());
        setText(i, COL_CANTLPRESUPUESTO, linea->cantlpresupuesto());
        setText(i, COL_DESCUENTOLPRESUPUESTO, linea->descuentolpresupuesto());
        setText(i, COL_IDPRESUPUESTO, linea->idpresupuesto());
        setText(i, COL_REMOVE, "XX");
        setText(i, COL_TASATIPO_IVA, linea->ivalpresupuesto()+"%");
        setText(i, COL_TIPO_IVA, linea->idlpresupuesto());
        setText(i, COL_PVPLPRESUPUESTO, linea->pvplpresupuesto());
        i++;
    }// end for
    fprintf(stderr,"FIN de pintalistlinpresupuesto\n");
}



void listlinpresupuestoview::contextMenu ( int row, int, const QPoint & pos ) {
    QPopupMenu *popup;
    int opcion;
    popup = new QPopupMenu;
    popup->insertItem(tr(tr("Borrar Linea")),1);
    opcion = popup->exec(pos);
    delete popup;
    switch(opcion) {
    case 1:
        borralinpresupuesto(row);
    }// end switch
}// end contextMenuRequested


void listlinpresupuestoview::borralinpresupuestoact() {
    borralinpresupuesto(currentRow());
}// end borralinpresupuestoact


void listlinpresupuestoview::pintalinlistlinpresupuesto(int pos) {
fprintf(stderr,"pintalinlistlinpresupuesto(%d)\n",pos);
    linpresupuesto *linea;
    linea = m_lista.at(pos);
    setText(pos, COL_IDLPRESUPUESTO, linea->idlpresupuesto());
    setText(pos, COL_IDARTICULO, linea->idarticulo());
    setText(pos, COL_CODARTICULO, linea->codigocompletoarticulo());
    setText(pos, COL_NOMARTICULO, linea->nomarticulo());
    setText(pos, COL_DESCLPRESUPUESTO, linea->desclpresupuesto());
    setText(pos, COL_CANTLPRESUPUESTO, linea->cantlpresupuesto());
    setText(pos, COL_DESCUENTOLPRESUPUESTO, linea->descuentolpresupuesto());
    setText(pos, COL_IDPRESUPUESTO, linea->idpresupuesto());
    setText(pos, COL_REMOVE, "XX");
    setText(pos, COL_TASATIPO_IVA, linea->ivalpresupuesto()+"%");
    setText(pos, COL_TIPO_IVA, linea->idlpresupuesto());
    setText(pos, COL_PVPLPRESUPUESTO, linea->pvplpresupuesto());

}


bool listlinpresupuestoview::eventFilter( QObject *obj, QEvent *ev ) {
	fprintf(stderr,"eventFilter()\n");
    QString idArticle;
//    linpresupuesto *linea=lineaact();
    linpresupuesto *linea;//=m_lista.at(currentRow());
    
        if ( ev->type() == QEvent::KeyRelease ) {
//        if ( ev->type() == QEvent::KeyPress ) {
            QKeyEvent *k = (QKeyEvent *)ev;
            int col=currentColumn();
            int row=currentRow();
            switch (k->key()) {
            case Qt::Key_Plus:
                break;
            case Qt::Key_Asterisk:
	    	linea = lineaact();
                idArticle = searchArticle();
                linea->setidarticulo(idArticle);
                pintalinlistlinpresupuesto(currentRow());
		return TRUE;
                break;
            case Qt::Key_Return:
            case Qt::Key_Enter:
                // Esto se hace porque en la ltima linea del qtable tiene un comportamiento raro. Se reportarï¿½como bug a trolltech.
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
    return QTable::eventFilter( obj, ev );
} //end eventFilter



void listlinpresupuestoview::valueBudgetLineChanged(int row, int col) {
    fprintf(stderr,"valueBudgetLineChanged \n");
    linpresupuesto *linea;
    linea = lineaat(row);
    if (linea != NULL) {
        switch (col) {
        case COL_DESCUENTOLPRESUPUESTO: {
                float discountLine = text(row, COL_DESCUENTOLPRESUPUESTO).replace(",",".").toFloat();
                linea->setdescuentolpresupuesto(QString::number(discountLine));
                break;
            }
        case COL_CODARTICULO:
            manageArticle(row);
            break;
        case COL_DESCLPRESUPUESTO:
            linea->setdesclpresupuesto(text(row,COL_DESCLPRESUPUESTO));
            break;
        case COL_CANTLPRESUPUESTO: {
                float cantLine = text(row, COL_CANTLPRESUPUESTO).replace(",",".").toFloat();
                linea->setcantlpresupuesto(QString::number(cantLine));
                break;
            }
        case COL_PVPLPRESUPUESTO:  {
                float pvpLine = text(row, COL_PVPLPRESUPUESTO).replace(",",".").toFloat();
                linea->setpvplpresupuesto(QString::number(pvpLine));
                break;
            }
        case COL_TASATIPO_IVA: {
                float ivaLine = text(row, COL_TASATIPO_IVA).replace(",",".").toFloat();
                linea->setivalpresupuesto(QString::number(ivaLine));
                break;
            }// end case
        }// end switch
        pintalinlistlinpresupuesto(row);
    }// end if
} //end valueBudgetLineChanged


/// Devuelve la linea que se esta tratando actualmente
linpresupuesto *listlinpresupuestoview::lineaact() {
    fprintf(stderr,"listlinpresupuestoview::lineaact()\n");
    return lineaat(currentRow());
}// end lineaact

/// Devuelve la linea especificada, y si no existe se van creando lineas hasta que exista.
linpresupuesto *listlinpresupuestoview::lineaat(int row) {
    fprintf(stderr,"listlinpresupuesto::lineaat(%d)\n", row);
    linpresupuesto *linea;
    if (row >=0) {
        while (m_lista.at(row) == 0 ) {
	    fprintf(stderr,"Creamos la linea\n");
            linea=new linpresupuesto(companyact);
            linea->setidpresupuesto(mdb_idpresupuesto);
            m_lista.append(linea);  
        }// end while	
	      return(m_lista.at(row));
    } else {
    	fprintf(stderr,"Linea inexistente\n");
        return NULL;
    }// end if

}// end lineaat


void listlinpresupuestoview::manageArticle(int row) {
	fprintf(stderr,"manageArticle(%d)\n",row);
    linpresupuesto *linea= lineaat(row);
    QString articleCode = text(row, COL_CODARTICULO);
    linea->setcodigocompletoarticulo(text(row,COL_CODARTICULO));
    pintalinlistlinpresupuesto(row);
} //end manageArticle


QString listlinpresupuestoview::searchArticle() {
    fprintf(stderr,"Busqueda de un artï¿½ulo\n");
    articleslist *artlist = new articleslist(companyact, NULL, theApp->translate("Seleccione Artï¿½ulo","company"));
    // , WType_Dialog| WShowModal
    artlist->modoseleccion();
    // Esto es convertir un QWidget en un sistema modal de dialogo.
    this->setEnabled(false);
    artlist->show();
    while(!artlist->isHidden())
        theApp->processEvents();
    this->setEnabled(true);
    QString idArticle = artlist->idArticle();
    delete artlist;
    return idArticle;
}// end searchArticle
