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

#ifndef BUSQUEDAZONACOMERCIAL_H
#define BUSQUEDAZONACOMERCIAL_H

#include <QComboBox>

#include "company.h"
#include "blpostgresqlclient.h"
#include "blfunctions.h"
#include "blwidget.h"


class BusquedaZonaComercial : public QComboBox, public BlMainCompanyPointer
{
    Q_OBJECT

private:
    BlDbRecordSet *m_cursorcombo;

public:
    BusquedaZonaComercial ( QWidget *parent = 0 );
    ~BusquedaZonaComercial();
    virtual void setidzonacomercial ( QString idzonacomercial );
    QString idzonacomercial() {
        return m_cursorcombo->valor ( "idzonacomercial", currentIndex() - 1 );
    };

public slots:
    void m_activated ( int index ) {
        _depura ( "Activado el combo box", 0 );
        if ( index > 0 ) {
            emit ( valueChanged ( m_cursorcombo->valor ( "idzonacomercial", index - 1 ) ) );
        } else {
            emit ( valueChanged ( "" ) );
        }
    };

signals:
    void valueChanged ( QString );
};

#endif
