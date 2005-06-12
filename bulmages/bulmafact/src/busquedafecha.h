//
// C++ Interface: busquedafecha
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BUSQUEDAFECHA_H
#define BUSQUEDAFECHA_H

#include <busquedafechabase.h>
#include <company.h>
#include <qlineedit.h>

/**
@author Tomeu Borras
*/
class BusquedaFecha : public BusquedaFechaBase
{
Q_OBJECT
private:
company *companyact;
public:
    BusquedaFecha(QWidget *parent = 0, const char *name = 0);
    ~BusquedaFecha();
    void setcompany(company *comp) {companyact = comp;}; 
        virtual void setfecha(QString val) {m_fecha->setText(val);};   
	virtual void setText(QString val) {m_fecha->setText(val);};
public slots:
	virtual void s_searchFecha();
	virtual void s_fechatextChanged(const QString &);   
	virtual void s_fechalostFocus(); 
signals:
	void valueChanged(QString);
};

#endif
