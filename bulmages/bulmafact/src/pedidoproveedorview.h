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
#ifndef PEDIDOPROVEEDORVIEW_H
#define PEDIDOPROVEEDORVIEW_H

#include <pedidoproveedorbase.h>
#include "factura.h"
#include "listlinpedidoproveedorview.h"
#include "listdescpedidoproveedorview.h"
#include "busquedaproveedor.h"
#include "busquedafecha.h"
#include "busquedaformapago.h"
#include "busquedaalmacen.h"
#include "busquedatrabajador.h"
#include "dialogchanges.h"
#include "fixed.h"


#include "pedidoproveedor.h"
#include <QLineEdit>
#include <Q3TextEdit>
#include <QLabel>
#include <QCheckBox>

#include "postgresiface2.h"

/**
@author Tomeu Borras
*/

class PedidoProveedorView : public PedidoProveedorBase, public PedidoProveedor, public dialogChanges {
Q_OBJECT
public:
    PedidoProveedorView(company *, QWidget *parent = 0, const char *name = 0);
    ~PedidoProveedorView();

    void closeEvent( QCloseEvent *);

    void generarAlbaran();
    
    void inicialize();
    void pintaidproveedor(QString id) {m_proveedor->setidproveedor(id);};
    void pintaidalmacen(QString id) {m_almacen->setidalmacen(id);};
    
    void pintaidpedidoproveedor(QString) {};
    void pintanumpedidoproveedor(QString id) {m_numpedidoproveedor->setText(id);};
    
    void pintafechapedidoproveedor(QString id) {m_fechapedidoproveedor->setText(id);};
    
    void pintadescpedidoproveedor(QString id) {m_descpedidoproveedor->setText(id);};
    void pintaidforma_pago(QString id) {m_forma_pago->setidforma_pago(id);};
    void pintaidtrabajador(QString id) {m_trabajador->setidtrabajador(id);};
    void pintacomentpedidoproveedor(QString id) {m_comentpedidoproveedor->setText(id);};
    void pintarefpedidoproveedor(QString id) {m_refpedidoproveedor->setText(id);};
    void pintacontactpedidoproveedor(QString id) {m_contactpedidoproveedor->setText(id);};
    void pintatelpedidoproveedor(QString id) {m_telpedidoproveedor->setText(id);};
    
    
    void pintatotales(Fixed iva, Fixed base, Fixed total, Fixed desc);   
    void pintaprocesadopedidoproveedor(QString id) {
	if (id == "t" || id == "TRUE") m_procesadopedidoproveedor->setChecked(TRUE);
	else m_procesadopedidoproveedor->setChecked(FALSE);
};    



    
public slots:
    virtual bool close(bool);
    virtual void s_comentpedidoproveedortextChanged() { setcomentpedidoproveedor(m_comentpedidoproveedor->text());};
    virtual void s_numpedidoproveedortextChanged(const QString &val) {setnumpedidoproveedor(val);};
    virtual void s_proveedorvalueChanged(QString val) {setidproveedor(val);};
    virtual void s_fechapedidoproveedorvalueChanged(QString val) {setfechapedidoproveedor(val);};
    virtual void s_almacenvalueChanged(QString val) {setidalmacen(val);};
    virtual void s_trabajadorvalueChanged(QString val) {setidtrabajador(val);};
    
    virtual void s_forma_pagovalueChanged(QString val) {setidforma_pago(val);};
    virtual void s_refpedidoproveedortextChanged(const QString &val) {setrefpedidoproveedor(val);};
    virtual void s_descpedidoproveedortextChanged(const QString &val) {setdescpedidoproveedor(val);};

    virtual void s_savePedidoProveedor() {guardaPedidoProveedor();};
    virtual int cargaPedidoProveedor(QString id);
    
    virtual void s_deletePedidoProveedor() {borraPedidoProveedor();};
    virtual void s_printPedidoProveedor(){imprimirPedidoProveedor();};
    
    virtual void s_contactpedidoproveedortextChanged(const QString &val) {setcontactpedidoproveedor(val);};
    virtual void s_telpedidoproveedortextChanged(const QString &val) {settelpedidoproveedor(val);};
    
    virtual void s_procesadopedidoproveedorstateChanged(int i) {
    	if (i) setprocesadopedidoproveedor("TRUE");
	else setprocesadopedidoproveedor("FALSE");
    };
    
     /// Este slot se activa cuando hay cambios en los subformularios.
     
    virtual void s_pintaTotales() {  
   	 calculaypintatotales();
    }// end pintaTotales
    
    virtual void s_verpresupuesto();
    virtual void s_generarAlbaran() {generarAlbaran();};
    
    virtual void s_nuevoCobro();
    
};

#endif
