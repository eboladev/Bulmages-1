//
// C++ Implementation: %{MODULE}
//
// Description:
//
//
// Author: Tomeu Borrás Riera, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "fpago.h"
#include "company.h"

#include <qlistview.h>
#include <qlineedit.h>
#include <qmessagebox.h>


#define COL_IDFPAGO   0
#define COL_NOMFPAGO  1
#define COL_DIASFPAGO 2
#define COL_DESCFPAGO 3

/** Constructor de la clase inicializa la clase y llama a la clase de pintar para que pinte */
fpago::fpago(company *emp,QWidget *parent, const char *name)
 : fpagobase(parent, name) {
	companyact=emp;
         m_listFPago->addColumn("id",0);
         m_listFPago->addColumn("nombre",-1);
         m_listFPago->addColumn("dias",0);
         m_listFPago->addColumn("descuento",0);
	pintar();
}


/** Carga el query de la base de datos y carga el qlistview */
void fpago::pintar() {
    QListViewItem * it;
    cursor2 *cursoraux1;
    m_listFPago->clear();    
    cursoraux1 = companyact->cargacursor("SELECT * FROM forma_pago ORDER BY idforma_pago","fpagos");
    while (!cursoraux1->eof()) {
        it =new QListViewItem(m_listFPago);
        it->setText(COL_IDFPAGO, cursoraux1->valor("idforma_pago"));
        it->setText(COL_NOMFPAGO, cursoraux1->valor("descforma_pago"));
	it->setText(COL_DIASFPAGO, cursoraux1->valor("dias1tforma_pago"));
	it->setText(COL_DESCFPAGO, cursoraux1->valor("descuentoforma_pago"));
        cursoraux1->siguienteregistro ();
    }// end while
    delete cursoraux1;
    s_releaseModificado();
}// end pintar


fpago::~fpago() {
}


void fpago::s_lista(QListViewItem *it) {
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (m_modificado) {
    	    if ( QMessageBox::warning( this, "Guardar Forma de Pago",
		"Desea guardar los cambios.",
		QMessageBox::Ok ,
		QMessageBox::Cancel ) == QMessageBox::Ok)
//		s_saveTipoIVA();	
		s_saveFPago();
    }// end if
    m_descforma_pago->setText(it->text(COL_NOMFPAGO));
    m_dias1tforma_pago->setText(it->text(COL_DIASFPAGO));
    m_descuentoforma_pago->setText(it->text(COL_DESCFPAGO));
    m_idforma_pago = it->text(COL_IDFPAGO);
    s_releaseModificado();
}// end s_lista



void fpago::close() {
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (m_modificado) {
    	    if ( QMessageBox::warning( this, "Guardar Forma de Pago",
		"Desea guardar los cambios.",
		QMessageBox::Ok ,
		QMessageBox::Cancel ) == QMessageBox::Ok)	
		s_saveFPago();
    }// end if
    QDialog::close();
}

void fpago::s_saveFPago() {
}// end if

