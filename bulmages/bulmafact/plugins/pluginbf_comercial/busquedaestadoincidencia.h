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

#ifndef BUSQUEDAESTADOINCIDENCIA_H
#define BUSQUEDAESTADOINCIDENCIA_H

#include <QtWidgets/QComboBox>

#include "blfunctions.h"


class BusquedaEstadoIncidencia : public QComboBox
{
    Q_OBJECT

private:
    QString m_textos[10];
    QString m_valores[10];

public:
    BusquedaEstadoIncidencia ( QWidget *parent = 0 );
    ~BusquedaEstadoIncidencia();
    virtual void setestado ( QString estado );
    QString estado() {
        return m_valores[currentIndex() ];
    };

public slots:
    void s_activated ( int index ) {
        BL_FUNC_DEBUG
        if ( index > 0 ) {
            emit ( valueChanged ( m_valores[currentIndex() ] ) );
        } else {
            emit ( valueChanged ( "" ) );
        } // end if
    };

signals:
    void valueChanged ( QString );
};

#endif

