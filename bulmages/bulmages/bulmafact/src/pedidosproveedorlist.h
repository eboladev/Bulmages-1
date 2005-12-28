//
// C++ Interface: pedidosproveedorlist
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PEDIDOSPROVEEDORLIST_H
#define PEDIDOSPROVEEDORLIST_H

#include <pedidosproveedorlistbase.h>
#include "busquedaproveedor.h"
#include "busquedaarticulo.h"
#include "company.h"
/**
@author Tomeu Borras
*/

class PedidosProveedorList : public PedidosProveedorListBase
{
Q_OBJECT
public:
   company *companyact;
   int m_modo; // == 0 es modo edici�
            // ==1 es modo selector.
   QString m_idpedidoproveedor;

    PedidosProveedorList(QWidget *parent = 0, const char *name = 0, Qt::WFlags flag = 0);
    PedidosProveedorList(company *, QWidget *parent = 0, const char *name = 0, Qt::WFlags flag = 0);

    ~PedidosProveedorList();
    void inicializa();
    void modoseleccion() {m_modo=1;};
    void modoedicion() {m_modo=0;};
    void imprimir();
    void setcompany (company *comp) {companyact=comp;m_proveedor->setcompany(comp);};
    void hideBotonera() {m_botonera->hide();};
    void showBotonera() {m_botonera->show();};
    void hideBusqueda() {fprintf(stderr,"Ocultar busqueda\n");m_busqueda->hide();};
    void showBusqueda() {m_busqueda->show();};    
    void hideConfiguracion() {m_configuracion->hide();};
    void showConfiguracion() {m_configuracion->show();};    
    QString idpedidoproveedor() {return m_idpedidoproveedor;}; 
    void setidproveedor(QString val) {m_proveedor->setidproveedor(val);};  
        void meteWindow(QString nom, QObject *obj) {
    if (companyact != NULL)
    companyact->meteWindow(nom, obj);
    };  
    QString generarFiltro();
   /// Funciones que se encarga en guardar y cargar la configuracion del listado.
    void guardaconfig();
    void cargaconfig();
public slots:
	virtual void doubleclicked(int, int , int , const QPoint &) ;  
	virtual void s_printPedidosProveedor() {imprimir();};
	virtual void s_searchPedidosProveedor() {inicializa();};
	virtual void s_newPedidoProveedor() {companyact->s_newPedidoPro();};
	virtual void s_filtrar() {inicializa();};
	
	virtual void s_borrarPedidosProveedor();
	virtual void s_editarPedidosProveedor();
	
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
