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

#include <QComboBox>

#include "empresa.h"
#include "postgresiface2.h"


class BusquedaCanalDelegate : public QComboBox {
    Q_OBJECT

private:
    Empresa *m_companyact;
    /// Este cursor almacena el listado de series de factura para poder trabajar con ellas.
    cursor2 *m_cursorcombo;
    QCompleter *completar;

public:
    BusquedaCanalDelegate(QWidget *parent = 0);
    ~BusquedaCanalDelegate();
    void setcompany(Empresa *comp);

public slots:
    virtual void s_editTextChanged(const QString &);
};



class BusquedaCanal : public QComboBox {
    Q_OBJECT

private:
    Empresa *companyact;
    cursor2 *m_cursorcombo;

public:
    BusquedaCanal(QWidget *parent = 0);
    ~BusquedaCanal();
    void setcompany(Empresa *comp);
    virtual void setidcanal(QString idcanal);
    QString idcanal();

public slots:
    void m_activated(int index);

signals:
    void valueChanged(QString);
};

#endif

