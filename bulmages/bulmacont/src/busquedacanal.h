/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org Asociación Iglues -- Contabilidad Linux         *
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

#ifndef BUSQUEDACANAL_H
#define BUSQUEDACANAL_H

#include "blcombobox.h"
#include "blwidget.h"
#include "empresa.h"
#include "blpostgresqlclient.h"


class BusquedaCanalDelegate : public BlComboBox
{
    Q_OBJECT

private:
    /// Este cursor almacena el listado de series de factura para poder trabajar con ellas.
    BlDbRecordSet *m_cursorcombo;
    QCompleter *completar;

public:
    BusquedaCanalDelegate ( QWidget *parent = 0 );
    ~BusquedaCanalDelegate();
    virtual void set ( const QString & );
};



class BusquedaCanal : public BlComboBox
{
    Q_OBJECT

private:
    BlDbRecordSet *m_cursorcombo;

public:
    BusquedaCanal ( QWidget *parent = 0 );
    ~BusquedaCanal();
    virtual void setidcanal ( QString idcanal );
    virtual void setValorCampo ( QString idcanal );
    QString idcanal();
    virtual QString valorCampo();

public slots:
    void m_activated ( int index );

signals:
    void valueChanged ( QString );
};

#endif

