//
// C++ Implementation: %{MODULE}
//
// Description:
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "fpagoview.h"
#include "empresa.h"

fpagoview::fpagoview(empresa *emp, QWidget *parent, const char *name)
 : fpagodlg(parent, name)
{
	empresaactual=emp;
	conexionbase = emp->bdempresa();
    m_curfpago = NULL;
    pintar();
}


fpagoview::~fpagoview()
{
    s_saveFPago();
    if (m_curfpago != NULL)
        delete m_curfpago;
}


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
  * Esta funcion muestra el tipo de iva en la ventana.
  * \param pos si es distinto de cero se busca en el combo la posición indicada sino se usa la posición actual del combo.
  */
void fpagoview::mostrarplantilla(int pos) {
    fprintf(stderr,"mostrarplantilla\n");
    if (m_comboFPago->count() > 0) {
        int i;
        if (pos != 0)
            m_comboFPago->setCurrentItem(pos);
        i = m_comboFPago->currentItem();
        m_nomFPago->setText(m_curfpago->valor("nomfpago", i));
        m_plazoPrimerPagoFPago->setText(m_curfpago->valor("plazoprimerpagofpago",i));
        m_nPlazosFPago->setText(m_curfpago->valor("nplazosfpago",i));
        m_tipoPlazoPrimerPago->setText(m_curfpago->valor("tipoplazoprimerpago",i));
	m_plazoEntreReciboFPago->setText(m_curfpago->valor("plazoentrerecibofpago",i));
	m_tipoPlazoEntreReciboFPago->setText(m_curfpago->valor("tipoplazoentrerecibofpago",i));
    }// end if
}// end mostrarplantilla


/*****************************************************
  Esta funcion sirve para hacer el cambio sobre un
  centro de coste .
  ****************************************************/
void fpagoview::cambiacombo(int) {
    fprintf(stderr,"cambiado el combo\n");
    mostrarplantilla();
}// end cambiacombo

/** SLOT que responde a la pulsación del botón de guardar el tipo de iva que se está editando.
  * Lo que hace es que se hace un update de todos los campos
  */
void fpagoview::s_saveFPago() {
    int posicion = m_comboFPago->currentItem();
    QString idfpago = m_curfpago->valor("idfpago",posicion);
    QString query = "UPDATE fpago SET nomfpago='"+m_nomFPago->text()+"', nplazosfpago= "+m_nPlazosFPago->text()+" , plazoprimerpagofpago = "+m_plazoPrimerPagoFPago->text()+", plazoentrerecibofpago="+m_plazoEntreReciboFPago->text()+" WHERE idfpago="+m_curfpago->valor("idfpago", posicion);
    conexionbase->ejecuta(query);
    pintar(m_curfpago->valor("idfpago", posicion));
}// end s_saveFPago


/** SLOT que responde a la pulsación del botón de nuevo tipo de iva
  * Inserta en la tabla de ivas
  */
void fpagoview::s_newFPago() {
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
        "porque igual su trabajo se va a tomar por culo.",
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


