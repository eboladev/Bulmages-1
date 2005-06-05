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

#include <facturaslistbase.h>

/**
@author Tomeu Borras
*/

class company;

class FacturasList : public FacturasListBase
{
Q_OBJECT
private:
   company *companyact;
   int m_modo; // == 0 es modo edición
            // ==1 es modo selector.
   QString m_idfactura;
public:
    FacturasList(company *,QWidget *parent = 0, const char *name = 0);
    ~FacturasList();
    void inicializa();
    void modoseleccion() {m_modo=1;};
    void modoedicion() {m_modo=0;};
    QString idfactura() {return m_idfactura;};   
public slots:

	virtual void doubleclicked(int, int , int , const QPoint &) ;
};

#endif
