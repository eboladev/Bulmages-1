/***************************************************************************
 *   Copyright (C) 2005 by Alvaro de Miguel                                *
 *   alvaro.demiguel@gmail.com                                             *
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
#ifndef ALBARANCLIENTEVIEW_H
#define ALBARANCLIENTEVIEW_H

// ALBARANES.
#include <qlineedit.h>
#include <qtextedit.h>
#include <qlabel.h>
#include "listdescalbarancliente.h"
#include "dialogchanges.h"

#include "fixed.h"

#include "busquedacliente.h"
#include "busquedafecha.h"
#include "busquedaformapago.h"
#include "busquedaalmacen.h"
#include "busquedatrabajador.h"
#include "albaranclientebase.h"
#include "albarancliente.h"
#include "postgresiface2.h"
#include <qcheckbox.h>

/** @author Tomeu Borr� & Alvaro de Miguel */
class company;


class AlbaranClienteView : public AlbaranClienteBase, public AlbaranCliente, public dialogChanges {
Q_OBJECT
	
public:
	AlbaranClienteView(company *, QWidget *, const char *);
	~AlbaranClienteView();
	void inicialize();

	
    void pintaIdAlbaran(QString) {};
    void pintaNumAlbaran(QString val) {m_numalbaran->setText(val);};
    void pintafechaalbaran(QString val) {m_fechaalbaran->setText(val);};
    void pintaIdUsuario(QString) {};
    void pintaComentAlbaran(QString val) {m_comentalbaran->setText(val);};
    void pintaComentPrivAlbaran(QString val) {m_comentprivalbaran->setText(val);};
    void pintaidcliente(QString val) {m_cliente->setidcliente(val);};
    void pintaidforma_pago(QString val) {m_forma_pago->setidforma_pago(val);};
    void pintaIdFactura(QString){};
    void pintaidalmacen(QString id){m_almacen->setidalmacen(id);};
    void pintaidtrabajador(QString id) {m_trabajador->setidtrabajador(id);};

    void pintadescalbaran(QString val) {m_descalbaran->setText(val);};
    void pintarefalbaran(QString val) {m_refalbaran->setText(val);};
    void pintacontactalbaran(QString val) {m_contactalbaran->setText(val);};
    void pintatelalbaran(QString val) {m_telalbaran->setText(val);};
    
void    pintaprocesadoalbaran(QString id) {
	if (id == "t" || id == "TRUE") m_procesadoalbaran->setChecked(TRUE);
	else m_procesadoalbaran->setChecked(FALSE);
};    
    
    
    void pintaNumFactura(QString) {};
    
    void pintatotales(Fixed, Fixed, Fixed, Fixed);	
    void generarFactura();
    void agregarFactura();

     
public slots:
	    virtual bool close(bool);
    virtual void s_comentalbarantextChanged() { setcomentalbaran(m_comentalbaran->text());};
    virtual void s_comentprivalbarantextChanged() {setcomentprivalbaran(m_comentprivalbaran->text());};
    virtual void s_almacenvalueChanged(QString val) {setidalmacen(val);};
    virtual void s_numalbarantextChanged(const QString &val) {setNumAlbaran(val);};
    virtual void s_clientevalueChanged(QString val) {setidcliente(val);};
    
    virtual void s_procesadoalbaranstateChanged(int val) {
        if (val) setprocesadoalbaran("TRUE");
	else setprocesadoalbaran("FALSE");
    } 
    
    virtual void s_contactalbarantextChanged(const QString &val){setcontactalbaran(val);};
    
    virtual void s_telalbarantextChanged(const QString &val){settelalbaran(val);};
    
    
    virtual void s_fechaalbaranvalueChanged(QString val) {
    fprintf(stderr,"s_fechaalbaranvalueChanged()\n");
    setfechaalbaran(val);};
    virtual void s_forma_pagovalueChanged(QString val) {setidforma_pago(val);};
    virtual void s_trabajadorvalueChanged(QString val) {setidtrabajador(val);};
    virtual void s_refalbarantextChanged(const QString &val) {setrefalbaran(val);};
    virtual void s_descalbarantextChanged(const QString &val) {setdescalbaran(val);};
    virtual void s_saveAlbaranCliente() {guardaAlbaranCliente();};
    virtual void cargaAlbaranCliente(QString id);    
    virtual void s_deleteAlbaranCliente() {borraAlbaranCliente();};
    virtual void s_printAlbaranCliente(){};
    
     /// Este slot se activa cuando hay cambios en los subformularios.
    virtual void s_pintaTotales() {  
   	 calculaypintatotales();
    }// end pintaTotales     
     
    virtual void s_verpresupuesto();
    virtual void s_verpedidocliente();
    virtual void s_imprimirAlbaranCliente() {imprimirAlbaranCliente();};
    virtual void s_generarFactura() {generarFactura();};
    virtual void s_informeReferencia();
    virtual void s_agregarFactura() {agregarFactura();};
};

#endif
