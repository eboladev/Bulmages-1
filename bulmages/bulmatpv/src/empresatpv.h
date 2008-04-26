/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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

#ifndef EMPRESATPV_H
#define EMPRESATPV_H

#include <QObject>
#include <QAssistantClient>
#include <QLibraryInfo>

#include "postgresiface2.h"
#include "listventanas.h"
#include "qworkspace2.h"
#include "empresabase.h"
#include "splashscreen.h"
#include "ticket.h"
#include "input.h"


class BulmaTPV;

/// Clase company (empresa).
/** Clase principal del programa donde se almacenan y gestionan
    todos los datos de la empresa con la que se est&aacute; trabajando.
    Deriva de postgresiface ya que tiene el tratamiento de la base de datos. */
class EmpresaTPV : public EmpresaBase, public Input
{
private:
    /// Puntero al mainWindow
    BulmaTPV *m_bulmaTPV;
    QList<Ticket *> m_listaTickets;
    Ticket *m_ticketActual;

public:
    EmpresaTPV ( BulmaTPV *bges );
    virtual ~EmpresaTPV();
    void createMainWindows ( Splash * );
    void guardaConf();
    void cargaConf();
    Ticket *newTicket();
    void cobrar();
    void z();
    void x();
    Ticket *ticketActual() {
        return m_ticketActual;
    };
    QList<Ticket *> *listaTickets() {
        return & m_listaTickets;
    };
    void setTicketActual ( Ticket *tick ) {
        m_ticketActual = tick;
    };

};

#endif

