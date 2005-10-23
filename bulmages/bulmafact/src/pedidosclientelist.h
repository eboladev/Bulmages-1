//
// C++ Interface: pedidosclientelist
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PEDIDOSCLIENTELIST_H
#define PEDIDOSCLIENTELIST_H

#include <pedidosclientelistbase.h>
#include "busquedacliente.h"
#include "busquedaarticulo.h"
#include "company.h"
/**
@author Tomeu Borras
*/

class PedidosClienteList : public PedidosClienteListBase
{
Q_OBJECT
public:
   company *companyact;
   int m_modo; // == 0 es modo edici�
            // ==1 es modo selector.
   QString m_idpedidocliente;

    PedidosClienteList(QWidget *parent = 0, const char *name = 0, int flag = 0);
    PedidosClienteList(company *, QWidget *parent = 0, const char *name = 0, int flag = 0);

    ~PedidosClienteList();
    void inicializa();
    void modoseleccion() {m_modo=1;};
    void modoedicion() {m_modo=0;};
    void imprimir();
    void setcompany (company *comp) {companyact=comp;m_cliente->setcompany(comp);};
    void hideBotonera() {m_botonera->hide();};
    void showBotonera() {m_botonera->show();};
    void hideBusqueda() {fprintf(stderr,"Ocultar busqueda\n");m_busqueda->hide();};
    void showBusqueda() {m_busqueda->show();};    
    void hideConfiguracion() {m_configuracion->hide();};
    void showConfiguracion() {m_configuracion->show();};    
    QString idpedidocliente() {return m_idpedidocliente;}; 
    void setidcliente(QString val) {m_cliente->setidcliente(val);};  
        void meteWindow(QString nom, QObject *obj) {
    if (companyact != NULL)
    companyact->meteWindow(nom, obj);
    };  
    QString generarFiltro();
public slots:
	virtual void doubleclicked(int, int , int , const QPoint &) ;  
	virtual void s_printPedidosCliente() {imprimir();};
	virtual void s_searchPedidosCliente() {inicializa();};
	virtual void s_newPedidoCliente() {companyact->newPedidoCliente();};
	virtual void s_filtrar() {inicializa();};
	
	virtual void s_borrarPedidosCliente();
	virtual void s_editarPedidosCliente();
	
    virtual void s_mostrarBusqueda() {
    	fprintf(stderr,"s_mostrarBusqueda\n");
    	if (m_busqueda->isVisible())
		hideBusqueda();
	else
		 showBusqueda();
	};
	
    virtual void s_mostrarConfiguracion() {
    	fprintf(stderr,"s_mostrarConfiguracion\n");
    	if (m_configuracion->isVisible())
		hideConfiguracion();
	else
		 showConfiguracion();
	};

	virtual void s_configurar();		
};

#endif
