/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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

#ifndef COMERCIAL_H
#define COMERCIAL_H

#include <QtCore/QObject>

#include "bfbulmafact.h"
#include "blwidget.h"

class PluginBf_Comercial : public QObject, BlMainCompanyPointer
{
    Q_OBJECT

private:
    BfBulmaFact *m_bges;
public:
    PluginBf_Comercial();
    virtual ~PluginBf_Comercial();
    virtual void inicializa ( BfBulmaFact * );

public slots:
    void elslot();
    void elslot1();
};



#endif
