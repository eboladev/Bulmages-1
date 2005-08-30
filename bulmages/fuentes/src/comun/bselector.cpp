/***************************************************************************
 *   Copyright (C) 2003 by Josep Burcion                                   *
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
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include "bselector.h"



BSelector::BSelector(QWidget * parent,const char * name) : UIselector(parent,name)
{
//Al crear el selector, todos los modulos estan cerrados = NULL
    ModuloContabilidad=NULL;
    ModuloVentas=NULL; 
    ModuloCompras=NULL;
    ModuloAlmacen=NULL;
    ModuloProduccion=NULL;
    ModuloNominas=NULL;
    
    show();
    seleccionaempresa_clicked();
}


BSelector::~BSelector()
{
}

//Boton Salir
void BSelector::salir_clicked() {
ctllog->add(LOG_SEG | LOG_TRA, 1,"BslSld002","---Saliendo de la aplicación---" );
    close();
}

//Boton cambio de Empresa y/o Usuario
void BSelector::seleccionaempresa_clicked() {
    if (ModuloContabilidad || ModuloVentas || ModuloCompras || ModuloAlmacen || ModuloProduccion || ModuloNominas) 
        QMessageBox::warning(0, "Atención...", QString("Cierre todas las aplicaciones antes de cambiar de usuario o empresa."));
    else  
        (new abreempresaview("validar",true))->show(); 
}

//Boton para abrir el dialogo de configuraciones personalizadas
void BSelector::configura_clicked() {
    BConfiguracion * VentanaConfiguracion = new BConfiguracion(this, 0,"VentanaConfiguracion",true);
    VentanaConfiguracion->show();
}

//Boton para entrar en el modulo de VENTAS
void BSelector::ventas_clicked() {
//Al crear un nuevo modulo, le paso como primer parametro un puntero al selector.
//De este modo puedo acceder facilmente al selector desde el modulo.
    if (ModuloVentas==NULL) ModuloVentas = new BModVentas(this, 0,"ModuloVentas",Qt::WDestructiveClose);
    else ModuloVentas->hide(); //ocultamos para que al volver a mostrar aparezca en primer plano.
    this->hide(); //Oculta el Selector
    ModuloVentas->showMaximized(); 
}

//Boton para entrar en el modulo de COMPRAS
void BSelector::compras_clicked() {
//Al crear un nuevo modulo, le paso como primer parametro un puntero al selector.
//De este modo puedo acceder facilmente al selector desde el modulo.
    if (ModuloCompras==NULL) ModuloCompras = new BModCompras(this,0,"ModuloCompras",Qt::WDestructiveClose);
    else ModuloCompras->hide(); //ocultamos para que al volver a mostrar aparezca en primer plano.
    this->hide(); //Oculta el Selector
    ModuloCompras->showMaximized();
}


// Boton para entrar en el modulo de CONTABILIDAD 
void BSelector::contabilidad_clicked() {
//Abro Bulmages (contabilidad) con el nombre de la base de datos, Usuario y password
//que han sido introducidos al principio al cargar el selector de modulos.
    if (ModuloContabilidad==NULL) ModuloContabilidad = new BModContabilidad(this, 0,"ModuloContabilidad",Qt::WDestructiveClose);
    else ModuloContabilidad->hide(); //ocultamos para que al volver a mostrar aparezca en primer plano.
    this->hide(); //Oculta el Selector
    ModuloContabilidad->showMaximized();
}

//Boton para entrar en el modulo de PRODUCCION
void BSelector::produccion_clicked() {
//Al crear un nuevo modulo, le paso como primer parametro un puntero al selector.
//De este modo puedo acceder facilmente al selector desde el modulo.
    if (ModuloProduccion==NULL)  ModuloProduccion = new BModProduccion(this,0,"ModuloProduccion",Qt::WDestructiveClose);
    else ModuloProduccion->hide(); //ocultamos para que al volver a mostrar aparezca en primer plano.
    this->hide(); //Oculta el Selector
    ModuloProduccion->showMaximized();
}

//Boton para entrar en el modulo de STOCKS Y ALMACENES
void BSelector::almacen_clicked() {
    if (ModuloAlmacen==NULL)  ModuloAlmacen = new BModAlmacen(this,0,"ModuloAlmacen",Qt::WDestructiveClose);
    else ModuloAlmacen->hide(); //ocultamos para que al volver a mostrar aparezca en primer plano.
    this->hide(); //Oculta el Selector
    ModuloAlmacen->showMaximized();
}

//Boton para entrar en el modulo de NOMINAS
void BSelector::nominas_clicked() {
    if (ModuloNominas==NULL) ModuloNominas = new BModNominas(this,0,"ModuloNominas",Qt::WDestructiveClose);
    else ModuloNominas->hide(); //ocultamos para que al volver a mostrar aparezca en primer plano.
    this->hide(); //Oculta el Selector
    ModuloNominas->showMaximized();
}


void BSelector::closeEvent(QCloseEvent * e) {
    if (ModuloContabilidad || ModuloVentas || ModuloCompras || ModuloAlmacen || ModuloProduccion || ModuloNominas) {
        hide();
        e->ignore();
    } else {
        e->accept();
    }
}


