/***************************************************************************
                          canalview.cpp  -  description
                             -------------------
    begin                : lun mar 24 2003
    copyright            : (C) 2003 by Tomeu Borrás Riera
    email                : tborras@conetxia.com
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "canalview.h"
#include "empresa.h"
#include "selectcanalview.h"

canalview::canalview(empresa *emp, QWidget *parent, const char *name, bool modal ) : canaldlg(parent,name, modal) , dialogChanges(this) {
    empresaactual = emp;
    conexionbase=empresaactual->bdempresa();
    idcanal=0;
    dialogChanges_cargaInicial();
    pintar();
}// end canalview

canalview::~canalview() {}// end ~canalview

void canalview::pintar() {
    // Vamos a inicializar el combo de los canales
    QString query = "SELECT * from canal ORDER BY nombre";
    conexionbase->begin();
    cursor2 *cursorcanal = conexionbase->cargacursor(query,"canales");
    conexionbase->commit();
    int i = 0;
    combocanal->clear();
    while (!cursorcanal->eof()) {
        combocanal->insertItem(cursorcanal->valor(2), -1);
        canales[i++] = atoi (cursorcanal->valor(0).ascii());
        cursorcanal->siguienteregistro();
    }// end while
    delete cursorcanal;

    // Si el combocoste no está vacio se muestra el registro que
    // contiene
    if (idcanal != 0) {
        mostrarplantilla();
    } else if (combocanal->count() > 0) {
        idcanal = canales[combocanal->currentItem()];
        mostrarplantilla();
    }// end if

    // Si se han cambiado los canales, se rehace el selector de canales.
    selectcanalview *scanal = empresaactual->getselcanales();
    scanal->cargacanales();
}// end pintar

/*****************************************************
  Esta funcion sirve para hacer el cambio sobre un
  centro de coste .
  ****************************************************/
void canalview::cambiacombo(int numcombo) {
    //  fprintf(stderr,"idasientointeligente %d\n",listasientos[num]);
    int idcanal1 = canales[numcombo];
    if (dialogChanges_hayCambios()) {
        if ( QMessageBox::warning( this, tr("Guardar Canal"),
                                   tr("Desea guardar los cambios."),
                                   tr("Guardar"), tr("Cancelar"), 0 , 0, 1) == 0)
            boton_guardar();
    }// end if
    idcanal = idcanal1;
    mostrarplantilla();
}// end cambiacombo


/**
  Esta funcion muestra el canal en la ventana.
  */
void canalview::mostrarplantilla() {
    int i;
    QString query;
    query.sprintf("SELECT * from canal WHERE idcanal=%d",idcanal);
    cursor2 *cursorcanal = conexionbase->cargacursor(query);
    if (!cursorcanal->eof()) {
        nomcanal->setText(cursorcanal->valor("nombre"));
        desccanal->setText(cursorcanal->valor("descripcion"));
    }// end if
    i=0;
    while (canales[i] != idcanal)
        i++;
    combocanal->setCurrentItem(i);
    dialogChanges_cargaInicial();
}// end mostrarplantilla


void canalview::boton_guardar() {
    QString nom = nomcanal->text();
    QString desc = desccanal->text();
    QString query;
    query.sprintf ("UPDATE canal SET nombre='%s', descripcion='%s' WHERE idcanal=%d",
    conexionbase->sanearCadena(nom).ascii(), 
    conexionbase->sanearCadena(desc).ascii(), 
    idcanal);
    conexionbase->begin();
    conexionbase->ejecuta(query);
    conexionbase->commit();
    dialogChanges_cargaInicial();
    pintar();
}// end boton_guardar


void canalview::boton_nuevo() {
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
        if ( QMessageBox::warning( this, tr("Guardar Canal"),
                                   tr("Desea guardar los cambios."),
                                   tr("Guardar"), tr("Cancelar"), 0 ,0 ,1 ) == 0)
            boton_guardar();
    }// end if
    QString query;
    query.sprintf("INSERT INTO canal (nombre, descripcion) VALUES ('Nuevo Canal','Escriba su descripcion')");
    conexionbase->begin();
    conexionbase->ejecuta(query);
    query.sprintf("SELECT MAX(idcanal) AS id FROM canal");
    cursor2 *cur = conexionbase->cargacursor(query,"queryy");
    idcanal= atoi(cur->valor("id").ascii());
    delete cur;
    conexionbase->commit();
    pintar();
}// end boton_nuevo


void canalview::boton_borrar() {
    switch( QMessageBox::warning( this, tr("Borrar Canal"),
                                  tr("Se va a borrar la Forma de Pago,\n"
                                  "Esto puede ocasionar pérdida de datos\n") ,
                                  tr("Borrar"), tr("Cancelar"), 0 , 0, 1)) {
    case 0: // Retry clicked or Enter pressed
        QString query;
        query.sprintf("DELETE FROM canal WHERE idcanal=%d", idcanal);
        conexionbase->begin();
        conexionbase->ejecuta(query);
        conexionbase->commit();
        idcanal=0;
        pintar();
    }// end switch
}// end boton_borrar


/** Antes de salir de la ventana debemos hacer la comprobación de si se ha modificado algo */
void canalview::close() {
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
    	    if ( QMessageBox::warning( this, tr("Guardar Canal"),
		tr("Desea guardar los cambios."),
		tr("Guardar"), tr("Cancelar"),0 , 0, 1 ) == 0)
		boton_guardar();	
    }// end if
    QDialog::close();
}// end close

