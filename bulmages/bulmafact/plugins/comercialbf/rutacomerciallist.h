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

#include <QLineEdit>

#include "company.h"
#include "busquedacliente.h"
#include "busquedaarticulo.h"
#include "subform2bf.h"
#include "listado.h"

class RutaComercialListSubForm : public SubForm2Bf
{
    Q_OBJECT

public:
    RutaComercialListSubForm ( QWidget *parent = 0 );
    ~RutaComercialListSubForm()
    {}
    ;

public slots:
    virtual void cargar()
    {
        _depura ( "RutaComercialListSubForm::cargar\n", 0 );
        QString SQLQuery = "SELECT * FROM presupuesto";
        SubForm3::cargar ( SQLQuery );
    };
    virtual void cargar ( QString query )
    {
        _depura ( "RutaComercialListSubForm::cargar\n", 0 );
        SubForm3::cargar ( query );
    };
};


#include "ui_rutacomerciallistbase.h"


class RutaComercialList : public Listado, public Ui_RutaComercialListBase
{
    Q_OBJECT

private:

    QString m_idpresupuesto;

public:
    RutaComercialList ( QWidget *parent = 0 );
    RutaComercialList ( Company *comp = NULL, QWidget *parent = 0 );
    ~RutaComercialList();
    void presenta();

    QString idpresupuesto();

    void setEmpresaBase ( Company *comp );
    void imprimir();
    void setidcliente ( QString val );
    QString generaFiltro();
    void editar ( int );

public slots:


    virtual void on_mui_crear_clicked();
    virtual void on_mui_borrar_clicked();



signals:
    void selected ( QString );
};

#endif

