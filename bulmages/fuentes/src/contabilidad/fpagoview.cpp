//
// C++ Implementation: %{MODULE}
//
// Description:

// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//


/** \file fpagoview.cpp
  * Contiene la implementación de la clase \ref fpagoview
  * \author Tomeu Borrás Riera
  */

#include "fpagoview.h"
#include "empresa.h"

/** El constructor de la clase prepara las variables globales y llama a la función pintar
  */
fpagoview::fpagoview(empresa *emp, QWidget *parent, const char *name) : fpagodlg(parent, name) , dialogChanges(this) {
    empresaactual=emp;
    conexionbase = emp->bdempresa();
    m_curfpago = NULL;  
    dialogChanges_cargaInicial(); 
    pintar();
}// end fpagoview


/** El destructor de la clase guarda los datos (por si ha habido cambios)
  * y libera la memoria que se haya ocupado
  */
fpagoview::~fpagoview() {
    s_saveFPago();
    if (m_curfpago != NULL)
        delete m_curfpago;
}// end ~fpagoview


/** Pinta la ventana, recarga el combo y si se pasa el parametro muestra el identificador indicado
  */
void fpagoview::pintar(QString idfpago) {
    int posicion=0;
    /// Vamos a inicializar el combo de los tipos de IVA
    if (m_curfpago != NULL)
        delete m_curfpago;
    QString query = "SELECT * from fpago ORDER BY nomfpago";
    m_curfpago = conexionbase->cargacursor(query);
    m_comboFPago->clear();
    int i = 0;
    while (!m_curfpago->eof()) {
        m_comboFPago->insertItem(m_curfpago->valor("nomfpago"), i);
        if (idfpago == m_curfpago->valor("idfpago") )
            posicion=i;
        m_curfpago->siguienteregistro();
        i++;
    }// end while
    mostrarplantilla(posicion);
}// end pintar


/**
  * Esta funcion muestra la forma de pago en la ventana.
  * \param pos si es distinto de cero se busca en el combo la posición indicada sino se usa la posición actual del combo.
  */
void fpagoview::mostrarplantilla(int pos) {
    fprintf(stderr,"mostrarplantilla\n");
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
    	    if ( QMessageBox::warning( this, "Guardar Forma de Pago",
		"Desea guardar los cambios.",
		QMessageBox::Ok ,
		QMessageBox::Cancel ) == QMessageBox::Ok)
		s_saveFPago();	
    }// end if    
    if (m_comboFPago->count() > 0) {
        if (pos != 0)
            m_comboFPago->setCurrentItem(pos);
        m_posactual = m_comboFPago->currentItem();
        m_nomFPago->setText(m_curfpago->valor("nomfpago", m_posactual));
        m_plazoPrimerPagoFPago->setText(m_curfpago->valor("plazoprimerpagofpago",m_posactual));
        m_nPlazosFPago->setText(m_curfpago->valor("nplazosfpago",m_posactual));
        m_tipoPlazoPrimerPago->setText(m_curfpago->valor("tipoplazoprimerpago",m_posactual));
        m_plazoEntreReciboFPago->setText(m_curfpago->valor("plazoentrerecibofpago",m_posactual));
        m_tipoPlazoEntreReciboFPago->setText(m_curfpago->valor("tipoplazoentrerecibofpago",m_posactual));
	/// Comprobamos cual es la cadena inicial.
	dialogChanges_cargaInicial();
    }// end if
}// end mostrarplantilla


/**
  Esta funcion sirve para hacer el cambio sobre un
  centro de coste .
  */
void fpagoview::cambiacombo(int) {
    fprintf(stderr,"cambiado el combo\n");
    mostrarplantilla();
}// end cambiacombo


/** SLOT que responde a la pulsación del botón de guardar el tipo de iva que se está editando.
  * Lo que hace es que se hace un update de todos los campos
  */
void fpagoview::s_saveFPago() {
    QString idfpago = m_curfpago->valor("idfpago",m_posactual);
    QString query = "UPDATE fpago SET nomfpago='"+m_nomFPago->text()+"', nplazosfpago= "+m_nPlazosFPago->text()+" , plazoprimerpagofpago = "+m_plazoPrimerPagoFPago->text()+", plazoentrerecibofpago="+m_plazoEntreReciboFPago->text()+" WHERE idfpago="+m_curfpago->valor("idfpago", m_posactual);
    conexionbase->ejecuta(query);
    dialogChanges_cargaInicial();    
    pintar(m_curfpago->valor("idfpago", m_posactual));
}// end s_saveFPago


/** SLOT que responde a la pulsación del botón de nuevo tipo de iva
  * Inserta en la tabla de ivas
  */
void fpagoview::s_newFPago() {
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
    	    if ( QMessageBox::warning( this, "Guardar Forma de Pago",
		"Desea guardar los cambios.",
		QMessageBox::Ok ,
		QMessageBox::Cancel ) == QMessageBox::Ok)
		s_saveFPago();	
    }// end if
    QString query = "INSERT INTO fpago (nomfpago, nplazosfpago, plazoprimerpagofpago, plazoentrerecibofpago) VALUES ('NUEVA FORMA DE PAGO',0,0,0)";
    conexionbase->begin();
    conexionbase->ejecuta(query);
    cursor2 *cur = conexionbase->cargacursor("SELECT max(idfpago) AS idfpago FROM fpago");
    conexionbase->commit();
    pintar(cur->valor("idfpago"));
    delete cur;
}// end s_newTipoIVA



/** SLOT que responde a la pulsación del botón de borrar un tipo de IVA
  * Borra en la tabla de tiposiva el TIPO de iva concreto
  */
void fpagoview::s_deleteFPago() {
    switch( QMessageBox::warning( this, "Borrar Forma de Pago",
                                  "Se va a borrar la Forma de Pago,\n"
                                  "Esto puede ocasionar pérdida de datos\n"
                                  "Tal vez deberia pensarselo mejor antes\n"
                                  "porque igual su trabajo se pierde.",
                                  QMessageBox::Ok ,
                                  QMessageBox::Cancel )) {
    case QMessageBox::Ok: // Retry clicked or Enter pressed
        conexionbase->ejecuta("DELETE FROM fpago WHERE idfpago ="+m_curfpago->valor("idfpago",m_comboFPago->currentItem()));
        pintar();
        break;
    case QMessageBox::Cancel: // Abort clicked or Escape pressed
        break;
    }// end switch
}// s_deleteTipoIVA



/** Antes de salir de la ventana debemos hacer la comprobación de si se ha modificado algo */
bool fpagoview::close(bool ok) {
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
    	    if ( QMessageBox::warning( this, "Guardar Forma de Pago",
		"Desea guardar los cambios.",
		QMessageBox::Ok ,
		QMessageBox::Cancel ) == QMessageBox::Ok)
		s_saveFPago();	
    }// end if
    return QDialog::close(ok);
}// end close


