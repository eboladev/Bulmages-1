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
    //Para evitar que se conecte a postgres, desactiva la funcion cargaUsuario!!
    cargaUsuario();
}


BModVentas::~BModVentas() {
}

/*****************************************************************************/
/* Pide confirmaciòn antes de cerrar la aplicación                           */
/*****************************************************************************/
void BModVentas::closeEvent(QCloseEvent * e) {
  int exit=1;
  if (intentosFallidosPassword < 3) exit=QMessageBox::information(this, "Cerrar...","Realmente deseas Salir?",QMessageBox::Ok, QMessageBox::Cancel);
  if (exit==1) e->accept();
  else e->ignore();
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

//Estos SLOTS son los que usamos para abrir las distintas ventanas que forman
//la aplicación. El nombre de cada función es suficientemente explicativo.
//Estos SLOTS estan conectados a los "actions" del menú principal desde QTDesigner.
void BModVentas::abrirFormAlbaranes()      { (new BAlbaVenta(empresaTrabajo, zona0,"albaran"))->show(); }
void BModVentas::abrirFormPresupuestos()   { (new Bpresupuesto(empresaTrabajo, zona0,"presupuesto"))->show(); }
void BModVentas::abrirFormPedidos()        { (new BPediVenta(empresaTrabajo, zona0,"pedidoVenta"))->show(); }
void BModVentas::abrirFormClientes()       { (new BClientes(empresaTrabajo, zona0,"cliente"))->showMaximized(); }
void BModVentas::abrirFormProveedores()    { (new Bproveedor(empresaTrabajo, zona0,"proveedor"))->show(); }
void BModVentas::abrirFormListadoProveed() { (new BProvList(empresaTrabajo, zona0,"proveedlist"))->show(); }
void BModVentas::abrirFormArticulos()      { (new BArticulos(empresaTrabajo, zona0,"articulo"))->show(); }
void BModVentas::abrirFormMarcas()         { (new Bmarca(empresaTrabajo, zona0,"marca"))->show(); }
void BModVentas::abrirFormTipos_iva()      { (new Btipo_iva(empresaTrabajo, zona0,"tipo_iva"))->show(); }
void BModVentas::abrirFormFamilias()       { (new Bfamilia(empresaTrabajo, zona0,"familia"))->show(); }
void BModVentas::abrirFormCatalogos()      { (new Bcatalogo(empresaTrabajo, zona0,"catalogo"))->show(); }
void BModVentas::abrirFormDivisiones()     { (new BProvDiv(empresaTrabajo, zona0,"ProvDiv"))->show(); }
void BModVentas::abrirFormSucursales()     { (new Bsucursal(empresaTrabajo, zona0,"sucursal"))->show(); }
void BModVentas::abrirFormAlbaranes_prov() { (new BAlbaProv(empresaTrabajo, zona0,"alba_pro"))->show(); }
void BModVentas::abrirFormPedidos_prov()   { (new BPediCompra(empresaTrabajo, zona0,"pedi_compra"))->show(); }
void BModVentas::abrirFormListAlbProv()    { (new BAlbProList(empresaTrabajo, zona0,"albprolist"))->show(); }
void BModVentas::abrirFormListPediProv()   { (new BPediComList(empresaTrabajo, zona0,"pedicomlist"))->show(); }
void BModVentas::abrirFormModalidades_g()  { (new Bmodalidad_g(empresaTrabajo, zona0,"modalidad_g"))->show(); }
void BModVentas::abrirFormCond_garantia()  { (new Bcond_garantia(empresaTrabajo, zona0,"cond_garantia"))->show(); }
void BModVentas::abrirFormCondiciones_v()  { (new Bcondiciones_v(empresaTrabajo, zona0,"condiciones_v"))->show(); }
void BModVentas::abrirFormContratosCompra(){ (new BContrCompra(empresaTrabajo, zona0,"contratos_compra"))->show(); }
void BModVentas::abrirFormOfertas()        { (new Boferta(empresaTrabajo, zona0,"oferta"))->show(); }
void BModVentas::abrirFormFormas_pago()    { (new Bforma_pago(empresaTrabajo, zona0,"forma_pago"))->show(); }
void BModVentas::abrirFormTerminios_fp()   { (new Btermino_fp(empresaTrabajo, zona0,"termino_fp"))->show(); }
void BModVentas::abrirFormAlmacenes()      { (new Balmacen(empresaTrabajo, zona0,"almacen"))->show(); }
void BModVentas::abrirFormAbout()          { (new BAbout(empresaTrabajo, zona0,"acerca_de"))->show(); }

//Emite una señal que se puede conectar a un SLOT.
void BModVentas::mostrar_selector() { emit clickEnlace(); }

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
