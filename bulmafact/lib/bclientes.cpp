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
#include "bclientes.h"
#include "bbuscador.h"

BClientes::BClientes(BfEmpresa* punteroEmpresaTrabajo, QWidget * parent, const char * name) : UIClientes(parent, name) {
    EmpresaTrabajo=punteroEmpresaTrabajo;
}


BClientes::~BClientes(){
}

//Abre el dialogo de búsqueda
void BClientes::iniciarBusqueda() {
    QString Retorno("3"); //columna que va a retornar en la variable retorno.
    QString tabla("cliente");
    QString* arg1 = new QString("nomcliente");  //Columna 0
    QString* arg2 = new QString("poblcliente"); //Col. 1
    QString* arg3 = new QString("telcliente");  //Col. 2
    QString* arg4 = new QString("idcliente");   //Col. 3
    (new Bbuscador(this, "buscaCli",WType_TopLevel,EmpresaTrabajo,&Retorno,&tabla, arg1,arg2,arg3,arg4,0))->exec();
    codigo_cli->setText(Retorno);
    clienteReturnPressed();
}

//Pulsado Boton Nuevo Cliente
//Vaciamos el formulario clientes y lo dejamos listo para dar de alta un nuevo cliente.
void BClientes::crearNuevoCliente() {
    int codi=EmpresaTrabajo->nuevoCliente(NULL);
    QString codi_s;
    codi_s.setNum(codi,10);
    codigo_cli->setText(codi_s);
}

//Pulsado Boton Eliminar Cliente
//Borramos al cliente actualmente visualizado en la pantalla clientes.
void BClientes::eliminarCliente() {
    int codi=EmpresaTrabajo->eliminarCliente(&codigo_cli->text());
}

//Pulsado Boton Salvar datos Cliente
//Salvamos los datos del cliente actualmente visualizado en la pantalla clientes.
void BClientes::salvarCliente() {
    int codi=EmpresaTrabajo->salvaCliente(this);
}

//Pulsado return en el codigo del cliente
//Si el cliente existe cargamos los datos en pantalla.
void BClientes::clienteReturnPressed() {
    int codi=EmpresaTrabajo->cargaCliente(this);
}

void BClientes::cerrarVentana() {
    delete this;
}

void BClientes::closeEvent(QCloseEvent * e) {
   e->ignore();
   cerrarVentana();    
}
