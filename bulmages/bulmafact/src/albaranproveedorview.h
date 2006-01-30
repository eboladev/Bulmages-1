/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borr� Riera                              *
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
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef ALBARANPROVEEDORVIEW_H
#define ALBARANPROVEEDORVIEW_H

// ALBARANES.
#include <qlineedit.h>
#include <q3textedit.h>
#include <qlabel.h>
#include "busquedaproveedor.h"
#include "busquedafecha.h"
#include "busquedaformapago.h"
#include "busquedaalmacen.h"
#include "albaranproveedorbase.h"
#include "albaranproveedor.h"
#include "postgresiface2.h"
#include "dialogchanges.h"

/** @author Tomeu Borras� & Alvaro de Miguel */
class company;


class AlbaranProveedorView : public AlbaranProveedorBase, public AlbaranProveedor, public dialogChanges {
Q_OBJECT
	
public:
	AlbaranProveedorView(company *, QWidget *, const char *);
	~AlbaranProveedorView();
	void inicialize();
	
	
    void pintaidalbaranp(QString) {};
    void pintanumalbaranp(QString val) {m_numalbaranp->setText(val);};
    void pintafechaalbaranp(QString val) {m_fechaalbaranp->setText(val);};
    void pintaIdUsuario(QString) {};
    void pintaComentAlbaran(QString val) {m_comentalbaranp->setText(val);};
    void pintaidproveedor(QString val) {m_proveedor->setidproveedor(val);};
    void pintaidforma_pago(QString val) {m_forma_pago->setidforma_pago(val);};
//    void pintaIdFactura(QString){};
    void pintaidalmacen(QString id){m_almacen->setidalmacen(id);};

    void pintadescalbaranp(QString val) {m_descalbaranp->setText(val);};
    void pintarefalbaranp(QString val) {m_refalbaranp->setText(val);};
//    void pintaNumFactura(QString) {};
    
    void pintatotales(float, float);	
    void generarFactura();

	void closeEvent( QCloseEvent *);

public slots:
    virtual void s_comentalbaranptextChanged() { setcomentalbaranp(m_comentalbaranp->text());};
    virtual void s_almacenvalueChanged(QString val) {setidalmacen(val);};
    virtual void s_numalbaranptextChanged(const QString &val) {setnumalbaranp(val);};
    virtual void s_proveedorvalueChanged(QString val) {setidproveedor(val);};
    
    virtual void s_fechaalbaranpvalueChanged(QString val) {
    fprintf(stderr,"s_fechaalbaranpvalueChanged()\n");
    setfechaalbaranp(val);};
    virtual void s_forma_pagovalueChanged(QString val) {setidforma_pago(val);};
    virtual void s_refalbaranptextChanged(const QString &val) {setrefalbaranp(val);};
    virtual void s_descalbaranptextChanged(const QString &val) {setdescalbaranp(val);};
    virtual void s_saveAlbaranProveedor() {guardaAlbaranProveedor();};
    virtual int cargaAlbaranProveedor(QString id);
    virtual void s_deleteAlbaranProveedor() {borraAlbaranProveedor();};
    virtual void s_printAlbaranProveedor(){};
    
     /// Este slot se activa cuando hay cambios en los subformularios.
    virtual void s_pintaTotales() {  
   	 pintatotales(listalineas->calculabase(), listalineas->calculaiva());
    }// end pintaTotales     
     
    virtual void s_verpedidoproveedor();
    virtual void s_imprimirAlbaranProveedor() {imprimirAlbaranProveedor();};
    virtual void s_generarFactura() {generarFactura();};
};

#endif
