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
#ifndef PULSERA_H
#define PULSERA_H

#include <QtCore/QString>
#include <QtCore/QDateTime>
#include "btticket.h"
#include "pdefs_pluginbt_pulseras.h"


extern "C" PLUGINBT_PULSERAS_EXPORT class Pulsera {
public:
  QString m_nombrepulsera;
  QString m_nombrepropietario;
  BtTicket *m_ticketpulsera;
  QTime m_horainicial;
//  QDateTime m_horafinal;
  BlDbRecord *m_lineaticket;
  BlDbRecord *m_lineaticketfraccion;
  bool m_sinfracciones;
  
  Pulsera(BtTicket *tick, const QString nombres, BlDbRecord *linea, const QString propietario = "");
  ~Pulsera();
  
};


extern QList<Pulsera *> g_pulseras;



#endif