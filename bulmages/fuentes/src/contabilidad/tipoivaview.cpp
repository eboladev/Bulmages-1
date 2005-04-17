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
/** \file tipoivaview.cpp
  * Contiene la implementación de la clase \ref tipoivaview
  * \author Tomeu Borrás Riera
  */
  
#include "tipoivaview.h"
#include "empresa.h"

/** El constructor de la clase prepara las variables globales y llama a la función pintar
  */
tipoivaview::tipoivaview(empresa *emp,QWidget *parent, const char *name) : tipoivadlg(parent, name) , dialogChanges(this) {
    empresaactual = emp;
    conexionbase = empresaactual->bdempresa();
    m_curtipoiva = NULL;
    dialogChanges_cargaInicial(); 
    pintar();
}// end tipoivaview


/** El destructor de la clase guarda los datos (por si ha habido cambios)
  * y libera la memoria que se haya ocupado
  */
tipoivaview::~tipoivaview() {
    s_saveTipoIVA();
    if (m_curtipoiva != NULL)
        delete m_curtipoiva;
}// end ~tipoivaview


/** Pinta la ventana, recarga el combo y si se pasa el parametro muestra el identificador indicado
  */
void tipoivaview::pintar(QString idtipoiva) {
    int posicion=0;
    /// Vamos a inicializar el combo de los tipos de IVA
    if (m_curtipoiva != NULL)
        delete m_curtipoiva;
    QString query = "SELECT * from tipoiva left join cuenta ON tipoiva.idcuenta = cuenta.idcuenta ORDER BY nombretipoiva";
    m_curtipoiva = conexionbase->cargacursor(query);
    m_comboTipoIVA->clear();
    int i = 0;
    while (!m_curtipoiva->eof()) {
        m_comboTipoIVA->insertItem(m_curtipoiva->valor("nombretipoiva"), i);
        if (idtipoiva == m_curtipoiva->valor("idtipoiva") )
            posicion=i;
        m_curtipoiva->siguienteregistro();
        i++;
    }// end while
    mostrarplantilla(posicion);
}// end pintar


/**
  * Esta funcion muestra el tipo de iva en la ventana.
  * \param pos si es distinto de cero se busca en el combo la posición indicada sino se usa la posición actual del combo.
  */
void tipoivaview::mostrarplantilla(int pos) {
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
    	    if ( QMessageBox::warning( this, "Guardar Tipo de IVA",
		"Desea guardar los cambios.",
		QMessageBox::Ok ,
		QMessageBox::Cancel ) == QMessageBox::Ok)
		s_saveTipoIVA();	
    }// end if
    if (m_comboTipoIVA->count() > 0) {
        if (pos != 0)
            m_comboTipoIVA->setCurrentItem(pos);
        m_posactual = m_comboTipoIVA->currentItem();
        m_nombreTipoIVA->setText(m_curtipoiva->valor("nombretipoiva", m_posactual));
        m_codigoCtaTipoIVA->setText(m_curtipoiva->valor("codigo",m_posactual));
        m_porcentTipoIVA->setText(m_curtipoiva->valor("porcentajetipoiva",m_posactual));
	/// Comprobamos cual es la cadena inicial.
	dialogChanges_cargaInicial();
    }// end if
}// end mostrarplantilla


/*****************************************************
  Esta funcion sirve para hacer el cambio sobre un
  centro de coste .
  ****************************************************/
void tipoivaview::cambiacombo(int) {
    mostrarplantilla();
}// end cambiacombo


/** SLOT que responde a la pulsación del botón de guardar el tipo de iva que se está editando.
  * Lo que hace es que se hace un update de todos los campos
  */
void tipoivaview::s_saveTipoIVA() {
    QString idtipoiva = m_curtipoiva->valor("idtipoiva",m_posactual);
    QString query = "UPDATE tipoiva SET nombretipoiva='"+m_nombreTipoIVA->text()+"', porcentajetipoiva= "+m_porcentTipoIVA->text()+" , idcuenta = id_cuenta('"+m_codigoCtaTipoIVA->text()+"') WHERE idtipoiva="+m_curtipoiva->valor("idtipoiva", m_posactual);
    conexionbase->ejecuta(query);
    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
    pintar(m_curtipoiva->valor("idtipoiva", m_posactual));
}// end s_saveTipoIVA


/** SLOT que responde a la pulsación del botón de nuevo tipo de iva
  * Inserta en la tabla de ivas
  */
void tipoivaview::s_newTipoIVA() {
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
    	    if ( QMessageBox::warning( this, "Guardar Tipo de IVA",
		"Desea guardar los cambios.",
		QMessageBox::Ok ,
		QMessageBox::Cancel ) == QMessageBox::Ok)
		s_saveTipoIVA();	
    }// end if
    QString query = "INSERT INTO tipoiva (nombretipoiva, porcentajetipoiva, idcuenta) VALUES ('NUEVO TIPO IVA',0,id_cuenta('47'))";
    conexionbase->begin();
    conexionbase->ejecuta(query);
    cursor2 *cur = conexionbase->cargacursor("SELECT max(idtipoiva) AS idtipoiva FROM tipoiva");
    conexionbase->commit();
    pintar(cur->valor("idtipoiva"));
    delete cur;
}// end s_newTipoIVA


/** SLOT que responde a la pulsación del botón de borrar un tipo de IVA
  * Borra en la tabla de tiposiva el TIPO de iva concreto
  */
void tipoivaview::s_deleteTipoIVA() {

    switch( QMessageBox::warning( this, "Borrar Tipo de IVA",
        "Se va a borrar el Tipo de IVA,\n"
        "Esto puede ocasionar pérdida de datos\n"
        "Tal vez deberia pensarselo mejor antes\n"
        "porque igual su trabajo se va a tomar por culo.",
        QMessageBox::Ok ,
        QMessageBox::Cancel )) {
    case QMessageBox::Ok: // Retry clicked or Enter pressed
	conexionbase->ejecuta("DELETE FROM tipoiva WHERE idtipoiva ="+m_curtipoiva->valor("idtipoiva",m_comboTipoIVA->currentItem()));
	pintar();	
        break;
    case QMessageBox::Cancel: // Abort clicked or Escape pressed
        break;
    }// end switch

}// s_deleteTipoIVA

/** SLOT que responde a la pulsación del botón de buscar una cuenta */
void tipoivaview::s_searchAccount() {
   listcuentasview1 *listcuentas = new listcuentasview1(empresaactual);
   listcuentas->modo=1;
   listcuentas->inicializa();
   listcuentas->exec();
   m_codigoCtaTipoIVA->setText(listcuentas->codcuenta);
   delete listcuentas;
}// end s_searchAccount


/** Antes de salir de la ventana debemos hacer la comprobación de si se ha modificado algo
  * Esta función está dedicada a Francina, Bienvenida al mundo 
  */
void tipoivaview::close() {
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
    	    if ( QMessageBox::warning( this, "Guardar Tipo de IVA",
		"Desea guardar los cambios.",
		QMessageBox::Ok ,
		QMessageBox::Cancel ) == QMessageBox::Ok)
		s_saveTipoIVA();	
    }// end if
    QDialog::close();
}// end close




