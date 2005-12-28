/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borr�                                    *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ***************************************************************************/
#include "importContaplus.h"
#include "pgimportfiles.h"

#include <qfile.h>
#include <q3filedialog.h>
#include <qlineedit.h>
#include <q3progressbar.h>
#include <q3textbrowser.h>
#include <qcheckbox.h>

#include "busquedafecha.h"

#include <qtoolbutton.h>

#include "calendario.h"

Q3ProgressBar *progress;
Q3TextBrowser *mensajes;
QString mensajein="";
void importContaplus::alerta(int a, int b) {
	fprintf(stderr,"mensaje publicado");
	progress->setProgress(a,b);
}// end realizado

void importContaplus::mensajeria(QString mensaje) {
	mensajein+=mensaje;
	mensajes->setText(mensajein);
	mensajes->scrollBy(0,400);
}// end publicamensaje


importContaplus::importContaplus(postgresiface2 * con, QWidget * parent, const char * name, Qt::WFlags f=0) :  importContaplusBase(parent,name,f), pgimportfiles(con) {
	progress = m_progressbar;
	mensajes = m_mensajes;
	conexionbase = con;
}//end importContaplus

/// Se ha pulsado sobre el bot� de bsqueda de una subcuenta.
void importContaplus::botonBuscarXML() {
	m_XML->setText( Q3FileDialog::getSaveFileName(confpr->valor(CONF_DIR_USER),"Contaplus (*.xml)", this, "select file", "Elija el Archivo"));
}// end botonBuscarSubCta

/// Se ha pulsado sobre el bot� de bsqueda de una subcuenta.
void importContaplus::botonBuscarSubCta() {
	m_subCta->setText( Q3FileDialog::getSaveFileName(confpr->valor(CONF_DIR_USER),"Contaplus (*.txt)", this, "select file", "Elija el Archivo"));
}// end botonBuscarSubCta

/** \brief SLOT que responde a la pulsaci� de selecci� de archivo.
  */
void importContaplus::botonBuscarDiario() {
	m_diario->setText( Q3FileDialog::getSaveFileName(confpr->valor(CONF_DIR_USER),"Contaplus (*.txt)", this, "select file", "Elija el Archivo"));
}// end botonBuscarDiario	

void importContaplus::botonImportar() {
	QString finicial = m_fechainicial->text();
	QString ffinal = m_fechafinal->text();
	if (m_subCta->text() != "") {
		QFile filecont (m_subCta->text());
		QFile fileasie (m_diario->text());
		filecont.open(QIODevice::ReadOnly);
		fileasie.open(QIODevice::ReadOnly);
		setFInicial(finicial);
		setFFinal(ffinal);	
		if (m_test->isChecked() ) {
			setModoTest();
		}// end if
		contaplus2Bulmages(filecont, fileasie);
		filecont.close();
		fileasie.close();
	} else {
		QFile filexml (m_XML->text());
		filexml.open(QIODevice::ReadOnly);
		XML2Bulmages(filexml);
		filexml.close();
	}// end if
	mensajein="";
}// end botonImportar 


/** \brief SLOT que responde a la pulsaci� del bot� de exportar
  * 
  * Se ha pulsado sobre el bot� de exportar. Lee los campos del formulario
  * mira si la opci� de exportaci� es XML o contaplus y llama a las funciones apropiadas
  * de la clase \ref pgimportfiles 
  * Esta funci� utiliza los punteros a funci� para inicializar \ref pgimportfiles con las funciones que se van a
  * encargar de presentaci� del estado de la importaci�.
  * \todo Los punteros a funci� deber�n ser reemplazados por funciones virtuales y haciendo derivar esta clase de pgimportfiles.
  */
void importContaplus::botonExportar() {
	/// Leemos las fechas entre las que tiene que ser el listado.
	QString finicial = m_fechainicial->text();
	QString ffinal = m_fechafinal->text();

	setFInicial(finicial);
	setFFinal(ffinal);	
	if (m_test->isChecked() ) {
		setModoTest();
	}// end if	
	if (m_subCta->text() != "") {
		QFile filecont (m_subCta->text());
		QFile fileasie (m_diario->text());
		filecont.open(QIODevice::WriteOnly);
		fileasie.open(QIODevice::WriteOnly);
		bulmages2Contaplus(filecont, fileasie);
		filecont.close();
		fileasie.close();
	} // end if
	if (m_XML->text() != "") {
		QFile filexml (m_XML->text());
		filexml.open(QIODevice::WriteOnly);
		bulmages2XML(filexml);
		filexml.close();
	}// end if
	mensajein="";
}// end botonExportar



