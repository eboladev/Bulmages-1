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

#ifndef BUSQUEDACOLOR_H
#define BUSQUEDACOLOR_H

#include "blcombobox.h"
#include "blwidget.h"
#include "company.h"
#include "blpostgresqlclient.h"


/** Convierte en los SubForms los datos del tipo desctipo_iva en selectores del tipo QComboBox
    de esta forma la introduccion de tipos de IVA es sencilla.
    Esta clase se usa conjuntamente con BfSubForm para el cambio del editor
    estandar por un ComboBox */
class BusquedaColorDelegate : public BlComboBox
{
    Q_OBJECT

private:
    /// Este cursor almacena el listado de series de factura para poder trabajar con ellas.
    BlDbRecordSet *m_cursorcombo;

public:
    BusquedaColorDelegate ( QWidget *parent = 0 );
    ~BusquedaColorDelegate();
    virtual void set ( const QString & );
    QString id();
};


/// Clase que sirve para seleccionar un almac&eacute;n.
/** Creamos un QComboBox que sirve para presentar la lista de almacenes
    disponibles para poder seleccionar uno de ellos.*/
class BusquedaColor : public BlComboBox
{
    Q_OBJECT

private:
    /// El puntero a company para que se pueda trabajar con la base de datos.
    BlDbRecordSet *m_cursorcombo;

public:
    BusquedaColor ( QWidget *parent = 0, const char *name = 0 );
    ~BusquedaColor();
    virtual void setidtc_color ( QString idtc_color );
    virtual void setValorCampo ( QString idtc_color );
    QString idtc_color();
    void setEmpresaBase ( Company * );
    virtual QString valorCampo();

public slots:
    void m_activated ( int index );

signals:
    void valueChanged ( QString );
};

#endif

