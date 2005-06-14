//
// C++ Implementation: ListLinAlbaranProveedorView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#define COL_NUMLALBARANP 0
#define COL_IDARTICULO 1
#define COL_CODARTICULO 2
#define COL_NOMARTICULO 3
#define COL_DESCLALBARANP 4
#define COL_CANTLALBARANP 5
#define COL_PVPLALBARANP 6
#define COL_DESCONTLALBARANP 7
#define COL_IDALBARAN 8
#define COL_REMOVE 9
#define COL_TASATIPO_IVA 10
#define COL_TIPO_IVA 11
#define COL_IVALALBARANP 12


#include <qtable.h>
#include <qmessagebox.h>
#include <qpopupmenu.h>

#include "listlinalbaranproveedorview.h"
#include "articleslist.h"



ListLinAlbaranProveedorView::ListLinAlbaranProveedorView(QWidget * parent, const char * name) : QTable(parent, name), ListLinAlbaranProveedor() {
    /// Inicializamos la tabla de lineas de Factura
    setNumCols(13);
    setNumRows(100);
    horizontalHeader()->setLabel( COL_NUMLALBARANP, tr( "N Lï¿½ea" ) );
    horizontalHeader()->setLabel( COL_DESCLALBARANP, tr( "Descripciï¿½" ) );
    horizontalHeader()->setLabel( COL_CANTLALBARANP, tr( "Cantidad" ) );
    horizontalHeader()->setLabel( COL_PVPLALBARANP, tr( "Precio" ) );
    horizontalHeader()->setLabel( COL_DESCONTLALBARANP, tr( "Descuento" ) );
    horizontalHeader()->setLabel( COL_IDALBARAN, tr( "N Pedido" ) );
    horizontalHeader()->setLabel( COL_IDARTICULO, tr( "Artï¿½ulo" ) );
    horizontalHeader()->setLabel( COL_CODARTICULO, tr( "Cï¿½igo Artï¿½ulo" ) );
    horizontalHeader()->setLabel( COL_NOMARTICULO, tr( "Descripciï¿½ Artï¿½ulo" ) );
    horizontalHeader()->setLabel( COL_TASATIPO_IVA, tr( "% IVA" ) );
    horizontalHeader()->setLabel( COL_TIPO_IVA, tr( "Tipo IVA" ) );
    horizontalHeader()->setLabel( COL_IVALALBARANP, tr( "IVA" ) );

    setColumnWidth(COL_NUMLALBARANP,100);
    setColumnWidth(COL_DESCLALBARANP,300);
    setColumnWidth(COL_CANTLALBARANP,100);
    setColumnWidth(COL_PVPLALBARANP,100);
    setColumnWidth(COL_DESCONTLALBARANP,74);
    setColumnWidth(COL_IDALBARAN,100);
    setColumnWidth(COL_IDARTICULO,100);
    setColumnWidth(COL_CODARTICULO,100);
    setColumnWidth(COL_NOMARTICULO,300);
    setColumnWidth(COL_TASATIPO_IVA,50);
    setColumnWidth(COL_TIPO_IVA,50);
    setColumnWidth(COL_IVALALBARANP,50);

    hideColumn(COL_NUMLALBARANP);
    hideColumn(COL_IDALBARAN);
    hideColumn(COL_IDARTICULO);
    hideColumn(COL_REMOVE);
    hideColumn(COL_TASATIPO_IVA);
    hideColumn(COL_TIPO_IVA);

    setSelectionMode( QTable::SingleRow );

    setColumnReadOnly(COL_NOMARTICULO,true);
    // Establecemos el color de fondo de la rejilla. El valor lo tiene la clase configuracion que es global.
    setPaletteBackgroundColor("#DDAADD");

    connect(this, SIGNAL(valueChanged(int, int)), this, SLOT(valueBudgetLineChanged(int , int )));

    connect(this, SIGNAL(contextMenuRequested(int, int, const QPoint &)), this, SLOT(contextMenu(int, int, const QPoint &)));

    installEventFilter(this);

}


ListLinAlbaranProveedorView::~ListLinAlbaranProveedorView() {}


void ListLinAlbaranProveedorView::pintaListLinAlbaranProveedor() {
    fprintf(stderr,"INICIO de pintaListLinAlbaranProveedor\n");
    setNumRows(0);
    setNumRows(100);
    /// \todo Habrï¿½ que vaciar la tabla para que el pintado fuera exacto.
    LinAlbaranProveedor *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        pintalinListLinAlbaranProveedor(linea,i);
        i++;
    }// end for
    fprintf(stderr,"FIN de pintaListLinAlbaranProveedor\n");
}


void ListLinAlbaranProveedorView::pintalinListLinAlbaranProveedor(LinAlbaranProveedor *linea, int i) {
    setText(i, COL_NUMLALBARANP, linea->numlalbaranp());
    setText(i, COL_IDARTICULO, linea->idarticulo());
    setText(i, COL_CODARTICULO, linea->codigocompletoarticulo());
    setText(i, COL_NOMARTICULO, linea->nomarticulo());
    setText(i, COL_DESCLALBARANP, linea->desclalbaranp());
    setText(i, COL_CANTLALBARANP, linea->cantlalbaranp());
    setText(i, COL_DESCONTLALBARANP, linea->descontlalbaranp());
    setText(i, COL_IDALBARAN, linea->idalbaranp());
    setText(i, COL_REMOVE, "XX");
    setText(i, COL_TASATIPO_IVA, "XX");
    setText(i, COL_TIPO_IVA, "XX");
    setText(i, COL_PVPLALBARANP, linea->pvplalbaranp());
    setText(i, COL_IVALALBARANP, linea->ivalalbaranp());
}


void ListLinAlbaranProveedorView::contextMenu ( int row, int, const QPoint & pos ) {
    QPopupMenu *popup;
    int opcion;
    popup = new QPopupMenu;
    popup->insertItem(tr(tr("Borrar Linea")),1);
    opcion = popup->exec(pos);
    delete popup;
    switch(opcion) {
    case 1:
        borraLinAlbaranProveedor(row);
    }// end switch
}// end contextMenuRequested


void ListLinAlbaranProveedorView::borraLinAlbaranProveedoract() {
    borraLinAlbaranProveedor(currentRow());
}// end borraLinAlbaranProveedoract


void ListLinAlbaranProveedorView::pintalinListLinAlbaranProveedor(int pos) {
    fprintf(stderr,"pintalinListLinAlbaranProveedor(%d)\n",pos);
    LinAlbaranProveedor *linea;
    linea = m_lista.at(pos);
    pintalinListLinAlbaranProveedor(linea, pos);
}


bool ListLinAlbaranProveedorView::eventFilter( QObject *obj, QEvent *ev ) {
    fprintf(stderr,"eventFilter()\n");
    QString idArticle;
    //    LinAlbaranProveedor *linea=lineaact();
    LinAlbaranProveedor *linea;//=m_lista.at(currentRow());

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
            pintalinListLinAlbaranProveedor(currentRow());
            return TRUE;
            break;
        case Qt::Key_Return:
        case Qt::Key_Enter:
            // Esto se hace porque en la ltima linea del qtable tiene un comportamiento raro. Se reportarï¿½como bug a trolltech.
            switch (col) {
            case COL_CODARTICULO:
                setCurrentCell(row, COL_DESCLALBARANP);
                break;
            case COL_DESCLALBARANP:
                setCurrentCell(row, COL_CANTLALBARANP);
                break;
            case COL_CANTLALBARANP:
                setCurrentCell(row, COL_PVPLALBARANP);
                break;
            case COL_PVPLALBARANP:
                setCurrentCell(row+1, COL_CODARTICULO);
                break;
            }// end switch
            return TRUE;
            break;
        }// end switch
    }// end if
    return QTable::eventFilter( obj, ev );
} //end eventFilter



void ListLinAlbaranProveedorView::valueBudgetLineChanged(int row, int col) {
    fprintf(stderr,"valueBudgetLineChanged \n");
    LinAlbaranProveedor *linea;
    linea = lineaat(row);
    if (linea != NULL) {
        switch (col) {
        case COL_DESCONTLALBARANP: {
                float discountLine = text(row, COL_DESCONTLALBARANP).replace(",",".").toFloat();
                linea->setdescontlalbaranp(QString::number(discountLine));
                break;
            }
        case COL_CODARTICULO:
            manageArticle(row);
            break;
        case COL_DESCLALBARANP:
            linea->setdesclalbaranp(text(row,COL_DESCLALBARANP));
            break;
        case COL_CANTLALBARANP: {
                float cantLine = text(row, COL_CANTLALBARANP).replace(",",".").toFloat();
                linea->setcantlalbaranp(QString::number(cantLine));
                break;
            }
        case COL_PVPLALBARANP:  {
                float pvpLine = text(row, COL_PVPLALBARANP).replace(",",".").toFloat();
                linea->setpvplalbaranp(QString::number(pvpLine));
                break;
            }
        }// end switch
        pintalinListLinAlbaranProveedor(row);
    }// end if
} //end valueBudgetLineChanged


/// Devuelve la linea que se esta tratando actualmente
LinAlbaranProveedor *ListLinAlbaranProveedorView::lineaact() {
    fprintf(stderr,"ListLinAlbaranProveedorView::lineaact()\n");
    return lineaat(currentRow());
}// end lineaact


/// Devuelve la linea especificada, y si no existe se van creando lineas hasta que exista.
LinAlbaranProveedor *ListLinAlbaranProveedorView::lineaat(int row) {
    fprintf(stderr,"ListLinAlbaranProveedor::lineaat(%d)\n", row);
    LinAlbaranProveedor *linea;
    if (row >=0) {
        while (m_lista.at(row) == 0 ) {
            fprintf(stderr,"Creamos la linea\n");
            linea=new LinAlbaranProveedor(companyact);
            linea->setidalbaranp(mdb_idalbaranp);
            m_lista.append(linea);
        }// end while
        return(m_lista.at(row));
    } else {
        fprintf(stderr,"Linea inexistente\n");
        return NULL;
    }// end if

}// end lineaat


void ListLinAlbaranProveedorView::manageArticle(int row) {
    fprintf(stderr,"manageArticle(%d)\n",row);
    LinAlbaranProveedor *linea= lineaat(row);
    QString articleCode = text(row, COL_CODARTICULO);
    linea->setcodigocompletoarticulo(text(row,COL_CODARTICULO));
    pintalinListLinAlbaranProveedor(row);
} //end manageArticle


QString ListLinAlbaranProveedorView::searchArticle() {
    fprintf(stderr,"Busqueda de un artï¿½ulo\n");
    articleslist *artlist = new articleslist(companyact, NULL, theApp->translate("Seleccione Artículo","company"));
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
