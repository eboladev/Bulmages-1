//
// C++ Interface: facturapslist
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FACTURASPROVEEDORLIST_H
#define FACTURASPROVEEDORLIST_H

#include "facturasplistbase.h"
#include "busquedaproveedor.h"
#include "busquedaarticulo.h"

#include <qframe.h>

/**
@author Tomeu Borras
*/

#include "company.h"

class FacturasProveedorList : public FacturasProveedorListBase {
    Q_OBJECT
private:
    company *companyact;
    int m_modo; // == 0 es modo edición
    // ==1 es modo selector.
    QString m_idfacturap;
public:
    FacturasProveedorList(QWidget *parent = 0, const char *name = 0, int flag = 0);
    FacturasProveedorList(company *,QWidget *parent = 0, const char *name = 0);
    ~FacturasProveedorList();
    void inicializa();
    void setcompany (company *comp) {
        companyact=comp;
        m_proveedor->setcompany(comp);
        m_articulo->setcompany(comp);
    };
    void meteWindow(QString nom, QObject *obj) {
        if (companyact != NULL)
            companyact->meteWindow(nom, obj);
    };
    void modoseleccion() {
        m_modo=1;
    };
    void modoedicion() {
        m_modo=0;
    };
    QString idfacturap() {
        return m_idfacturap;
    };
    void hideBusqueda() {
        m_busqueda->hide();
    };
    void showBusqueda() {
        m_busqueda->show();
    };
    void hideConfiguracion() {
        m_configuracion->hide();
    };
    void showConfiguracion() {
        m_configuracion->show();
    };
    void hideBotonera() {
       m_botonera->hide();
    };
    void showBotonera() {
       m_botonera->show();
    };
    void setidproveedor(QString val) {
        m_proveedor->setidproveedor(val);
    };
    void setidarticulo(QString val) {
        m_articulo->setidarticulo(val);
    };

    QString generaFiltro();
public slots:

    virtual void doubleclicked(int, int , int , const QPoint &) ;
    virtual void s_filtrar() {
        inicializa();
    };
    virtual void s_nuevaFacturaProveedor();
    virtual void s_editarFacturaProveedor();
    virtual void s_borrarFacturaProveedor();
    virtual void s_actualizarFacturaProveedor() {inicializa();};
    virtual void s_mostrarBusqueda() {
        fprintf(stderr,"s_mostrarBusqueda\n");
        if (m_busqueda->isVisible())
            hideBusqueda();
        else
            showBusqueda();
    }
    ;
    virtual void s_mostrarConfiguracion() {
        fprintf(stderr,"s_mostrarConfiguracion\n");
        if (m_configuracion->isVisible())
            hideConfiguracion();
        else
            showConfiguracion();
    }
    ;
};

#endif
