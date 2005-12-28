/***************************************************************************
                          cuentaview.cpp  -  description
                             -------------------
    begin                : Wed Nov 27 2002
    copyright            : (C) 2002 by Tomeu Borrás Riera
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

#include "cuentaview.h"
#include "empresa.h"

cuentaview::cuentaview(empresa *emp, QWidget *parent, const char *name, int fl): cuentadlg(parent,name,fl) ,dialogChanges(this) {
    idcuenta=0;
    empresaactual = emp;
    conexionbase = emp->bdempresa();
    numdigitos = emp->numdigitosempresa();
    inicializa();
}// end cuentaview

/** Saca el formulario, crea una cuenta y devuelve su identificador
  */
void cuentaview::cuentanueva(QString cod) {
    codigo->setText(cod);
    /// VAmos a buscar el posible padre de una cuenta y lo pondremos como toca.
    QString cpadre = conexionbase->searchParent(cod);
    codigopadre->setText(cpadre);
    /// También buscaremos el grupo y lo pondremos como toca
    QString query = "SELECT * FROM cuenta WHERE codigo = '"+cpadre+"'";
    cursor2 *cur = conexionbase->cargacursor(query);

    /// Y también buscaremos los tipos y los usaremos.
    if (!cur->eof()) {
        descripcion->setText(cur->valor("descripcion"));
        debe->setText("0");
        haber->setText("0");
        if (cur->valor("bloqueada") != "f") {
            bloqueada->setChecked(true);
        } else {
            bloqueada->setChecked(false);
        }// end if
        if (cur->valor("imputacion")!= "f") {
            imputacion->setChecked(true);
        } else {
            imputacion->setChecked(false);
        }// end if
        if (cur->valor("nodebe") != "f") {
            nodebe->setChecked(true);
        } else {
            nodebe->setChecked(false);
        }// end if
        if (cur->valor("nohaber") != "f") {
            nohaber->setChecked(true);
        } else {
            nohaber->setChecked(false);
        }// end if
        if (cur->valor("regularizacion") != "f") {
            regularizacion->setChecked(true);
        } else {
            regularizacion->setChecked(false);
        }// end if
        /// Vamos a hacer la carga del tipocuenta
        int tipocuenta = atoi(cur->valor("tipocuenta").ascii());
        switch(tipocuenta) {
        case 0:
            cuentasintipo->setChecked(TRUE);
            break;
        case 1:
            cuentaactivo->setChecked(TRUE);
            break;
        case 2:
            cuentapasivo->setChecked(TRUE);
            break;
        case 3:
            cuentaneto->setChecked(TRUE);
            break;
        case 4:
            cuentaingreso->setChecked(TRUE);
            break;
        case 5:
            cuentagasto->setChecked(TRUE);
            break;
        }// end switch
        // Vamos a hacer la carga del grupo
        int idgrupo = atoi(cur->valor(6).ascii());
        int i=0;
        while (idgrupos[i]!=idgrupo && i<100)
            i++;
        combogrupos->setCurrentItem(i);
        delete cur;
    }// end if
}// end cuentanueva


cuentaview::~cuentaview() {}// end ~cuentaview


/*********************************************************************
 * Esta función se activa cada vez quese pulsa una tecla sobre la cuenta
 *********************************************************************/
void cuentaview::cambiapadre(const QString &cadena) {
    QString cad;
    cursor2 *cursoraux;
    cad = codigopadre->text();
    unsigned int i = cad.find(".",0);
    if (i+1 < (unsigned int)cad.length()) {
        if (i != 0) {
            cad.replace(i,1,"%");
        }// end if
        conexionbase->begin();
        cursoraux = conexionbase->cargacuenta(0, cad );
        conexionbase->commit();
        if (!cursoraux->eof()) {
            codigopadre->setText(cursoraux->valor(1));
        }// end if
        delete cursoraux;
    }// end if

    // PAra quitar el warning
    cadena.isNull();
}// end cambiapadre


/**
 * Este es el slot que se activa al pulsar el boton ok del formulario
 * Lo que hace es recoger los datos del formulario y hacer una insercion
 * o una modificacion de la tabla de cuentas.
 */
void cuentaview::close() {
    if (dialogChanges_hayCambios()) {
        if ( QMessageBox::warning( this, "Guardar Cuenta",
                                   "Desea guardar los cambios.",
                                   QMessageBox::Ok ,
                                   QMessageBox::Cancel ) == QMessageBox::Ok)
            saveAccount();
    }// end if
    // Liberamos memoria y terminamos.
    done(1);
}// end accept

int cuentaview::inicializa() {
    cursor2 *cursorgrupos;
    for (int i=0;i<100;i++) {
        idgrupos[i]=-1;
    }// end for
    conexionbase->begin();
    cursorgrupos = conexionbase->cargagrupos();
    conexionbase->commit();
    while (!cursorgrupos->eof()) {
        combogrupos->insertItem(cursorgrupos->valor(1),-1);
        idgrupos[combogrupos->count()-1]=atoi(cursorgrupos->valor(0));
        cursorgrupos->siguienteregistro();
    }// end while
    delete cursorgrupos;
    return(0);
}// end inicializa


int cuentaview::cargacuenta(int idcuenta1) {
    QString cadena;
    int cpadre;
    cursor2 *cursorcuenta, *cursorpadre;
    idcuenta = idcuenta1;
    conexionbase->begin();
    cursorcuenta = conexionbase->cargacuenta(idcuenta1);
    conexionbase->commit();
    codigo->setText(cursorcuenta->valor("codigo"));
    descripcion->setText(cursorcuenta->valor("descripcion"));
    debe->setText(cursorcuenta->valor("debe"));
    haber->setText(cursorcuenta->valor("haber"));
    if (cursorcuenta->valor("bloqueada") != "f") {
        bloqueada->setChecked(true);
    } else {
        bloqueada->setChecked(false);
    }// end if
    if (cursorcuenta->valor("imputacion")!= "f") {
        imputacion->setChecked(true);
    } else {
        imputacion->setChecked(false);
    }// end if
    if (cursorcuenta->valor("nodebe") != "f") {
        nodebe->setChecked(true);
    } else {
        nodebe->setChecked(false);
    }// end if
    if (cursorcuenta->valor("nohaber") != "f") {
        nohaber->setChecked(true);
    } else {
        nohaber->setChecked(false);
    }// end if
    if (cursorcuenta->valor("regularizacion") != "f") {
        regularizacion->setChecked(true);
    } else {
        regularizacion->setChecked(false);
    }// end if
    // Vamos a hacer la carga del tipocuenta
    int tipocuenta = atoi(cursorcuenta->valor("tipocuenta").ascii());
    switch(tipocuenta) {
    case 0:
        cuentasintipo->setChecked(TRUE);
        break;
    case 1:
        cuentaactivo->setChecked(TRUE);
        break;
    case 2:
        cuentapasivo->setChecked(TRUE);
        break;
    case 3:
        cuentaneto->setChecked(TRUE);
        break;
    case 4:
        cuentaingreso->setChecked(TRUE);
        break;
    case 5:
        cuentagasto->setChecked(TRUE);
        break;
    }// end switch
    // Vamos a hacer la carga del grupo
    int idgrupo = atoi(cursorcuenta->valor(6).ascii());
    int i=0;
    while (idgrupos[i]!=idgrupo && i<100)
        i++;
    combogrupos->setCurrentItem(i);
    // Vamos a coger el código del padre de la cuenta para que también
    // aparezca en el formulario.
    cpadre = atoi( cursorcuenta->valor(4).ascii());
    if (cpadre != 0) {
        conexionbase->begin();
        cursorpadre = conexionbase->cargacuenta(cpadre);
        conexionbase->commit();
        cadena = cursorpadre->valor(1);
        codigopadre->setText(cadena);
        codigopadre->setReadOnly(TRUE);
        delete cursorpadre;
    }// end if
    // Vamos a hacer las inserciones pertinentes para los campos de la entidad
    nombreent->setText(cursorcuenta->valor("nombreent_cuenta"));
    cif->setText(cursorcuenta->valor("cifent_cuenta"));
    direccion->setText(cursorcuenta->valor("dirent_cuenta"));
    cp->setText(cursorcuenta->valor("cpent_cuenta"));
    telf->setText(cursorcuenta->valor("telent_cuenta"));
    coments->setText(cursorcuenta->valor("coment_cuenta"));
    banco->setText(cursorcuenta->valor("bancoent_cuenta"));
    email->setText(cursorcuenta->valor("emailent_cuenta"));
    web->setText(cursorcuenta->valor("webent_cuenta"));
    delete cursorcuenta;


    // Estamos probando la nueva forma de almacenar cambios
    dialogChanges_cargaInicial();

    return(0);
}// end cargacuenta


int cuentaview::nuevacuenta(QString codpadre, int idgrupo) {
    // Suponiendo que las cuentas son numericas, al crear una nueva cuenta
    // Buscamos entre las que seran sus hermanas y le asignamos el numero siguiente
    // que le corresponda.
    QString cpadreaux;
    QString query;
    query.sprintf("SELECT codigo FROM cuenta WHERE padre = id_cuenta('%s') ORDER BY codigo DESC",codpadre.ascii());
    cursor2 *cur = conexionbase->cargacursor(query);
    if (!cur->eof()) {
        long int valor = atol(cur->valor("codigo").ascii());
        valor ++;
        cpadreaux.setNum(valor);
        codigo->setText(cpadreaux);
    } else {
        QString codint = codpadre;
        while ((unsigned int)codint.length() < numdigitos-1) {
            codint = codint +"0";
        }// end while
        codint = codint+"0";
        codigo->setText(codint);
    }

    // Establecemos el valor del padre y del grupo.
    codigopadre->setText(codpadre);
    int i=0;
    while (idgrupos[i]!=idgrupo && i<100)
        i++;
    if (idgrupos[i] == idgrupo)
        combogrupos->setCurrentItem(i);
    return(0);
}// end inicializa

/***************************************************
 * Esta funcion se dispara cuando se ha pulsado
 * return sobre el boton del codigo                *
 ***************************************************/
void cuentaview::codigo_ret() {
    QString cod=codigo->text();
    cod = extiendecodigo(cod,numdigitos);
    codigo->setText(cod);
}// end codigo_return

void cuentaview::saveAccount() {
    fprintf(stderr,"cuentaview::aceptar()\n");
    QString codigocuenta;
    int idpadre=0;
    cursor2 *cursoraux;
    // Comprobamos que el campo codigopadre tiene un numero de cuenta y
    // calculamos el id correspondiente consultando en la base de datos.
    codigocuenta = codigopadre->text();
    if (codigocuenta!="") {
        conexionbase->begin();
        cursoraux = conexionbase->cargacuenta(0,codigocuenta);
        conexionbase->commit();
        idpadre = atoi(cursoraux->valor("idcuenta").ascii());
        delete cursoraux;
    }// end if
    // Recogemos el valor de tipocuenta
    int tipocuenta=0;
    if (cuentaactivo->isChecked())
        tipocuenta= 1;
    else if (cuentapasivo->isChecked())
        tipocuenta = 2;
    else if (cuentaneto->isChecked())
        tipocuenta = 3;
    else if (cuentaingreso->isChecked())
        tipocuenta = 4;
    else if (cuentagasto->isChecked())
        tipocuenta = 5;
    else
        tipocuenta = 0;
    // Si se trata de una modificacion modificamos y si se trata de
    // una insercion insertamos.
    if (idcuenta != 0) {
        conexionbase->begin();
        conexionbase->modificacuenta(idcuenta,
                                     conexionbase->sanearCadena(descripcion->text()) ,
                                     conexionbase->sanearCadena(codigo->text()),
                                     imputacion->isChecked(),
                                     bloqueada->isChecked(),
                                     idgrupos[combogrupos->currentItem()],
                                     TRUE,
                                     conexionbase->sanearCadena(nombreent->text()),
                                     conexionbase->sanearCadena(cif->text()),
                                     conexionbase->sanearCadena(direccion->text()),
                                     conexionbase->sanearCadena(cp->text()),
                                     conexionbase->sanearCadena(telf->text()),
                                     conexionbase->sanearCadena(coments->text()),
                                     conexionbase->sanearCadena(banco->text()),
                                     conexionbase->sanearCadena(email->text()),
                                     conexionbase->sanearCadena(web->text()),
                                     tipocuenta ,
                                     nodebe->isChecked(),
                                     nohaber->isChecked());
        conexionbase->commit();
    } else {
        conexionbase->begin();
        conexionbase->nuevacuenta(
            conexionbase->sanearCadena(descripcion->text()),
            conexionbase->sanearCadena(codigo->text()),
            idpadre,
            idgrupos[combogrupos->currentItem()],
            conexionbase->sanearCadena(nombreent->text()),
            conexionbase->sanearCadena(cif->text()),
            conexionbase->sanearCadena(direccion->text()),
            conexionbase->sanearCadena(cp->text()),
            conexionbase->sanearCadena(telf->text()),
            conexionbase->sanearCadena(coments->text()),
            conexionbase->sanearCadena(banco->text()),
            conexionbase->sanearCadena(email->text()),
            conexionbase->sanearCadena(web->text()),
            tipocuenta,
            nodebe->isChecked(),
            nohaber->isChecked() );
        QString query = "SELECT max(idcuenta) AS id from cuenta";
        cursoraux = conexionbase->cargacursor(query, "maxidcuenta");
        idcuenta = atoi(cursoraux->valor("id").ascii());
        conexionbase->commit();
        delete cursoraux;
    }// end if

    // Estamos probando la nueva forma de almacenar cambios
    dialogChanges_cargaInicial();
}// end saveAccount

void cuentaview::deleteAccount() {
    switch( QMessageBox::warning( this, "Borrar Cuenta",
                                  "Se va a borrar la Cuenta,\n"
                                  "Esto puede ocasionar pérdida de datos\n"
                                  "Tal vez debería pensarselo mejor antes\n"
                                  "porque igual su trabajo se pierde.",
                                  QMessageBox::Ok ,
                                  QMessageBox::Cancel )) {
    case QMessageBox::Ok: // Retry clicked or Enter pressed
        conexionbase->ejecuta("DELETE FROM cuenta WHERE idcuenta ="+QString::number(idcuenta));
        done(1);
        break;
    case QMessageBox::Cancel: // Abort clicked or Escape pressed
        break;
    }// end switch
}// end deleteAccount



