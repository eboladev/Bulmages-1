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


listlinpresupuestoview::listlinpresupuestoview(company *comp,  QWidget * parent, const char * name) : QTable(parent, name), listlinpresupuesto (comp) {
	/// Inicializamos la tabla de lineas de presupuesto
	setNumCols(12);
	setNumRows(10);
	horizontalHeader()->setLabel( COL_IDLPRESUPUESTO, tr( "N L�ea" ) );
	horizontalHeader()->setLabel( COL_DESCLPRESUPUESTO, tr( "Descripci�" ) );
	horizontalHeader()->setLabel( COL_CANTLPRESUPUESTO, tr( "Cantidad" ) );
	horizontalHeader()->setLabel( COL_PVPLPRESUPUESTO, tr( "Precio" ) );
	horizontalHeader()->setLabel( COL_DESCUENTOLPRESUPUESTO, tr( "Descuento" ) );
	horizontalHeader()->setLabel( COL_IDPRESUPUESTO, tr( "N Pedido" ) );
	horizontalHeader()->setLabel( COL_IDARTICULO, tr( "Art�ulo" ) );
	horizontalHeader()->setLabel( COL_CODARTICULO, tr( "C�igo Art�ulo" ) );
	horizontalHeader()->setLabel( COL_NOMARTICULO, tr( "Descripci� Art�ulo" ) );
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
	setSelectionMode( QTable::SingleRow );	
	
	setColumnReadOnly(COL_NOMARTICULO,true);
	// Establecemos el color de fondo de la rejilla. El valor lo tiene la clase configuracion que es global.
	setPaletteBackgroundColor("#DDDDDD");   
	connect(this, SIGNAL(valueChanged(int, int)), this, SLOT(valueBudgetLineChanged(int , int )));
	installEventFilter(this);

}


listlinpresupuestoview::~listlinpresupuestoview() {
}
    
    
void listlinpresupuestoview::pintalistlinpresupuesto() {
    	fprintf(stderr,"INICIO de pintalistlinpresupuesto\n");
	/// \todo Habr� que vaciar la tabla para que el pintado fuera exacto.
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

    
    
bool listlinpresupuestoview::eventFilter( QObject *obj, QEvent *ev ) {
		if ( ev->type() == QEvent::KeyRelease ) {
			QKeyEvent *k = (QKeyEvent *)ev;
			switch (k->key()) {
				//esta linea ha sido modificada por Javier
				case Qt::Key_Return:
				case Qt::Key_Enter:
					QMessageBox::warning( this, "Se ha pulsado el return", "Aun no est�implementado.", "Si", "No");
					// Esto se hace porque en la ltima linea del qtable tiene un comportamiento raro. Se reportar�como bug a trolltech.
					return TRUE;
				
				case Qt::Key_Asterisk:

					return TRUE;
			} 
		}

	return QTable::eventFilter( obj, ev );
} //end eventFilter  



void listlinpresupuestoview::valueBudgetLineChanged(int row, int col) {
QMessageBox::warning( this, "Se ha cambiado un valor", "Aun no est�implementado.", "Si", "No");
	switch (col) {
		case COL_DESCUENTOLPRESUPUESTO: {
			setText(row, COL_DESCUENTOLPRESUPUESTO, text(row, COL_DESCUENTOLPRESUPUESTO).replace(",","."));
			float discountLine = text(row, COL_DESCUENTOLPRESUPUESTO).toFloat();
			setText(row, COL_DESCUENTOLPRESUPUESTO, QString().sprintf("%0.2f", discountLine));
			break;
		}
		case COL_CODARTICULO: {
			manageArticle(row);
//			calculateImports();
			break;
		}
		case COL_CANTLPRESUPUESTO: {
			setText(row, COL_CANTLPRESUPUESTO, text(row, COL_CANTLPRESUPUESTO).replace(",","."));
			float cantLine = text(row, COL_CANTLPRESUPUESTO).toFloat();
			setText(row, COL_CANTLPRESUPUESTO, QString().sprintf("%0.3f", cantLine));
//			calculateImports();
			break;
		}
		case COL_PVPLPRESUPUESTO: {
			setText(row, COL_PVPLPRESUPUESTO, text(row, COL_PVPLPRESUPUESTO).replace(",","."));
			float pvpLine = text(row, COL_PVPLPRESUPUESTO).toFloat();
			setText(row, COL_PVPLPRESUPUESTO, QString().sprintf("%0.2f", pvpLine));
//			calculateImports();
			break;
		}
	}
} //end valueBudgetLineChanged  



void listlinpresupuestoview::manageArticle(int row) {

	QString articleCode = text(row, COL_CODARTICULO);
	if (articleCode == "+") {
		QString idArticle = "";
		idArticle = searchArticle();
		setText(row, COL_CODARTICULO, idArticle);
		articleCode = idArticle;
	}// end if
	bool ok;
	if (articleCode.toInt(&ok, 10)>0) {
		setText(row, COL_NOMARTICULO, "");
		setText(row, COL_IDARTICULO, "");
		setText(row, COL_TASATIPO_IVA, "");
		setText(row, COL_TIPO_IVA, "");
		
		cursor2 * cur2= companyact->cargacursor("SELECT * FROM articulo WHERE codarticulo="+text(row, COL_CODARTICULO),"unquery");
		if (!cur2->eof()) {
			setText(row, COL_NOMARTICULO, cur2->valor("nomarticulo"));
			setText(row, COL_IDARTICULO, cur2->valor("idarticulo"));
			QString taxType = cur2->valor("idtipo_iva");
			companyact->begin();
			cursor2 * cur3= companyact->cargacursor("SELECT idtipo_iva, porcentasa_iva FROM tasa_iva WHERE idtipo_iva="+taxType+" ORDER BY fechatasa_iva DESC","unquery");
			companyact->commit();
			if (!cur3->eof()) {
				setText(row, COL_TASATIPO_IVA, cur3->valor("porcentasa_iva"));
				setText(row, COL_TIPO_IVA, cur3->valor("idtipo_iva"));
			}// end if
			delete cur3;
		}// end if
		delete cur2;
	}// end if
} //end manageArticle


QString listlinpresupuestoview::searchArticle() {

   fprintf(stderr,"Busqueda de un art�ulo\n");
   articleslist *artlist = new articleslist(companyact, NULL, theApp->translate("Seleccione Art�ulo","company"));
// , WType_Dialog| WShowModal   
   artlist->modoseleccion();
   // Esto es convertir un QWidget en un sistema modal de dialogo.
   this->setEnabled(false);
   artlist->show();
   while(!artlist->isHidden()) theApp->processEvents();
   this->setEnabled(true);
   QString idArticle = artlist->idArticle();
   delete artlist;
	return idArticle;
}// end searchArticle
