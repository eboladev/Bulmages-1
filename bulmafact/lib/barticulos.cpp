/***************************************************************************
 *   Copyright (C) 2004 by Josep Burcion                                   *
 *   josep@burcion.com                                                     *
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

#include "barticulos.h"
#include "bbuscador.h"

BArticulos::BArticulos(BfEmpresa* punteroEmpresaTrabajo, QWidget * parent, const char * name, WFlags f) : UIArticulos(parent, name, f) {
    EmpresaTrabajo=punteroEmpresaTrabajo;
    botonSalvar->setEnabled(FALSE);
}


BArticulos::~BArticulos(){
}

//Abre el dialogo de búsqueda
void BArticulos::iniciarBusqueda() {
    int cargar=TRUE;
    QString Retorno("1"); //columna que va a retornar en la variable retorno.
    QString tabla("articulo");
    QString* arg1 = new QString("nomarticulo");  //Columna 0
    QString* arg2 = new QString("codarticulo");  //Columna 1
    QString* arg3 = new QString("descarticulo"); //Columna 2
    QString* arg4 = new QString("idarticulo");   //Columna 3
    //Bbuscador(padre,nombreInstancia,flags,PunteroEmpresaActual,CadenaRetorno,TablaBusqueda, Campos a mostrar terminado por 0)
    if (botonSalvar->isEnabled())
        cargar = ! QMessageBox::information(this, "Atención","Desea cambiar de articulo sin salvar\nlos cambios realizados. ","Si","No");
            
    if ( cargar ) {
        (new Bbuscador(this, "buscaArt",WType_TopLevel,EmpresaTrabajo,&Retorno,&tabla, arg1,arg2,arg3,arg4,0))->exec();
        codigo_art->setText(Retorno);
        articuloReturnPressed();
    }
}

//Pulsado Boton Nuevo Articulo
//Vaciamos el formulario articulo y lo dejamos listo para dar de alta un nuevo articulo.
void BArticulos::crearNuevoArticulo() {
    QString *cadena = new QString(codigo_art->text());
    int cod = EmpresaTrabajo->nuevoArticulo(cadena);
    codigo_art->setText(*cadena);
    if (cod != -1) articuloReturnPressed();
}

//Pulsado Boton Eliminar Articulo
//Borramos al articulo actualmente visualizado en la pantalla articulos.
void BArticulos::eliminarArticulo() {
    ID_art=EmpresaTrabajo->eliminarArticulo(&codigo_art->text());
    vaciarFicha();
}

//Pulsado Boton Salvar datos Articulo
//Salvamos los datos del articulo actualmente visualizado en la pantalla articulos.
void BArticulos::salvarArticulo() {
    ID_art=EmpresaTrabajo->salvaArticulo(this);
    articuloReturnPressed();
    
}

//Pulsado return en el codigo del articulo
//Si el articulo existe cargamos los datos en pantalla.
void BArticulos::articuloReturnPressed() {
    ID_art=EmpresaTrabajo->cargaArticulo(this);
    botonSalvar->setEnabled(FALSE);
    if (ID_art == -1) vaciarFicha();
}

void BArticulos::cerrarVentana() {
    if (botonSalvar->isEnabled()) {
        if (! QMessageBox::information(this, "Atención","Desea salir sin salvar los\ncambios realizados. ","Si","No") )
            delete this;
    } else delete this;
}

void BArticulos::closeEvent(QCloseEvent * e) {
   e->ignore();
   cerrarVentana();   
}

void BArticulos::vaciarFicha() {
    codigo_art->setText("");
    nombre_art->setText("");
    cbarras_art->setText("");
}

void BArticulos::cambioEnFicha(const QString& texte) {
    botonSalvar->setEnabled(TRUE);
}


