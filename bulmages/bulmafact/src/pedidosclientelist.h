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

/**
@author Tomeu Borras
*/


class company;

class PedidosClienteList : public PedidosClienteListBase
{
Q_OBJECT
public:
   company *companyact;
   int m_modo; // == 0 es modo edición
            // ==1 es modo selector.
   QString m_idpedidocliente;


    PedidosClienteList(company *, QWidget *parent = 0, const char *name = 0);

    ~PedidosClienteList();
    void inicializa();
    void modoseleccion() {m_modo=1;};
    void modoedicion() {m_modo=0;};
    QString idpedidocliente() {return m_idpedidocliente;}; 
public slots:

	virtual void doubleclicked(int, int , int , const QPoint &) ;    
};

#endif
