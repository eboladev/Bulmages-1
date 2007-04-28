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

#ifndef ALBARANPROVEEDOR_H
#define ALBARANPROVEEDOR_H

#include <QString>
#include <QWidget>

#include "company.h"
#include "fichabf.h"
#include "listlinalbaranproveedorview.h"
#include "listdescalbaranprovview.h"


/** Esta clase se encarga de la manipulacion y operacion con la base de datos
    desde la ficha de albaran de proveedor.
*/
class AlbaranProveedor : public FichaBf {
    Q_OBJECT

public:
    AlbaranProveedor(company *, QWidget *);
    virtual ~AlbaranProveedor();
    virtual void pintar();
    virtual int guardar();
    virtual int borrar();
    virtual int cargar(QString);
    virtual void pintaidalbaranp(QString);
    virtual void pintanumalbaranp(QString);
    virtual void pintafechaalbaranp(QString);
    virtual void pintacomentalbaranp(QString);
    virtual void pintaidproveedor(QString);
    virtual void pintaidforma_pago(QString);
    virtual void pintaidalmacen(QString);
    virtual void pintadescalbaranp(QString);
    virtual void pintarefalbaranp(QString);
    void setidalbaranp(QString val);
    void setnumalbaranp(QString val);
    void setfechaalbaranp(QString val);
    void setloginusuario(QString val);
    void setcomentalbaranp(QString val);
    void setidproveedor(QString val);
    void setidforma_pago(QString val);
    void setidalmacen(QString val);
    void setrefalbaranp(QString val);
    void setdescalbaranp(QString val);
    void vaciaAlbaranProveedor();
};

#endif

