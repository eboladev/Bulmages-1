/***************************************************************************
 *   Copyright (C) 2005 by Alvaro de Miguel                                *
 *   alvaro.demiguel@gmail.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef CLIENTINVOICESLIST_H
#define CLIENTINVOICESLIST_H

// Listado de albaranes de clientes.
// Clients delivery notes list.

#include "clientinvoiceslistbase.h"


class company;


class ClientInvoicesList : public ClientInvoicesListBase {
    Q_OBJECT
private:
    company *companyact;
    int m_modo; // == 0 es modo edición
    // ==1 es modo selector.
    QString m_idcliinvoice;

public:
    ClientInvoicesList(company *, QWidget *parent = 0, const char *name = 0, int flag = 0);
    ~ClientInvoicesList();
    void inicializa();
    void modoseleccion() {
        m_modo=1;
    };
    void modoedicion() {
        m_modo=0;
    };
    QString idCliInvoice() {
        return m_idcliinvoice;
    };

public slots:
    virtual void s_doubleclicked(int, int, int, const QPoint &);
    virtual void s_newClientInvoice();
    virtual void s_removeClientInvoice();
    virtual void s_contextMenu(int, int, int, const QPoint &);
};

#endif
