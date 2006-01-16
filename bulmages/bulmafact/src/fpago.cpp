//
// C++ Implementation: %{MODULE}
//
// Description:
//
//
// Author: Tomeu Borr� Riera, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "fpago.h"
#include "company.h"
#include "funcaux.h"

#include <q3listview.h>
#include <qlineedit.h>
#include <qmessagebox.h>
#include <QCloseEvent>

#define COL_IDFPAGO   0
#define COL_NOMFPAGO  1
#define COL_DIASFPAGO 2
#define COL_DESCFPAGO 3

/** Constructor de la clase inicializa la clase y llama a la clase de pintar para que pinte */
fpago::fpago(company *emp,QWidget *parent, const char *name)
        : fpagobase(parent, name) , dialogChanges(this) {
    companyact=emp;
    m_listFPago->addColumn("id",0);
    m_listFPago->addColumn("nombre",-1);
    m_listFPago->addColumn("dias",0);
    m_listFPago->addColumn("descuento",0);
    setModoEdicion();
    pintar();
}


/** Carga el query de la base de datos y carga el qlistview */
void fpago::pintar() {
    Q3ListViewItem * it;
    cursor2 *cursoraux1;
    m_listFPago->clear();
    cursoraux1 = companyact->cargacursor("SELECT * FROM forma_pago ORDER BY idforma_pago","fpagos");
    while (!cursoraux1->eof()) {
        it =new Q3ListViewItem(m_listFPago);
        it->setText(COL_IDFPAGO, cursoraux1->valor("idforma_pago"));
        it->setText(COL_NOMFPAGO, cursoraux1->valor("descforma_pago"));
        it->setText(COL_DIASFPAGO, cursoraux1->valor("dias1tforma_pago"));
        it->setText(COL_DESCFPAGO, cursoraux1->valor("descuentoforma_pago"));
        cursoraux1->siguienteregistro ();
    }// end while
    delete cursoraux1;
    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
}// end pintar


fpago::~fpago() {}


void fpago::s_lista(Q3ListViewItem *it) {
    if (it != NULL) {
	/// Si se ha modificado el contenido advertimos y guardamos.
	trataModificado();
	m_descforma_pago->setText(it->text(COL_NOMFPAGO));
	m_dias1tforma_pago->setText(it->text(COL_DIASFPAGO));
	m_descuentoforma_pago->setText(it->text(COL_DESCFPAGO));
	m_idforma_pago = it->text(COL_IDFPAGO);
	/// Comprobamos cual es la cadena inicial.
	dialogChanges_cargaInicial();
    }// end if
}// end s_lista


/*
void fpago::close() {
    /// Si se ha modificado el contenido advertimos y guardamos.
    trataModificado();
    QDialog::close();
}
*/

void fpago::s_saveFPago() {
    QString query = "UPDATE forma_pago SET descforma_pago='"+
	companyact->sanearCadena(m_descforma_pago->text())+"', dias1tforma_pago= "+
	companyact->sanearCadena(m_dias1tforma_pago->text())+" , descuentoforma_pago = "+
	companyact->sanearCadena(m_descuentoforma_pago->text())+" WHERE idforma_pago="+m_idforma_pago;
    int error = companyact->ejecuta(query);
	if (error) {
		companyact->rollback();
		return;
	}// end if
    Q3ListViewItem *it =  m_listFPago->findItem(m_idforma_pago, COL_IDFPAGO);
    it->setText(COL_IDFPAGO, m_idforma_pago);
    it->setText(COL_NOMFPAGO, m_descforma_pago->text());
    it->setText(COL_DIASFPAGO, m_dias1tforma_pago->text());
    it->setText(COL_DESCFPAGO, m_descuentoforma_pago->text());
    dialogChanges_cargaInicial();
}// end if


bool fpago::trataModificado() {
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
        if ( QMessageBox::warning( this, "Guardar Forma de Pago",
                                   "Desea guardar los cambios.",
                                   QMessageBox::Ok ,
                                   QMessageBox::Cancel ) == QMessageBox::Ok)
            s_saveFPago();
        return (TRUE);
    }// end if
    return(FALSE);
}// end trataModificado


/** SLOT que responde a la pulsación del botón de nuevo tipo de iva
  * Inserta en la tabla de ivas
  */
void fpago::s_newFPago() {
    /// Si se ha modificado el contenido advertimos y guardamos.
    trataModificado();
    QString query = "INSERT INTO forma_pago (descforma_pago, dias1tforma_pago, descuentoforma_pago) VALUES ('NUEVA FORMA DE PAGO',0,0)";
    companyact->begin();
    int error = companyact->ejecuta(query);
	if (error) {
		companyact->rollback();
		return;
	}// end if
    cursor2 *cur = companyact->cargacursor("SELECT max(idforma_pago) AS idfpago FROM forma_pago");
    companyact->commit();
    m_idforma_pago = cur->valor("idfpago");
    delete cur;
    pintar();
}// end s_newTipoIVA


/** SLOT que responde a la pulsaci� del bot� de borrar la familia que se est�editando.
  * Lo que hace es que se hace un update de todos los campos
  */
void fpago::s_deleteFPago() {
    trataModificado();
	companyact->begin();
    QString query = "DELETE FROM forma_pago WHERE idforma_pago="+m_idforma_pago;
    int error = companyact->ejecuta(query);
	if (error) {
		companyact->rollback();
		return;
	}// end if
	companyact->commit();
    pintar();
}// end s_saveTipoIVA



void fpago::closeEvent( QCloseEvent *e) {
	_depura("fpago::closeEvent",0);
    if (dialogChanges_hayCambios())  {
        int val = QMessageBox::warning( this, "Guardar Forma de Pago",
                                   "Desea guardar los cambios.","Si","No","Cancelar",0,2);
	if (val == 0) 
            s_saveFPago();
	if (val == 2)
	    e->ignore();
    }// end if	
}
