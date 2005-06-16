//
// C++ Interface: factura
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PEDIDOCLIENTEVIEW_H
#define PEDIDOCLIENTEVIEW_H

#include <pedidoclientebase.h>
#include "factura.h"
#include "listlinpedidoclienteview.h"
#include "busquedacliente.h"
#include "busquedafecha.h"
#include "busquedaformapago.h"
#include "busquedaalmacen.h"

#include "pedidocliente.h"
#include <qlineedit.h>
#include <qtextedit.h>
#include <qlabel.h>
#include <qcheckbox.h>

#include "postgresiface2.h"

/**
@author Tomeu Borras
*/

class PedidoClienteView : public PedidoClienteBase, public PedidoCliente {
Q_OBJECT
public:
    PedidoClienteView(company *, QWidget *parent = 0, const char *name = 0);
    ~PedidoClienteView();
    
    void generarAlbaran();
    
    void inicialize();
    void pintaidcliente(QString id) {m_cliente->setidcliente(id);};
    void pintaidalmacen(QString id) {m_almacen->setidalmacen(id);};
    
    void pintaidpedidocliente(QString) {};
    void pintanumpedidocliente(QString id) {m_numpedidocliente->setText(id);};
    
    void pintafechapedidocliente(QString id) {m_fechapedidocliente->setText(id);};
    
    void pintadescpedidocliente(QString id) {m_descpedidocliente->setText(id);};
    void pintaidforma_pago(QString id) {m_forma_pago->setidforma_pago(id);};
    void pintacomentpedidocliente(QString id) {m_comentpedidocliente->setText(id);};
    void pintarefpedidocliente(QString id) {m_refpedidocliente->setText(id);};
    void pintatotales(float base, float iva);   
    void pintaprocesadopedidocliente(QString id) {
	if (id == "t" || id == "TRUE") m_procesadopedidocliente->setChecked(TRUE);
	else m_procesadopedidocliente->setChecked(FALSE);
};    

public slots:
    virtual void s_comentpedidoclientetextChanged() { setcomentpedidocliente(m_comentpedidocliente->text());};
    virtual void s_numpedidoclientetextChanged(const QString &val) {setnumpedidocliente(val);};
    virtual void s_clientevalueChanged(QString val) {setidcliente(val);};
    virtual void s_fechapedidoclientevalueChanged(QString val) {setfechapedidocliente(val);};
    virtual void s_almacenvalueChanged(QString val) {setidalmacen(val);};
    
    virtual void s_forma_pagovalueChanged(QString val) {setidforma_pago(val);};
    virtual void s_refpedidoclientetextChanged(const QString &val) {setrefpedidocliente(val);};
    virtual void s_descpedidoclientetextChanged(const QString &val) {setdescpedidocliente(val);};

    virtual void s_savePedidoCliente() {guardaPedidoCliente();};
    virtual void cargaPedidoCliente(QString id) {PedidoCliente::cargaPedidoCliente(id);setCaption("Pedido Cliente  "+mdb_refpedidocliente);companyact->meteWindow(caption(),this);};
    
    virtual void s_deletePedidoCliente() {borraPedidoCliente();};
    virtual void s_printPedidoCliente(){imprimirPedidoCliente();};
    
    virtual void s_procesadopedidoclientestateChanged(int i) {
    	if (i) setprocesadopedidocliente("TRUE");
	else setprocesadopedidocliente("FALSE");
    };
    
     /// Este slot se activa cuando hay cambios en los subformularios.
     
    virtual void s_pintaTotales() {  
   	 pintatotales(listalineas->calculabase(), listalineas->calculaiva());
    }// end pintaTotales
    
    virtual void s_verpresupuesto();
    virtual void s_generarAlbaran() {generarAlbaran();};
    
    virtual void s_nuevoCobro();
    
};

#endif
