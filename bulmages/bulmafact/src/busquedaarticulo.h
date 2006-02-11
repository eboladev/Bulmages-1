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
#ifndef BUSQUEDAARTICULO_H
#define BUSQUEDAARTICULO_H

#include <busquedaarticulobase.h>
#include <QLineEdit>
#include <QLabel>

class company;
/**
@author Tomeu Borras
*/
class BusquedaArticulo : public BusquedaArticuloBase
{
Q_OBJECT
private:
company *companyact;
QString mdb_idarticulo;
QString mdb_nomarticulo;
QString mdb_codigocompletoarticulo;
public:
    BusquedaArticulo(QWidget *parent = 0, const char *name = 0);
    ~BusquedaArticulo();
    void setcompany(company *comp) {companyact = comp;};
    virtual QString codigocompletoarticulo() {return m_codigocompletoarticulo->text();};
    virtual QString idarticulo() {return mdb_idarticulo;};
    virtual QString nomarticulo() {return m_nomarticulo->text();};
    virtual void setidarticulo(QString val);
    virtual void setcodigocompletoarticulo(QString val);
public slots:
	virtual void s_searchArticulo();
	virtual void s_codigocompletoarticulotextChanged(const QString &);
signals:
	void valueChanged(QString );
};

#endif
