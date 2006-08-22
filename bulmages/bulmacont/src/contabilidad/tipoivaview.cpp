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

#include <QWidget>
#include "tipoivaview.h"
#include "empresa.h"
#include "busquedacuenta.h"


/** El constructor de la clase prepara las variables globales y llama a la función pintar
  */
tipoivaview::tipoivaview(empresa *emp,QWidget *parent) : QWidget(parent, Qt::WDestructiveClose) , dialogChanges(this) {
    _depura("tipoivaview::tipoivaview", 0);
    setupUi(this);
    empresaactual = emp;
    m_codigoCtaTipoIVA->setempresa(emp);
    m_curtipoiva = NULL;
    dialogChanges_cargaInicial();
    pintar();
    empresaactual->meteWindow(windowTitle(), this);
    _depura("END tipoivaview::tipoivaview", 0);
}


/** El destructor de la clase guarda los datos (por si ha habido cambios)
  * y libera la memoria que se haya ocupado
  */
tipoivaview::~tipoivaview() {
    _depura("tipoivaview::~tipoivaview", 0);
    s_saveTipoIVA();
    if (m_curtipoiva != NULL)
        delete m_curtipoiva;
    empresaactual->sacaWindow(this);
    _depura("END tipoivaview::~tipoivaview", 0);
}


/** Pinta la ventana, recarga el combo y si se pasa el parametro muestra el identificador indicado
  */
void tipoivaview::pintar(QString idtipoiva) {
    int posicion=0;
    /// Vamos a inicializar el combo de los tipos de IVA
    if (m_curtipoiva != NULL)
        delete m_curtipoiva;
    QString query = "SELECT * from tipoiva left join cuenta ON tipoiva.idcuenta = cuenta.idcuenta ORDER BY nombretipoiva";
    m_curtipoiva = empresaactual->cargacursor(query);
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
}


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
}


/*****************************************************
  Esta funcion sirve para hacer el cambio sobre un
  centro de coste .
  ****************************************************/
void tipoivaview::cambiacombo(int) {
    mostrarplantilla();
}


/** SLOT que responde a la pulsación del botón de guardar el tipo de iva que se está editando.
  * Lo que hace es que se hace un update de todos los campos
  */
void tipoivaview::s_saveTipoIVA() {
    QString idtipoiva = m_curtipoiva->valor("idtipoiva",m_posactual);
    QString query = "UPDATE tipoiva SET nombretipoiva='"+m_nombreTipoIVA->text()+"', porcentajetipoiva= "+m_porcentTipoIVA->text()+" , idcuenta = id_cuenta('"+m_codigoCtaTipoIVA->text()+"') WHERE idtipoiva="+m_curtipoiva->valor("idtipoiva", m_posactual);
    empresaactual->ejecuta(query);
    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
    pintar(m_curtipoiva->valor("idtipoiva", m_posactual));
}


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
    empresaactual->begin();
    empresaactual->ejecuta(query);
    cursor2 *cur = empresaactual->cargacursor("SELECT max(idtipoiva) AS idtipoiva FROM tipoiva");
    empresaactual->commit();
    pintar(cur->valor("idtipoiva"));
    delete cur;
}


/** SLOT que responde a la pulsación del botón de borrar un tipo de IVA
  * Borra en la tabla de tiposiva el TIPO de iva concreto
  */
void tipoivaview::s_deleteTipoIVA() {

    switch( QMessageBox::warning( this, "Borrar Tipo de IVA",
                                  "Se va a borrar el Tipo de IVA,\n"
                                  "Esto puede ocasionar pérdida de datos\n"
                                  "Tal vez deberia pensarselo mejor antes\n"
                                  "porque igual su trabajo se va perder.",
                                  QMessageBox::Ok ,
                                  QMessageBox::Cancel )) {
    case QMessageBox::Ok: // Retry clicked or Enter pressed
        empresaactual->ejecuta("DELETE FROM tipoiva WHERE idtipoiva ="+m_curtipoiva->valor("idtipoiva",m_comboTipoIVA->currentItem()));
        pintar();
        break;
    case QMessageBox::Cancel: // Abort clicked or Escape pressed
        break;
    }// end switch
}


/** Antes de salir de la ventana debemos hacer la comprobación de si se ha modificado algo
  * Esta función está dedicada a Francina, Bienvenida al mundo. :) 
  */
bool tipoivaview::close(bool ok) {
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
        if ( QMessageBox::warning( this, "Guardar Tipo de IVA",
                                   "Desea guardar los cambios.",
                                   QMessageBox::Ok ,
                                   QMessageBox::Cancel ) == QMessageBox::Ok)
            s_saveTipoIVA();
    }// end if
    return QWidget::close(ok);
}




