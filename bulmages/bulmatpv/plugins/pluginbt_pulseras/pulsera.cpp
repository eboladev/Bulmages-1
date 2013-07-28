/***************************************************************************
 *   Copyright (C) 2011 by Tomeu Borras Riera                              *
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

#include "pulsera.h"


QList<Pulsera *> g_pulseras;


Pulsera::Pulsera(BtTicket *tick, const QString nombres, BlDbRecord *linea, const QString propietario) {

  m_ticketpulsera = tick;
  m_nombrepulsera = nombres;
  m_nombrepropietario = propietario;
  m_horainicial = QTime::currentTime ();
//  m_horafinal = QDateTime::currentDateTime ();
  g_pulseras.append(this);
  m_lineaticket = linea;
  m_lineaticketfraccion = NULL;
  m_sinfracciones = false;
  
}


Pulsera::~Pulsera() {
  g_pulseras.removeAll(this);
}