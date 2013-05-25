/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
 *                                                                         *
 *   Copyright (C) 2009 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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

#ifndef BTCOMPANY_H
#define BTCOMPANY_H

#include <QtCore/QObject>
#include <QtCore/QLibraryInfo>



#include "blpostgresqlclient.h"
#include "blwindowlistdock.h"
#include "blworkspace.h"
#include "blmaincompany.h"
#include "blsplashscreen.h"
#include "btticket.h"
#include "btinput.h"


class BtBulmaTPV;

/// Clase company (empresa).
/** Clase principal del programa donde se almacenan y gestionan
    todos los datos de la empresa con la que se est&aacute; trabajando.
    Deriva de postgresiface ya que tiene el tratamiento de la base de datos. */
class BT_EXPORT BtCompany : public BlMainCompany, public BtInput
{
private:
    /// Puntero al mainWindow
    BtBulmaTPV *m_bulmaTPV;
    QList<BtTicket *> m_listaTickets;
    BtTicket *m_ticketActual;
    int m_decimalesCantidad; // Indica cual es la precision a aplicar en las cantidades.
    int m_decimalesPrecio; // Indica cual es la precision a aplicar en los precios.

public:
    BtCompany ( BtBulmaTPV *bges );
    virtual ~BtCompany();
    void createMainWindows ( BlSplashScreen * );
    void guardaConf();
    void cargaConf();
    BtTicket *newBtTicket();
    void cobrar(bool imprimir = true);
    void z();
    void x();
    BtTicket *ticketActual();
    QList<BtTicket *> *listaTickets();
    void setTicketActual ( BtTicket *tick );
    void compruebaUltimaZ();
    virtual QString exportXML();
    virtual void syncXML(const QString &);
    int decimalesCantidad();
    void setDecimalesCantidad(int numd);
    int decimalesPrecio();
    void setDecimalesPrecio(int numd);
    void joinTickets(BtTicket *origin, BtTicket *destiny);
};

#endif

