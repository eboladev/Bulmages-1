//
// C++ Interface: busquedaproveedor
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BUSQUEDAPROVEEDOR_H
#define BUSQUEDAPROVEEDOR_H

#include <busquedaproveedorbase.h>
#include <qlineedit.h>
#include <qlabel.h>

class company;
/**
@author Tomeu Borras
*/
class BusquedaProveedor : public BusquedaProveedorBase
{
Q_OBJECT
private:
company *companyact;
QString mdb_idproveedor;
QString mdb_nomproveedor;
QString mdb_cifproveedor;
public:
    BusquedaProveedor(QWidget *parent = 0, const char *name = 0);
    ~BusquedaProveedor();
    void setcompany(company *comp) {companyact = comp;};
//    virtual QString text()  {return m_cifproveedor->text();};
    virtual QString cifproveedor() {return m_cifproveedor->text();};
    virtual QString idproveedor() {return mdb_idproveedor;};
    virtual QString nomproveedor() {return nomproveedor();};
    virtual void setidproveedor(QString val);
    virtual void setcifproveedor(QString val);
public slots:
	virtual void s_searchProveedor();
	virtual void s_cifproveedortextChanged(const QString &);
signals:
	void valueChanged(QString );
};

#endif
