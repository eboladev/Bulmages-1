/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#ifndef BUSQUEDAFECHA_H
#define BUSQUEDAFECHA_H

#include <QLineEdit>
#include <QWidget>

#include "ui_busquedafechabase.h"


class BusquedaFecha : public QWidget, public Ui_BusquedaFechaBase {
    Q_OBJECT

private:

public:
    BusquedaFecha(QWidget *parent = 0, const char *name = 0);
    ~BusquedaFecha();
    virtual void setfecha(QString val) {
        m_fecha->setText(val);
    };
    virtual void setText(QString val) {
        m_fecha->setText(val);
    };
    virtual QString fecha() {
        return(m_fecha->text());
    };
    virtual QString text() {
        return m_fecha->text();
    };

public slots:
    virtual void s_searchFecha();
    virtual void s_fechatextChanged(const QString &);
    virtual void s_fechalostFocus();
    virtual void s_returnPressed() {
                             s_fechalostFocus();
                             emit returnPressed();
                         };
    virtual void selectAll() {
        m_fecha->selectAll();
    };
    virtual void setFocus() {
        m_fecha->setFocus();
    };

signals:
    void valueChanged(QString);
    void returnPressed();
};

#endif

