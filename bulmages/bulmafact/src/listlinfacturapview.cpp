//
// C++ Implementation: ListLinFacturaProveedorView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#define COL_IDLFACTURA 0
#define COL_IDARTICULO 1
#define COL_CODARTICULO 2
#define COL_NOMARTICULO 3
#define COL_DESCLFACTURA 4
#define COL_CANTLFACTURA 5
#define COL_PVPLFACTURA 6
#define COL_DESCUENTOLFACTURA 7
#define COL_IDFACTURA 8
#define COL_REMOVE 9
#define COL_TASATIPO_IVA 10
#define COL_TIPO_IVA 11


#include "articleslist.h"
#include "listlinfacturapview.h"
#include "funcaux.h"

#include <q3table.h>
#include <qmessagebox.h>
#include <q3popupmenu.h>
//Added by qt3to4:
#include <QKeyEvent>
#include <QEvent>

void ListLinFacturaProveedorView::guardaconfig() {
    _depura("ListLinFacturaProveedorView::guardaconfig",0);
    QString aux = "";

    QFile file( confpr->valor(CONF_DIR_USER)+"conflistlinfacturapview.cfn" );
    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        for (int i = 0; i < numCols(); i++) {
            stream << columnWidth(i) << "\n";
        }// end for
        file.close();
    }// end if
}// end guardaconfig()

void ListLinFacturaProveedorView::cargaconfig() {
	_depura("ListLinFacturaProveedorView::cargaconfig",0);
    QFile file( confpr->valor(CONF_DIR_USER)+"conflistlinfacturapview.cfn" );
    QString line;
    if ( file.open( QIODevice::ReadOnly ) ) {
        QTextStream stream( &file );
        for (int i = 0; i < numCols(); i++) {
            QString linea = stream.readLine();
            setColumnWidth(i, linea.toInt());
        }// end for
        file.close();
    }
}// end cargaconfig


ListLinFacturaProveedorView::ListLinFacturaProveedorView(QWidget * parent, const char * name) : Q3Table(parent, name), ListLinFacturaProveedor() {
    /// Inicializamos la tabla de lineas de FacturaProveedor
    setNumCols(12);
    setNumRows(100);
    horizontalHeader()->setLabel( COL_IDLFACTURA, tr( "N L�ea" ) );
    horizontalHeader()->setLabel( COL_DESCLFACTURA, tr( "Descripci�" ) );
    horizontalHeader()->setLabel( COL_CANTLFACTURA, tr( "Cantidad" ) );
    horizontalHeader()->setLabel( COL_PVPLFACTURA, tr( "Precio" ) );
    horizontalHeader()->setLabel( COL_DESCUENTOLFACTURA, tr( "Descuento" ) );
    horizontalHeader()->setLabel( COL_IDFACTURA, tr( "N Pedido" ) );
    horizontalHeader()->setLabel( COL_IDARTICULO, tr( "Art�ulo" ) );
    horizontalHeader()->setLabel( COL_CODARTICULO, tr( "C�igo Art�ulo" ) );
    horizontalHeader()->setLabel( COL_NOMARTICULO, tr( "Descripci� Art�ulo" ) );
    horizontalHeader()->setLabel( COL_TASATIPO_IVA, tr( "% IVA" ) );
    horizontalHeader()->setLabel( COL_TIPO_IVA, tr( "Tipo IVA" ) );

    hideColumn(COL_IDLFACTURA);
    hideColumn(COL_IDFACTURA);
    hideColumn(COL_IDARTICULO);
    hideColumn(COL_REMOVE);
    hideColumn(COL_TASATIPO_IVA);
    hideColumn(COL_TIPO_IVA);

    setSelectionMode( Q3Table::SingleRow );

    setColumnReadOnly(COL_NOMARTICULO,true);
    // Establecemos el color de fondo de la rejilla. El valor lo tiene la clase configuracion que es global.
    setPaletteBackgroundColor(confpr->valor(CONF_BG_LINFACTURASCLIENTE));

    connect(this, SIGNAL(valueChanged(int, int)), this, SLOT(valueBudgetLineChanged(int , int )));

    connect(this, SIGNAL(contextMenuRequested(int, int, const QPoint &)), this, SLOT(contextMenu(int, int, const QPoint &)));

    installEventFilter(this);
	cargaconfig();
}


ListLinFacturaProveedorView::~ListLinFacturaProveedorView() {
	guardaconfig();
}


void ListLinFacturaProveedorView::pintaListLinFacturaProveedor() {
    fprintf(stderr,"INICIO de pintaListLinFacturaProveedor\n");
    setNumRows(0);
    setNumRows(100);
    /// \todo Habr� que vaciar la tabla para que el pintado fuera exacto.
    LinFacturaProveedor *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        setText(i, COL_IDLFACTURA, linea->idlfacturap());
        setText(i, COL_IDARTICULO, linea->idarticulo());
        setText(i, COL_CODARTICULO, linea->codigocompletoarticulo());
        setText(i, COL_NOMARTICULO, linea->nomarticulo());
        setText(i, COL_DESCLFACTURA, linea->desclfacturap());
        setText(i, COL_CANTLFACTURA, linea->cantlfacturap());
        setText(i, COL_DESCUENTOLFACTURA, linea->descuentolfacturap());
        setText(i, COL_IDFACTURA, linea->idfacturap());
        setText(i, COL_REMOVE, "XX");
        setText(i, COL_TASATIPO_IVA, linea->ivalfacturap()+"%");
        setText(i, COL_TIPO_IVA, linea->idlfacturap());
        setText(i, COL_PVPLFACTURA, linea->pvplfacturap());
	adjustRow(i);
        i++;
    }// end for
    fprintf(stderr,"FIN de pintaListLinFacturaProveedor\n");
}



void ListLinFacturaProveedorView::contextMenu ( int row, int, const QPoint & pos ) {
    Q3PopupMenu *popup;
    int opcion;
    popup = new Q3PopupMenu;
    popup->insertItem(tr(tr("Borrar Linea")),1);
    opcion = popup->exec(pos);
    delete popup;
    switch(opcion) {
    case 1:
        borraLinFacturaProveedor(row);
    }// end switch
}// end contextMenuRequested


void ListLinFacturaProveedorView::borraLinFacturaProveedoract() {
    borraLinFacturaProveedor(currentRow());
}// end borraLinFacturaProveedoract


void ListLinFacturaProveedorView::pintalinListLinFacturaProveedor(int pos) {
    fprintf(stderr,"pintalinListLinFacturaProveedor(%d)\n",pos);
    LinFacturaProveedor *linea;
    linea = m_lista.at(pos);
    setText(pos, COL_IDLFACTURA, linea->idlfacturap());
    setText(pos, COL_IDARTICULO, linea->idarticulo());
    setText(pos, COL_CODARTICULO, linea->codigocompletoarticulo());
    setText(pos, COL_NOMARTICULO, linea->nomarticulo());
    setText(pos, COL_DESCLFACTURA, linea->desclfacturap());
    setText(pos, COL_CANTLFACTURA, linea->cantlfacturap());
    setText(pos, COL_DESCUENTOLFACTURA, linea->descuentolfacturap());
    setText(pos, COL_IDFACTURA, linea->idfacturap());
    setText(pos, COL_REMOVE, "XX");
    setText(pos, COL_TASATIPO_IVA, linea->ivalfacturap()+"%");
    setText(pos, COL_TIPO_IVA, linea->idlfacturap());
    setText(pos, COL_PVPLFACTURA, linea->pvplfacturap());
	adjustRow(pos);
}


bool ListLinFacturaProveedorView::eventFilter( QObject *obj, QEvent *ev ) {
    fprintf(stderr,"eventFilter()\n");
    QString idArticle;
    LinFacturaProveedor *linea;
    if ( ev->type() == QEvent::KeyRelease ) {
        QKeyEvent *k = (QKeyEvent *)ev;
        int col=currentColumn();
        int row=currentRow();
        switch (k->key()) {
        case Qt::Key_Plus:
		if (col == COL_DESCLFACTURA) {
                setText(row,col,editaTexto(text(row,col)));
		valueBudgetLineChanged(row,col);
                return TRUE;
		}// end if
            break;
        case Qt::Key_Asterisk:
            linea = lineaact();
            idArticle = searchArticle();
            linea->setidarticulo(idArticle);
            pintalinListLinFacturaProveedor(currentRow());
            return TRUE;
            break;
        case Qt::Key_Return:
        case Qt::Key_Enter:
            // Esto se hace porque en la ltima linea del qtable tiene un comportamiento raro. Se reportar�como bug a trolltech.
            switch (col) {
            case COL_CODARTICULO:
                setCurrentCell(row, COL_DESCLFACTURA);
                break;
            case COL_DESCLFACTURA:
                setCurrentCell(row, COL_CANTLFACTURA);
                break;
            case COL_CANTLFACTURA:
                setCurrentCell(row, COL_PVPLFACTURA);
                break;
            case COL_PVPLFACTURA:
                setCurrentCell(row+1, COL_CODARTICULO);
                break;
            }// end switch
            return TRUE;
            break;
        }// end switch
    }// end if
    return Q3Table::eventFilter( obj, ev );
} //end eventFilter



void ListLinFacturaProveedorView::valueBudgetLineChanged(int row, int col) {
    fprintf(stderr,"valueBudgetLineChanged \n");
    LinFacturaProveedor *linea;
    linea = lineaat(row);
    if (linea != NULL) {
        switch (col) {
        case COL_DESCUENTOLFACTURA: {
                float discountLine = text(row, COL_DESCUENTOLFACTURA).replace(",",".").toFloat();
                linea->setdescuentolfacturap(QString::number(discountLine));
                break;
            }
        case COL_CODARTICULO:
            manageArticle(row);
            break;
        case COL_DESCLFACTURA:
            linea->setdesclfacturap(text(row,COL_DESCLFACTURA));
            break;
        case COL_CANTLFACTURA: {
                float cantLine = text(row, COL_CANTLFACTURA).replace(",",".").toFloat();
                linea->setcantlfacturap(QString::number(cantLine));
                break;
            }
        case COL_PVPLFACTURA:  {
                float pvpLine = text(row, COL_PVPLFACTURA).replace(",",".").toFloat();
                linea->setpvplfacturap(QString::number(pvpLine));
                break;
            }
        case COL_TASATIPO_IVA: {
                float ivaLine = text(row, COL_TASATIPO_IVA).replace(",",".").toFloat();
                linea->setivalfacturap(QString::number(ivaLine));
                break;
            }// end case
        }// end switch
        pintalinListLinFacturaProveedor(row);
    }// end if
} //end valueBudgetLineChanged


/// Devuelve la linea que se esta tratando actualmente
LinFacturaProveedor *ListLinFacturaProveedorView::lineaact() {
    fprintf(stderr,"ListLinFacturaProveedorView::lineaact()\n");
    return lineaat(currentRow());
}// end lineaact


/// Devuelve la linea especificada, y si no existe se van creando lineas hasta que exista.
LinFacturaProveedor *ListLinFacturaProveedorView::lineaat(int row) {
    fprintf(stderr,"ListLinFacturaProveedor::lineaat(%d)\n", row);
    LinFacturaProveedor *linea;
    if (row >=0) {
        while (m_lista.at(row) == 0 ) {
            fprintf(stderr,"Creamos la linea\n");
            linea=new LinFacturaProveedor(companyact);
            linea->setidfacturap(mdb_idfacturap);
            m_lista.append(linea);
        }// end while
        return(m_lista.at(row));
    } else {
        fprintf(stderr,"Linea inexistente\n");
        return NULL;
    }// end if
}// end lineaat


void ListLinFacturaProveedorView::manageArticle(int row) {
    fprintf(stderr,"manageArticle(%d)\n",row);
    LinFacturaProveedor *linea= lineaat(row);
    QString articleCode = text(row, COL_CODARTICULO);
    linea->setcodigocompletoarticulo(text(row,COL_CODARTICULO));
    pintalinListLinFacturaProveedor(row);
} //end manageArticle


QString ListLinFacturaProveedorView::searchArticle() {
    fprintf(stderr,"Busqueda de un art�ulo\n");
    articleslist *artlist = new articleslist(companyact, NULL, theApp->translate("Seleccione Art�ulo","company"),0,articleslist::SelectMode);
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
