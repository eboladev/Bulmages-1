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

#ifndef RUTACOMERCIALLIST_H
#define RUTACOMERCIALLIST_H

#include <QtWidgets/QLineEdit>

#include "bfcompany.h"
#include "bfbuscararticulo.h"
#include "bfsubform.h"
#include "blformlist.h"


class RutaComercialListSubForm : public BfSubForm
{
    Q_OBJECT

public:
    RutaComercialListSubForm ( QWidget *parent = 0 );
    ~RutaComercialListSubForm() {};

public slots:
    virtual void load() {
        BL_FUNC_DEBUG
        QString SQLQuery = "SELECT * FROM presupuesto";
        BlSubForm::load ( SQLQuery );
    };
    virtual void load ( QString query ) {
        BL_FUNC_DEBUG
        BlSubForm::load ( query );
    };
};


#include "ui_rutacomerciallistbase.h"


class RutaComercialList : public BlFormList, public Ui_RutaComercialListBase
{
    Q_OBJECT

private:
    QString m_idpresupuesto;

public:
    RutaComercialList ( QWidget *parent = 0 );
    RutaComercialList ( BfCompany *comp = NULL, QWidget *parent = 0 );
    ~RutaComercialList();
    void presenta();
    QString idpresupuesto();
    void setMainCompany ( BfCompany *comp );
    void imprimir();
    void setidcliente ( QString val );
    const QString generaFiltro();
    void editar ( int );
    virtual void crear();
    virtual void remove();

signals:
    void selected ( QString );
};

#endif

