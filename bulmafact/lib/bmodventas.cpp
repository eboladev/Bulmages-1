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
 ***************************************************************************/
#include "bmodventas.h"

#include <qmessagebox.h>

//BModVentas::BModVentas(QWidget * ref, QWidget * parent, const char * name, WFlags f)
BModVentas::BModVentas(QString* usuario, QString* passwd, QString* dataBase, QWidget * parent, const char * name, WFlags f)
 : UIVentas(parent,name,f)
{
    //PunteroAlSelector=ref;
    empresaTrabajo = new BfEmpresa();
    QVBox* vb = new QVBox( this );
    vb->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    zona0 = new QWorkspace (vb);
    zona0->setScrollBarsEnabled( TRUE );
    setCentralWidget( vb );  
    Usuario = usuario;
    if (Usuario==NULL) Usuario = new QString(""); //NOMBRE_USUARIO_SIN_DETERMINAR
    Password = passwd;
    if (Password==NULL) Password = new QString(""); //CONTRASEÑA_SIN_DETERMINAR
    DataBase = dataBase;
    if (DataBase == NULL) DataBase = new QString(""); //NOMBRE_DE_LA_BASE_DE_DATOS_SIN_DETERMINAR
    intentosFallidosPassword=0;
    cargaUsuario();
}


BModVentas::~BModVentas() {
}

/*****************************************************************************/
/* Muestra el dialogo para entrar el nombre de usuario y la contraseña       */
/*****************************************************************************/
void BModVentas::cargaUsuario(){
    while (seleccionaEmpresa() && (intentosFallidosPassword++ < 3)) {
        (new BPasswd(Usuario, Password,this,"userBox",TRUE,0))->exec();
        if (*Usuario=="") intentosFallidosPassword=10;
    }
 if (intentosFallidosPassword > 3) close();
 }

/*****************************************************************************/
/* Comprueba en que empresas puede entrar el usuario y las muestra.          */
/*****************************************************************************/
int BModVentas::seleccionaEmpresa(){
    BfCursor* cursorEmpresas=empresaTrabajo->pg_database(Usuario, Password);
    if (cursorEmpresas) {
        if (*DataBase!= "") { 
            while (!cursorEmpresas->eof() && *DataBase !=cursorEmpresas->valor(0)) cursorEmpresas->siguienteregistro();
            if (!cursorEmpresas->eof()) cursorEmpresas->primerregistro();
            else cursorEmpresas=NULL;
        } else {
            (new BVisorEmpresas(DataBase,cursorEmpresas,this,"VisorEmpresas"))->exec();
            if (*DataBase=="") cursorEmpresas=NULL;
        }
    }
    
    if (cursorEmpresas==NULL) return 1; //El usuario no tiene acceso a ninguna base de datos.
    
    setCaption(QString("BulmaFact - ") + DataBase->ascii()); //debug....
    empresaTrabajo->cargaEmpresa(DataBase);
    return 0;
}

//Abro la ficha de mantenimiento de los clientes
void BModVentas::fichaClientes() {
    (new BClientes(empresaTrabajo, zona0,"cliente"))->show();
}

//Abro la ficha de mantenimiento de los articulos
void BModVentas::fichaArticulos() {
    (new BArticulos(empresaTrabajo, zona0,"articulo"))->show();
}

//Abro la ventana de los albaranes de Venta.
void BModVentas::albaranes() {
    (new BAlbaVenta(empresaTrabajo, zona0,"albaran"))->show();
}

//Abro la ventana de los presupuestos de Venta.
void BModVentas::presupuestos() {
    (new Bpresupuesto(empresaTrabajo, zona0,"presupuesto"))->show();
}

//Abro la ventana de los pedidos de Venta.
void BModVentas::pedidos() {
    (new BPediVenta(empresaTrabajo, zona0,"pedido"))->show();
}

//Emite una señal que se puede conectar a un SLOT
void BModVentas::mostrar_selector() {
    emit clickEnlace();
}

/********************************************************************************************/

/********************************************************************************************/
/* Punto de unión de la libreria libbulmafact.so con el mundo exterior.                     */
/********************************************************************************************/
extern "C" {
    BModVentas* enlace (QWorkspace * parent, int modo) {
        BModVentas * bulmafact=0;
        switch (modo) {
            case 0: //Modo entorno ventanas independientes
                bulmafact = new BModVentas(NULL,NULL,NULL,parent,"ModuloVentas", Qt::WDestructiveClose); break;
            case 1: //Modo entorno MDI-CHILD
                bulmafact = new BModVentas(NULL,NULL,NULL,parent, "ModuloVentas",Qt::WStyle_Customize | Qt::WStyle_NoBorder | Qt::WDestructiveClose); break;
        }
        return bulmafact;
    }
}
