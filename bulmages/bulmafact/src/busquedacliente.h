//
// C++ Interface: busquedacliente
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BUSQUEDACLIENTE_H
#define BUSQUEDACLIENTE_H

#include <busquedaclientebase.h>
#include <QLineEdit>
#include <QLabel>

class company;
/**
@author Tomeu Borras
*/
class BusquedaCliente : public BusquedaClienteBase
{
Q_OBJECT
private:
company *companyact;
QString mdb_idcliente;
QString mdb_nomcliente;
QString mdb_cifcliente;
public:
    BusquedaCliente(QWidget *parent = 0, const char *name = 0);
    ~BusquedaCliente();
    void setcompany(company *comp) {companyact = comp;};
    virtual QString cifcliente() {return m_cifcliente->text();};
    virtual QString idcliente() {return mdb_idcliente;};
    virtual QString nomcliente() {return nomcliente();};
    virtual void setidcliente(QString val);
    virtual void setcifcliente(QString val);
public slots:
	virtual void s_searchCliente();
	virtual void s_cifclientetextChanged(const QString &);
signals:
	void valueChanged(QString );
};

#endif
