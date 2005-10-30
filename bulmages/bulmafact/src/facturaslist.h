//
// C++ Interface: facturaslist
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FACTURASLIST_H
#define FACTURASLIST_H

#include "facturaslistbase.h"
#include "busquedacliente.h"
#include "busquedaarticulo.h"

#include <qframe.h>

/**
@author Tomeu Borras
*/

#include "company.h"

class FacturasList : public FacturasListBase
{
Q_OBJECT
private:
   company *companyact;
   int m_modo; // == 0 es modo edici�
            // ==1 es modo selector.
   QString m_idfactura;
public:
    FacturasList(QWidget *parent = 0, const char *name = 0, int flag = 0);
    FacturasList(company *,QWidget *parent = 0, const char *name = 0);
    ~FacturasList();
    void inicializa();
    void setcompany (company *comp) {companyact=comp;m_cliente->setcompany(comp);m_articulo->setcompany(comp);};
    void meteWindow(QString nom, QObject *obj) {
    if (companyact != NULL)
    companyact->meteWindow(nom, obj);
    };
    void modoseleccion() {m_modo=1;};
    void modoedicion() {m_modo=0;};
    QString idfactura() {return m_idfactura;};   
    void hideBusqueda() {m_busqueda->hide();};
    void showBusqueda() {m_busqueda->show();};   
    void hideConfiguracion() {m_configuracion->hide();};
    void showConfiguracion() {m_configuracion->show();};   
    void setidcliente(QString val) {m_cliente->setidcliente(val);};
    void setidarticulo(QString val) {m_articulo->setidarticulo(val);};     
    QString generaFiltro(); 

   /// Funciones que se encarga en guardar y cargar la configuracion del listado.
    void guardaconfig();
    void cargaconfig();

public slots:

	virtual void doubleclicked(int, int , int , const QPoint &) ;
	virtual void s_filtrar() {inicializa();}
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
