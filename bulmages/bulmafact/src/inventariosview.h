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
#ifndef INVENTARIOSVIEW_H
#define INVENTARIOSVIEW_H

#include "inventariosbase.h"

#include <Q3Frame>

/**
@author Tomeu Borras
*/

#include "company.h"

class InventariosView : public InventariosBase
{
Q_OBJECT
private:
   company *companyact;
public:
    InventariosView(QWidget *parent = 0, const char *name = 0, Qt::WFlags flag = 0);
    InventariosView(company *,QWidget *parent = 0, const char *name = 0);
    ~InventariosView();
    void inicializa();
    void setcompany (company *comp) {companyact=comp;};
    void meteWindow(QString nom, QObject *obj) {
    if (companyact != NULL)
    companyact->meteWindow(nom, obj);
    };
public slots:
	virtual void doubleclicked(int, int , int , const QPoint &) ;
	virtual void s_new() {companyact->s_newInventario();};
    	virtual void s_edit();
	virtual void s_refresh() {inicializa();};
	virtual void s_delete();
};

#endif
