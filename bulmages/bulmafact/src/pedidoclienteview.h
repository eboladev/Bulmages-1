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
private:
    cursor2 *m_cursorcombo;
public:
    PedidoClienteView(company *, QWidget *parent = 0, const char *name = 0);
    ~PedidoClienteView();
    void inicialize();
    void pintaidcliente(QString id) {};
    void pintaidalmacen(QString id) {};
    void pintacodigoalmacen(QString id) {m_codigoalmacen-> setText(id);}; 
    void pintaidpedidocliente(QString id) {};
    void pintanumpedidocliente(QString id) {m_numpedidocliente->setText(id);};
    void pintafechapedidocliente(QString id) {m_fechapedidocliente->setText(id);};
    void pintadescpedidocliente(QString id) {m_descpedidocliente->setText(id);};
    void pintanomcliente(QString id) {m_nomcliente->setText(id);};
    void pintanomalmacen(QString id) {m_nomalmacen-> setText(id);};
    void pintaidforma_pago(QString id);
    void pintacifcliente(QString id) {m_cifcliente->setText(id);};   
    void pintacomentpedidocliente(QString id) {m_comentpedidocliente->setText(id);};
    void pintarefpedidocliente(QString id) {m_refpedidocliente->setText(id);};
    void pintatotales(float base, float iva);   
    void pintaprocesadopedidocliente(QString id) {
	if (id == "t" || id == "TRUE") m_procesadopedidocliente->setChecked(TRUE);
	else m_procesadopedidocliente->setChecked(FALSE);
};    

public slots:
    virtual void s_comentpedidoclientetextChanged() { setcomentpedidocliente(m_comentpedidocliente->text());};
    virtual void s_codigoalmacentextChanged(const QString &val) {setcodigoalmacen(val);};
    virtual void s_numpedidoclientetextChanged(const QString &val) {setnumpedidocliente(val);};
    virtual void s_cifclienttextChanged(const QString &val) {setcifcliente(val);};
    virtual void s_fechapedidoclientetextChanged(const QString &val) {setfechapedidocliente(val);};
    virtual void s_formapagoactivated(int a) {setidforma_pago(m_cursorcombo->valor("idforma_pago",a));};
    virtual void s_refpedidoclientetextChanged(const QString &val) {setrefpedidocliente(val);};
    virtual void s_descpedidoclientetextChanged(const QString &val) {setdescpedidocliente(val);};

    virtual void s_savePedidoCliente() {guardaPedidoCliente();};
    virtual void s_deletePedidoCliente() {borraPedidoCliente();};
    virtual void s_printPedidoCliente(){};
    virtual void s_procesadopedidoclientestateChanged(int i) {
    	if (i) setprocesadopedidocliente("TRUE");
	else setprocesadopedidocliente("FALSE");
    };
    
    virtual void s_searchClient();
     /// Este slot se activa cuando hay cambios en los subformularios.
     
    virtual void s_pintaTotales() {  
   	 pintatotales(listalineas->calculabase(), listalineas->calculaiva());
    }// end pintaTotales
};

#endif
