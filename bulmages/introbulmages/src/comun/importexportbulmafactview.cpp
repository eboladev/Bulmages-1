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
#include "importexportbulmafactview.h"
#include "pgimportfiles.h"

#include <qfile.h>
#include <qfiledialog.h>
#include <qlineedit.h>
#include <qprogressbar.h>
#include <qtextbrowser.h>
#include <qcheckbox.h>

#include "busquedafecha.h"

#include <qtoolbutton.h>

#include "calendario.h"

void ImportExportBulmafactView::alerta(int a, int b) {
	m_progressbar->setProgress(a,b);
}// end realizado

void ImportExportBulmafactView::mensajeria(QString mensaje) {
	fprintf(stderr,"MENSAJE:\n\n\n%s\n\n\n", mensaje.ascii());
	mensajein += mensaje+"<BR>";
	m_mensajes->setText("<HTML><BODY BGCOLOR='#CCCCCC'>"+QString(mensajein.latin1())+"</BODY></HTML>");
//	m_mensajes->scrollBy(0,400);
}// end publicamensaje


ImportExportBulmafactView::ImportExportBulmafactView(postgresiface2 * con, QWidget * parent, const char * name, WFlags f=0) :  ImportExportBulmafactBase(parent,name,f), pgimportfiles(con) {
	conexionbase = con;
	inicializaMensajeria();
}//end importContaplus

/// Se ha pulsado sobre el bot� de bsqueda de una subcuenta.
void ImportExportBulmafactView::botonBuscarXML() {
	m_XML->setText( QFileDialog::getSaveFileName(confpr->valor(CONF_DIR_USER),"Contaplus (*.xml)", this, "select file", "Elija el Archivo"));
}// end botonBuscarSubCta
	

void ImportExportBulmafactView::botonImportar() {
	inicializaMensajeria();
	fprintf(stderr,"ImportExportBulmafactView::botonImportar()\n");
	QString finicial = m_fechainicial->text();
	QString ffinal = m_fechafinal->text();
		QFile filexml (m_XML->text());
		filexml.open(IO_ReadOnly);
		XML2BulmaFact(filexml);
		filexml.close();
//	m_mensajes->setText("");
}// end botonImportar 


/** \brief SLOT que responde a la pulsacion� del boton� de exportar
  * 
  * Se ha pulsado sobre el bot� de exportar. Lee los campos del formulario
  * mira si la opci� de exportaci� es XML o contaplus y llama a las funciones apropiadas
  * de la clase \ref pgimportfiles 
  * Esta funci� utiliza los punteros a funci� para inicializar \ref pgimportfiles con las funciones que se van a
  * encargar de presentaci� del estado de la importaci�.
  * \todo Los punteros a funci� deber�n ser reemplazados por funciones virtuales y haciendo derivar esta clase de pgimportfiles.
  */
void ImportExportBulmafactView::botonExportar() {
	/// Leemos las fechas entre las que tiene que ser el listado.
	inicializaMensajeria();
	QString finicial = m_fechainicial->text();
	QString ffinal = m_fechafinal->text();

	setFInicial(finicial);
	setFFinal(ffinal);	
	if (m_test->isChecked() ) {
		setModoTest();
	}// end if	
	if (m_XML->text() != "") {
		QFile filexml (m_XML->text());
		filexml.open(IO_WriteOnly);
		bulmafact2XML(filexml);
		filexml.close();
	}// end if
//	m_mensajes->setText("");
}// end botonExportar



