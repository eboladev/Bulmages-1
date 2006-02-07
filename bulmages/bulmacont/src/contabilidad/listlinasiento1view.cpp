//
// C++ Implementation: ListLinAsiento1View
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#define COL_FECHA       0
#define COL_CODIGO   1
#define COL_CONCEPTO    6
#define COL_DEBE        4
#define COL_HABER       5
#define COL_NOMCUENTA   2
#define COL_CONTRAPARTIDA 3
#define COL_IDBORRADOR    7
#define COL_TIPOIVA       8
#define COL_IVA           9
#define COL_NOMBREC_COSTE       10
#define COL_NOMBRECANAL        11
#define COL_IDCUENTA     12
#define COL_IDCONTRAPARTIDA 13
#define COL_IDCANAL         14
#define COL_IDCCOSTE        15
#define COL_ORDEN	    16


#include "listlinasiento1view.h"
#include "funcaux.h"

#include <q3table.h>
#include <qmessagebox.h>
#include <q3popupmenu.h>
//Added by qt3to4:
#include <QKeyEvent>
#include <QEvent>

void ListLinAsiento1View::guardaconfig() {
    _depura("ListLinAsiento1View::guardaconfig",0);
    QString aux = "";

    QFile file( confpr->valor(CONF_DIR_USER)+"conflistlinasiento1view.cfn" );
    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        for (int i = 0; i < numCols(); i++) {
            showColumn(i);
            stream << columnWidth(i) << "\n";
        }// end for
        file.close();
    }// end if
}// end guardaconfig()

void ListLinAsiento1View::cargaconfig() {
    _depura("ListLinAsiento1View::cargaconfig",0);
    QFile file( confpr->valor(CONF_DIR_USER)+"conflistlinasiento1view.cfn" );
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

ListLinAsiento1View::ListLinAsiento1View(QWidget * parent, const char * name) : Q3Table(parent, name), ListLinAsiento1() {
    /// Inicializamos la tabla de lineas de Borrador
    setNumCols(17);
    setNumRows(10000);
    horizontalHeader()->setLabel( COL_FECHA, tr( "COL_FECHA" ) );
    horizontalHeader()->setLabel( COL_CODIGO, tr( "COL_CODIGO" ) );
    horizontalHeader()->setLabel( COL_CONCEPTO, tr( "COL_CONCEPTO" ) );
    horizontalHeader()->setLabel( COL_DEBE, tr( "COL_DEBE" ) );
    horizontalHeader()->setLabel( COL_HABER, tr( "COL_HABER" ) );
    horizontalHeader()->setLabel( COL_NOMCUENTA, tr( "COL_NOMCUENTA" ) );
    horizontalHeader()->setLabel( COL_CONTRAPARTIDA, tr( "COL_CONTRAPARTIDA" ) );
    horizontalHeader()->setLabel( COL_IDBORRADOR, tr( "COL_IDBORRADOR" ) );
    horizontalHeader()->setLabel( COL_TIPOIVA, tr( "COL_TIPOIVA" ) );
    horizontalHeader()->setLabel( COL_IVA, tr( "COL_IVA" ) );
    horizontalHeader()->setLabel( COL_NOMBREC_COSTE, tr( "COL_NOMBREC_COSTE" ) );
    horizontalHeader()->setLabel( COL_NOMBRECANAL, tr( "COL_NOMBRECANAL" ) );
    horizontalHeader()->setLabel( COL_IDCUENTA, tr( "COL_IDCUENTA" ) );
    horizontalHeader()->setLabel( COL_IDCONTRAPARTIDA, tr( "COL_IDCONTRAPARTIDA" ) );
    horizontalHeader()->setLabel( COL_IDCANAL, tr( "COL_IDCANAL" ) );
    horizontalHeader()->setLabel( COL_IDCCOSTE, tr( "COL_IDCCOSTE" ) );
    horizontalHeader()->setLabel( COL_ORDEN, tr( "COL_ORDEN" ) );

    setSelectionMode( Q3Table::SingleRow );
    setColumnReadOnly(COL_NOMCUENTA,true);
    // Establecemos el color de fondo de la rejilla. El valor lo tiene la clase configuracion que es global.
    setPaletteBackgroundColor("#DDAAAA");

    connect(this, SIGNAL(valueChanged(int, int)), this, SLOT(valueBudgetLineChanged(int , int )));

    connect(this, SIGNAL(contextMenuRequested(int, int, const QPoint &)), this, SLOT(contextMenu(int, int, const QPoint &)));
    installEventFilter(this);
    cargaconfig();
}


ListLinAsiento1View::~ListLinAsiento1View() {
    guardaconfig();
}


void ListLinAsiento1View::pintaListLinAsiento1() {
    _depura("INICIO de pintaListLinAsiento1\n",0);
    setNumRows(0);
    setNumRows(100);
    /// \todo Habr� que vaciar la tabla para que el pintado fuera exacto.
    LinAsiento1 *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        pintalinListLinAsiento1(i);
        adjustRow(i);
        i++;
    }// end for
    _depura("FIN de pintaListLinAsiento1\n",0);
}



void ListLinAsiento1View::contextMenu ( int row, int, const QPoint & pos ) {
    Q3PopupMenu *popup;
    int opcion;
    popup = new Q3PopupMenu;
    popup->insertItem(tr(tr("Borrar Linea")),1);
    opcion = popup->exec(pos);
    delete popup;
    switch(opcion) {
    case 1:
        borraLinAsiento1(row);
    }// end switch
}// end contextMenuRequested


void ListLinAsiento1View::borraLinAsiento1act() {
    borraLinAsiento1(currentRow());
}// end borraLinAsiento1act


void ListLinAsiento1View::pintalinListLinAsiento1(int pos) {
    _depura("ListLinAsiento1View::pintalinListLinAsiento1(int)\n",0);
    LinAsiento1 *linea;
    linea = m_lista.at(pos);
    setText(pos, COL_FECHA, linea->fecha());
    setText(pos, COL_CODIGO, linea->codigo());
    setText(pos, COL_CONCEPTO, linea->conceptocontable());
    setText(pos, COL_DEBE, linea->debe());
    setText(pos, COL_HABER, linea->haber());
    setText(pos, COL_NOMCUENTA, linea->descripcioncuenta());
    setText(pos, COL_CONTRAPARTIDA, linea->contrapartida());
    setText(pos, COL_IDBORRADOR, linea->idborrador());
    setText(pos, COL_TIPOIVA, linea->idtipoiva());
    setText(pos, COL_IVA, linea->idtipoiva());
    setText(pos, COL_NOMBREC_COSTE, linea->nombrec_coste());
    setText(pos, COL_NOMBRECANAL, linea->nombrecanal());
    setText(pos, COL_IDCUENTA, linea->idcuenta());
    setText(pos, COL_IDCONTRAPARTIDA, linea->contrapartida());
    setText(pos, COL_IDCANAL, linea->idcanal());
    setText(pos, COL_IDCCOSTE, linea->idc_coste());
    setText(pos, COL_ORDEN, linea->orden());
    adjustRow(pos);

}


bool ListLinAsiento1View::eventFilter( QObject *obj, QEvent *ev ) {
    /*
        fprintf(stderr,"eventFilter()\n");
        QString idArticle;
        LinAsiento1 *linea;
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
                pintalinListLinAsiento1(currentRow());
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
    */
    return Q3Table::eventFilter( obj, ev );
} //end eventFilter



void ListLinAsiento1View::valueBudgetLineChanged(int row, int col) {
    _depura("valueBudgetLineChanged \n",0);
    LinAsiento1 *linea;
    linea = lineaat(row);
    if (linea != NULL) {
        switch (col) {
        case COL_FECHA:
            linea->setfecha(normalizafecha(text(row,COL_FECHA)).toString("dd/MM/yyyy"));
            break;
        case COL_CODIGO:
            linea->setcodigo(text(row, COL_CODIGO));
            break;
        case COL_DEBE: {
                Fixed val = Fixed(text(row, COL_DEBE));
                linea->setdebe(val.toQString());
                break;
            }
        case COL_HABER: {
                Fixed val = Fixed(text(row, COL_HABER));
                linea->sethaber(val.toQString());
                break;
            }
        case COL_NOMBRECANAL:
            linea->setnombrecanal(text(row, COL_NOMBRECANAL));
            break;
        case COL_CONCEPTO:
            linea->setdescripcion(text(row, COL_CONCEPTO));
            linea->setconceptocontable(text(row, COL_CONCEPTO));
            break;
        case COL_NOMBREC_COSTE:
            linea->setnombrec_coste(text(row, COL_NOMBREC_COSTE));
            break;
        }// end switch
        pintalinListLinAsiento1(row);
    }// end if

} //end valueBudgetLineChanged


/// Devuelve la linea que se esta tratando actualmente
LinAsiento1 *ListLinAsiento1View::lineaact() {
    _depura("ListLinAsiento1View::lineaact()\n",0);
    return lineaat(currentRow());
}// end lineaact


/// Devuelve la linea especificada, y si no existe se van creando lineas hasta que exista.
LinAsiento1 *ListLinAsiento1View::lineaat(int row) {
    _depura("ListLinAsiento1::lineaat(%d)\n", 0);
    LinAsiento1 *linea;
    if (row >=0) {
        while (m_lista.at(row) == 0 ) {
            fprintf(stderr,"Creamos la linea\n");
            linea=new LinAsiento1(companyact);
            linea->setidasiento(mdb_idasiento);
            m_lista.append(linea);
        }// end while
        return(m_lista.at(row));
    } else {
        return NULL;
    }// end if
}// end lineaat


void ListLinAsiento1View::guardaListLinAsiento1() {
    LinAsiento1 *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        linea->setDBvalue("orden",QString::number(i));
        i++;
    }// end for
    ListLinAsiento1::guardaListLinAsiento1();
}

/*
 
void ListLinAsiento1View::manageArticle(int row) {
    fprintf(stderr,"manageArticle(%d)\n",row);
    LinAsiento1 *linea= lineaat(row);
    QString articleCode = text(row, COL_CODARTICULO);
    linea->setcodigocompletoarticulo(text(row,COL_CODARTICULO));
    pintalinListLinAsiento1(row);
} //end manageArticle
 
 
QString ListLinAsiento1View::searchArticle() {
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
*/

