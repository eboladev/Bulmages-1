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

#ifndef BUSQUEDAFAMILIA_H
#define BUSQUEDAFAMILIA_H

#include <QLineEdit>
#include <QLabel>

#include "ui_busquedafamiliabase.h"


class company;


/// Permite buscar y seleccionar una familia de art&iacute;culos.
/** Este Widget permite buscar y seleccionar una familia de art&iacute;culos. 
    Para usar en forma de selector de articulos.
*/
class BusquedaFamilia : public QWidget, public Ui_BusquedaFamiliaBase {
    Q_OBJECT

private:
    /// Mediante este puntero podemos trabajar con la base de datos y pasar mensajes.
    company *companyact;
    /// Almacena el idfamilia que se haya seleccionado.
    QString mdb_idfamilia;
    /// Almacena el nombrefamilia que se haya seleccionado.
    QString mdb_nombrefamilia;
    /// Almacena el codigocompletofamilia que se haya seleccionado.
    QString mdb_codigocompletofamilia;

public:
    BusquedaFamilia(QWidget *parent = 0);
    ~BusquedaFamilia();
    void setEmpresaBase(company *comp);
    virtual QString codigocompletofamilia();
    virtual QString idfamilia();
    virtual QString nombrefamilia();
    virtual void setidfamilia(QString val);
    virtual void setcodigocompletofamilia(QString val);

public slots:
    virtual void on_mui_buscar_clicked();
    virtual void on_m_codigocompletofamilia_textChanged(const QString &);

signals:
    void valueChanged(QString);
};

#endif

