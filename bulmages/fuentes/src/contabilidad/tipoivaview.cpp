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
#include "tipoivaview.h"

#include "empresa.h"

tipoivaview::tipoivaview(empresa *emp,QWidget *parent, const char *name)
 : tipoivadlg(parent, name) {
	empresaactual = emp;
	conexionbase = empresaactual->bdempresa();
	m_idtipoiva = "";
	m_curtipoiva = NULL;
	pintar();
}


tipoivaview::~tipoivaview() {
  if (m_curtipoiva != NULL) 
  	delete m_curtipoiva;
}// end ~tipoivaview

void tipoivaview::pintar() {
  // Vamos a inicializar el combo de los tipos de IVA
  if (m_curtipoiva != NULL) 
  	delete m_curtipoiva;
  
  QString query = "SELECT * from tipoiva left join cuenta ON tipoiva.idcuenta = cuenta.idcuenta ORDER BY nombretipoiva";
  m_curtipoiva = conexionbase->cargacursor(query);

  m_comboTipoIVA->clear();
  
  int i = 0;
  while (!m_curtipoiva->eof()) {
    m_comboTipoIVA->insertItem(m_curtipoiva->valor("nombretipoiva"), i);
    m_curtipoiva->siguienteregistro();
    i++;
  }// end while

  mostrarplantilla();
}// end pintar

/**
  Esta funcion muestra el canal en la ventana.
  */
void tipoivaview::mostrarplantilla() {
    fprintf(stderr,"mostrarplantilla\n");
    if (m_comboTipoIVA->count() > 0) {
	int i = m_comboTipoIVA->currentItem();
	m_nombreTipoIVA->setText(m_curtipoiva->valor("nombretipoiva", i));
    }// end if
}// end mostrarplantilla


/*****************************************************
  Esta funcion sirve para hacer el cambio sobre un
  centro de coste .
  ****************************************************/
void tipoivaview::cambiacombo(int) {
   fprintf(stderr,"cambiado el combo\n");
   mostrarplantilla();
}// end cambiacombo




