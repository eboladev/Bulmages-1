/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borrás                                    *
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
#include <qfiledialog.h>
#include <qlineedit.h>
#include <qprogressbar.h>
#include <qtextbrowser.h>
#include <qcheckbox.h>


QProgressBar *progress;
QTextBrowser *mensajes;
QString mensajein="";
void realizado(int a, int b) {
	fprintf(stderr,"mensaje publicado");
	progress->setProgress(a,b);
}// end realizado

void publicamensaje(QString mensaje) {
	mensajein+=mensaje;
	mensajes->setText(mensajein);
}// end publicamensaje


importContaplus::importContaplus(postgresiface2 * con, QWidget * parent, const char * name, WFlags f=0) :  importContaplusBase(parent,name,f){
	progress = m_progressbar;
	mensajes = m_mensajes;
	conexionbase = con;
}//end importContaplus


/// Se ha pulsado sobre el botón de búsqueda de una subcuenta.
void importContaplus::botonBuscarSubCta() {
	m_subCta->setText( QFileDialog::getOpenFileName("","Contaplus (*.txt)", this, "open file dialo", "Elija el Archivo"));
}// end botonBuscarSubCta

void importContaplus::botonBuscarDiario() {
	m_diario->setText( QFileDialog::getOpenFileName("","Contaplus (*.txt)", this, "open file dialo", "Elija el Archivo"));
}// end botonBuscarDiario	

void importContaplus::botonImportar() {

	void (*func) (int,int);
	func = realizado;
	void (*func1) (QString);
	func1 = publicamensaje;
	QFile filecont (m_subCta->text());
	QFile fileasie (m_diario->text());

	filecont.open(IO_ReadOnly);
	fileasie.open(IO_ReadOnly);
	pgimportfiles *importacion = new pgimportfiles(conexionbase,func, func1);
	if (m_test->isChecked() ) {
		importacion->setModoTest();
	}// end if
	importacion->contaplus2Bulmages(filecont, fileasie);
	filecont.close();
	fileasie.close();
	delete importacion;
	
	mensajein="";
}// end botonImportar 

